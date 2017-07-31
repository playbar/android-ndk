/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *  Copyright (C) 2016-2017 - Brad Parker
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <compat/strl.h>
#include <compat/posix_string.h>
#include <file/file_path.h>
#include <lists/dir_list.h>
#include <string/stdstring.h>
#include <streams/stdin_stream.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_COMMAND
#ifdef HAVE_NETWORKING
#include <net/net_compat.h>
#include <net/net_socket.h>
#endif
#include <string/stdstring.h>
#endif

#ifdef HAVE_CHEEVOS
#include "cheevos/cheevos.h"
#endif

#ifdef HAVE_MENU
#include "menu/menu_driver.h"
#include "menu/menu_content.h"
#include "menu/menu_shader.h"
#include "menu/widgets/menu_dialog.h"
#endif

#ifdef HAVE_NETWORKING
#include <net/net_compat.h>
#include "network/netplay/netplay.h"
#endif

#include "command.h"

#include "defaults.h"
#include "driver.h"
#include "input/input_config.h"
#include "frontend/frontend_driver.h"
#include "audio/audio_driver.h"
#include "record/record_driver.h"
#include "file_path_special.h"
#include "autosave.h"
#include "core_info.h"
#include "core_type.h"
#include "performance_counters.h"
#include "dynamic.h"
#include "content.h"
#include "dirs.h"
#include "movie.h"
#include "paths.h"
#include "msg_hash.h"
#include "retroarch.h"
#include "managers/cheat_manager.h"
#include "managers/state_manager.h"
#include "ui/ui_companion_driver.h"
#include "tasks/tasks_internal.h"
#include "list_special.h"

#include "core.h"
#include "verbosity.h"
#include "retroarch.h"
#include "configuration.h"
#include "input/input_remapping.h"

#define DEFAULT_NETWORK_CMD_PORT 55355
#define STDIN_BUF_SIZE           4096

extern int libui_main(void);

struct command
{
#ifdef HAVE_STDIN_CMD
   bool stdin_enable;
   char stdin_buf[STDIN_BUF_SIZE];
   size_t stdin_buf_ptr;
#endif

#if defined(HAVE_NETWORKING) && defined(HAVE_NETWORK_CMD)
   int net_fd;
#endif

   bool state[RARCH_BIND_LIST_END];
};

enum cmd_source_t
{
   CMD_NONE = 0,
   CMD_STDIN,
   CMD_NETWORK
};

#if defined(HAVE_STDIN_CMD) || defined(HAVE_NETWORK_CMD) && defined(HAVE_NETWORKING)
static enum cmd_source_t lastcmd_source;
#endif
#if defined(HAVE_NETWORKING) && defined(HAVE_NETWORK_CMD)
static int lastcmd_net_fd;
static struct sockaddr_storage lastcmd_net_source;
static socklen_t lastcmd_net_source_len;
#endif

#ifdef HAVE_CHEEVOS
#if defined(HAVE_STDIN_CMD) || defined(HAVE_NETWORK_CMD) && defined(HAVE_NETWORKING)
static bool command_reply(const char * data, size_t len)
{
#ifdef HAVE_STDIN_CMD
   if (lastcmd_source == CMD_STDIN)
   {
      fwrite(data, 1,len, stdout);
      return true;
   }
#endif
#if defined(HAVE_NETWORKING) && defined(HAVE_NETWORK_CMD)
   if (lastcmd_source == CMD_NETWORK)
   {
      sendto(lastcmd_net_fd, data, len, 0,
            (struct sockaddr*)&lastcmd_net_source, lastcmd_net_source_len);
      return true;
   }
#endif
   return false;
}
#endif
#endif

struct cmd_map
{
   const char *str;
   unsigned id;
};

struct cmd_action_map
{
   const char *str;
   bool (*action)(const char *arg);
   const char *arg_desc;
};

bool command_set_shader(const char *arg)
{
   char msg[256];
   enum rarch_shader_type type = RARCH_SHADER_NONE;

   switch (msg_hash_to_file_type(msg_hash_calculate(path_get_extension(arg))))
   {
      case FILE_TYPE_SHADER_GLSL:
      case FILE_TYPE_SHADER_PRESET_GLSLP:
         type = RARCH_SHADER_GLSL;
         break;
      case FILE_TYPE_SHADER_CG:
      case FILE_TYPE_SHADER_PRESET_CGP:
         type = RARCH_SHADER_CG;
         break;
      case FILE_TYPE_SHADER_SLANG:
      case FILE_TYPE_SHADER_PRESET_SLANGP:
         type = RARCH_SHADER_SLANG;
         break;
      default:
         return false;
   }

   snprintf(msg, sizeof(msg), "Shader: \"%s\"", arg);
   runloop_msg_queue_push(msg, 1, 120, true);
   RARCH_LOG("%s \"%s\".\n",
         msg_hash_to_str(MSG_APPLYING_SHADER),
         arg);

   return video_driver_set_shader(type, arg);
}

#ifdef HAVE_COMMAND
#ifdef HAVE_CHEEVOS
static bool command_read_ram(const char *arg)
{
   cheevos_var_t var;
   unsigned i;
   unsigned nbytes;
   char reply[256];
   const uint8_t * data = NULL;
   char *reply_at       = NULL;

   reply[0]             = '\0';

   strlcpy(reply, "READ_CORE_RAM ", sizeof(reply));
   reply_at = reply + strlen("READ_CORE_RAM ");
   strlcpy(reply_at, arg, sizeof(reply)-strlen(reply));

   cheevos_parse_guest_addr(&var, strtoul(reply_at, (char**)&reply_at, 16));
   data = cheevos_get_memory(&var);

   if (data)
   {
      unsigned nbytes = strtol(reply_at, NULL, 10);

      for (i=0;i<nbytes;i++)
      {
         sprintf(reply_at+3*i, " %.2X", data[i]);
      }
      reply_at[3*nbytes] = '\n';
      command_reply(reply, reply_at+3*nbytes+1 - reply);
   }
   else
   {
      strlcpy(reply_at, " -1\n", sizeof(reply)-strlen(reply));
      command_reply(reply, reply_at+strlen(" -1\n") - reply);
   }

   return true;
}

static bool command_write_ram(const char *arg)
{
   int i;
   cheevos_var_t var;
   unsigned nbytes;
   uint8_t * data    = NULL;

   cheevos_parse_guest_addr(&var, strtoul(arg, (char**)&arg, 16));

   data = cheevos_get_memory(&var);

   if (!data)
      return false;

   while (*arg)
   {
      *data = strtoul(arg, (char**)&arg, 16);
      data++;
   }

   return true;
}
#endif

static const struct cmd_action_map action_map[] = {
   { "SET_SHADER", command_set_shader, "<shader path>" },
#ifdef HAVE_CHEEVOS
   { "READ_CORE_RAM", command_read_ram, "<address> <number of bytes>" },
   { "WRITE_CORE_RAM", command_write_ram, "<address> <byte1> <byte2> ..." },
#endif
};

static const struct cmd_map map[] = {
   { "FAST_FORWARD",           RARCH_FAST_FORWARD_KEY },
   { "FAST_FORWARD_HOLD",      RARCH_FAST_FORWARD_HOLD_KEY },
   { "LOAD_STATE",             RARCH_LOAD_STATE_KEY },
   { "SAVE_STATE",             RARCH_SAVE_STATE_KEY },
   { "FULLSCREEN_TOGGLE",      RARCH_FULLSCREEN_TOGGLE_KEY },
   { "QUIT",                   RARCH_QUIT_KEY },
   { "STATE_SLOT_PLUS",        RARCH_STATE_SLOT_PLUS },
   { "STATE_SLOT_MINUS",       RARCH_STATE_SLOT_MINUS },
   { "REWIND",                 RARCH_REWIND },
   { "MOVIE_RECORD_TOGGLE",    RARCH_MOVIE_RECORD_TOGGLE },
   { "PAUSE_TOGGLE",           RARCH_PAUSE_TOGGLE },
   { "FRAMEADVANCE",           RARCH_FRAMEADVANCE },
   { "RESET",                  RARCH_RESET },
   { "SHADER_NEXT",            RARCH_SHADER_NEXT },
   { "SHADER_PREV",            RARCH_SHADER_PREV },
   { "CHEAT_INDEX_PLUS",       RARCH_CHEAT_INDEX_PLUS },
   { "CHEAT_INDEX_MINUS",      RARCH_CHEAT_INDEX_MINUS },
   { "CHEAT_TOGGLE",           RARCH_CHEAT_TOGGLE },
   { "SCREENSHOT",             RARCH_SCREENSHOT },
   { "MUTE",                   RARCH_MUTE },
   { "OSK",                    RARCH_OSK },
   { "NETPLAY_FLIP",           RARCH_NETPLAY_FLIP },
   { "NETPLAY_GAME_WATCH",     RARCH_NETPLAY_GAME_WATCH },
   { "SLOWMOTION",             RARCH_SLOWMOTION },
   { "VOLUME_UP",              RARCH_VOLUME_UP },
   { "VOLUME_DOWN",            RARCH_VOLUME_DOWN },
   { "OVERLAY_NEXT",           RARCH_OVERLAY_NEXT },
   { "DISK_EJECT_TOGGLE",      RARCH_DISK_EJECT_TOGGLE },
   { "DISK_NEXT",              RARCH_DISK_NEXT },
   { "DISK_PREV",              RARCH_DISK_PREV },
   { "GRAB_MOUSE_TOGGLE",      RARCH_GRAB_MOUSE_TOGGLE },
   { "GAME_FOCUS_TOGGLE",      RARCH_GAME_FOCUS_TOGGLE },
   { "MENU_TOGGLE",            RARCH_MENU_TOGGLE },
   { "MENU_UP",                RETRO_DEVICE_ID_JOYPAD_UP },
   { "MENU_DOWN",              RETRO_DEVICE_ID_JOYPAD_DOWN },
   { "MENU_LEFT",              RETRO_DEVICE_ID_JOYPAD_LEFT },
   { "MENU_RIGHT",             RETRO_DEVICE_ID_JOYPAD_RIGHT },
   { "MENU_A",                 RETRO_DEVICE_ID_JOYPAD_A },
   { "MENU_B",                 RETRO_DEVICE_ID_JOYPAD_B },
   { "MENU_B",                 RETRO_DEVICE_ID_JOYPAD_B },
};

static bool command_get_arg(const char *tok,
      const char **arg, unsigned *index)
{
   unsigned i;

   for (i = 0; i < ARRAY_SIZE(map); i++)
   {
      if (string_is_equal(tok, map[i].str))
      {
         if (arg)
            *arg = NULL;

         if (index)
            *index = i;

         return true;
      }
   }

   for (i = 0; i < ARRAY_SIZE(action_map); i++)
   {
      const char *str = strstr(tok, action_map[i].str);
      if (str == tok)
      {
         const char *argument = str + strlen(action_map[i].str);
         if (*argument != ' ')
            return false;

         if (arg)
            *arg = argument + 1;

         if (index)
            *index = i;

         return true;
      }
   }

   return false;
}

static void command_parse_sub_msg(command_t *handle, const char *tok)
{
   const char *arg = NULL;
   unsigned index  = 0;

   if (command_get_arg(tok, &arg, &index))
   {
      if (arg)
      {
         if (!action_map[index].action(arg))
            RARCH_ERR("Command \"%s\" failed.\n", arg);
      }
      else
         handle->state[map[index].id] = true;
   }
   else
      RARCH_WARN("%s \"%s\" %s.\n",
            msg_hash_to_str(MSG_UNRECOGNIZED_COMMAND),
            tok,
            msg_hash_to_str(MSG_RECEIVED));
}

static void command_parse_msg(command_t *handle, char *buf, enum cmd_source_t source)
{
   char *save      = NULL;
   const char *tok = strtok_r(buf, "\n", &save);

   lastcmd_source = source;

   while (tok)
   {
      command_parse_sub_msg(handle, tok);
      tok = strtok_r(NULL, "\n", &save);
   }
   lastcmd_source = CMD_NONE;
}

#if defined(HAVE_NETWORKING) && defined(HAVE_NETWORK_CMD)
static bool command_network_init(command_t *handle, uint16_t port)
{
   struct addrinfo *res  = NULL;
   int fd                = socket_init((void**)&res, port,
         NULL, SOCKET_TYPE_DATAGRAM);

   RARCH_LOG("%s %hu.\n",
         msg_hash_to_str(MSG_BRINGING_UP_COMMAND_INTERFACE_ON_PORT),
         (unsigned short)port);


   if (fd < 0)
      goto error;

   handle->net_fd = fd;

   if (!socket_nonblock(handle->net_fd))
      goto error;

   if (!socket_bind(handle->net_fd, (void*)res))
   {
      RARCH_ERR("%s.\n",
            msg_hash_to_str(MSG_FAILED_TO_BIND_SOCKET));
      goto error;
   }

   freeaddrinfo_retro(res);
   return true;

error:
   if (res)
      freeaddrinfo_retro(res);
   return false;
}

static bool command_verify(const char *cmd)
{
   unsigned i;

   if (command_get_arg(cmd, NULL, NULL))
      return true;

   RARCH_ERR("Command \"%s\" is not recognized by the program.\n", cmd);
   RARCH_ERR("\tValid commands:\n");
   for (i = 0; i < sizeof(map) / sizeof(map[0]); i++)
      RARCH_ERR("\t\t%s\n", map[i].str);

   for (i = 0; i < sizeof(action_map) / sizeof(action_map[0]); i++)
      RARCH_ERR("\t\t%s %s\n", action_map[i].str, action_map[i].arg_desc);

   return false;
}

bool command_network_send(const char *cmd_)
{
   bool ret            = false;
   char *command       = NULL;
   char *save          = NULL;
   const char *cmd     = NULL;
   const char *host    = NULL;
   const char *port_   = NULL;
   uint16_t port       = DEFAULT_NETWORK_CMD_PORT;

   if (!network_init())
      return false;

   if (!(command = strdup(cmd_)))
      return false;

   cmd = strtok_r(command, ";", &save);
   if (cmd)
      host = strtok_r(NULL, ";", &save);
   if (host)
      port_ = strtok_r(NULL, ";", &save);

   if (!host)
   {
#ifdef _WIN32
      host = "127.0.0.1";
#else
      host = "localhost";
#endif
   }

   if (port_)
      port = strtoul(port_, NULL, 0);

   if (cmd)
   {
      RARCH_LOG("%s: \"%s\" to %s:%hu\n",
            msg_hash_to_str(MSG_SENDING_COMMAND),
            cmd, host, (unsigned short)port);

      ret = command_verify(cmd) && udp_send_packet(host, port, cmd);
   }
   free(command);

   return ret;
}


#ifdef HAVE_COMMAND
static void command_network_poll(command_t *handle)
{
   fd_set fds;
   struct timeval tmp_tv = {0};

   if (handle->net_fd < 0)
      return;

   FD_ZERO(&fds);
   FD_SET(handle->net_fd, &fds);

   if (socket_select(handle->net_fd + 1, &fds, NULL, NULL, &tmp_tv) <= 0)
      return;

   if (!FD_ISSET(handle->net_fd, &fds))
      return;

   for (;;)
   {
      ssize_t ret;
      char buf[1024];

      buf[0] = '\0';

      lastcmd_net_fd = handle->net_fd;
      lastcmd_net_source_len = sizeof(lastcmd_net_source);
      ret = recvfrom(handle->net_fd, buf,
            sizeof(buf) - 1, 0, (struct sockaddr*)&lastcmd_net_source, &lastcmd_net_source_len);

      if (ret <= 0)
         break;

      buf[ret] = '\0';

      command_parse_msg(handle, buf, CMD_NETWORK);
   }
}
#endif
#endif

#ifdef HAVE_STDIN_CMD
static bool command_stdin_init(command_t *handle)
{
#ifndef _WIN32
#ifdef HAVE_NETWORKING
   if (!socket_nonblock(STDIN_FILENO))
      return false;
#endif
#endif

   handle->stdin_enable = true;
   return true;
}
#endif

command_t *command_new(void)
{
   command_t *handle = (command_t*)calloc(1, sizeof(*handle));
   if (!handle)
      return NULL;

   return handle;
}

bool command_network_new(
      command_t *handle,
      bool stdin_enable,
      bool network_enable,
      uint16_t port)
{
   if (!handle)
      return false;

#if defined(HAVE_NETWORKING) && defined(HAVE_NETWORK_CMD)
   handle->net_fd = -1;
   if (network_enable && !command_network_init(handle, port))
      goto error;
#endif

#ifdef HAVE_STDIN_CMD
   handle->stdin_enable = stdin_enable;
   if (stdin_enable && !command_stdin_init(handle))
      goto error;
#endif

   return true;

#if (defined(HAVE_NETWORK_CMD) && defined(HAVE_NETWORKING)) || defined(HAVE_STDIN_CMD)
error:
   command_free(handle);
   return false;
#endif
}

#ifdef HAVE_STDIN_CMD


static void command_stdin_poll(command_t *handle)
{
   ssize_t ret;
   ptrdiff_t msg_len;
   char *last_newline = NULL;

   if (!handle->stdin_enable)
      return;

   ret = read_stdin(handle->stdin_buf + handle->stdin_buf_ptr,
         STDIN_BUF_SIZE - handle->stdin_buf_ptr - 1);
   if (ret == 0)
      return;

   handle->stdin_buf_ptr += ret;
   handle->stdin_buf[handle->stdin_buf_ptr] = '\0';

   last_newline = strrchr(handle->stdin_buf, '\n');

   if (!last_newline)
   {
      /* We're receiving bogus data in pipe
       * (no terminating newline), flush out the buffer. */
      if (handle->stdin_buf_ptr + 1 >= STDIN_BUF_SIZE)
      {
         handle->stdin_buf_ptr = 0;
         handle->stdin_buf[0] = '\0';
      }

      return;
   }

   *last_newline++ = '\0';
   msg_len = last_newline - handle->stdin_buf;

   command_parse_msg(handle, handle->stdin_buf, CMD_STDIN);

   memmove(handle->stdin_buf, last_newline,
         handle->stdin_buf_ptr - msg_len);
   handle->stdin_buf_ptr -= msg_len;
}
#endif
#endif

bool command_poll(command_t *handle)
{
#if defined(HAVE_NETWORKING) && defined(HAVE_NETWORK_CMD)
#ifdef HAVE_COMMAND
   command_network_poll(handle);
#endif
#endif

#ifdef HAVE_STDIN_CMD
   command_stdin_poll(handle);
#endif

   return true;
}

bool command_get(command_handle_t *handle)
{
   if (!handle || !handle->handle)
      return false;
   return handle->id < RARCH_BIND_LIST_END
      && handle->handle->state[handle->id];
}

bool command_set(command_handle_t *handle)
{
   if (!handle || !handle->handle)
      return false;
   if (handle->id < RARCH_BIND_LIST_END)
      handle->handle->state[handle->id] = true;
   return true;
}

bool command_free(command_t *handle)
{
#if defined(HAVE_NETWORKING) && defined(HAVE_NETWORK_CMD)
#ifdef HAVE_COMMAND
   if (handle && handle->net_fd >= 0)
      socket_close(handle->net_fd);
#endif
#endif

   free(handle);

   return true;
}

/**
 * command_event_disk_control_set_eject:
 * @new_state            : Eject or close the virtual drive tray.
 *                         false (0) : Close
 *                         true  (1) : Eject
 * @print_log            : Show message onscreen.
 *
 * Ejects/closes of the virtual drive tray.
 **/
static void command_event_disk_control_set_eject(bool new_state, bool print_log)
{
   char msg[128];
   bool error                                        = false;
   const struct retro_disk_control_callback *control = NULL;
   rarch_system_info_t *info                         = runloop_get_system_info();

   msg[0] = '\0';

   if (info)
      control = (const struct retro_disk_control_callback*)&info->disk_control_cb;

   if (!control || !control->get_num_images)
      return;

   if (control->set_eject_state(new_state))
      snprintf(msg, sizeof(msg), "%s %s",
            new_state ?
            msg_hash_to_str(MSG_DISK_EJECTED) :
            msg_hash_to_str(MSG_DISK_CLOSED),
            msg_hash_to_str(MSG_VIRTUAL_DISK_TRAY));
   else
   {
      error = true;
      snprintf(msg, sizeof(msg), "%s %s %s",
            msg_hash_to_str(MSG_FAILED_TO),
            new_state ? "eject" : "close",
            msg_hash_to_str(MSG_VIRTUAL_DISK_TRAY));
   }

   if (!string_is_empty(msg))
   {
      if (error)
         RARCH_ERR("%s\n", msg);
      else
         RARCH_LOG("%s\n", msg);

      /* Only noise in menu. */
      if (print_log)
         runloop_msg_queue_push(msg, 1, 180, true);
   }
}

/**
 * command_event_disk_control_set_index:
 * @idx                : Index of disk to set as current.
 *
 * Sets current disk to @index.
 **/
static void command_event_disk_control_set_index(unsigned idx)
{
   unsigned num_disks;
   char msg[128];
   bool error                                        = false;
   const struct retro_disk_control_callback *control = NULL;
   rarch_system_info_t *info                         = runloop_get_system_info();

   msg[0] = '\0';

   if (info)
      control = (const struct retro_disk_control_callback*)&info->disk_control_cb;

   if (!control || !control->get_num_images)
      return;

   num_disks = control->get_num_images();

   if (control->set_image_index(idx))
   {
      if (idx < num_disks)
         snprintf(msg, sizeof(msg), "%s: %u/%u.",
               msg_hash_to_str(MSG_SETTING_DISK_IN_TRAY),
               idx + 1, num_disks);
      else
         strlcpy(msg,
               msg_hash_to_str(MSG_REMOVED_DISK_FROM_TRAY),
               sizeof(msg));
   }
   else
   {
      if (idx < num_disks)
         snprintf(msg, sizeof(msg), "%s %u/%u.",
               msg_hash_to_str(MSG_FAILED_TO_SET_DISK),
               idx + 1, num_disks);
      else
         strlcpy(msg,
               msg_hash_to_str(MSG_FAILED_TO_REMOVE_DISK_FROM_TRAY),
               sizeof(msg));
      error = true;
   }

   if (!string_is_empty(msg))
   {
      if (error)
         RARCH_ERR("%s\n", msg);
      else
         RARCH_LOG("%s\n", msg);
      runloop_msg_queue_push(msg, 1, 180, true);
   }
}

/**
 * command_event_disk_control_append_image:
 * @path                 : Path to disk image.
 *
 * Appends disk image to disk image list.
 **/
static bool command_event_disk_control_append_image(const char *path)
{
   unsigned new_idx;
   char msg[128];
   struct retro_game_info info                        = {0};
   const struct retro_disk_control_callback *control  = NULL;
   rarch_system_info_t *sysinfo                      = runloop_get_system_info();

   msg[0] = '\0';

   if (sysinfo)
      control = (const struct retro_disk_control_callback*)
         &sysinfo->disk_control_cb;

   if (!control)
      return false;

   command_event_disk_control_set_eject(true, false);

   control->add_image_index();
   new_idx = control->get_num_images();
   if (!new_idx)
      return false;
   new_idx--;

   info.path = path;
   control->replace_image_index(new_idx, &info);

   snprintf(msg, sizeof(msg), "%s: ", msg_hash_to_str(MSG_APPENDED_DISK));
   strlcat(msg, path, sizeof(msg));
   RARCH_LOG("%s\n", msg);
   runloop_msg_queue_push(msg, 0, 180, true);

   command_event(CMD_EVENT_AUTOSAVE_DEINIT, NULL);

   /* TODO: Need to figure out what to do with subsystems case. */
   if (path_is_empty(RARCH_PATH_SUBSYSTEM))
   {
      /* Update paths for our new image.
       * If we actually use append_image, we assume that we
       * started out in a single disk case, and that this way
       * of doing it makes the most sense. */
      path_set(RARCH_PATH_NAMES, path);
      path_fill_names();
   }

   command_event(CMD_EVENT_AUTOSAVE_INIT, NULL);
   command_event_disk_control_set_index(new_idx);
   command_event_disk_control_set_eject(false, false);

   return true;
}

/**
 * command_event_check_disk_prev:
 * @control              : Handle to disk control handle.
 *
 * Perform disk cycle to previous index action (Core Disk Options).
 **/
static void command_event_check_disk_prev(
      const struct retro_disk_control_callback *control)
{
   unsigned num_disks    = 0;
   unsigned current      = 0;
   bool disk_prev_enable = false;

   if (!control || !control->get_num_images)
      return;
   if (!control->get_image_index)
      return;

   num_disks        = control->get_num_images();
   current          = control->get_image_index();
   disk_prev_enable = num_disks && num_disks != UINT_MAX;

   if (!disk_prev_enable)
   {
      RARCH_ERR("%s.\n", msg_hash_to_str(MSG_GOT_INVALID_DISK_INDEX));
      return;
   }

   if (current > 0)
      current--;
   command_event_disk_control_set_index(current);
}

/**
 * command_event_check_disk_next:
 * @control              : Handle to disk control handle.
 *
 * Perform disk cycle to next index action (Core Disk Options).
 **/
static void command_event_check_disk_next(
      const struct retro_disk_control_callback *control)
{
   unsigned num_disks        = 0;
   unsigned current          = 0;
   bool     disk_next_enable = false;

   if (!control || !control->get_num_images)
      return;
   if (!control->get_image_index)
      return;

   num_disks        = control->get_num_images();
   current          = control->get_image_index();
   disk_next_enable = num_disks && num_disks != UINT_MAX;

   if (!disk_next_enable)
   {
      RARCH_ERR("%s.\n", msg_hash_to_str(MSG_GOT_INVALID_DISK_INDEX));
      return;
   }

   if (current < num_disks - 1)
      current++;
   command_event_disk_control_set_index(current);
}

/**
 * event_set_volume:
 * @gain      : amount of gain to be applied to current volume level.
 *
 * Adjusts the current audio volume level.
 *
 **/
static void command_event_set_volume(float gain)
{
   char msg[128];
   settings_t *settings      = config_get_ptr();
   float new_volume          = settings->floats.audio_volume + gain;

   new_volume                = MAX(new_volume, -80.0f);
   new_volume                = MIN(new_volume, 12.0f);

   configuration_set_float(settings, settings->floats.audio_volume, new_volume);

   snprintf(msg, sizeof(msg), "%s: %.1f dB",
         msg_hash_to_str(MSG_AUDIO_VOLUME),
         new_volume);
   runloop_msg_queue_push(msg, 1, 180, true);
   RARCH_LOG("%s\n", msg);

   audio_set_float(AUDIO_ACTION_VOLUME_GAIN, new_volume);
}

/**
 * event_set_mixer_volume:
 * @gain      : amount of gain to be applied to current volume level.
 *
 * Adjusts the current audio volume level.
 *
 **/
static void command_event_set_mixer_volume(float gain)
{
   char msg[128];
   settings_t *settings      = config_get_ptr();
   float new_volume          = settings->floats.audio_mixer_volume + gain;

   new_volume                = MAX(new_volume, -80.0f);
   new_volume                = MIN(new_volume, 12.0f);

   configuration_set_float(settings, settings->floats.audio_mixer_volume, new_volume);

   snprintf(msg, sizeof(msg), "%s: %.1f dB",
         msg_hash_to_str(MSG_AUDIO_VOLUME),
         new_volume);
   runloop_msg_queue_push(msg, 1, 180, true);
   RARCH_LOG("%s\n", msg);

   audio_set_float(AUDIO_ACTION_VOLUME_GAIN, new_volume);
}

/**
 * command_event_init_controllers:
 *
 * Initialize libretro controllers.
 **/
static void command_event_init_controllers(void)
{
   unsigned i;
   rarch_system_info_t *info = runloop_get_system_info();

   for (i = 0; i < MAX_USERS; i++)
   {
      retro_ctx_controller_info_t pad;
      const char *ident                               = NULL;
      bool set_controller                             = false;
      const struct retro_controller_description *desc = NULL;
      unsigned device                                 = input_config_get_device(i);

      if (info)
      {
         if (i < info->ports.size)
            desc = libretro_find_controller_description(
                  &info->ports.data[i], device);
      }

      if (desc)
         ident = desc->desc;

      if (!ident)
      {
         /* If we're trying to connect a completely unknown device,
          * revert back to JOYPAD. */

         if (device != RETRO_DEVICE_JOYPAD && device != RETRO_DEVICE_NONE)
         {
            /* Do not fix device,
             * because any use of dummy core will reset this,
             * which is not a good idea. */
            RARCH_WARN("Input device ID %u is unknown to this "
                  "libretro implementation. Using RETRO_DEVICE_JOYPAD.\n",
                  device);
            device = RETRO_DEVICE_JOYPAD;
         }
         ident = "Joypad";
      }

      switch (device)
      {
         case RETRO_DEVICE_NONE:
            RARCH_LOG("%s %u.\n",
                  msg_hash_to_str(MSG_VALUE_DISCONNECTING_DEVICE_FROM_PORT),
                  i + 1);
            set_controller = true;
            break;
         case RETRO_DEVICE_JOYPAD:
            break;
         default:
            /* Some cores do not properly range check port argument.
             * This is broken behavior of course, but avoid breaking
             * cores needlessly. */
            RARCH_LOG("%s %u: %s (ID: %u).\n",
                    msg_hash_to_str(MSG_CONNECTING_TO_PORT),
                    device, ident, i+1);
            set_controller = true;
            break;
      }

      if (set_controller)
      {
         pad.device     = device;
         pad.port       = i;
         core_set_controller_port_device(&pad);
      }
   }
}

static void command_event_deinit_core(bool reinit)
{
#ifdef HAVE_CHEEVOS
   cheevos_unload();
#endif

   core_unload_game();
   core_unload();
   core_uninit_symbols();

   if (reinit)
      driver_uninit(DRIVERS_CMD_ALL);

   command_event(CMD_EVENT_DISABLE_OVERRIDES, NULL);
   command_event(CMD_EVENT_RESTORE_DEFAULT_SHADER_PRESET, NULL);
}

static void command_event_init_cheats(void)
{
   bool allow_cheats = true;
#ifdef HAVE_NETWORKING
   allow_cheats &= !netplay_driver_ctl(
         RARCH_NETPLAY_CTL_IS_DATA_INITED, NULL);
#endif
   allow_cheats &= !bsv_movie_ctl(BSV_MOVIE_CTL_IS_INITED, NULL);

   if (!allow_cheats)
      return;

   /* TODO/FIXME - add some stuff here. */
}

static void command_event_load_auto_state(void)
{
   bool ret;
   char msg[128]                             = {0};
   char savestate_name_auto[PATH_MAX_LENGTH] = {0};
   settings_t *settings                      = config_get_ptr();
   global_t   *global                        = global_get_ptr();

#ifdef HAVE_NETWORKING
   if (netplay_driver_ctl(RARCH_NETPLAY_CTL_IS_ENABLED, NULL))
      return;
#endif

#ifdef HAVE_CHEEVOS
   if (settings->bools.cheevos_hardcore_mode_enable)
      return;
#endif

   if (!settings->bools.savestate_auto_load)
      return;

   if (global)
      fill_pathname_noext(savestate_name_auto, global->name.savestate,
            file_path_str(FILE_PATH_AUTO_EXTENSION),
            sizeof(savestate_name_auto));

   if (!path_file_exists(savestate_name_auto))
      return;

   ret = content_load_state(savestate_name_auto, false, true);

   RARCH_LOG("%s: %s\n", msg_hash_to_str(MSG_FOUND_AUTO_SAVESTATE_IN),
         savestate_name_auto);

   snprintf(msg, sizeof(msg), "%s \"%s\" %s.",
         msg_hash_to_str(MSG_AUTOLOADING_SAVESTATE_FROM),
         savestate_name_auto, ret ? "succeeded" : "failed");
   RARCH_LOG("%s\n", msg);
}

static void command_event_set_savestate_auto_index(void)
{
   size_t i;
   char state_dir[PATH_MAX_LENGTH]  = {0};
   char state_base[PATH_MAX_LENGTH] = {0};
   struct string_list *dir_list     = NULL;
   unsigned max_idx                 = 0;
   settings_t *settings             = config_get_ptr();
   global_t   *global               = global_get_ptr();

   if (!settings->bools.savestate_auto_index)
      return;

   if (global)
   {
      /* Find the file in the same directory as global->savestate_name
       * with the largest numeral suffix.
       *
       * E.g. /foo/path/content.state, will try to find
       * /foo/path/content.state%d, where %d is the largest number available.
       */
      fill_pathname_basedir(state_dir, global->name.savestate,
            sizeof(state_dir));
      fill_pathname_base(state_base, global->name.savestate,
            sizeof(state_base));
   }

   dir_list = dir_list_new_special(state_dir, DIR_LIST_PLAIN, NULL);

   if (!dir_list)
      return;

   for (i = 0; i < dir_list->size; i++)
   {
      unsigned idx;
      char elem_base[128]             = {0};
      const char *end                 = NULL;
      const char *dir_elem            = dir_list->elems[i].data;

      fill_pathname_base(elem_base, dir_elem, sizeof(elem_base));

      if (strstr(elem_base, state_base) != elem_base)
         continue;

      end = dir_elem + strlen(dir_elem);
      while ((end > dir_elem) && isdigit((int)end[-1]))
         end--;

      idx = (unsigned)strtoul(end, NULL, 0);
      if (idx > max_idx)
         max_idx = idx;
   }

   dir_list_free(dir_list);

   configuration_set_int(settings, settings->ints.state_slot, max_idx);

   RARCH_LOG("%s: #%d\n",
         msg_hash_to_str(MSG_FOUND_LAST_STATE_SLOT),
         max_idx);
}

static bool event_init_content(void)
{
   bool contentless = false;
   bool is_inited   = false;

   content_get_status(&contentless, &is_inited);

   rarch_ctl(RARCH_CTL_SET_SRAM_ENABLE, NULL);

   /* No content to be loaded for dummy core,
    * just successfully exit. */
   if (rarch_ctl(RARCH_CTL_IS_DUMMY_CORE, NULL))
      return true;

   if (!contentless)
      path_fill_names();

   if (!content_init())
      return false;

   content_get_status(&contentless, &is_inited);

   command_event_set_savestate_auto_index();

   if (event_load_save_files())
      RARCH_LOG("%s.\n",
            msg_hash_to_str(MSG_SKIPPING_SRAM_LOAD));

   command_event_load_auto_state();
   command_event(CMD_EVENT_BSV_MOVIE_INIT, NULL);
   command_event(CMD_EVENT_NETPLAY_INIT, NULL);

   return true;
}

static bool command_event_init_core(enum rarch_core_type *data)
{
   retro_ctx_environ_info_t info;
   settings_t *settings            = config_get_ptr();

   if (!core_init_symbols(data))
      return false;

   rarch_ctl(RARCH_CTL_SYSTEM_INFO_INIT, NULL);

   /* auto overrides: apply overrides */
   if(settings->bools.auto_overrides_enable)
   {
      if (config_load_override())
         rarch_ctl(RARCH_CTL_SET_OVERRIDES_ACTIVE, NULL);
      else
         rarch_ctl(RARCH_CTL_UNSET_OVERRIDES_ACTIVE, NULL);
   }

   /* Auto-remap: apply shader preset files */
   if(settings->bools.auto_shaders_enable)
      config_load_shader_preset();


   /* reset video format to libretro's default */
   video_driver_set_pixel_format(RETRO_PIXEL_FORMAT_0RGB1555);

   info.env = rarch_environment_cb;
   core_set_environment(&info);

   /* Auto-remap: apply remap files */
   if(settings->bools.auto_remaps_enable)
      config_load_remap();

   /* Per-core saves: reset redirection paths */
   path_set_redirect();

   if (!core_init())
      return false;

   if (!event_init_content())
      return false;

   if (!core_load(settings->uints.input_poll_type_behavior))
      return false;

   rarch_ctl(RARCH_CTL_SET_FRAME_LIMIT, NULL);

   return true;
}

static void command_event_disable_overrides(void)
{
   if (!rarch_ctl(RARCH_CTL_IS_OVERRIDES_ACTIVE, NULL))
      return;

   /* reload the original config */

   config_unload_override();
   rarch_ctl(RARCH_CTL_UNSET_OVERRIDES_ACTIVE, NULL);
}

static void command_event_restore_default_shader_preset(void)
{
   if (!path_is_empty(RARCH_PATH_DEFAULT_SHADER_PRESET))
   {
      /* auto shader preset: reload the original shader */
      settings_t *settings = config_get_ptr();

      RARCH_LOG("%s %s\n",
            msg_hash_to_str(MSG_RESTORING_DEFAULT_SHADER_PRESET_TO),
            path_get(RARCH_PATH_DEFAULT_SHADER_PRESET));
      strlcpy(settings->paths.path_shader,
            path_get(RARCH_PATH_DEFAULT_SHADER_PRESET),
            sizeof(settings->paths.path_shader));
   }

   path_clear(RARCH_PATH_DEFAULT_SHADER_PRESET);
}

static bool command_event_save_auto_state(void)
{
   char savestate_name_auto[PATH_MAX_LENGTH] = {0};
   bool ret             = false;
   bool contentless     = false;
   bool is_inited       = false;
   settings_t *settings = config_get_ptr();
   global_t   *global   = global_get_ptr();

   if (!settings || !settings->bools.savestate_auto_save)
      return false;
   if (!global)
      return false;
   if (rarch_ctl(RARCH_CTL_IS_DUMMY_CORE, NULL))
      return false;

   content_get_status(&contentless, &is_inited);

   if (contentless)
      return false;

#ifdef HAVE_CHEEVOS
   if (settings->bools.cheevos_hardcore_mode_enable)
      return false;
#endif

   fill_pathname_noext(savestate_name_auto, global->name.savestate,
         file_path_str(FILE_PATH_AUTO_EXTENSION),
         sizeof(savestate_name_auto));

   ret = content_save_state((const char*)savestate_name_auto, true, true);
   RARCH_LOG("%s \"%s\" %s.\n",
         msg_hash_to_str(MSG_AUTO_SAVE_STATE_TO),
         savestate_name_auto, ret ?
         "succeeded" : "failed");

   return true;
}

static bool command_event_save_config(const char *config_path,
      char *s, size_t len)
{
   if (string_is_empty(config_path) || !config_save_file(config_path))
   {
      snprintf(s, len, "%s \"%s\".",
            msg_hash_to_str(MSG_FAILED_SAVING_CONFIG_TO),
            path_get(RARCH_PATH_CONFIG));
      RARCH_ERR("%s\n", s);

      return false;
   }

   snprintf(s, len, "[Config]: %s \"%s\".",
         msg_hash_to_str(MSG_SAVED_NEW_CONFIG_TO),
         path_get(RARCH_PATH_CONFIG));
   RARCH_LOG("%s\n", s);
   return true;
}

/**
 * command_event_save_core_config:
 *
 * Saves a new (core) configuration to a file. Filename is based
 * on heuristics to avoid typing.
 *
 * Returns: true (1) on success, otherwise false (0).
 **/
static bool command_event_save_core_config(void)
{
   char config_dir[PATH_MAX_LENGTH];
   char config_name[PATH_MAX_LENGTH];
   char config_path[PATH_MAX_LENGTH];
   char msg[128];
   bool ret                          = false;
   bool found_path                   = false;
   bool overrides_active             = false;
   settings_t *settings              = config_get_ptr();

   config_dir[0]  = config_name[0]   =
   config_path[0] = msg[0]           = '\0';

   if (!string_is_empty(settings->paths.directory_menu_config))
      strlcpy(config_dir, settings->paths.directory_menu_config,
            sizeof(config_dir));
   else if (!path_is_empty(RARCH_PATH_CONFIG)) /* Fallback */
      fill_pathname_basedir(config_dir, path_get(RARCH_PATH_CONFIG),
            sizeof(config_dir));
   else
   {
      runloop_msg_queue_push(msg_hash_to_str(MSG_CONFIG_DIRECTORY_NOT_SET), 1, 180, true);
      RARCH_ERR("[Config]: %s\n", msg_hash_to_str(MSG_CONFIG_DIRECTORY_NOT_SET));
      return false;
   }

   /* Infer file name based on libretro core. */
   if (!string_is_empty(path_get(RARCH_PATH_CORE)) && path_file_exists(path_get(RARCH_PATH_CORE)))
   {
      unsigned i;
      RARCH_LOG("%s\n", msg_hash_to_str(MSG_USING_CORE_NAME_FOR_NEW_CONFIG));

      /* In case of collision, find an alternative name. */
      for (i = 0; i < 16; i++)
      {
         char tmp[64] = {0};

         fill_pathname_base_noext(
               config_name,
               path_get(RARCH_PATH_CORE),
               sizeof(config_name));

         fill_pathname_join(config_path, config_dir, config_name,
               sizeof(config_path));

         if (i)
            snprintf(tmp, sizeof(tmp), "-%u%s",
                  i,
                  file_path_str(FILE_PATH_CONFIG_EXTENSION));
         else
            strlcpy(tmp,
                  file_path_str(FILE_PATH_CONFIG_EXTENSION),
                  sizeof(tmp));

         strlcat(config_path, tmp, sizeof(config_path));
         if (!path_file_exists(config_path))
         {
            found_path = true;
            break;
         }
      }
   }

   if (!found_path)
   {
      /* Fallback to system time... */
      RARCH_WARN("[Config]: %s\n",
            msg_hash_to_str(MSG_CANNOT_INFER_NEW_CONFIG_PATH));
      fill_dated_filename(config_name,
            file_path_str(FILE_PATH_CONFIG_EXTENSION),
            sizeof(config_name));
      fill_pathname_join(config_path, config_dir, config_name,
            sizeof(config_path));
   }

   if (rarch_ctl(RARCH_CTL_IS_OVERRIDES_ACTIVE, NULL))
   {
      /* Overrides block config file saving,
       * make it appear as overrides weren't enabled
       * for a manual save. */
      rarch_ctl(RARCH_CTL_UNSET_OVERRIDES_ACTIVE, NULL);
      overrides_active = true;
   }

   command_event_save_config(config_path, msg, sizeof(msg));

   runloop_msg_queue_push(msg, 1, 180, true);

   if (overrides_active)
      rarch_ctl(RARCH_CTL_SET_OVERRIDES_ACTIVE, NULL);
   else
      rarch_ctl(RARCH_CTL_UNSET_OVERRIDES_ACTIVE, NULL);
   return ret;
}

/**
 * event_save_current_config:
 *
 * Saves current configuration file to disk, and (optionally)
 * autosave state.
 **/
static void command_event_save_current_config(enum override_type type)
{
   char msg[128];

   msg[0] = '\0';

   switch (type)
   {
      case OVERRIDE_NONE:
         if (path_is_empty(RARCH_PATH_CONFIG))
            strlcpy(msg, "[Config]: Config directory not set, cannot save configuration.",
                  sizeof(msg));
         else
            command_event_save_config(path_get(RARCH_PATH_CONFIG), msg, sizeof(msg));
         break;
      case OVERRIDE_GAME:
      case OVERRIDE_CORE:
         if (config_save_overrides(type))
         {
            strlcpy(msg, msg_hash_to_str(MSG_OVERRIDES_SAVED_SUCCESSFULLY), sizeof(msg));
            RARCH_LOG("[Config]: [overrides] %s\n", msg);

            /* set overrides to active so the original config can be
               restored after closing content */
            rarch_ctl(RARCH_CTL_SET_OVERRIDES_ACTIVE, NULL);
         }
         else
         {
            strlcpy(msg, msg_hash_to_str(MSG_OVERRIDES_ERROR_SAVING), sizeof(msg));
            RARCH_ERR("[Config]: [overrides] %s\n", msg);
         }
         break;
   }


   if (!string_is_empty(msg))
      runloop_msg_queue_push(msg, 1, 180, true);
}

static void command_event_undo_save_state(char *s, size_t len)
{
   if (content_undo_save_buf_is_empty())
   {
      strlcpy(s,
         msg_hash_to_str(MSG_NO_SAVE_STATE_HAS_BEEN_OVERWRITTEN_YET), len);
      return;
   }

   if (!content_undo_save_state())
      return;
}

static void command_event_undo_load_state(char *s, size_t len)
{

   if (content_undo_load_buf_is_empty())
   {
      strlcpy(s,
         msg_hash_to_str(MSG_NO_STATE_HAS_BEEN_LOADED_YET),
         len);
      return;
   }

   if (!content_undo_load_state())
   {
      snprintf(s, len, "%s \"%s\".",
            msg_hash_to_str(MSG_FAILED_TO_UNDO_LOAD_STATE),
            "RAM");
      return;
   }

#ifdef HAVE_NETWORKING
   netplay_driver_ctl(RARCH_NETPLAY_CTL_LOAD_SAVESTATE, NULL);
#endif

   strlcpy(s,
         msg_hash_to_str(MSG_UNDID_LOAD_STATE), len);
}

static bool command_event_main_state(unsigned cmd)
{
   retro_ctx_size_info_t info;
   char path[PATH_MAX_LENGTH];
   char msg[128];
   bool ret                   = false;
   global_t *global           = global_get_ptr();
   bool push_msg              = true;

   path[0] = msg[0]           = '\0';

   if (global)
   {
      settings_t *settings    = config_get_ptr();
      int state_slot          = settings->ints.state_slot;

      if (state_slot > 0)
         snprintf(path, sizeof(path), "%s%d",
               global->name.savestate, state_slot);
      else if (state_slot < 0)
         fill_pathname_join_delim(path,
               global->name.savestate, "auto", '.', sizeof(path));
      else
         strlcpy(path, global->name.savestate, sizeof(path));
   }

   core_serialize_size(&info);

   if (info.size)
   {
      switch (cmd)
      {
         case CMD_EVENT_SAVE_STATE:
            content_save_state(path, true, false);
            ret      = true;
            push_msg = false;
            break;
         case CMD_EVENT_LOAD_STATE:
            if (content_load_state(path, false, false))
            {
               ret = true;
#ifdef HAVE_NETWORKING
               netplay_driver_ctl(RARCH_NETPLAY_CTL_LOAD_SAVESTATE, NULL);
#endif
            }
            push_msg = false;
            break;
         case CMD_EVENT_UNDO_LOAD_STATE:
            command_event_undo_load_state(msg, sizeof(msg));
            ret = true;
            break;
         case CMD_EVENT_UNDO_SAVE_STATE:
            command_event_undo_save_state(msg, sizeof(msg));
            ret = true;
            break;
      }
   }
   else
      strlcpy(msg, msg_hash_to_str(
               MSG_CORE_DOES_NOT_SUPPORT_SAVESTATES), sizeof(msg));

   if (push_msg)
      runloop_msg_queue_push(msg, 2, 180, true);
   RARCH_LOG("%s\n", msg);

   return ret;
}

static bool command_event_resize_windowed_scale(void)
{
   unsigned idx           = 0;
   unsigned *window_scale = NULL;
   settings_t *settings   = config_get_ptr();

   if (rarch_ctl(RARCH_CTL_GET_WINDOWED_SCALE, &window_scale))
   {
      if (!window_scale || *window_scale == 0)
         return false;

      configuration_set_float(settings, settings->floats.video_scale, *window_scale);
   }

   if (!settings->bools.video_fullscreen)
      command_event(CMD_EVENT_REINIT, NULL);

   rarch_ctl(RARCH_CTL_SET_WINDOWED_SCALE, &idx);

   return true;
}

/**
 * command_event:
 * @cmd                  : Event command index.
 *
 * Performs program event command with index @cmd.
 *
 * Returns: true (1) on success, otherwise false (0).
 **/
bool command_event(enum event_command cmd, void *data)
{
   bool boolean              = false;

   switch (cmd)
   {
      case CMD_EVENT_MENU_REFRESH:
#ifdef HAVE_MENU
         menu_driver_ctl(RARCH_MENU_CTL_REFRESH, NULL);
#endif
         break;
      case CMD_EVENT_SET_PER_GAME_RESOLUTION:
#if defined(GEKKO)
         {
            unsigned width = 0, height = 0;

            command_event(CMD_EVENT_VIDEO_SET_ASPECT_RATIO, NULL);

            if (video_driver_get_video_output_size(&width, &height))
            {
               char msg[128] = {0};

               video_driver_set_video_mode(width, height, true);

               if (width == 0 || height == 0)
                  snprintf(msg, sizeof(msg), "%s: DEFAULT",
                        msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SCREEN_RESOLUTION));
               else
                  snprintf(msg, sizeof(msg),"%s: %dx%d",
                        msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SCREEN_RESOLUTION),
                        width, height);
               runloop_msg_queue_push(msg, 1, 100, true);
            }
         }
#endif
         break;
      case CMD_EVENT_LOAD_CORE_PERSIST:
         {
#ifdef HAVE_MENU
            core_info_ctx_find_t info_find;
            rarch_system_info_t *system_info = runloop_get_system_info();
            struct retro_system_info *system = &system_info->info;

#if defined(HAVE_DYNAMIC)
            if (string_is_empty(path_get(RARCH_PATH_CORE)))
               return false;
#endif
            libretro_get_system_info(
                  path_get(RARCH_PATH_CORE),
                  system,
                  &system_info->load_no_content);
            info_find.path = path_get(RARCH_PATH_CORE);

            if (!core_info_load(&info_find))
            {
#ifdef HAVE_DYNAMIC
               return false;
#endif
            }
#endif
         }
         break;
      case CMD_EVENT_LOAD_CORE:
         command_event(CMD_EVENT_LOAD_CORE_PERSIST, NULL);
#ifndef HAVE_DYNAMIC
         command_event(CMD_EVENT_QUIT, NULL);
#endif
         break;
      case CMD_EVENT_LOAD_STATE:
         /* Immutable - disallow savestate load when
          * we absolutely cannot change game state. */
         if (bsv_movie_ctl(BSV_MOVIE_CTL_IS_INITED, NULL))
            return false;

#ifdef HAVE_CHEEVOS
         {
            settings_t *settings      = config_get_ptr();
            if (settings->bools.cheevos_hardcore_mode_enable)
               return false;
         }
#endif

         return command_event_main_state(cmd);
      case CMD_EVENT_UNDO_LOAD_STATE:
         return command_event_main_state(cmd);
      case CMD_EVENT_UNDO_SAVE_STATE:
         return command_event_main_state(cmd);
      case CMD_EVENT_RESIZE_WINDOWED_SCALE:
         return command_event_resize_windowed_scale();
      case CMD_EVENT_MENU_TOGGLE:
#ifdef HAVE_MENU
         if (menu_driver_is_alive())
            rarch_menu_running_finished();
         else
            rarch_menu_running();
#endif
         break;
      case CMD_EVENT_CONTROLLERS_INIT:
         command_event_init_controllers();
         break;
      case CMD_EVENT_RESET:
         RARCH_LOG("%s.\n", msg_hash_to_str(MSG_RESET));
         runloop_msg_queue_push(msg_hash_to_str(MSG_RESET), 1, 120, true);

#ifdef HAVE_CHEEVOS
         cheevos_set_cheats();
#endif
         core_reset();
#ifdef HAVE_CHEEVOS
         cheevos_reset_game();
#endif
#if HAVE_NETWORKING
         netplay_driver_ctl(RARCH_NETPLAY_CTL_RESET, NULL);
#endif
         break;
      case CMD_EVENT_SAVE_STATE:
         {
            settings_t *settings      = config_get_ptr();
#ifdef HAVE_CHEEVOS
            if (settings->bools.cheevos_hardcore_mode_enable)
               return false;
#endif

            if (settings->bools.savestate_auto_index)
            {
               int new_state_slot = settings->ints.state_slot + 1;
               configuration_set_int(settings, settings->ints.state_slot, new_state_slot);
            }
         }
         return command_event_main_state(cmd);
      case CMD_EVENT_SAVE_STATE_DECREMENT:
         {
            settings_t *settings      = config_get_ptr();
            /* Slot -1 is (auto) slot. */
            if (settings->ints.state_slot >= 0)
            {
               int new_state_slot = settings->ints.state_slot - 1;
               configuration_set_int(settings, settings->ints.state_slot, new_state_slot);
            }
         }
         break;
      case CMD_EVENT_SAVE_STATE_INCREMENT:
         {
            settings_t *settings      = config_get_ptr();
            int new_state_slot        = settings->ints.state_slot + 1;
            configuration_set_int(settings, settings->ints.state_slot, new_state_slot);
         }
         break;
      case CMD_EVENT_TAKE_SCREENSHOT:
         if (!take_screenshot(path_get(RARCH_PATH_BASENAME), false,
                  video_driver_cached_frame_has_valid_framebuffer()))
            return false;
         break;
      case CMD_EVENT_UNLOAD_CORE:
         {
            bool contentless                = false;
            bool is_inited                  = false;
            content_ctx_info_t content_info = {0};

            content_get_status(&contentless, &is_inited);

            command_event(CMD_EVENT_AUTOSAVE_STATE, NULL);
            command_event(CMD_EVENT_DISABLE_OVERRIDES, NULL);
            command_event(CMD_EVENT_RESTORE_DEFAULT_SHADER_PRESET, NULL);

            if (is_inited)
               if (!task_push_start_dummy_core(&content_info))
                  return false;
#ifdef HAVE_DYNAMIC
            path_clear(RARCH_PATH_CORE);
            rarch_ctl(RARCH_CTL_SYSTEM_INFO_FREE, NULL);
#endif
            core_unload_game();
            if (!rarch_ctl(RARCH_CTL_IS_DUMMY_CORE, NULL))
               core_unload();
         }
         break;
      case CMD_EVENT_QUIT:
         return retroarch_main_quit();
      case CMD_EVENT_CHEEVOS_HARDCORE_MODE_TOGGLE:
#ifdef HAVE_CHEEVOS
         cheevos_toggle_hardcore_mode();
#endif
         break;
      case CMD_EVENT_REINIT:
         video_driver_reinit();
         /* Poll input to avoid possibly stale data to corrupt things. */
         input_driver_poll();
         command_event(CMD_EVENT_GAME_FOCUS_TOGGLE, (void*)(intptr_t)-1);
#ifdef HAVE_MENU
         menu_display_set_framebuffer_dirty_flag();
         if (menu_driver_is_alive())
            command_event(CMD_EVENT_VIDEO_SET_BLOCKING_STATE, NULL);
#endif
         break;
      case CMD_EVENT_CHEATS_DEINIT:
         cheat_manager_state_free();
         break;
      case CMD_EVENT_CHEATS_INIT:
         command_event(CMD_EVENT_CHEATS_DEINIT, NULL);
         command_event_init_cheats();
         break;
      case CMD_EVENT_CHEATS_APPLY:
         cheat_manager_apply_cheats();
         break;
      case CMD_EVENT_REWIND_DEINIT:
         {
#ifdef HAVE_CHEEVOS
            settings_t *settings      = config_get_ptr();
            if (settings->bools.cheevos_hardcore_mode_enable)
               return false;
#endif

            state_manager_event_deinit();
         }
         break;
      case CMD_EVENT_REWIND_INIT:
         {
            settings_t *settings      = config_get_ptr();
#ifdef HAVE_CHEEVOS
            if (settings->bools.cheevos_hardcore_mode_enable)
               return false;
#endif

            if (settings->bools.rewind_enable)
               state_manager_event_init((unsigned)settings->rewind_buffer_size);
         }
         break;
      case CMD_EVENT_REWIND_TOGGLE:
         {
            settings_t *settings      = config_get_ptr();
            if (settings->bools.rewind_enable)
               command_event(CMD_EVENT_REWIND_INIT, NULL);
            else
               command_event(CMD_EVENT_REWIND_DEINIT, NULL);
         }
         break;
      case CMD_EVENT_AUTOSAVE_DEINIT:
#ifdef HAVE_THREADS
         if (!rarch_ctl(RARCH_CTL_IS_SRAM_USED, NULL))
            return false;
         autosave_deinit();
#endif
         break;
      case CMD_EVENT_AUTOSAVE_INIT:
         command_event(CMD_EVENT_AUTOSAVE_DEINIT, NULL);
#ifdef HAVE_THREADS
         if (autosave_init())
            runloop_set(RUNLOOP_ACTION_AUTOSAVE);
         else
            runloop_unset(RUNLOOP_ACTION_AUTOSAVE);
#endif
         break;
      case CMD_EVENT_AUTOSAVE_STATE:
         command_event_save_auto_state();
         break;
      case CMD_EVENT_AUDIO_STOP:
         return audio_driver_stop();
      case CMD_EVENT_AUDIO_START:
         return audio_driver_start(rarch_ctl(RARCH_CTL_IS_SHUTDOWN, NULL));
      case CMD_EVENT_AUDIO_MUTE_TOGGLE:
         {
            bool audio_mute_enable    = *(audio_get_bool_ptr(AUDIO_ACTION_MUTE_ENABLE));
            const char *msg           = !audio_mute_enable ?
               msg_hash_to_str(MSG_AUDIO_MUTED):
               msg_hash_to_str(MSG_AUDIO_UNMUTED);

            if (!audio_driver_toggle_mute())
            {
               RARCH_ERR("%s.\n",
                     msg_hash_to_str(MSG_FAILED_TO_UNMUTE_AUDIO));
               return false;
            }

            runloop_msg_queue_push(msg, 1, 180, true);
            RARCH_LOG("%s\n", msg);
         }
         break;
      case CMD_EVENT_OVERLAY_DEINIT:
#ifdef HAVE_OVERLAY
         input_overlay_free(overlay_ptr);
         overlay_ptr = NULL;
#endif
         break;
      case CMD_EVENT_OVERLAY_INIT:
         {
            settings_t *settings      = config_get_ptr();
            command_event(CMD_EVENT_OVERLAY_DEINIT, NULL);
#ifdef HAVE_OVERLAY
            if (settings->bools.input_overlay_enable)
               task_push_overlay_load_default(input_overlay_loaded, NULL);
#endif
         }
         break;
      case CMD_EVENT_OVERLAY_NEXT:
         {
            settings_t *settings      = config_get_ptr();
#ifdef HAVE_OVERLAY
            input_overlay_next(overlay_ptr, settings->floats.input_overlay_opacity);
#endif
         }
         break;
      case CMD_EVENT_DSP_FILTER_DEINIT:
         audio_driver_dsp_filter_free();
         break;
      case CMD_EVENT_DSP_FILTER_INIT:
         {
            settings_t *settings      = config_get_ptr();
            command_event(CMD_EVENT_DSP_FILTER_DEINIT, NULL);
            if (string_is_empty(settings->paths.path_audio_dsp_plugin))
               break;
            audio_driver_dsp_filter_init(settings->paths.path_audio_dsp_plugin);
         }
         break;
      case CMD_EVENT_GPU_RECORD_DEINIT:
         video_driver_gpu_record_deinit();
         break;
      case CMD_EVENT_RECORD_DEINIT:
         if (!recording_deinit())
            return false;
         break;
      case CMD_EVENT_RECORD_INIT:
         command_event(CMD_EVENT_HISTORY_DEINIT, NULL);
         if (!recording_init())
            return false;
         break;
      case CMD_EVENT_HISTORY_DEINIT:
         if (g_defaults.content_history)
         {
            playlist_write_file(g_defaults.content_history);
            playlist_free(g_defaults.content_history);
         }
         g_defaults.content_history = NULL;

         if (g_defaults.music_history)
         {
            playlist_write_file(g_defaults.music_history);
            playlist_free(g_defaults.music_history);
         }
         g_defaults.music_history = NULL;

#ifdef HAVE_FFMPEG
         if (g_defaults.video_history)
         {
            playlist_write_file(g_defaults.video_history);
            playlist_free(g_defaults.video_history);
         }
         g_defaults.video_history = NULL;

#endif

#ifdef HAVE_IMAGEVIEWER
         if (g_defaults.image_history)
         {
            playlist_write_file(g_defaults.image_history);
            playlist_free(g_defaults.image_history);
         }
         g_defaults.image_history = NULL;
#endif
         break;
      case CMD_EVENT_HISTORY_INIT:
         {
            settings_t *settings          = config_get_ptr();
            unsigned content_history_size = settings->uints.content_history_size;

            command_event(CMD_EVENT_HISTORY_DEINIT, NULL);

            if (!settings->bools.history_list_enable)
               return false;

            RARCH_LOG("%s: [%s].\n",
                  msg_hash_to_str(MSG_LOADING_HISTORY_FILE),
                  settings->paths.path_content_history);
            g_defaults.content_history = playlist_init(
                  settings->paths.path_content_history,
                  content_history_size);

            RARCH_LOG("%s: [%s].\n",
                  msg_hash_to_str(MSG_LOADING_HISTORY_FILE),
                  settings->paths.path_content_music_history);
            g_defaults.music_history = playlist_init(
                  settings->paths.path_content_music_history,
                  content_history_size);

#ifdef HAVE_FFMPEG
            RARCH_LOG("%s: [%s].\n",
                  msg_hash_to_str(MSG_LOADING_HISTORY_FILE),
                  settings->paths.path_content_video_history);
            g_defaults.video_history = playlist_init(
                  settings->paths.path_content_video_history,
                  content_history_size);
#endif

#ifdef HAVE_IMAGEVIEWER
            RARCH_LOG("%s: [%s].\n",
                  msg_hash_to_str(MSG_LOADING_HISTORY_FILE),
                  settings->paths.path_content_image_history);
            g_defaults.image_history = playlist_init(
                  settings->paths.path_content_image_history,
                  content_history_size);
#endif
         }
         break;
      case CMD_EVENT_CORE_INFO_DEINIT:
         core_info_deinit_list();
         break;
      case CMD_EVENT_CORE_INFO_INIT:
         {
            settings_t *settings      = config_get_ptr();
            command_event(CMD_EVENT_CORE_INFO_DEINIT, NULL);

            if (!string_is_empty(settings->paths.directory_libretro))
               core_info_init_list();
         }
         break;
      case CMD_EVENT_CORE_DEINIT:
         {
            struct retro_hw_render_callback *hwr = NULL;
            content_reset_savestate_backups();
            hwr = video_driver_get_hw_context();
            command_event_deinit_core(true);

            if (hwr)
               memset(hwr, 0, sizeof(*hwr));

            break;
         }
      case CMD_EVENT_CORE_INIT:
         content_reset_savestate_backups();
         if (!command_event_init_core((enum rarch_core_type*)data))
            return false;
         break;
      case CMD_EVENT_VIDEO_APPLY_STATE_CHANGES:
         video_driver_apply_state_changes();
         break;
      case CMD_EVENT_VIDEO_SET_NONBLOCKING_STATE:
         boolean = true; /* fall-through */
      case CMD_EVENT_VIDEO_SET_BLOCKING_STATE:
         video_driver_set_nonblock_state(boolean);
         break;
      case CMD_EVENT_VIDEO_SET_ASPECT_RATIO:
         video_driver_set_aspect_ratio();
         break;
      case CMD_EVENT_AUDIO_SET_NONBLOCKING_STATE:
         boolean = true; /* fall-through */
      case CMD_EVENT_AUDIO_SET_BLOCKING_STATE:
         audio_driver_set_nonblocking_state(boolean);
         break;
      case CMD_EVENT_OVERLAY_SET_SCALE_FACTOR:
         {
#ifdef HAVE_OVERLAY
            settings_t *settings      = config_get_ptr();
            input_overlay_set_scale_factor(overlay_ptr, settings->floats.input_overlay_scale);
#endif
         }
         break;
      case CMD_EVENT_OVERLAY_SET_ALPHA_MOD:
         {
#ifdef HAVE_OVERLAY
            settings_t *settings      = config_get_ptr();
            input_overlay_set_alpha_mod(overlay_ptr, settings->floats.input_overlay_opacity);
#endif
         }
         break;
      case CMD_EVENT_AUDIO_REINIT:
         {
            driver_uninit(DRIVER_AUDIO_MASK);
            drivers_init(DRIVER_AUDIO_MASK);
         }
         break;
      case CMD_EVENT_RESET_CONTEXT:
         {
            /* RARCH_DRIVER_CTL_UNINIT clears the callback struct so we
             * need to make sure to keep a copy */
            struct retro_hw_render_callback hwr_copy;
            struct retro_hw_render_callback *hwr = video_driver_get_hw_context();
            const struct retro_hw_render_context_negotiation_interface *iface = 
               video_driver_get_context_negotiation_interface();
            memcpy(&hwr_copy, hwr, sizeof(hwr_copy));

            driver_uninit(DRIVERS_CMD_ALL);

            memcpy(hwr, &hwr_copy, sizeof(*hwr));
            video_driver_set_context_negotiation_interface(iface);

            drivers_init(DRIVERS_CMD_ALL);
         }
         break;
      case CMD_EVENT_SHUTDOWN:
#if defined(__linux__) && !defined(ANDROID)
         runloop_msg_queue_push(msg_hash_to_str(MSG_VALUE_SHUTTING_DOWN), 1, 180, true);
         command_event(CMD_EVENT_MENU_SAVE_CURRENT_CONFIG, NULL);
         command_event(CMD_EVENT_QUIT, NULL);
         system("shutdown -P now");
#endif
         break;
      case CMD_EVENT_REBOOT:
#if defined(__linux__) && !defined(ANDROID)
         runloop_msg_queue_push(msg_hash_to_str(MSG_VALUE_REBOOTING), 1, 180, true);
         command_event(CMD_EVENT_MENU_SAVE_CURRENT_CONFIG, NULL);
         command_event(CMD_EVENT_QUIT, NULL);
         system("shutdown -r now");
#endif
         break;
      case CMD_EVENT_RESUME:
         rarch_menu_running_finished();
         if (ui_companion_is_on_foreground())
            ui_companion_driver_toggle();
         break;
      case CMD_EVENT_RESTART_RETROARCH:
         if (!frontend_driver_set_fork(FRONTEND_FORK_RESTART))
            return false;
#ifndef HAVE_DYNAMIC
         command_event(CMD_EVENT_QUIT, NULL);
#endif
         break;
      case CMD_EVENT_MENU_SAVE_CURRENT_CONFIG:
         command_event_save_current_config(OVERRIDE_NONE);
         break;
      case CMD_EVENT_MENU_SAVE_CURRENT_CONFIG_OVERRIDE_CORE:
         command_event_save_current_config(OVERRIDE_CORE);
         break;
      case CMD_EVENT_MENU_SAVE_CURRENT_CONFIG_OVERRIDE_GAME:
         command_event_save_current_config(OVERRIDE_GAME);
         break;
      case CMD_EVENT_MENU_SAVE_CONFIG:
         if (!command_event_save_core_config())
            return false;
         break;
      case CMD_EVENT_SHADERS_APPLY_CHANGES:
#ifdef HAVE_MENU
         menu_shader_manager_apply_changes();
#endif
         break;
      case CMD_EVENT_PAUSE_CHECKS:
         {
            bool is_paused            = false;
            bool is_idle              = false;
            bool is_slowmotion        = false;
            bool is_perfcnt_enable    = false;

            runloop_get_status(&is_paused, &is_idle, &is_slowmotion,
                  &is_perfcnt_enable);

            if (is_paused)
            {
               RARCH_LOG("%s\n", msg_hash_to_str(MSG_PAUSED));
               command_event(CMD_EVENT_AUDIO_STOP, NULL);

               runloop_msg_queue_push(msg_hash_to_str(MSG_PAUSED), 1, 
                     1, true);

               if (!is_idle)
                  video_driver_cached_frame();
            }
            else
            {
               RARCH_LOG("%s\n", msg_hash_to_str(MSG_UNPAUSED));
               command_event(CMD_EVENT_AUDIO_START, NULL);
            }
         }
         break;
      case CMD_EVENT_PAUSE_TOGGLE:
         boolean = rarch_ctl(RARCH_CTL_IS_PAUSED,  NULL);
         boolean = !boolean;
         rarch_ctl(RARCH_CTL_SET_PAUSED, &boolean);
         command_event(CMD_EVENT_PAUSE_CHECKS, NULL);
         break;
      case CMD_EVENT_UNPAUSE:
         boolean = false;

         rarch_ctl(RARCH_CTL_SET_PAUSED, &boolean);
         command_event(CMD_EVENT_PAUSE_CHECKS, NULL);
         break;
      case CMD_EVENT_PAUSE:
         boolean = true;

         rarch_ctl(RARCH_CTL_SET_PAUSED, &boolean);
         command_event(CMD_EVENT_PAUSE_CHECKS, NULL);
         break;
      case CMD_EVENT_MENU_PAUSE_LIBRETRO:
#ifdef HAVE_MENU
         if (menu_driver_is_alive())
         {
            settings_t *settings      = config_get_ptr();
            if (settings->bools.menu_pause_libretro)
               command_event(CMD_EVENT_AUDIO_STOP, NULL);
            else
               command_event(CMD_EVENT_AUDIO_START, NULL);
         }
         else
         {
            settings_t *settings      = config_get_ptr();
            if (settings->bools.menu_pause_libretro)
               command_event(CMD_EVENT_AUDIO_START, NULL);
         }
#endif
         break;
      case CMD_EVENT_SHADER_DIR_DEINIT:
         dir_free_shader();
         break;
      case CMD_EVENT_SHADER_DIR_INIT:
         command_event(CMD_EVENT_SHADER_DIR_DEINIT, NULL);

         if (!dir_init_shader())
            return false;
         break;
      case CMD_EVENT_BSV_MOVIE_DEINIT:
         bsv_movie_deinit();
         break;
      case CMD_EVENT_BSV_MOVIE_INIT:
         command_event(CMD_EVENT_BSV_MOVIE_DEINIT, NULL);
         bsv_movie_init();
         break;
#ifdef HAVE_NETWORKING
      case CMD_EVENT_NETPLAY_DEINIT:
         deinit_netplay();
         break;
      case CMD_EVENT_NETWORK_DEINIT:
         network_deinit();
         break;
      case CMD_EVENT_NETWORK_INIT:
         network_init();
         break;
      /* init netplay manually */
      case CMD_EVENT_NETPLAY_INIT:
         {
            char       *hostname = (char *) data;
            settings_t *settings = config_get_ptr();

            command_event(CMD_EVENT_NETPLAY_DEINIT, NULL);

            if (!init_netplay(NULL, hostname ? hostname : 
               settings->paths.netplay_server,
               settings->uints.netplay_port))
            {
               command_event(CMD_EVENT_NETPLAY_DEINIT, NULL);
               return false;
            }
         }
         break;
      /* init netplay via lobby when content is loaded */
      case CMD_EVENT_NETPLAY_INIT_DIRECT:
         {
            /* buf is expected to be address|port */
            char *buf = (char *)data;
            static struct string_list *hostname = NULL;
            hostname = string_split(buf, "|");

            command_event(CMD_EVENT_NETPLAY_DEINIT, NULL);

            RARCH_LOG("[netplay] connecting to %s:%d\n", 
               hostname->elems[0].data, !string_is_empty(hostname->elems[1].data) 
               ? atoi(hostname->elems[1].data) : 55435);

            if (!init_netplay(NULL, hostname->elems[0].data, 
               !string_is_empty(hostname->elems[1].data) 
               ? atoi(hostname->elems[1].data) : 55435))
            {
               command_event(CMD_EVENT_NETPLAY_DEINIT, NULL);
               string_list_free(hostname);
               return false;
            }

            string_list_free(hostname);
         }
         break;
      /* init netplay via lobby when content is not loaded */
      case CMD_EVENT_NETPLAY_INIT_DIRECT_DEFERRED:
         {
            /* buf is expected to be address|port */
            char *buf = (char *)data;
            static struct string_list *hostname = NULL;
            hostname = string_split(buf, "|");

            command_event(CMD_EVENT_NETPLAY_DEINIT, NULL);

            RARCH_LOG("[netplay] connecting to %s:%d\n", 
               hostname->elems[0].data, !string_is_empty(hostname->elems[1].data) 
               ? atoi(hostname->elems[1].data) : 55435);

            if (!init_netplay_deferred(hostname->elems[0].data,
               !string_is_empty(hostname->elems[1].data) 
               ? atoi(hostname->elems[1].data) : 55435))
            {
               command_event(CMD_EVENT_NETPLAY_DEINIT, NULL);
               string_list_free(hostname);
               return false;
            }

            string_list_free(hostname);
         }
         break;
      case CMD_EVENT_NETPLAY_FLIP_PLAYERS:
         netplay_driver_ctl(RARCH_NETPLAY_CTL_FLIP_PLAYERS, NULL);
         break;
      case CMD_EVENT_NETPLAY_GAME_WATCH:
         netplay_driver_ctl(RARCH_NETPLAY_CTL_GAME_WATCH, NULL);
         break;
#else
      case CMD_EVENT_NETPLAY_DEINIT:
      case CMD_EVENT_NETWORK_DEINIT:
      case CMD_EVENT_NETWORK_INIT:
      case CMD_EVENT_NETPLAY_INIT:
      case CMD_EVENT_NETPLAY_INIT_DIRECT:
      case CMD_EVENT_NETPLAY_INIT_DIRECT_DEFERRED:
      case CMD_EVENT_NETPLAY_FLIP_PLAYERS:
      case CMD_EVENT_NETPLAY_GAME_WATCH:
         return false;
#endif
      case CMD_EVENT_FULLSCREEN_TOGGLE:
         {
            settings_t *settings      = config_get_ptr();
            bool new_fullscreen_state = !settings->bools.video_fullscreen;
            if (!video_driver_has_windowed())
               return false;

            /* If we go fullscreen we drop all drivers and
             * reinitialize to be safe. */
            configuration_set_bool(settings, settings->bools.video_fullscreen,
                  new_fullscreen_state);

            command_event(CMD_EVENT_REINIT, NULL);
            if (settings->bools.video_fullscreen)
               video_driver_hide_mouse();
            else
               video_driver_show_mouse();
         }
         break;
      case CMD_EVENT_COMMAND_DEINIT:
         input_driver_deinit_command();
         break;
      case CMD_EVENT_COMMAND_INIT:
         command_event(CMD_EVENT_COMMAND_DEINIT, NULL);
         input_driver_init_command();
         break;
      case CMD_EVENT_REMOTE_DEINIT:
         input_driver_deinit_remote();
         break;
      case CMD_EVENT_REMOTE_INIT:
         command_event(CMD_EVENT_REMOTE_DEINIT, NULL);
         input_driver_init_remote();
         break;
      case CMD_EVENT_LOG_FILE_DEINIT:
         retro_main_log_file_deinit();
         break;
      case CMD_EVENT_DISK_APPEND_IMAGE:
         {
            const char *path = (const char*)data;
            if (string_is_empty(path))
               return false;
            return command_event_disk_control_append_image(path);
         }
      case CMD_EVENT_DISK_EJECT_TOGGLE:
         {
            rarch_system_info_t *info = runloop_get_system_info();

            if (info && info->disk_control_cb.get_num_images)
            {
               const struct retro_disk_control_callback *control =
                  (const struct retro_disk_control_callback*)
                  &info->disk_control_cb;

               if (control)
               {
                  bool new_state = !control->get_eject_state();
                  command_event_disk_control_set_eject(new_state, true);
               }
            }
            else
               runloop_msg_queue_push(
                     msg_hash_to_str(MSG_CORE_DOES_NOT_SUPPORT_DISK_OPTIONS),
                     1, 120, true);
         }
         break;
      case CMD_EVENT_DISK_NEXT:
         {
            rarch_system_info_t *info = runloop_get_system_info();

            if (info && info->disk_control_cb.get_num_images)
            {
               const struct retro_disk_control_callback *control =
                  (const struct retro_disk_control_callback*)
                  &info->disk_control_cb;

               if (!control)
                  return false;

               if (!control->get_eject_state())
                  return false;

               command_event_check_disk_next(control);
            }
            else
               runloop_msg_queue_push(
                     msg_hash_to_str(MSG_CORE_DOES_NOT_SUPPORT_DISK_OPTIONS),
                     1, 120, true);
         }
         break;
      case CMD_EVENT_DISK_PREV:
         {
            rarch_system_info_t *info = runloop_get_system_info();

            if (info && info->disk_control_cb.get_num_images)
            {
               const struct retro_disk_control_callback *control =
                  (const struct retro_disk_control_callback*)
                  &info->disk_control_cb;

               if (!control)
                  return false;

               if (!control->get_eject_state())
                  return false;

               command_event_check_disk_prev(control);
            }
            else
               runloop_msg_queue_push(
                     msg_hash_to_str(MSG_CORE_DOES_NOT_SUPPORT_DISK_OPTIONS),
                     1, 120, true);
         }
         break;
      case CMD_EVENT_RUMBLE_STOP:
         {
            unsigned i;
            for (i = 0; i < MAX_USERS; i++)
            {
               input_driver_set_rumble_state(i, RETRO_RUMBLE_STRONG, 0);
               input_driver_set_rumble_state(i, RETRO_RUMBLE_WEAK, 0);
            }
         }
         break;
      case CMD_EVENT_GRAB_MOUSE_TOGGLE:
         {
            bool ret = false;
            static bool grab_mouse_state  = false;

            grab_mouse_state = !grab_mouse_state;

            if (grab_mouse_state)
               ret = input_driver_grab_mouse();
            else
               ret = input_driver_ungrab_mouse();

            if (!ret)
               return false;

            RARCH_LOG("%s: %s.\n",
                  msg_hash_to_str(MSG_GRAB_MOUSE_STATE),
                  grab_mouse_state ? "yes" : "no");

            if (grab_mouse_state)
               video_driver_hide_mouse();
            else
               video_driver_show_mouse();
         }
         break;
      case CMD_EVENT_GAME_FOCUS_TOGGLE:
         {
            static bool game_focus_state  = false;
            intptr_t                 mode = (intptr_t)data;
            
            /* mode = -1: restores current game focus state
             * mode =  1: force set game focus, instead of toggling
             * any other: toggle
             */
            if (mode == 1)
                game_focus_state = true;
            else if (mode != -1)
                game_focus_state = !game_focus_state;

            RARCH_LOG("%s: %s.\n",
                  "Game focus is: ",
                  game_focus_state ? "on" : "off");

            if (game_focus_state)
            {
               input_driver_grab_mouse();
               video_driver_hide_mouse();
               input_driver_set_hotkey_block();
               input_driver_keyboard_mapping_set_block(1);
               if (mode != -1)
                  runloop_msg_queue_push(msg_hash_to_str(MSG_GAME_FOCUS_ON),
                        1, 120, true);
            }
            else
            {
               input_driver_ungrab_mouse();
               video_driver_show_mouse();
               input_driver_unset_hotkey_block();
               input_driver_keyboard_mapping_set_block(0);
               if (mode != -1)
                  runloop_msg_queue_push(msg_hash_to_str(MSG_GAME_FOCUS_OFF),
                        1, 120, true);
            }

         }
         break;
      case CMD_EVENT_PERFCNT_REPORT_FRONTEND_LOG:
         rarch_perf_log();
         break;
      case CMD_EVENT_VOLUME_UP:
         command_event_set_volume(0.5f);
         break;
      case CMD_EVENT_VOLUME_DOWN:
         command_event_set_volume(-0.5f);
         break;
      case CMD_EVENT_MIXER_VOLUME_UP:
         command_event_set_mixer_volume(0.5f);
         break;
      case CMD_EVENT_MIXER_VOLUME_DOWN:
         command_event_set_mixer_volume(-0.5f);
         break;
      case CMD_EVENT_SET_FRAME_LIMIT:
         rarch_ctl(RARCH_CTL_SET_FRAME_LIMIT, NULL);
         break;
      case CMD_EVENT_DISABLE_OVERRIDES:
         command_event_disable_overrides();
         break;
      case CMD_EVENT_RESTORE_DEFAULT_SHADER_PRESET:
         command_event_restore_default_shader_preset();
         break;
      case CMD_EVENT_LIBUI_TEST:
#if 0
         libui_main();
#endif
         break;
      case CMD_EVENT_NONE:
         return false;
   }

   return true;
}