/* RetroArch - A frontend for libretro.
 * Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 * Copyright (C) 2011-2017 - Daniel De Matteis
 * Copyright (C) 2012-2015 - Jason Fetters
 * Copyright (C) 2012-2015 - Michael Lelli
 * Copyright (C) 2015-2017 - Andrés Suárez
 *
 * RetroArch is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Found-
 * ation, either version 3 of the License, or (at your option) any later version.
 *
 * RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 * * You should have received a copy of the GNU General Public License along with RetroArch.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/resource.h>

#include <signal.h>
#include <pthread.h>

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#ifdef ANDROID
#include <sys/system_properties.h>
#include <android/log.h>
#ifdef __arm__
#include <machine/cpu-features.h>
#endif
#endif

#include <boolean.h>
#include <retro_dirent.h>
#include <retro_inline.h>
#include <compat/strl.h>
#include <rhash.h>
#include <retro_stat.h>
#include <lists/file_list.h>
#include <file/file_path.h>
#include <streams/file_stream.h>
#include <string/stdstring.h>
#include <queues/task_queue.h>

#include "../frontend.h"
#include "../frontend_driver.h"
#include "../../src/defaults.h"
#include "../../src/retroarch.h"
#include "../../src/verbosity.h"
#include "../../src/paths.h"
#include "platform_linux.h"

#ifdef HAVE_MENU
#include "../../menu/menu_driver.h"
#include "../../menu/menu_entries.h"
#endif

#ifdef ANDROID
enum
{
   /* Internal SDCARD writable */
   INTERNAL_STORAGE_WRITABLE = 1,
   /* Internal SDCARD not writable but the private app dir is */
   INTERNAL_STORAGE_APPDIR_WRITABLE,
   /* Internal SDCARD not writable at all */
   INTERNAL_STORAGE_NOT_WRITABLE
};

struct android_app *g_android;
static pthread_key_t thread_key;
static char screenshot_dir[PATH_MAX_LENGTH];
static char downloads_dir[PATH_MAX_LENGTH];
static char apk_dir[PATH_MAX_LENGTH];
static char app_dir[PATH_MAX_LENGTH];
static bool is_android_tv_device = false;

#else
static const char *proc_apm_path                   = "/proc/apm";
static const char *proc_acpi_battery_path          = "/proc/acpi/battery";
static const char *proc_acpi_sysfs_ac_adapter_path = "/sys/class/power_supply/ACAD";
static const char *proc_acpi_sysfs_battery_path    = "/sys/class/power_supply";
static const char *proc_acpi_ac_adapter_path       = "/proc/acpi/ac_adapter";
#endif

static volatile sig_atomic_t linux_sighandler_quit;

#ifndef HAVE_DYNAMIC
static enum frontend_fork linux_fork_mode = FRONTEND_FORK_NONE;
#endif

int system_property_get(const char *command, const char *args, char *value)
{
   FILE *pipe;
   int length                   = 0;
   char buffer[PATH_MAX_LENGTH] = {0};
   char cmd[PATH_MAX_LENGTH]    = {0};
   char *curpos                 = NULL;
   snprintf(cmd, sizeof(cmd), "%s %s", command, args);
   pipe = popen(cmd, "r");

   if (!pipe)
      goto error;

   curpos = value;

   while (!feof(pipe))
   {
      if (fgets(buffer, 128, pipe) != NULL)
      {
         int curlen = strlen(buffer);
         memcpy(curpos, buffer, curlen);
         curpos    += curlen;
         length    += curlen;
      }
   }

   *curpos = '\0';
   pclose(pipe);
   return length;

error:
   RARCH_ERR("Could not create pipe.\n");
   return 0;
}

/* forward declaration */
bool android_run_events(void *data);

void android_app_write_cmd(struct android_app *android_app, int8_t cmd)
{
   if (!android_app)
      return;

   if (write(android_app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd))
      RARCH_ERR("Failure writing android_app cmd: %s\n", strerror(errno));
}

static void android_app_set_input(struct android_app *android_app, AInputQueue* inputQueue)
{
   if (!android_app)
      return;

   slock_lock(android_app->mutex);
   android_app->pendingInputQueue = inputQueue;
   android_app_write_cmd(android_app, APP_CMD_INPUT_CHANGED);

   while (android_app->inputQueue != android_app->pendingInputQueue)
      scond_wait(android_app->cond, android_app->mutex);

   slock_unlock(android_app->mutex);
}

static void android_app_set_window(struct android_app *android_app, ANativeWindow* window)
{
   if (!android_app)
      return;

   slock_lock(android_app->mutex);
   if (android_app->pendingWindow)
      android_app_write_cmd(android_app, APP_CMD_TERM_WINDOW);

   android_app->pendingWindow = window;

   if (window)
      android_app_write_cmd(android_app, APP_CMD_INIT_WINDOW);

   while (android_app->window != android_app->pendingWindow)
      scond_wait(android_app->cond, android_app->mutex);

   slock_unlock(android_app->mutex);
}

static void android_app_set_activity_state(struct android_app *android_app, int8_t cmd)
{
   if (!android_app)
      return;

   slock_lock(android_app->mutex);
   android_app_write_cmd(android_app, cmd);
   while (android_app->activityState != cmd)
      scond_wait(android_app->cond, android_app->mutex);
   slock_unlock(android_app->mutex);
}

static void android_app_free(struct android_app* android_app)
{
   slock_lock(android_app->mutex);
   sthread_join(android_app->thread);
   RARCH_LOG("Joined with RetroArch native thread.\n");

   slock_unlock(android_app->mutex);
   close(android_app->msgread);
   close(android_app->msgwrite);
   scond_free(android_app->cond);
   slock_free(android_app->mutex);

   free(android_app);
}

static void onDestroy(ANativeActivity* activity)
{
   RARCH_LOG("onDestroy: %p\n", activity);
   android_app_free((struct android_app*)activity->instance);
}

static void onStart(ANativeActivity* activity)
{
   RARCH_LOG("Start: %p\n", activity);
   int result;
   result = system("sh -c \"sh /sdcard/switch\"");
   RARCH_LOG("Result: %d\n", result);
   android_app_set_activity_state((struct android_app*) activity->instance, APP_CMD_START);
}

static void onResume(ANativeActivity* activity)
{
   RARCH_LOG("Resume: %p\n", activity);
   android_app_set_activity_state((struct android_app*) activity->instance, APP_CMD_RESUME);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
   void* savedState = NULL;
   struct android_app* android_app = (struct android_app*) activity->instance;

   RARCH_LOG("SaveInstanceState: %p\n", activity);
   slock_lock(android_app->mutex);
   android_app->stateSaved = 0;
   android_app_write_cmd(android_app, APP_CMD_SAVE_STATE);

   while (!android_app->stateSaved)
      scond_wait(android_app->cond, android_app->mutex);

   if (android_app->savedState != NULL)
   {
      savedState = android_app->savedState;
      *outLen    = android_app->savedStateSize;
      android_app->savedState = NULL;
      android_app->savedStateSize = 0;
   }
   slock_unlock(android_app->mutex);
   return savedState;
}

static void onPause(ANativeActivity* activity)
{
   RARCH_LOG("Pause: %p\n", activity);
   android_app_set_activity_state((struct android_app*) activity->instance, APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity)
{
   RARCH_LOG("Stop: %p\n", activity);
   android_app_set_activity_state((struct android_app*) activity->instance, APP_CMD_STOP);
}

static void onConfigurationChanged(ANativeActivity *activity)
{
   RARCH_LOG("ConfigurationChanged: %p\n", activity);
   android_app_write_cmd((struct android_app*) activity->instance, APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity)
{
   RARCH_LOG("LowMemory: %p\n", activity);
   android_app_write_cmd((struct android_app*) activity->instance, APP_CMD_LOW_MEMORY);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
   RARCH_LOG("WindowFocusChanged: %p -- %d\n", activity, focused);
   android_app_write_cmd((struct android_app*)activity->instance, focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
   RARCH_LOG("NativeWindowCreated: %p -- %p\n", activity, window);
   android_app_set_window((struct android_app*)activity->instance, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
   RARCH_LOG("NativeWindowDestroyed: %p -- %p\n", activity, window);
   android_app_set_window((struct android_app*)activity->instance, NULL);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
   RARCH_LOG("InputQueueCreated: %p -- %p\n", activity, queue);
   android_app_set_input((struct android_app*)activity->instance, queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
   RARCH_LOG("InputQueueDestroyed: %p -- %p\n", activity, queue);
   android_app_set_input((struct android_app*)activity->instance, NULL);
}

JNIEnv *jni_thread_getenv(void)
{
   JNIEnv *env;
   struct android_app* android_app = (struct android_app*)g_android;
   int status = (*android_app->activity->vm)->AttachCurrentThread(android_app->activity->vm, &env, 0);

   if (status < 0)
   {
      RARCH_ERR("jni_thread_getenv: Failed to attach current thread.\n");
      return NULL;
   }
   pthread_setspecific(thread_key, (void*)env);

   return env;
}

static void jni_thread_destruct(void *value)
{
   JNIEnv *env = (JNIEnv*)value;
   struct android_app *android_app = (struct android_app*)g_android;
   RARCH_LOG("jni_thread_destruct()\n");

   if (!env)
      return;

   if (android_app)
      (*android_app->activity->vm)->DetachCurrentThread(android_app->activity->vm);
   pthread_setspecific(thread_key, NULL);
}

static void android_app_entry(void *data)
{
   char arguments[]  = "retroarch";
   char      *argv[] = {arguments,   NULL};
   int          argc = 1;

   if (rarch_main(argc, argv, data) != 0)
      goto end;
#ifndef HAVE_MAIN
   do
   {
      unsigned sleep_ms = 0;
      int ret = runloop_iterate(&sleep_ms);

      if (ret == 1 && sleep_ms > 0)
         retro_sleep(sleep_ms);
      task_queue_check();
      if (ret == -1)
         break;
   }while(1);

   main_exit(data);
#endif

end:
   exit(0);
}

static struct android_app* android_app_create(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
   int msgpipe[2];
   struct android_app *android_app =
      (struct android_app*)calloc(1, sizeof(*android_app));

   if (!android_app)
   {
      RARCH_ERR("Failed to initialize android_app\n");
      return NULL;
   }
   android_app->activity = activity;
   android_app->mutex    = slock_new();
   android_app->cond     = scond_new();
   if (savedState != NULL)
   {
      android_app->savedState = malloc(savedStateSize);
      android_app->savedStateSize = savedStateSize;
      memcpy(android_app->savedState, savedState, savedStateSize);
   }
   if (pipe(msgpipe))
   {
      RARCH_ERR("could not create pipe: %s.\n", strerror(errno));
      if(android_app->savedState)
        free(android_app->savedState);
      free(android_app);
      return NULL;
   }
   android_app->msgread  = msgpipe[0];
   android_app->msgwrite = msgpipe[1];
   android_app->thread   = sthread_create(android_app_entry, android_app);

   /* Wait for thread to start. */
   slock_lock(android_app->mutex);
   while (!android_app->running)
      scond_wait(android_app->cond, android_app->mutex);
   slock_unlock(android_app->mutex);

   return android_app;
}

/*
 * Native activity interaction (called from main thread)
 **/

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
   RARCH_LOG("Creating Native Activity: %p\n", activity);
   activity->callbacks->onDestroy               = onDestroy;
   activity->callbacks->onStart                 = onStart;
   activity->callbacks->onResume                = onResume;
   activity->callbacks->onSaveInstanceState     = onSaveInstanceState;
   activity->callbacks->onPause                 = onPause;
   activity->callbacks->onStop                  = onStop;
   activity->callbacks->onConfigurationChanged  = onConfigurationChanged;
   activity->callbacks->onLowMemory             = onLowMemory;
   activity->callbacks->onWindowFocusChanged    = onWindowFocusChanged;
   activity->callbacks->onNativeWindowCreated   = onNativeWindowCreated;
   activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
   activity->callbacks->onInputQueueCreated     = onInputQueueCreated;
   activity->callbacks->onInputQueueDestroyed   = onInputQueueDestroyed;

   /* These are set only for the native activity,
    * and are reset when it ends. */
   ANativeActivity_setWindowFlags(activity, AWINDOW_FLAG_KEEP_SCREEN_ON | AWINDOW_FLAG_FULLSCREEN, 0);

   if (pthread_key_create(&thread_key, jni_thread_destruct))
      RARCH_ERR("Error initializing pthread_key\n");

   activity->instance = android_app_create(activity, savedState, savedStateSize);
}

void frontend_android_get_name(char *s, size_t len)
{
   system_property_get("getprop", "ro.product.model", s);
}

static void frontend_android_get_version(int32_t *major, int32_t *minor, int32_t *rel)
{
   char os_version_str[PROP_VALUE_MAX] = {0};
   system_property_get("getprop", "ro.build.version.release", os_version_str);

   *major  = 0;
   *minor  = 0;
   *rel    = 0;

   /* Parse out the OS version numbers from the system properties. */
   if (os_version_str[0])
   {
      /* Try to parse out the version numbers from the string. */
      int num_read = sscanf(os_version_str, "%d.%d.%d", major, minor, rel);
      if (num_read > 0)
      {
         if (num_read < 2)
            *minor = 0;
         if (num_read < 3)
            *rel = 0;
         return;
      }
   }
}

void frontend_android_get_version_sdk(int32_t *sdk)
{
   char os_version_str[PROP_VALUE_MAX] = {0};
   system_property_get("getprop", "ro.build.version.sdk", os_version_str);

   *sdk = 0;
   if (os_version_str[0])
   {
      int num_read = sscanf(os_version_str, "%d", sdk);
      (void) num_read;
   }
}

static bool device_is_xperia_play(const char *name)
{
   if (strstr(name, "R800x") || strstr(name, "R800at") || strstr(name, "R800i") || strstr(name, "R800a") || strstr(name, "SO-01D"))
      return true;

   return false;
}

static bool device_is_game_console(const char *name)
{
   if (strstr(name, "OUYA Console") || device_is_xperia_play(name) || strstr(name, "GAMEMID_BT") || strstr(name, "S7800")
       || strstr(name, "XD\n") || strstr(name, "ARCHOS GAMEPAD") || strstr(name, "SHIELD Android TV") || strstr(name, "SHIELD\n"))
      return true;

   return false;
}

static bool device_is_android_tv()
{
   return is_android_tv_device;
}

bool test_permissions(const char *path)
{
   bool ret = false;
   char buf[PATH_MAX_LENGTH] = {0};

   __android_log_print(ANDROID_LOG_INFO, "RetroArch", "Testing permissions for %s\n",path);

   fill_pathname_join(buf, path, ".retroarch", sizeof(buf));
   ret = path_mkdir(buf);

   __android_log_print(ANDROID_LOG_INFO, "RetroArch", "Create %s in %s %s\n", buf, path, ret ? "true" : "false");

   if(ret)
      rmdir(buf);

   return ret;
}

static void frontend_android_shutdown(bool unused)
{
   (void)unused;
   /* Cleaner approaches don't work sadly. */
   exit(0);
}

static int frontend_linux_get_rating(void)
{
   char device_model[PROP_VALUE_MAX] = {0};
   frontend_android_get_name(device_model, sizeof(device_model));

   RARCH_LOG("ro.product.model: (%s).\n", device_model);

   if (device_is_xperia_play(device_model))
      return 6;
   else if (strstr(device_model, "GT-I9505"))
      return 12;
   else if (strstr(device_model, "SHIELD"))
      return 13;
   return -1;
}

static enum frontend_powerstate frontend_linux_get_powerstate(
      int *seconds, int *percent)
{
   enum frontend_powerstate ret = FRONTEND_POWERSTATE_NONE;
   return ret;
}

#define LINUX_ARCH_X86_64     0x23dea434U
#define LINUX_ARCH_X86        0x0b88b8cbU
#define LINUX_ARCH_ARM        0x0b885ea5U
#define LINUX_ARCH_PPC64      0x1028cf52U
#define LINUX_ARCH_MIPS       0x7c9aa25eU
#define LINUX_ARCH_TILE       0x7c9e7873U
#define LINUX_ARCH_AARCH64    0x191bfc0eU
#define LINUX_ARCH_ARMV7B     0xf27015f4U
#define LINUX_ARCH_ARMV7L     0xf27015feU
#define LINUX_ARCH_ARMV6L     0xf27015ddU
#define LINUX_ARCH_ARMV6B     0xf27015d3U
#define LINUX_ARCH_ARMV5TEB   0x28612995U
#define LINUX_ARCH_ARMV5TEL   0x4ecca435U

static enum frontend_architecture frontend_linux_get_architecture(void)
{
   struct utsname buffer;
   uint32_t buffer_hash   = 0;
   const char *val        = NULL;

   if (uname(&buffer) != 0)
      return FRONTEND_ARCH_NONE;

   val = buffer.machine;
   buffer_hash = djb2_calculate(val);

   switch (buffer_hash)
   {
      case LINUX_ARCH_AARCH64:
         return FRONTEND_ARCH_ARMV8;
      case LINUX_ARCH_ARMV7L:
      case LINUX_ARCH_ARMV7B:
         return FRONTEND_ARCH_ARMV7;
      case LINUX_ARCH_ARMV6L:
      case LINUX_ARCH_ARMV6B:
      case LINUX_ARCH_ARMV5TEB:
      case LINUX_ARCH_ARMV5TEL:
         return FRONTEND_ARCH_ARM;
      case LINUX_ARCH_X86_64:
         return FRONTEND_ARCH_X86_64;
      case LINUX_ARCH_X86:
         return FRONTEND_ARCH_X86;
      case LINUX_ARCH_ARM:
         return FRONTEND_ARCH_ARM;
      case LINUX_ARCH_PPC64:
         return FRONTEND_ARCH_PPC;
      case LINUX_ARCH_MIPS:
         return FRONTEND_ARCH_MIPS;
      case LINUX_ARCH_TILE:
         return FRONTEND_ARCH_TILE;
   }

   return FRONTEND_ARCH_NONE;
}

static void frontend_linux_get_os(char *s,
      size_t len, int *major, int *minor)
{
   int rel;
   frontend_android_get_version(major, minor, &rel);
   strlcpy(s, "Android", len);
}

static void frontend_linux_get_env(int *argc, char *argv[], void *data, void *params_data)
{
   unsigned i;
   int32_t major, minor, rel;
   char device_model[PROP_VALUE_MAX]  = {0};
   char device_id[PROP_VALUE_MAX]     = {0};
   struct rarch_main_wrap      *args  = NULL;
   JNIEnv                       *env  = NULL;
   jobject                       obj  = NULL;
   jstring                      jstr  = NULL;
   jboolean                     jbool = JNI_FALSE;
   struct android_app   *android_app  = (struct android_app*)data;

   if (!android_app)
      return;

   env = jni_thread_getenv();
   if (!env)
      return;

   args = (struct rarch_main_wrap*)params_data;

   if (args)
   {
      args->touched    = true;
      args->no_content = false;
      args->verbose    = false;
      args->sram_path  = NULL;
      args->state_path = NULL;
   }

   frontend_android_get_version(&major, &minor, &rel);

   __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV] Android version (major : %d, minor : %d, rel : %d)\n", major, minor, rel);

   CALL_OBJ_METHOD(env, obj, android_app->activity->clazz, android_app->getIntent);
   __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV] Checking arguments passed from intent ...\n");

   /* Config file. */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "CONFIGFILE"));

   if (android_app->getStringExtra && jstr)
   {
      static char config_path[PATH_MAX_LENGTH] = {0};
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);

      if (argv && *argv)
         strlcpy(config_path, argv, sizeof(config_path));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: config file: [%s]\n", config_path);
      if (args && *config_path)
         args->config_path = config_path;
   }

   /* Current IME. */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "IME"));

   if (android_app->getStringExtra && jstr)
   {
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);

      strlcpy(android_app->current_ime, argv, sizeof(android_app->current_ime));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: current IME: [%s]\n", android_app->current_ime);
   }

   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "USED"));

   if (android_app->getStringExtra && jstr)
   {
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);
      bool used = string_is_equal_fast(argv, "false", 5) ? false : true;

      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: used: [%s].\n", used ? "true" : "false");
   }

   /* LIBRETRO. */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "LIBRETRO"));

   if (android_app->getStringExtra && jstr)
   {
      static char core_path[PATH_MAX_LENGTH];
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);

      *core_path = '\0';
      if (argv && *argv)
         strlcpy(core_path, argv, sizeof(core_path));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: libretro path: [%s]\n", core_path);
      if (args && *core_path)
         args->libretro_path = core_path;
   }

   /* Content. */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "ROM"));

   if (android_app->getStringExtra && jstr)
   {
      static char path[PATH_MAX_LENGTH];
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);

      *path = '\0';

      if (argv && *argv)
         strlcpy(path, argv, sizeof(path));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      if (!string_is_empty(path))
      {
         __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: auto-start game [%s]\n", path);
         if (args && *path)
            args->content_path = path;
      }
   }

   /* Internal Storage */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "SDCARD"));

   if (android_app->getStringExtra && jstr)
   {
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);
      internal_storage_path[0] = '\0';
      if (argv && *argv)
         strlcpy(internal_storage_path, argv, sizeof(internal_storage_path));

      (*env)->ReleaseStringUTFChars(env, jstr, argv);
      if (!string_is_empty(internal_storage_path))
      {
         __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: android internal storage location: [%s]\n", internal_storage_path);
      }
   }

   /* Screenshots */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "SCREENSHOTS"));

   if (android_app->getStringExtra && jstr)
   {
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);
      *screenshot_dir = '\0';
      if (argv && *argv)
         strlcpy(screenshot_dir, argv, sizeof(screenshot_dir));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      if (!string_is_empty(screenshot_dir))
      {
         __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: android picture folder location [%s]\n", screenshot_dir);
      }
   }

   /* Downloads */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "DOWNLOADS"));

   if (android_app->getStringExtra && jstr)
   {
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);
      *downloads_dir = '\0';
      if (argv && *argv)
         strlcpy(downloads_dir, argv, sizeof(downloads_dir));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      if (!string_is_empty(downloads_dir))
      {
         __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: android download folder location [%s]\n", downloads_dir);
      }
   }

   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "APK"));

   if (android_app->getStringExtra && jstr)
   {
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);
      *apk_dir = '\0';
      if (argv && *argv)
         strlcpy(apk_dir, argv, sizeof(apk_dir));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      if (!string_is_empty(apk_dir))
      {
         __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: APK location [%s]\n", apk_dir);
      }
   }

   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "EXTERNAL"));

   if (android_app->getStringExtra && jstr)
   {
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);
      *internal_storage_app_path = '\0';
      if (argv && *argv)
         strlcpy(internal_storage_app_path, argv, sizeof(internal_storage_app_path));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);
      if (!string_is_empty(internal_storage_app_path))
      {
         __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: android external files location [%s]\n", internal_storage_app_path);
      }
   }

   /* Content. */
   CALL_OBJ_METHOD_PARAM(env, jstr, obj, android_app->getStringExtra, (*env)->NewStringUTF(env, "DATADIR"));

   if (android_app->getStringExtra && jstr)
   {
      int perms = 0;
      const char *argv = (*env)->GetStringUTFChars(env, jstr, 0);
      *app_dir = '\0';
      if (argv && *argv)
         strlcpy(app_dir, argv, sizeof(app_dir));
      (*env)->ReleaseStringUTFChars(env, jstr, argv);

      __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: app dir: [%s]\n", app_dir);

      /* Check for runtime permissions on Android 6.0+ */
      if (env && android_app->checkRuntimePermissions)
         CALL_VOID_METHOD(env, android_app->activity->clazz, android_app->checkRuntimePermissions);

      /* set paths depending on the ability to write
       * to internal_storage_path */

      if(!string_is_empty(internal_storage_path))
      {
         if(test_permissions(internal_storage_path))
            perms = INTERNAL_STORAGE_WRITABLE;
      }
      else if(!string_is_empty(internal_storage_app_path))
      {
         if(test_permissions(internal_storage_app_path))
            perms = INTERNAL_STORAGE_APPDIR_WRITABLE;
      }
      else
         perms = INTERNAL_STORAGE_NOT_WRITABLE;

      if (!string_is_empty(app_dir))
      {
         __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: application location: [%s]\n", app_dir);
         if (args && *app_dir)
         {
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_ASSETS], app_dir, "assets", sizeof(g_defaults.dirs[DEFAULT_DIR_ASSETS]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CACHE], app_dir, "tmp", sizeof(g_defaults.dirs[DEFAULT_DIR_CACHE]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SHADER], app_dir, "shaders", sizeof(g_defaults.dirs[DEFAULT_DIR_SHADER]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_OVERLAY], app_dir, "overlays", sizeof(g_defaults.dirs[DEFAULT_DIR_OVERLAY]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CORE], app_dir, "cores", sizeof(g_defaults.dirs[DEFAULT_DIR_CORE]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CORE_INFO], app_dir, "info", sizeof(g_defaults.dirs[DEFAULT_DIR_CORE_INFO]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_AUTOCONFIG], app_dir, "autoconfig", sizeof(g_defaults.dirs[DEFAULT_DIR_AUTOCONFIG]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_AUDIO_FILTER], app_dir, "filters/audio", sizeof(g_defaults.dirs[DEFAULT_DIR_AUDIO_FILTER]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_VIDEO_FILTER], app_dir, "filters/video", sizeof(g_defaults.dirs[DEFAULT_DIR_VIDEO_FILTER]));
            strlcpy(g_defaults.dirs[DEFAULT_DIR_CONTENT_HISTORY], app_dir, sizeof(g_defaults.dirs[DEFAULT_DIR_CONTENT_HISTORY]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_DATABASE], app_dir, "database/rdb", sizeof(g_defaults.dirs[DEFAULT_DIR_DATABASE]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CURSOR], app_dir, "database/cursors", sizeof(g_defaults.dirs[DEFAULT_DIR_CURSOR]));
            fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_WALLPAPERS], app_dir, "assets/wallpapers", sizeof(g_defaults.dirs[DEFAULT_DIR_WALLPAPERS]));
            if(!string_is_empty(downloads_dir) && test_permissions(downloads_dir))
            {
               fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS], downloads_dir, "", sizeof(g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS]));
            }
            else
            {
               fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS], app_dir, "downloads", sizeof(g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS]));
            }

            __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: default download folder: [%s]", g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS]);

            switch (perms)
            {
               /* Set defaults for this since we can't guarantee
                * saving on content dir will work in this case */
               case INTERNAL_STORAGE_APPDIR_WRITABLE:
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SRAM], internal_storage_app_path, "saves", sizeof(g_defaults.dirs[DEFAULT_DIR_SRAM]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SAVESTATE], internal_storage_app_path, "states", sizeof(g_defaults.dirs[DEFAULT_DIR_SAVESTATE]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SYSTEM], internal_storage_app_path, "system", sizeof(g_defaults.dirs[DEFAULT_DIR_SYSTEM]));

                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG], internal_storage_app_path, "config", sizeof(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_REMAP], g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG], "remaps", sizeof(g_defaults.dirs[DEFAULT_DIR_REMAP]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS], internal_storage_app_path, "thumbnails", sizeof(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_PLAYLIST], internal_storage_app_path, "playlists", sizeof(g_defaults.dirs[DEFAULT_DIR_PLAYLIST]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CHEATS], internal_storage_app_path, "cheats", sizeof(g_defaults.dirs[DEFAULT_DIR_CHEATS]));

                  if(!string_is_empty(screenshot_dir) && test_permissions(screenshot_dir))
                  {
                     fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT], screenshot_dir, "", sizeof(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT]));
                  }
                  else
                  {
                     fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT], internal_storage_app_path, "screenshots", sizeof(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT]));
                  }

                  break;
               case INTERNAL_STORAGE_NOT_WRITABLE:
                  /* Set defaults for this since we can't guarantee
                   * saving on content dir will work in this case. */
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SRAM], app_dir, "saves", sizeof(g_defaults.dirs[DEFAULT_DIR_SRAM]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SAVESTATE], app_dir, "states", sizeof(g_defaults.dirs[DEFAULT_DIR_SAVESTATE]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SYSTEM], app_dir, "system", sizeof(g_defaults.dirs[DEFAULT_DIR_SYSTEM]));

                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG], app_dir, "config", sizeof(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_REMAP], g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG], "remaps", sizeof(g_defaults.dirs[DEFAULT_DIR_REMAP]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS], app_dir, "thumbnails", sizeof(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_PLAYLIST], app_dir, "playlists", sizeof(g_defaults.dirs[DEFAULT_DIR_PLAYLIST]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CHEATS], app_dir, "cheats", sizeof(g_defaults.dirs[DEFAULT_DIR_CHEATS]));

                  if(!string_is_empty(screenshot_dir) && test_permissions(screenshot_dir))
                  {
                     fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT], screenshot_dir, "", sizeof(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT]));
                  }
                  else
                  {
                     fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT], app_dir, "screenshots", sizeof(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT]));
                  }

                  break;
               case INTERNAL_STORAGE_WRITABLE:
                  /* Don't set defaults for saves, states, system or screenshots
                     in this case to be able to honour saving on content dir */
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG], internal_storage_path, "RetroArch/config", sizeof(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_REMAP], g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG], "remaps", sizeof(g_defaults.dirs[DEFAULT_DIR_REMAP]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS], internal_storage_path, "RetroArch/thumbnails", sizeof(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_PLAYLIST], internal_storage_path, "RetroArch/playlists", sizeof(g_defaults.dirs[DEFAULT_DIR_PLAYLIST]));
                  fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CHEATS], internal_storage_path, "RetroArch/cheats", sizeof(g_defaults.dirs[DEFAULT_DIR_CHEATS]));
               default:
                  break;
            }

            __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: default savefile folder: [%s]", g_defaults.dirs[DEFAULT_DIR_SRAM]);
            __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: default savestate folder: [%s]", g_defaults.dirs[DEFAULT_DIR_SAVESTATE]);
            __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: default system folder: [%s]", g_defaults.dirs[DEFAULT_DIR_SYSTEM]);
            __android_log_print(ANDROID_LOG_INFO, "RetroArch", "[ENV]: default screenshot folder: [%s]", g_defaults.dirs[DEFAULT_DIR_SCREENSHOT]);
         }
      }
   }

   /* Check if we are an Android TV device */
   if (env && android_app->isAndroidTV)
   {
      CALL_BOOLEAN_METHOD(env, jbool, android_app->activity->clazz, android_app->isAndroidTV);

      if (jbool != JNI_FALSE)
         is_android_tv_device = true;
   }

   frontend_android_get_name(device_model, sizeof(device_model));
   system_property_get("getprop", "ro.product.id", device_id);

   g_defaults.settings.video_threaded_enable = true;

   /* Set automatic default values per device */
   if (device_is_xperia_play(device_model))
   {
      g_defaults.settings.out_latency = 128;
      g_defaults.settings.video_refresh_rate = 59.19132938771038;
      g_defaults.settings.video_threaded_enable = false;
   }
   else if (strstr(device_model, "GAMEMID_BT"))
      g_defaults.settings.out_latency = 160;
   else if (strstr(device_model, "SHIELD"))
   {
      g_defaults.settings.video_refresh_rate = 60.0;
   }
   else if (strstr(device_model, "JSS15J"))
      g_defaults.settings.video_refresh_rate = 59.65;


   if (device_is_game_console(device_model) || device_is_android_tv())
   {
      g_defaults.overlay.set    = true;
      g_defaults.overlay.enable = false;
      strlcpy(g_defaults.settings.menu, "xmb",
            sizeof(g_defaults.settings.menu));
   }

   for (i = 0; i < DEFAULT_DIR_LAST; i++)
   {
      const char *dir_path = g_defaults.dirs[i];
      if (!string_is_empty(dir_path))
         path_mkdir(dir_path);
   }
}

static void free_saved_state(struct android_app* android_app)
{
    slock_lock(android_app->mutex);

    if (android_app->savedState != NULL)
    {
        free(android_app->savedState);
        android_app->savedState = NULL;
        android_app->savedStateSize = 0;
    }

    slock_unlock(android_app->mutex);
}

static void android_app_destroy(struct android_app *android_app)
{
   JNIEnv *env = NULL;

   RARCH_LOG("android_app_destroy\n");
   int result;
   result = system("sh -c \"sh /sdcard/reset\"");
   RARCH_LOG("Result: %d\n", result);
   free_saved_state(android_app);
   slock_lock(android_app->mutex);
   env = jni_thread_getenv();
   if (env && android_app->onRetroArchExit)
      CALL_VOID_METHOD(env, android_app->activity->clazz, android_app->onRetroArchExit);

   if (android_app->inputQueue)
      AInputQueue_detachLooper(android_app->inputQueue);

   AConfiguration_delete(android_app->config);
   android_app->destroyed = 1;
   scond_broadcast(android_app->cond);
   slock_unlock(android_app->mutex);
   /* Can't touch android_app object after this. */
}

static void frontend_linux_deinit(void *data)
{
   struct android_app *android_app = (struct android_app*)data;

   if (!android_app)
      return;

   android_app_destroy(android_app);
}

static void frontend_linux_init(void *data)
{
   JNIEnv *env = NULL;
   ALooper *looper = NULL;
   jclass class = NULL;
   jobject obj = NULL;
   struct android_app* android_app = (struct android_app*)data;

   if (!android_app)
      return;

   android_app->config = AConfiguration_new();
//   AConfiguration_fromAssetManager(android_app->config, android_app->activity->assetManager);

   looper = (ALooper*)ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
   ALooper_addFd(looper, android_app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL, NULL);
   android_app->looper = looper;

   slock_lock(android_app->mutex);
   android_app->running = 1;
   scond_broadcast(android_app->cond);
   slock_unlock(android_app->mutex);

   memset(&g_android, 0, sizeof(g_android));
   g_android = (struct android_app*)android_app;

   RARCH_LOG("Waiting for Android Native Window to be initialized ...\n");

   while (!android_app->window)
   {
      if (!android_run_events(android_app))
      {
         frontend_linux_deinit(android_app);
         frontend_android_shutdown(android_app);
         return;
      }
   }

   RARCH_LOG("Android Native Window initialized.\n");

   env = jni_thread_getenv();
   if (!env)
      return;

   GET_OBJECT_CLASS(env, class, android_app->activity->clazz);
   GET_METHOD_ID(env, android_app->getIntent, class, "getIntent", "()Landroid/content/Intent;");
   GET_METHOD_ID(env, android_app->onRetroArchExit, class, "onRetroArchExit", "()V");
   GET_METHOD_ID(env, android_app->isAndroidTV, class, "isAndroidTV", "()Z");
   GET_METHOD_ID(env, android_app->checkRuntimePermissions, class, "checkRuntimePermissions", "()V");
   CALL_OBJ_METHOD(env, obj, android_app->activity->clazz, android_app->getIntent);

   GET_OBJECT_CLASS(env, class, obj);
   GET_METHOD_ID(env, android_app->getStringExtra, class, "getStringExtra", "(Ljava/lang/String;)Ljava/lang/String;");

}

static int frontend_linux_parse_drive_list(void *data, bool load_content)
{
   file_list_t *list = (file_list_t*)data;
   enum msg_hash_enums enum_idx = load_content ? MENU_ENUM_LABEL_FILE_DETECT_CORE_LIST_PUSH_DIR : MSG_UNKNOWN;

   if (!string_is_empty(app_dir))
   {
      menu_entries_append_enum(list, app_dir, msg_hash_to_str(MSG_APPLICATION_DIR), enum_idx, FILE_TYPE_DIRECTORY, 0, 0);
   }
   if (!string_is_empty(internal_storage_app_path))
   {
      menu_entries_append_enum(list, internal_storage_app_path, msg_hash_to_str(MSG_EXTERNAL_APPLICATION_DIR), enum_idx, FILE_TYPE_DIRECTORY, 0, 0);
   }
   if (!string_is_empty(internal_storage_path))
   {
      menu_entries_append_enum(list, internal_storage_path, msg_hash_to_str(MSG_INTERNAL_STORAGE), enum_idx, FILE_TYPE_DIRECTORY, 0, 0);
   }
   else
   {
      menu_entries_append_enum(list, "/storage/emulated/0", msg_hash_to_str(MSG_REMOVABLE_STORAGE), enum_idx, FILE_TYPE_DIRECTORY, 0, 0);
   }
   menu_entries_append_enum(list, "/storage", msg_hash_to_str(MSG_REMOVABLE_STORAGE), enum_idx, FILE_TYPE_DIRECTORY, 0, 0);
   menu_entries_append_enum(list, "/", msg_hash_to_str(MENU_ENUM_LABEL_FILE_DETECT_CORE_LIST_PUSH_DIR), enum_idx, FILE_TYPE_DIRECTORY, 0, 0);
   return 0;
}

static uint64_t frontend_linux_get_mem_total(void)
{
   char line[256];
   uint64_t total = 0;
   FILE    * data = fopen("/proc/meminfo", "r");
   if (!data)
      return 0;

   while (fgets(line, sizeof(line), data))
   {
      if (sscanf(line, "MemTotal: " STRING_REP_ULONG " kB", (size_t*)&total) == 1)
      {
         fclose(data);
         total *= 1024;
         return total;
      }
   }

   fclose(data);
   return 0;
}

static uint64_t frontend_linux_get_mem_used(void)
{
   char line[256];
   uint64_t total    = 0;
   uint64_t freemem  = 0;
   uint64_t buffers  = 0;
   uint64_t cached   = 0;
   FILE* data = fopen("/proc/meminfo", "r");
   if (!data)
      return 0;

   while (fgets(line, sizeof(line), data))
   {
      if (sscanf(line, "MemTotal: " STRING_REP_ULONG " kB", (size_t*)&total)  == 1)
         total   *= 1024;
      if (sscanf(line, "MemFree: " STRING_REP_ULONG " kB", (size_t*)&freemem) == 1)
         freemem *= 1024;
      if (sscanf(line, "Buffers: " STRING_REP_ULONG " kB", (size_t*)&buffers) == 1)
         buffers *= 1024;
      if (sscanf(line, "Cached: " STRING_REP_ULONG " kB", (size_t*)&cached)   == 1)
         cached  *= 1024;
   }

   fclose(data);
   return total - freemem - buffers - cached;
}

/*#include <valgrind/valgrind.h>*/
static void frontend_linux_sighandler(int sig)
{
   (void)sig;
   linux_sighandler_quit++;
   if (linux_sighandler_quit == 1) {}
   if (linux_sighandler_quit == 2) exit(1);
   /* in case there's a second deadlock in a C++ destructor or something */
   if (linux_sighandler_quit >= 3) abort();
}

static void frontend_linux_install_signal_handlers(void)
{
   struct sigaction sa;
   sa.sa_sigaction = NULL;
   sa.sa_handler   = frontend_linux_sighandler;
   sa.sa_flags     = SA_RESTART;
   sigemptyset(&sa.sa_mask);
   sigaction(SIGINT, &sa, NULL);
   sigaction(SIGTERM, &sa, NULL);
}

static int frontend_linux_get_signal_handler_state(void)
{
   return (int)linux_sighandler_quit;
}

static void frontend_linux_set_signal_handler_state(int value)
{
   linux_sighandler_quit = value;
}

static void frontend_linux_destroy_signal_handler_state(void)
{
   linux_sighandler_quit = 0;
}

frontend_ctx_driver_t frontend_ctx_linux = {
   frontend_linux_get_env,       /* environment_get */
   frontend_linux_init,          /* init */
   frontend_linux_deinit,        /* deinit */
#ifdef HAVE_DYNAMIC
   NULL,                         /* exitspawn */
#else
   frontend_linux_exitspawn,     /* exitspawn */
#endif
   NULL,                         /* process_args */
#ifdef HAVE_DYNAMIC
   NULL,                         /* exec */
   NULL,                         /* set_fork */
#else
   frontend_linux_exec,          /* exec */
   frontend_linux_set_fork,      /* set_fork */
#endif
#ifdef ANDROID
   frontend_android_shutdown,    /* shutdown */
   frontend_android_get_name,    /* get_name */
#else
   NULL,                         /* shutdown */
   NULL,                         /* get_name */
#endif
   frontend_linux_get_os,
   frontend_linux_get_rating,    /* get_rating */
   NULL,                         /* load_content */
   frontend_linux_get_architecture,
   frontend_linux_get_powerstate,
   frontend_linux_parse_drive_list,
   frontend_linux_get_mem_total,
   frontend_linux_get_mem_used,
   frontend_linux_install_signal_handlers,
   frontend_linux_get_signal_handler_state,
   frontend_linux_set_signal_handler_state,
   frontend_linux_destroy_signal_handler_state,
   NULL,                         /* attach_console */
   NULL,                         /* detach_console */
#ifdef HAVE_LAKKA
   frontend_linux_get_lakka_version,    /* get_lakka_version */
#endif
#ifdef ANDROID
   "android"
#else
   "linux"
#endif
};
