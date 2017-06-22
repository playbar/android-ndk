/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *  Copyright (C) 2014-2017 - Jean-André Santoni
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

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <lists/file_list.h>
#include <file/file_path.h>
#include <string/stdstring.h>
#include <lists/string_list.h>

#include <compat/strl.h>

#include <audio/audio_resampler.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include "../src/config.def.h"
#include "../src/config.def.keybinds.h"

#if defined(__CELLOS_LV2__)
#include <sdk_version.h>

#if (CELL_SDK_VERSION > 0x340000)
#include <sysutil/sysutil_bgmplayback.h>
#endif

#endif

#include "../frontend/frontend_driver.h"

#include "widgets/menu_input_bind_dialog.h"

#include "menu_setting.h"
#include "menu_driver.h"
#include "menu_animation.h"
#include "menu_input.h"

#include "../src/core.h"
#include "../src/configuration.h"
#include "../src/msg_hash.h"
#include "../src/defaults.h"
#include "../src/driver.h"
#include "../src/dirs.h"
#include "../src/paths.h"
#include "../src/dynamic.h"
#include "../src/list_special.h"
#include "../src/verbosity.h"
#include "../camera/camera_driver.h"
#include "../wifi/wifi_driver.h"
#include "../location/location_driver.h"
#include "../record/record_driver.h"
#include "../audio/audio_driver.h"
#include "../input/input_config.h"
#include "../tasks/tasks_internal.h"
#include "../src/config.def.h"
#include "../ui/ui_companion_driver.h"
#include "../src/performance_counters.h"
#include "../src/setting_list.h"
#include "../src/lakka.h"
#include "../src/retroarch.h"

#include "../tasks/tasks_internal.h"

struct bool_entry
{
    bool *target;
    enum msg_hash_enums name_enum_idx;
    enum msg_hash_enums SHORT_enum_idx;
    bool default_value;
    enum msg_hash_enums off_enum_idx;
    enum msg_hash_enums on_enum_idx;
    uint32_t flags;
};

struct string_options_entry
{
    char *target;
    size_t len;
    enum msg_hash_enums name_enum_idx;
    enum msg_hash_enums SHORT_enum_idx;
    const char *default_value;
    const char *values;
};

#ifdef HAVE_CHEEVOS
static void setting_get_string_representation_cheevos_password(void *data,
                                                               char *s, size_t len)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;
    if (!setting)
        return;

    if (!string_is_empty(setting->value.target.string))
        strlcpy(s, "********", len);
    else
        *setting->value.target.string = '\0';
}
#endif

static void setting_get_string_representation_uint_video_monitor_index(void *data,
                                                                       char *s, size_t len)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;
    if (!setting)
        return;

    if (*setting->value.target.unsigned_integer)
        snprintf(s, len, "%u",
                 *setting->value.target.unsigned_integer);
    else
        strlcpy(s, "0 (Auto)", len);
}

static void setting_get_string_representation_uint_custom_viewport_width(void *data,
                                                                         char *s, size_t len)
{
    struct retro_game_geometry  *geom    = NULL;
    struct retro_system_av_info *av_info = NULL;
    rarch_setting_t *setting             = (rarch_setting_t*)data;
    if (!setting)
        return;

    av_info = video_viewport_get_system_av_info();
    geom    = (struct retro_game_geometry*)&av_info->geometry;

    if (*setting->value.target.unsigned_integer%geom->base_width == 0)
        snprintf(s, len, "%u (%ux)",
                 *setting->value.target.unsigned_integer,
                 *setting->value.target.unsigned_integer / geom->base_width);
    else
        snprintf(s, len, "%u",
                 *setting->value.target.unsigned_integer);
}

static void setting_get_string_representation_uint_custom_viewport_height(void *data,
                                                                          char *s, size_t len)
{
    struct retro_game_geometry  *geom    = NULL;
    struct retro_system_av_info *av_info = NULL;
    rarch_setting_t *setting             = (rarch_setting_t*)data;
    if (!setting)
        return;

    av_info = video_viewport_get_system_av_info();
    geom    = (struct retro_game_geometry*)&av_info->geometry;

    if (*setting->value.target.unsigned_integer%geom->base_height == 0)
        snprintf(s, len, "%u (%ux)",
                 *setting->value.target.unsigned_integer,
                 *setting->value.target.unsigned_integer / geom->base_height);
    else
        snprintf(s, len, "%u",
                 *setting->value.target.unsigned_integer);
}

#ifdef HAVE_WASAPI
static void setting_get_string_representation_int_audio_wasapi_sh_buffer_length(void *data,
      char *s, size_t len)
{
   rarch_setting_t *setting = (rarch_setting_t*)data;
   if (!setting)
      return;

   if (*setting->value.target.integer > 0)
      snprintf(s, len, "%d",
            *setting->value.target.integer);
   else if (*setting->value.target.integer == 0)
      strlcpy(s, "0 (Off)", len);
   else
      strlcpy(s, "Auto", len);
}
#endif

static int setting_uint_action_left_custom_viewport_width(void *data, bool wraparound)
{
    video_viewport_t vp;
    struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
    video_viewport_t            *custom  = video_viewport_get_custom();
    settings_t                 *settings = config_get_ptr();
    struct retro_game_geometry     *geom = (struct retro_game_geometry*)
            &av_info->geometry;

    if (!settings || !av_info)
        return -1;

    video_driver_get_viewport_info(&vp);

    if (custom->width <= 1)
        custom->width = 1;
    else if (settings->bools.video_scale_integer)
    {
        if (custom->width > geom->base_width)
            custom->width -= geom->base_width;
    }
    else
        custom->width -= 1;

    aspectratio_lut[ASPECT_RATIO_CUSTOM].value =
            (float)custom->width / custom->height;

    return 0;
}

static int setting_uint_action_right_custom_viewport_width(void *data, bool wraparound)
{
    video_viewport_t vp;
    struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
    video_viewport_t            *custom  = video_viewport_get_custom();
    settings_t                 *settings = config_get_ptr();
    struct retro_game_geometry     *geom = (struct retro_game_geometry*)
            &av_info->geometry;

    if (!settings || !av_info)
        return -1;

    video_driver_get_viewport_info(&vp);

    if (settings->bools.video_scale_integer)
        custom->width += geom->base_width;
    else
        custom->width += 1;

    aspectratio_lut[ASPECT_RATIO_CUSTOM].value =
            (float)custom->width / custom->height;

    return 0;
}

static int setting_uint_action_left_custom_viewport_height(void *data, bool wraparound)
{
    video_viewport_t vp;
    struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
    video_viewport_t            *custom  = video_viewport_get_custom();
    settings_t                 *settings = config_get_ptr();
    struct retro_game_geometry     *geom = (struct retro_game_geometry*)
            &av_info->geometry;

    if (!settings || !av_info)
        return -1;

    video_driver_get_viewport_info(&vp);

    if (custom->height <= 1)
        custom->height = 1;
    else if (settings->bools.video_scale_integer)
    {
        if (custom->height > geom->base_height)
            custom->height -= geom->base_height;
    }
    else
        custom->height -= 1;

    aspectratio_lut[ASPECT_RATIO_CUSTOM].value =
            (float)custom->width / custom->height;

    return 0;
}

static int setting_uint_action_right_custom_viewport_height(void *data, bool wraparound)
{
    video_viewport_t vp;
    struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
    video_viewport_t            *custom  = video_viewport_get_custom();
    settings_t                 *settings = config_get_ptr();
    struct retro_game_geometry     *geom = (struct retro_game_geometry*)
            &av_info->geometry;

    if (!settings || !av_info)
        return -1;

    video_driver_get_viewport_info(&vp);

    if (settings->bools.video_scale_integer)
        custom->height += geom->base_height;
    else
        custom->height += 1;

    aspectratio_lut[ASPECT_RATIO_CUSTOM].value =
            (float)custom->width / custom->height;

    return 0;
}

#if !defined(RARCH_CONSOLE)
static int setting_string_action_left_audio_device(void *data, bool wraparound)
{
    int audio_device_index;
    struct string_list *ptr  = NULL;
    rarch_setting_t *setting = (rarch_setting_t*)data;

    if (!audio_driver_get_devices_list((void**)&ptr))
        return -1;

    if (!ptr)
        return -1;

    /* Get index in the string list */
    audio_device_index = string_list_find_elem(ptr,setting->value.target.string) - 1;
    audio_device_index--;

    /* Reset index if needed */
    if (audio_device_index < 0)
        audio_device_index = (int)(ptr->size - 1);

    strlcpy(setting->value.target.string, ptr->elems[audio_device_index].data, setting->size);

    return 0;
}

static int setting_string_action_right_audio_device(void *data, bool wraparound)
{
    int audio_device_index;
    struct string_list *ptr  = NULL;
    rarch_setting_t *setting = (rarch_setting_t*)data;

    if (!audio_driver_get_devices_list((void**)&ptr))
        return -1;

    if (!ptr)
        return -1;

    /* Get index in the string list */
    audio_device_index = string_list_find_elem(ptr,setting->value.target.string) -1;
    audio_device_index++;

    /* Reset index if needed */
    if (audio_device_index == (signed)ptr->size)
        audio_device_index = 0;

    strlcpy(setting->value.target.string, ptr->elems[audio_device_index].data, setting->size);

    return 0;
}
#endif

static int setting_string_action_left_driver(void *data,
                                             bool wraparound)
{
    driver_ctx_info_t drv;
    rarch_setting_t *setting = (rarch_setting_t*)data;

    if (!setting)
        return -1;

    drv.label = setting->name;
    drv.s     = setting->value.target.string;
    drv.len   = setting->size;

    if (!driver_ctl(RARCH_DRIVER_CTL_FIND_PREV, &drv))
        return -1;

    if (setting->change_handler)
        setting->change_handler(setting);

    return 0;
}

static int setting_string_action_right_driver(void *data,
                                              bool wraparound)
{
    driver_ctx_info_t drv;
    rarch_setting_t *setting = (rarch_setting_t*)data;

    if (!setting)
        return -1;

    drv.label = setting->name;
    drv.s     = setting->value.target.string;
    drv.len   = setting->size;

    if (!driver_ctl(RARCH_DRIVER_CTL_FIND_NEXT, &drv))
    {
        settings_t *settings = config_get_ptr();

        if (settings && settings->bools.menu_navigation_wraparound_enable)
        {
            drv.label = setting->name;
            drv.s     = setting->value.target.string;
            drv.len   = setting->size;
            driver_ctl(RARCH_DRIVER_CTL_FIND_FIRST, &drv);
        }
    }

    if (setting->change_handler)
        setting->change_handler(setting);

    return 0;
}

static void setting_get_string_representation_uint_video_rotation(void *data,
                                                                  char *s, size_t len)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;
    if (setting)
    {
        char rotation_lut[4][32] =
                {
                        "Normal",
                        "90 deg",
                        "180 deg",
                        "270 deg"
                };

        strlcpy(s, rotation_lut[*setting->value.target.unsigned_integer],
                len);
    }
}

static void setting_get_string_representation_uint_aspect_ratio_index(void *data,
                                                                      char *s, size_t len)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;
    if (setting)
        strlcpy(s,
                aspectratio_lut[*setting->value.target.unsigned_integer].name,
                len);
}

static void setting_get_string_representation_uint_libretro_device(void *data,
                                                                   char *s, size_t len)
{
    unsigned index_offset, device;
    const struct retro_controller_description *desc = NULL;
    const char *name            = NULL;
    rarch_system_info_t *system = runloop_get_system_info();
    rarch_setting_t *setting    = (rarch_setting_t*)data;

    if (!setting)
        return;

    index_offset                = setting->index_offset;
    device                      = input_config_get_device(index_offset);

    if (system)
    {
        if (index_offset < system->ports.size)
            desc = libretro_find_controller_description(
                    &system->ports.data[index_offset],
                    device);
    }

    if (desc)
        name = desc->desc;

    if (!name)
    {
        /* Find generic name. */
        switch (device)
        {
            case RETRO_DEVICE_NONE:
                name = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NONE);
                break;
            case RETRO_DEVICE_JOYPAD:
                name = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_RETROPAD);
                break;
            case RETRO_DEVICE_ANALOG:
                name = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_RETROPAD_WITH_ANALOG);
                break;
            default:
                name = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_UNKNOWN);
                break;
        }
    }

    if (!string_is_empty(name))
        strlcpy(s, name, len);
}

static void setting_get_string_representation_uint_analog_dpad_mode(void *data,
                                                                    char *s, size_t len)
{
    const char *modes[3];
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    modes[0] = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NONE);
    modes[1] = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LEFT_ANALOG);
    modes[2] = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_RIGHT_ANALOG);


    if (setting)
    {
        unsigned index_offset = setting->index_offset;
        strlcpy(s, modes[settings->uints.input_analog_dpad_mode
                         [index_offset] % ANALOG_DPAD_LAST], len);
    }
}

#ifdef HAVE_THREADS
static void setting_get_string_representation_uint_autosave_interval(void *data,
                                                                     char *s, size_t len)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;
    if (!setting)
        return;

    if (*setting->value.target.unsigned_integer)
        snprintf(s, len, "%u %s",
                 *setting->value.target.unsigned_integer, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SECONDS));
    else
        strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_OFF), len);
}
#endif

#ifdef HAVE_LANGEXTRA
static void setting_get_string_representation_uint_user_language(void *data,
      char *s, size_t len)
{
   const char *modes[RETRO_LANGUAGE_LAST];

   modes[RETRO_LANGUAGE_ENGLISH]                = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_ENGLISH);
   modes[RETRO_LANGUAGE_JAPANESE]               = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_JAPANESE);
   modes[RETRO_LANGUAGE_FRENCH]                 = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_FRENCH);
   modes[RETRO_LANGUAGE_SPANISH]                = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_SPANISH);
   modes[RETRO_LANGUAGE_GERMAN]                 = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_GERMAN);
   modes[RETRO_LANGUAGE_ITALIAN]                = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_ITALIAN);
   modes[RETRO_LANGUAGE_DUTCH]                  = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_DUTCH);
   modes[RETRO_LANGUAGE_PORTUGUESE_BRAZIL]      = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_PORTUGUESE_BRAZIL);
   modes[RETRO_LANGUAGE_PORTUGUESE_PORTUGAL]    = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_PORTUGUESE_PORTUGAL);
   modes[RETRO_LANGUAGE_RUSSIAN]                = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_RUSSIAN);
   modes[RETRO_LANGUAGE_KOREAN]                 = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_KOREAN);
   modes[RETRO_LANGUAGE_CHINESE_TRADITIONAL]    = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_CHINESE_TRADITIONAL);
   modes[RETRO_LANGUAGE_CHINESE_SIMPLIFIED]     = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_CHINESE_SIMPLIFIED);
   modes[RETRO_LANGUAGE_ESPERANTO]              = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_ESPERANTO);
   modes[RETRO_LANGUAGE_POLISH]                 = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_POLISH);
   modes[RETRO_LANGUAGE_VIETNAMESE]             = msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LANG_VIETNAMESE);

   strlcpy(s, modes[*msg_hash_get_uint(MSG_HASH_USER_LANGUAGE)], len);
}
#endif

static void setting_get_string_representation_uint_libretro_log_level(void *data,
                                                                      char *s, size_t len)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;

    if (setting)
    {
        static const char *modes[] = {
                "0 (Debug)",
                "1 (Info)",
                "2 (Warning)",
                "3 (Error)"
        };
        strlcpy(s, modes[*setting->value.target.unsigned_integer],
                len);
    }
}

enum setting_type menu_setting_get_browser_selection_type(rarch_setting_t *setting)
{
    if (!setting)
        return ST_NONE;
    return setting->browser_selection_type;
}

static void menu_settings_info_list_free(rarch_setting_info_t *list_info)
{
    if (list_info)
        free(list_info);
}

void menu_settings_list_current_add_range(
        rarch_setting_t **list,
        rarch_setting_info_t *list_info,
        float min, float max, float step,
        bool enforce_minrange_enable, bool enforce_maxrange_enable)
{
    unsigned idx = list_info->index - 1;

    (*list)[idx].min               = min;
    (*list)[idx].step              = step;
    (*list)[idx].max               = max;
    (*list)[idx].enforce_minrange  = enforce_minrange_enable;
    (*list)[idx].enforce_maxrange  = enforce_maxrange_enable;

    (*list)[list_info->index - 1].flags |= SD_FLAG_HAS_RANGE;
}

static void menu_settings_list_current_add_values(
        rarch_setting_t **list,
        rarch_setting_info_t *list_info,
        const char *values)
{
    unsigned idx = list_info->index - 1;
    (*list)[idx].values = values;
}

void menu_settings_list_current_add_cmd(
        rarch_setting_t **list,
        rarch_setting_info_t *list_info,
        enum event_command values)
{
    unsigned idx = list_info->index - 1;
    (*list)[idx].cmd_trigger.idx = values;
}

void menu_settings_list_current_add_enum_idx(
        rarch_setting_t **list,
        rarch_setting_info_t *list_info,
        enum msg_hash_enums enum_idx)
{
    unsigned idx = list_info->index - 1;
    (*list)[idx].enum_idx = enum_idx;
}

void menu_settings_list_current_add_enum_value_idx(
        rarch_setting_t **list,
        rarch_setting_info_t *list_info,
        enum msg_hash_enums enum_idx)
{
    unsigned idx = list_info->index - 1;
    (*list)[idx].enum_value_idx = enum_idx;
}


int menu_setting_generic(rarch_setting_t *setting, bool wraparound)
{
    uint64_t flags = setting->flags;
    if (setting_generic_action_ok_default(setting, wraparound) != 0)
        return -1;

    if (setting->change_handler)
        setting->change_handler(setting);

    if ((flags & SD_FLAG_EXIT) && setting->cmd_trigger.triggered)
    {
        setting->cmd_trigger.triggered = false;
        return -1;
    }

    return 0;
}

static int setting_handler(rarch_setting_t *setting, unsigned action)
{
    if (!setting)
        return -1;

    switch (action)
    {
        case MENU_ACTION_UP:
            if (setting->action_up)
                return setting->action_up(setting);
            break;
        case MENU_ACTION_DOWN:
            if (setting->action_down)
                return setting->action_down(setting);
            break;
        case MENU_ACTION_LEFT:
            if (setting->action_left)
            {
                int ret = setting->action_left(setting, false);
                menu_driver_ctl(RARCH_MENU_CTL_UPDATE_SAVESTATE_THUMBNAIL_PATH, NULL);
                menu_driver_ctl(RARCH_MENU_CTL_UPDATE_SAVESTATE_THUMBNAIL_IMAGE, NULL);
                return ret;
            }
            break;
        case MENU_ACTION_RIGHT:
            if (setting->action_right)
            {
                int ret = setting->action_right(setting, false);
                menu_driver_ctl(RARCH_MENU_CTL_UPDATE_SAVESTATE_THUMBNAIL_PATH, NULL);
                menu_driver_ctl(RARCH_MENU_CTL_UPDATE_SAVESTATE_THUMBNAIL_IMAGE, NULL);
                return ret;
            }
            break;
        case MENU_ACTION_SELECT:
            if (setting->action_select)
                return setting->action_select(setting, true);
            break;
        case MENU_ACTION_OK:
            if (setting->action_ok)
                return setting->action_ok(setting, false);
            break;
        case MENU_ACTION_CANCEL:
            if (setting->action_cancel)
                return setting->action_cancel(setting);
            break;
        case MENU_ACTION_START:
            if (setting->action_start)
                return setting->action_start(setting);
            break;
    }

    return -1;
}

int menu_action_handle_setting(rarch_setting_t *setting,
                               unsigned type, unsigned action, bool wraparound)
{
    if (!setting)
        return -1;

    switch (setting_get_type(setting))
    {
        case ST_PATH:
            if (action == MENU_ACTION_OK)
            {
                menu_displaylist_info_t  info = {0};
                file_list_t       *menu_stack = menu_entries_get_menu_stack_ptr(0);
                const char      *name         = setting->name;
                size_t selection              = menu_navigation_get_selection();

                info.list           = menu_stack;
                info.directory_ptr  = selection;
                info.type           = type;
                info.enum_idx       = MSG_UNKNOWN;
                strlcpy(info.path,  setting->default_value.string, sizeof(info.path));
                strlcpy(info.label, name, sizeof(info.label));

                if (menu_displaylist_ctl(DISPLAYLIST_GENERIC, &info))
                    menu_displaylist_process(&info);
            }
            /* fall-through. */
        case ST_BOOL:
        case ST_INT:
        case ST_UINT:
        case ST_HEX:
        case ST_FLOAT:
        case ST_STRING:
        case ST_STRING_OPTIONS:
        case ST_DIR:
        case ST_BIND:
        case ST_ACTION:
            if (setting_handler(setting, action) == 0)
                return menu_setting_generic(setting, wraparound);
            break;
        default:
            break;
    }

    return -1;
}

static rarch_setting_t *menu_setting_find_internal(rarch_setting_t *setting,
                                                   const char *label)
{
    uint32_t needle        = msg_hash_calculate(label);
    rarch_setting_t **list = &setting;

    for (; setting_get_type(setting) != ST_NONE; (*list = *list + 1))
    {
        if (     (needle                    == setting->name_hash)
                 && (setting_get_type(setting) <= ST_GROUP))
        {
            const char *name              = setting->name;
            const char *short_description = setting->short_description;
            /* make sure this isn't a collision */
            if (!string_is_equal(label, name))
                continue;

            if (string_is_empty(short_description))
                return NULL;

            if (setting->read_handler)
                setting->read_handler(setting);

            return setting;
        }
    }

    return NULL;
}

static rarch_setting_t *menu_setting_find_internal_enum(rarch_setting_t *setting,
                                                        enum msg_hash_enums enum_idx)
{
    rarch_setting_t **list = &setting;
    for (; setting_get_type(setting) != ST_NONE; (*list = *list + 1))
    {
        if (setting->enum_idx == enum_idx && setting_get_type(setting) <= ST_GROUP)
        {
            const char *short_description = setting->short_description;
            if (string_is_empty(short_description))
                return NULL;

            if (setting->read_handler)
                setting->read_handler(setting);

            return setting;
        }
    }

    return NULL;
}

/**
 * menu_setting_find:
 * @settings           : pointer to settings
 * @name               : name of setting to search for
 *
 * Search for a setting with a specified name (@name).
 *
 * Returns: pointer to setting if found, NULL otherwise.
 **/
rarch_setting_t *menu_setting_find(const char *label)
{
    rarch_setting_t *setting = NULL;

    menu_entries_ctl(MENU_ENTRIES_CTL_SETTINGS_GET, &setting);

    if (!setting || !label)
        return NULL;

    return menu_setting_find_internal(setting, label);
}

rarch_setting_t *menu_setting_find_enum(enum msg_hash_enums enum_idx)
{
    rarch_setting_t *setting = NULL;

    menu_entries_ctl(MENU_ENTRIES_CTL_SETTINGS_GET, &setting);

    if (!setting || enum_idx == 0)
        return NULL;

    return menu_setting_find_internal_enum(setting, enum_idx);
}

int menu_setting_set_flags(rarch_setting_t *setting)
{
    if (!setting)
        return 0;

    switch (setting_get_type(setting))
    {
        case ST_STRING_OPTIONS:
            return MENU_SETTING_STRING_OPTIONS;
        case ST_ACTION:
            return MENU_SETTING_ACTION;
        case ST_PATH:
            return FILE_TYPE_PATH;
        case ST_GROUP:
            return MENU_SETTING_GROUP;
        case ST_SUB_GROUP:
            return MENU_SETTING_SUBGROUP;
        default:
            break;
    }

    return 0;
}

int menu_setting_set(unsigned type, const char *label,
                     unsigned action, bool wraparound)
{
    int ret                    = 0;
    file_list_t *selection_buf = menu_entries_get_selection_buf_ptr(0);
    size_t selection           = menu_navigation_get_selection();
    menu_file_list_cbs_t *cbs  = menu_entries_get_actiondata_at_offset(
            selection_buf, selection);

    if (!cbs)
        return 0;

    ret = menu_action_handle_setting(cbs->setting,
                                     type, action, wraparound);

    if (ret == -1)
        return 0;
    return ret;
}

void *setting_get_ptr(rarch_setting_t *setting)
{
    if (!setting)
        return NULL;

    switch (setting_get_type(setting))
    {
        case ST_BOOL:
            return setting->value.target.boolean;
        case ST_INT:
            return setting->value.target.integer;
        case ST_UINT:
            return setting->value.target.unsigned_integer;
        case ST_FLOAT:
            return setting->value.target.fraction;
        case ST_BIND:
            return setting->value.target.keybind;
        case ST_STRING:
        case ST_STRING_OPTIONS:
        case ST_PATH:
        case ST_DIR:
            return setting->value.target.string;
        default:
            break;
    }

    return NULL;
}

/**
 * setting_get_string_representation:
 * @setting            : pointer to setting
 * @s                  : buffer to write contents of string representation to.
 * @len                : size of the buffer (@s)
 *
 * Get a setting value's string representation.
 **/
void setting_get_string_representation(void *data, char *s, size_t len)
{
    rarch_setting_t* setting = (rarch_setting_t*)data;
    if (!setting || !s)
        return;

    if (setting->get_string_representation)
        setting->get_string_representation(setting, s, len);
}

/**
 * setting_action_start_savestates:
 * @data               : pointer to setting
 *
 * Function callback for 'Savestate' action's 'Action Start'
 * function pointer.
 *
 * Returns: 0 on success, -1 on error.
 **/
static int setting_action_start_bind_device(void *data)
{
    uint32_t index_offset;
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    if (!setting || !settings)
        return -1;

    index_offset = setting->index_offset;

    configuration_set_uint(settings, settings->uints.input_joypad_map[index_offset], index_offset);
    return 0;
}


static int setting_action_start_custom_viewport_width(void *data)
{
    video_viewport_t vp;
    struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
    video_viewport_t            *custom  = video_viewport_get_custom();
    settings_t                 *settings = config_get_ptr();
    struct retro_game_geometry     *geom = (struct retro_game_geometry*)
            &av_info->geometry;

    if (!settings || !av_info)
        return -1;

    video_driver_get_viewport_info(&vp);

    if (settings->bools.video_scale_integer)
        custom->width = ((custom->width + geom->base_width - 1) /
                         geom->base_width) * geom->base_width;
    else
        custom->width = vp.full_width - custom->x;

    aspectratio_lut[ASPECT_RATIO_CUSTOM].value =
            (float)custom->width / custom->height;

    return 0;
}

static int setting_action_start_custom_viewport_height(void *data)
{
    video_viewport_t vp;
    struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
    video_viewport_t            *custom  = video_viewport_get_custom();
    settings_t                 *settings = config_get_ptr();
    struct retro_game_geometry     *geom = (struct retro_game_geometry*)
            &av_info->geometry;

    if (!settings || !av_info)
        return -1;

    video_driver_get_viewport_info(&vp);

    if (settings->bools.video_scale_integer)
        custom->height = ((custom->height + geom->base_height - 1) /
                          geom->base_height) * geom->base_height;
    else
        custom->height = vp.full_height - custom->y;

    aspectratio_lut[ASPECT_RATIO_CUSTOM].value =
            (float)custom->width / custom->height;

    return 0;
}



static int setting_action_start_analog_dpad_mode(void *data)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;

    if (!setting)
        return -1;

    *setting->value.target.unsigned_integer = 0;

    return 0;
}

static int setting_action_start_libretro_device_type(void *data)
{
    retro_ctx_controller_info_t pad;
    unsigned index_offset, current_device;
    unsigned devices[128], types = 0, port = 0;
    const struct retro_controller_info *desc = NULL;
    rarch_system_info_t *system = runloop_get_system_info();
    rarch_setting_t   *setting  = (rarch_setting_t*)data;

    if (setting_generic_action_start_default(setting) != 0)
        return -1;

    index_offset     = setting->index_offset;
    port             = index_offset;

    devices[types++] = RETRO_DEVICE_NONE;
    devices[types++] = RETRO_DEVICE_JOYPAD;

    if (system)
    {
        /* Only push RETRO_DEVICE_ANALOG as default if we use an
       * older core which doesn't use SET_CONTROLLER_INFO. */
        if (!system->ports.size)
            devices[types++] = RETRO_DEVICE_ANALOG;

        if (port < system->ports.size)
            desc = &system->ports.data[port];
    }

    if (desc)
    {
        unsigned i;

        for (i = 0; i < desc->num_types; i++)
        {
            unsigned id = desc->types[i].id;
            if (types < ARRAY_SIZE(devices) &&
                id != RETRO_DEVICE_NONE &&
                id != RETRO_DEVICE_JOYPAD)
                devices[types++] = id;
        }
    }

    current_device = RETRO_DEVICE_JOYPAD;

    input_config_set_device(port, current_device);

    pad.port   = port;
    pad.device = current_device;
    core_set_controller_port_device(&pad);

    return 0;
}

static int setting_action_start_video_refresh_rate_auto(
        void *data)
{
    video_driver_monitor_reset();
    return 0;
}

/**
 ******* ACTION TOGGLE CALLBACK FUNCTIONS *******
**/

static int setting_action_left_analog_dpad_mode(void *data, bool wraparound)
{
    unsigned port = 0;
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    if (!setting)
        return -1;

    port = setting->index_offset;

    configuration_set_uint(settings, settings->uints.input_analog_dpad_mode[port],
                           (settings->uints.input_analog_dpad_mode
                            [port] + ANALOG_DPAD_LAST - 1) % ANALOG_DPAD_LAST);

    return 0;
}

static int setting_action_right_analog_dpad_mode(void *data, bool wraparound)
{
    unsigned port = 0;
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    if (!setting)
        return -1;

    port = setting->index_offset;

    settings->modified                     = true;
    settings->uints.input_analog_dpad_mode[port] =
            (settings->uints.input_analog_dpad_mode[port] + 1)
            % ANALOG_DPAD_LAST;

    return 0;
}

static int setting_action_left_libretro_device_type(
        void *data, bool wraparound)
{
    retro_ctx_controller_info_t pad;
    unsigned current_device, current_idx, i, devices[128],
            types = 0, port = 0;
    const struct retro_controller_info *desc = NULL;
    rarch_setting_t *setting    = (rarch_setting_t*)data;
    rarch_system_info_t *system = NULL;

    if (!setting)
        return -1;

    port = setting->index_offset;

    devices[types++] = RETRO_DEVICE_NONE;
    devices[types++] = RETRO_DEVICE_JOYPAD;

    system           = runloop_get_system_info();

    if (system)
    {
        /* Only push RETRO_DEVICE_ANALOG as default if we use an
       * older core which doesn't use SET_CONTROLLER_INFO. */
        if (!system->ports.size)
            devices[types++] = RETRO_DEVICE_ANALOG;

        if (port < system->ports.size)
            desc = &system->ports.data[port];
    }

    if (desc)
    {
        for (i = 0; i < desc->num_types; i++)
        {
            unsigned id = desc->types[i].id;
            if (types < ARRAY_SIZE(devices) &&
                id != RETRO_DEVICE_NONE &&
                id != RETRO_DEVICE_JOYPAD)
                devices[types++] = id;
        }
    }

    current_device = input_config_get_device(port);
    current_idx    = 0;
    for (i = 0; i < types; i++)
    {
        if (current_device != devices[i])
            continue;

        current_idx = i;
        break;
    }

    current_device = devices
    [(current_idx + types - 1) % types];

    input_config_set_device(port, current_device);

    pad.port   = port;
    pad.device = current_device;

    core_set_controller_port_device(&pad);

    return 0;
}

static int setting_action_right_libretro_device_type(
        void *data, bool wraparound)
{
    retro_ctx_controller_info_t pad;
    unsigned current_device, current_idx, i, devices[128],
            types = 0, port = 0;
    const struct retro_controller_info *desc = NULL;
    rarch_setting_t *setting    = (rarch_setting_t*)data;
    rarch_system_info_t *system = runloop_get_system_info();

    if (!setting)
        return -1;

    port = setting->index_offset;

    devices[types++] = RETRO_DEVICE_NONE;
    devices[types++] = RETRO_DEVICE_JOYPAD;

    if (system)
    {
        /* Only push RETRO_DEVICE_ANALOG as default if we use an
       * older core which doesn't use SET_CONTROLLER_INFO. */
        if (!system->ports.size)
            devices[types++] = RETRO_DEVICE_ANALOG;

        if (port < system->ports.size)
            desc = &system->ports.data[port];
    }

    if (desc)
    {
        for (i = 0; i < desc->num_types; i++)
        {
            unsigned id = desc->types[i].id;
            if (types < ARRAY_SIZE(devices) &&
                id != RETRO_DEVICE_NONE &&
                id != RETRO_DEVICE_JOYPAD)
                devices[types++] = id;
        }
    }

    current_device = input_config_get_device(port);
    current_idx    = 0;
    for (i = 0; i < types; i++)
    {
        if (current_device != devices[i])
            continue;

        current_idx = i;
        break;
    }

    current_device = devices
    [(current_idx + 1) % types];

    input_config_set_device(port, current_device);

    pad.port   = port;
    pad.device = current_device;

    core_set_controller_port_device(&pad);

    return 0;
}

static int setting_action_left_bind_device(void *data, bool wraparound)
{
    unsigned index_offset;
    unsigned               *p = NULL;
    unsigned max_users        = *(input_driver_get_uint(INPUT_ACTION_MAX_USERS));
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    if (!setting)
        return -1;

    index_offset = setting->index_offset;

    p = &settings->uints.input_joypad_map[index_offset];

    if ((*p) >= max_users)
        *p = max_users - 1;
    else if ((*p) > 0)
        (*p)--;

    return 0;
}

static int setting_action_right_bind_device(void *data, bool wraparound)
{
    unsigned index_offset;
    unsigned               *p = NULL;
    unsigned max_users        = *(input_driver_get_uint(INPUT_ACTION_MAX_USERS));
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    if (!setting)
        return -1;

    index_offset = setting->index_offset;

    p = &settings->uints.input_joypad_map[index_offset];

    if (*p < max_users)
        (*p)++;

    return 0;
}



/**
 ******* ACTION OK CALLBACK FUNCTIONS *******
**/

static int setting_action_ok_bind_all(void *data, bool wraparound)
{
    (void)wraparound;
    if (!menu_input_key_bind_set_mode(MENU_INPUT_BINDS_CTL_BIND_ALL, data))
        return -1;
    return 0;
}

static int setting_action_ok_bind_all_save_autoconfig(void *data, bool wraparound)
{
    unsigned index_offset;
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    const char *name          = NULL;

    (void)wraparound;

    if (!setting)
        return -1;

    index_offset = setting->index_offset;
    name         = input_config_get_device_name(index_offset);

    if(!string_is_empty(name) && config_save_autoconf_profile(name, index_offset))
        runloop_msg_queue_push(
                msg_hash_to_str(MSG_AUTOCONFIG_FILE_SAVED_SUCCESSFULLY), 1, 100, true);
    else
        runloop_msg_queue_push(
                msg_hash_to_str(MSG_AUTOCONFIG_FILE_ERROR_SAVING), 1, 100, true);


    return 0;
}

static int setting_action_ok_bind_defaults(void *data, bool wraparound)
{
    unsigned i;
    menu_input_ctx_bind_limits_t lim;
    struct retro_keybind *target          = NULL;
    const struct retro_keybind *def_binds = NULL;
    rarch_setting_t *setting              = (rarch_setting_t*)data;

    (void)wraparound;

    if (!setting)
        return -1;

    target    =  &input_config_binds[setting->index_offset][0];
    def_binds =  (setting->index_offset) ?
                 retro_keybinds_rest : retro_keybinds_1;

    lim.min   = MENU_SETTINGS_BIND_BEGIN;
    lim.max   = MENU_SETTINGS_BIND_LAST;

    menu_input_key_bind_set_min_max(&lim);

    for (i = MENU_SETTINGS_BIND_BEGIN;
         i <= MENU_SETTINGS_BIND_LAST; i++, target++)
    {
        target->key     = def_binds[i - MENU_SETTINGS_BIND_BEGIN].key;
        target->joykey  = NO_BTN;
        target->joyaxis = AXIS_NONE;
    }

    return 0;
}

static void setting_get_string_representation_st_float_video_refresh_rate_auto(void *data,
                                                                               char *s, size_t len)
{
    double video_refresh_rate = 0.0;
    double deviation          = 0.0;
    unsigned sample_points    = 0;
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    if (!setting)
        return;

    if (video_monitor_fps_statistics(&video_refresh_rate, &deviation, &sample_points))
    {
        snprintf(s, len, "%.3f Hz (%.1f%% dev, %u samples)",
                 video_refresh_rate, 100.0 * deviation, sample_points);
        menu_animation_ctl(MENU_ANIMATION_CTL_SET_ACTIVE, NULL);
    }
    else
        strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NOT_AVAILABLE), len);
}

static int setting_action_ok_video_refresh_rate_auto(void *data, bool wraparound)
{
    double video_refresh_rate = 0.0;
    double deviation          = 0.0;
    unsigned sample_points    = 0;
    rarch_setting_t *setting  = (rarch_setting_t*)data;

    if (!setting)
        return -1;

    if (video_monitor_fps_statistics(&video_refresh_rate,
                                     &deviation, &sample_points))
    {
        float video_refresh_rate_float = (float)video_refresh_rate;
        driver_ctl(RARCH_DRIVER_CTL_SET_REFRESH_RATE, &video_refresh_rate_float);
        /* Incase refresh rate update forced non-block video. */
        command_event(CMD_EVENT_VIDEO_SET_BLOCKING_STATE, NULL);
    }

    if (setting_generic_action_ok_default(setting, wraparound) != 0)
        return -1;

    return 0;
}

static void get_string_representation_bind_device(void * data, char *s,
                                                  size_t len)
{
    unsigned index_offset, map = 0;
    unsigned max_users        = *(input_driver_get_uint(INPUT_ACTION_MAX_USERS));
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    if (!setting)
        return;

    index_offset = setting->index_offset;
    map          = settings->uints.input_joypad_map[index_offset];

    if (map < max_users)
    {
        const char *device_name = input_config_get_device_name(map);

        if (!string_is_empty(device_name))
            snprintf(s, len,
                     "%s (#%u)",
                     device_name,
                     input_autoconfigure_get_device_name_index(map));
        else
            snprintf(s, len,
                     "%s (%s #%u)",
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NOT_AVAILABLE),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_PORT),
                     map);
    }
    else
        strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_DISABLED), len);
}


/**
 * menu_setting_get_label:
 * @list               : File list on which to perform the search
 * @s                  : String for the type to be represented on-screen as
 *                       a label.
 * @len                : Size of @s
 * @w                  : Width of the string (for text label representation
 *                       purposes in the menu display driver).
 * @type               : Identifier of setting.
 * @menu_label         : Menu Label identifier of setting.
 * @label              : Label identifier of setting.
 * @idx                : Index identifier of setting.
 *
 * Get associated label of a setting.
 **/
void menu_setting_get_label(void *data, char *s,
                            size_t len, unsigned *w, unsigned type,
                            const char *menu_label, const char *label, unsigned idx)
{
    rarch_setting_t *setting = NULL;
    file_list_t *list        = (file_list_t*)data;
    if (!list || !label)
        return;

    setting = menu_setting_find(list->list[idx].label);

    if (setting && setting->get_string_representation)
        setting->get_string_representation(setting, s, len);
}

void general_read_handler(void *data)
{
    rarch_setting_t *setting  = (rarch_setting_t*)data;
    settings_t      *settings = config_get_ptr();

    if (!setting)
        return;

    if (setting->enum_idx != MSG_UNKNOWN)
    {
        switch (setting->enum_idx)
        {
            case MENU_ENUM_LABEL_AUDIO_RATE_CONTROL_DELTA:
                *setting->value.target.fraction = *(audio_get_float_ptr(AUDIO_ACTION_RATE_CONTROL_DELTA));
                if (*setting->value.target.fraction < 0.0005)
                {
                    configuration_set_bool(settings, settings->bools.audio_rate_control, false);
                    audio_set_float(AUDIO_ACTION_RATE_CONTROL_DELTA, 0.0f);
                }
                else
                {
                    configuration_set_bool(settings, settings->bools.audio_rate_control, true);
                    audio_set_float(AUDIO_ACTION_RATE_CONTROL_DELTA, *setting->value.target.fraction);
                }
                break;
            case MENU_ENUM_LABEL_AUDIO_MAX_TIMING_SKEW:
                *setting->value.target.fraction = settings->floats.audio_max_timing_skew;
                break;
            case MENU_ENUM_LABEL_VIDEO_REFRESH_RATE_AUTO:
                *setting->value.target.fraction = settings->floats.video_refresh_rate;
                break;
            case MENU_ENUM_LABEL_INPUT_PLAYER1_JOYPAD_INDEX:
                *setting->value.target.integer = settings->uints.input_joypad_map[0];
                break;
            case MENU_ENUM_LABEL_INPUT_PLAYER2_JOYPAD_INDEX:
                *setting->value.target.integer = settings->uints.input_joypad_map[1];
                break;
            case MENU_ENUM_LABEL_INPUT_PLAYER3_JOYPAD_INDEX:
                *setting->value.target.integer = settings->uints.input_joypad_map[2];
                break;
            case MENU_ENUM_LABEL_INPUT_PLAYER4_JOYPAD_INDEX:
                *setting->value.target.integer = settings->uints.input_joypad_map[3];
                break;
            case MENU_ENUM_LABEL_INPUT_PLAYER5_JOYPAD_INDEX:
                *setting->value.target.integer = settings->uints.input_joypad_map[4];
                break;
            default:
                break;
        }
    }
}

void general_write_handler(void *data)
{
    enum event_command rarch_cmd = CMD_EVENT_NONE;
    rarch_setting_t *setting     = (rarch_setting_t*)data;
    settings_t *settings         = config_get_ptr();

    if (!setting)
        return;

    if (setting->cmd_trigger.idx != CMD_EVENT_NONE)
    {
        uint64_t flags = setting->flags;

        if (flags & SD_FLAG_EXIT)
        {
            if (*setting->value.target.boolean)
                *setting->value.target.boolean = false;
        }
        if (setting->cmd_trigger.triggered ||
            (flags & SD_FLAG_CMD_APPLY_AUTO))
            rarch_cmd = setting->cmd_trigger.idx;
    }

    switch (setting->enum_idx)
    {
        case MENU_ENUM_LABEL_VIDEO_THREADED:
        {
            if (*setting->value.target.boolean)
                task_queue_set_threaded();
            else
                task_queue_unset_threaded();
        }
            break;
        case MENU_ENUM_LABEL_INPUT_POLL_TYPE_BEHAVIOR:
            core_set_poll_type((unsigned int*)setting->value.target.integer);
            break;
        case MENU_ENUM_LABEL_VIDEO_SCALE_INTEGER:
        {
            video_viewport_t vp;
            struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
            video_viewport_t            *custom  = video_viewport_get_custom();
            struct retro_game_geometry     *geom = (struct retro_game_geometry*)
                    &av_info->geometry;

            video_driver_get_viewport_info(&vp);

            if (*setting->value.target.boolean)
            {
                custom->x      = 0;
                custom->y      = 0;
                custom->width  = ((custom->width + geom->base_width   - 1) / geom->base_width)  * geom->base_width;
                custom->height = ((custom->height + geom->base_height - 1) / geom->base_height) * geom->base_height;
                aspectratio_lut[ASPECT_RATIO_CUSTOM].value =
                        (float)custom->width / custom->height;
            }
        }
            break;
        case MENU_ENUM_LABEL_HELP:
            if (*setting->value.target.boolean)
            {
                menu_displaylist_info_t info = {0};
                file_list_t *menu_stack      = menu_entries_get_menu_stack_ptr(0);

                info.list          = menu_stack;
                info.type          = 0;
                info.directory_ptr = 0;
                strlcpy(info.label,
                        msg_hash_to_str(MENU_ENUM_LABEL_HELP), sizeof(info.label));
                info.enum_idx      = MENU_ENUM_LABEL_HELP;

                if (menu_displaylist_ctl(DISPLAYLIST_GENERIC, &info))
                    menu_displaylist_process(&info);
                setting_set_with_string_representation(setting, "false");
            }
            break;
        case MENU_ENUM_LABEL_AUDIO_MAX_TIMING_SKEW:
        configuration_set_float(settings, settings->floats.audio_max_timing_skew,
                                *setting->value.target.fraction);
            break;
        case MENU_ENUM_LABEL_AUDIO_RATE_CONTROL_DELTA:
            if (*setting->value.target.fraction < 0.0005)
            {
                configuration_set_bool(settings, settings->bools.audio_rate_control, false);
                audio_set_float(AUDIO_ACTION_RATE_CONTROL_DELTA, 0.0f);
            }
            else
            {
                configuration_set_bool(settings, settings->bools.audio_rate_control, true);
                audio_set_float(AUDIO_ACTION_RATE_CONTROL_DELTA, *setting->value.target.fraction);
            }
            break;
        case MENU_ENUM_LABEL_VIDEO_REFRESH_RATE_AUTO:
            driver_ctl(RARCH_DRIVER_CTL_SET_REFRESH_RATE, setting->value.target.fraction);

            /* In case refresh rate update forced non-block video. */
            rarch_cmd = CMD_EVENT_VIDEO_SET_BLOCKING_STATE;
            break;
        case MENU_ENUM_LABEL_VIDEO_SCALE:
            settings->modified           = true;
            settings->floats.video_scale = roundf(*setting->value.target.fraction);

            if (!settings->bools.video_fullscreen)
                rarch_cmd = CMD_EVENT_REINIT;
            break;
        case MENU_ENUM_LABEL_INPUT_PLAYER1_JOYPAD_INDEX:
            settings->modified            = true;
            settings->uints.input_joypad_map[0] = *setting->value.target.integer;
            break;
        case MENU_ENUM_LABEL_INPUT_PLAYER2_JOYPAD_INDEX:
            settings->modified            = true;
            settings->uints.input_joypad_map[1] = *setting->value.target.integer;
            break;
        case MENU_ENUM_LABEL_INPUT_PLAYER3_JOYPAD_INDEX:
            settings->modified            = true;
            settings->uints.input_joypad_map[2] = *setting->value.target.integer;
            break;
        case MENU_ENUM_LABEL_INPUT_PLAYER4_JOYPAD_INDEX:
            settings->modified            = true;
            settings->uints.input_joypad_map[3] = *setting->value.target.integer;
            break;
        case MENU_ENUM_LABEL_INPUT_PLAYER5_JOYPAD_INDEX:
            settings->modified            = true;
            settings->uints.input_joypad_map[4] = *setting->value.target.integer;
            break;
        case MENU_ENUM_LABEL_LOG_VERBOSITY:
            if (setting
                && setting->value.target.boolean
                && *setting->value.target.boolean)
                verbosity_enable();
            else
                verbosity_disable();

            if (setting
                && setting->value.target.boolean
                && *setting->value.target.boolean)
                retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_VERBOSITY, NULL);
            else
                retroarch_override_setting_unset(RARCH_OVERRIDE_SETTING_VERBOSITY, NULL);
            break;
        case MENU_ENUM_LABEL_VIDEO_SMOOTH:
            video_driver_set_filtering(1, settings->bools.video_smooth);
            break;
        case MENU_ENUM_LABEL_VIDEO_ROTATION:
        {
            rarch_system_info_t *system = runloop_get_system_info();

            if (system)
                video_driver_set_rotation(
                        (*setting->value.target.unsigned_integer +
                         system->rotation) % 4);
        }
            break;
        case MENU_ENUM_LABEL_AUDIO_VOLUME:
            audio_driver_set_volume_gain(db_to_gain(*setting->value.target.fraction));
            break;
        case MENU_ENUM_LABEL_AUDIO_LATENCY:
        case MENU_ENUM_LABEL_AUDIO_DEVICE:
        case MENU_ENUM_LABEL_AUDIO_OUTPUT_RATE:
        case MENU_ENUM_LABEL_AUDIO_WASAPI_EXCLUSIVE_MODE:
        case MENU_ENUM_LABEL_AUDIO_WASAPI_FLOAT_FORMAT:
        case MENU_ENUM_LABEL_AUDIO_WASAPI_SH_BUFFER_LENGTH:
            rarch_cmd = CMD_EVENT_AUDIO_REINIT;
            break;
        case MENU_ENUM_LABEL_PAL60_ENABLE:
        {
            global_t *global             = global_get_ptr();
            if (*setting->value.target.boolean && global->console.screen.pal_enable)
                rarch_cmd = CMD_EVENT_REINIT;
            else
                setting_set_with_string_representation(setting, "false");
        }
            break;
        case MENU_ENUM_LABEL_SYSTEM_BGM_ENABLE:
            if (*setting->value.target.boolean)
            {
#if defined(__CELLOS_LV2__) && (CELL_SDK_VERSION > 0x340000)
                cellSysutilEnableBgmPlayback();
#endif
            }
            else
            {
#if defined(__CELLOS_LV2__) && (CELL_SDK_VERSION > 0x340000)
                cellSysutilDisableBgmPlayback();
#endif
            }
            break;
        case MENU_ENUM_LABEL_NETPLAY_IP_ADDRESS:
#ifdef HAVE_NETWORKING
        {
            bool val = (!string_is_empty(setting->value.target.string));
            if (val)
                retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_NETPLAY_IP_ADDRESS, NULL);
            else
                retroarch_override_setting_unset(RARCH_OVERRIDE_SETTING_NETPLAY_IP_ADDRESS, NULL);
        }
#endif
            break;
        case MENU_ENUM_LABEL_NETPLAY_MODE:
#ifdef HAVE_NETWORKING
            retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_NETPLAY_MODE, NULL);
#endif
            break;
        case MENU_ENUM_LABEL_NETPLAY_STATELESS_MODE:
#ifdef HAVE_NETWORKING
            retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_NETPLAY_STATELESS_MODE, NULL);
#endif
            break;
        case MENU_ENUM_LABEL_NETPLAY_CHECK_FRAMES:
#ifdef HAVE_NETWORKING
            retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_NETPLAY_CHECK_FRAMES, NULL);
#endif
            break;
        default:
            break;
    }

    if (rarch_cmd || setting->cmd_trigger.triggered)
        command_event(rarch_cmd, NULL);
}

#ifdef HAVE_OVERLAY
static void overlay_enable_toggle_change_handler(void *data)
{
    settings_t *settings  = config_get_ptr();
    rarch_setting_t *setting = (rarch_setting_t *)data;

    if (!setting)
        return;

    if (settings && settings->bools.input_overlay_hide_in_menu)
    {
        command_event(CMD_EVENT_OVERLAY_DEINIT, NULL);
        return;
    }

    if (setting->value.target.boolean)
        command_event(CMD_EVENT_OVERLAY_INIT, NULL);
    else
        command_event(CMD_EVENT_OVERLAY_DEINIT, NULL);
}
#endif

#ifdef HAVE_LAKKA
static void systemd_service_toggle(const char *path, char *unit, bool enable)
{
   int pid = fork();
   char* args[] = {(char*)"systemctl", NULL, NULL, NULL};

   if (enable)
      args[1] = (char*)"start";
   else
      args[1] = (char*)"stop";
   args[2] = unit;

   if (enable)
      fclose(fopen(path, "w"));
   else
      remove(path);

   if (pid == 0)
      execvp(args[0], args);

   return;
}

static void ssh_enable_toggle_change_handler(void *data)
{
   bool enable           = false;
   settings_t *settings  = config_get_ptr();
   if (settings && settings->bools.ssh_enable)
      enable = true;

   systemd_service_toggle(LAKKA_SSH_PATH, (char*)"sshd.service",
         enable);
}

static void samba_enable_toggle_change_handler(void *data)
{
   bool enable           = false;
   settings_t *settings  = config_get_ptr();
   if (settings && settings->bools.samba_enable)
      enable = true;

   systemd_service_toggle(LAKKA_SAMBA_PATH, (char*)"smbd.service",
         enable);
}

static void bluetooth_enable_toggle_change_handler(void *data)
{
   bool enable           = false;
   settings_t *settings  = config_get_ptr();
   if (settings && settings->bools.bluetooth_enable)
      enable = true;

   systemd_service_toggle(LAKKA_BLUETOOTH_PATH, (char*)"bluetooth.service",
         enable);
}
#endif

enum settings_list_type
{
    SETTINGS_LIST_NONE = 0,
    SETTINGS_LIST_MAIN_MENU,
};

static bool setting_append_list_input_player_options(
        rarch_setting_t **list,
        rarch_setting_info_t *list_info,
        const char *parent_group,
        unsigned user)
{
    /* This constants matches the string length.
    * Keep it up to date or you'll get some really obvious bugs.
    * 2 is the length of '99'; we don't need more users than that.
    */
    static char buffer[MAX_USERS][13+2+1];
    static char group_lbl[MAX_USERS][255];
    unsigned i;
    rarch_setting_group_info_t group_info      = {0};
    rarch_setting_group_info_t subgroup_info   = {0};
    settings_t *settings                       = config_get_ptr();
    rarch_system_info_t *system                = runloop_get_system_info();
    const struct retro_keybind* const defaults =
            (user == 0) ? retro_keybinds_1 : retro_keybinds_rest;
    const char *temp_value                     = msg_hash_to_str
            ((enum msg_hash_enums)(MENU_ENUM_LABEL_INPUT_USER_1_BINDS + user));

    snprintf(buffer[user],    sizeof(buffer[user]),
             "%s %u", msg_hash_to_str(MENU_ENUM_LABEL_VALUE_USER), user + 1);

    strlcpy(group_lbl[user], temp_value, sizeof(group_lbl[user]));

    START_GROUP(list, list_info, &group_info, group_lbl[user], parent_group);

    parent_group = msg_hash_to_str(MENU_ENUM_LABEL_SETTINGS);

    START_SUB_GROUP(
            list,
            list_info,
            buffer[user],
            &group_info,
            &subgroup_info,
            parent_group);

    {
        char tmp_string[PATH_MAX_LENGTH];
        /* These constants match the string lengths.
       * Keep them up to date or you'll get some really obvious bugs.
       * 2 is the length of '99'; we don't need more users than that.
       */
        /* FIXME/TODO - really need to clean up this mess in some way. */
        static char key[MAX_USERS][64];
        static char key_type[MAX_USERS][64];
        static char key_analog[MAX_USERS][64];
        static char key_bind_all[MAX_USERS][64];
        static char key_bind_all_save_autoconfig[MAX_USERS][64];
        static char key_bind_defaults[MAX_USERS][64];

        static char label[MAX_USERS][64];
        static char label_type[MAX_USERS][64];
        static char label_analog[MAX_USERS][64];
        static char label_bind_all[MAX_USERS][64];
        static char label_bind_all_save_autoconfig[MAX_USERS][64];
        static char label_bind_defaults[MAX_USERS][64];

        tmp_string[0] = '\0';

        snprintf(tmp_string, sizeof(tmp_string), "input_player%u", user + 1);

        fill_pathname_join_delim(key[user], tmp_string, "joypad_index", '_',
                                 sizeof(key[user]));
        snprintf(key_type[user], sizeof(key_type[user]),
                 msg_hash_to_str(MENU_ENUM_LABEL_INPUT_LIBRETRO_DEVICE),
                 user + 1);
        snprintf(key_analog[user], sizeof(key_analog[user]),
                 msg_hash_to_str(MENU_ENUM_LABEL_INPUT_PLAYER_ANALOG_DPAD_MODE),
                 user + 1);
        fill_pathname_join_delim(key_bind_all[user], tmp_string, "bind_all", '_',
                                 sizeof(key_bind_all[user]));
        fill_pathname_join_delim(key_bind_all_save_autoconfig[user],
                                 tmp_string, "bind_all_save_autoconfig", '_',
                                 sizeof(key_bind_all_save_autoconfig[user]));
        fill_pathname_join_delim(key_bind_defaults[user],
                                 tmp_string, "bind_defaults", '_',
                                 sizeof(key_bind_defaults[user]));

        snprintf(label[user], sizeof(label[user]),
                 "%s %u %s", msg_hash_to_str(MENU_ENUM_LABEL_VALUE_USER), user + 1,
                 msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_DEVICE_INDEX));
        snprintf(label_type[user], sizeof(label_type[user]),
                 "%s %u %s", msg_hash_to_str(MENU_ENUM_LABEL_VALUE_USER), user + 1,
                 msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_DEVICE_TYPE));
        snprintf(label_analog[user], sizeof(label_analog[user]),
                 "%s %u %s", msg_hash_to_str(MENU_ENUM_LABEL_VALUE_USER), user + 1,
                 msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_ADC_TYPE));
        snprintf(label_bind_all[user], sizeof(label_bind_all[user]),
                 "%s %u %s", msg_hash_to_str(MENU_ENUM_LABEL_VALUE_USER), user + 1,
                 msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_BIND_ALL));
        snprintf(label_bind_defaults[user], sizeof(label_bind_defaults[user]),
                 "%s %u %s", msg_hash_to_str(MENU_ENUM_LABEL_VALUE_USER), user + 1,
                 msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_BIND_DEFAULT_ALL));
        snprintf(label_bind_all_save_autoconfig[user], sizeof(label_bind_all_save_autoconfig[user]),
                 "%s %u %s", msg_hash_to_str(MENU_ENUM_LABEL_VALUE_USER), user + 1,
                 msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_SAVE_AUTOCONFIG));

        CONFIG_UINT_ALT(
                list, list_info,
                input_config_get_device_ptr(user),
                key_type[user],
                label_type[user],
                user,
                &group_info,
                &subgroup_info,
                parent_group,
                general_write_handler,
                general_read_handler);
        (*list)[list_info->index - 1].index = user + 1;
        (*list)[list_info->index - 1].index_offset = user;
        (*list)[list_info->index - 1].action_left   = &setting_action_left_libretro_device_type;
        (*list)[list_info->index - 1].action_right  = &setting_action_right_libretro_device_type;
        (*list)[list_info->index - 1].action_select = &setting_action_right_libretro_device_type;
        (*list)[list_info->index - 1].action_start  = &setting_action_start_libretro_device_type;
        (*list)[list_info->index - 1].get_string_representation =
                &setting_get_string_representation_uint_libretro_device;
        menu_settings_list_current_add_enum_idx(list, list_info, (enum msg_hash_enums)(MENU_ENUM_LABEL_INPUT_LIBRETRO_DEVICE + user));

        CONFIG_UINT_ALT(
                list, list_info,
                &settings->uints.input_analog_dpad_mode[user],
                key_analog[user],
                label_analog[user],
                user,
                &group_info,
                &subgroup_info,
                parent_group,
                general_write_handler,
                general_read_handler);
        (*list)[list_info->index - 1].index = user + 1;
        (*list)[list_info->index - 1].index_offset = user;
        (*list)[list_info->index - 1].action_left   = &setting_action_left_analog_dpad_mode;
        (*list)[list_info->index - 1].action_right  = &setting_action_right_analog_dpad_mode;
        (*list)[list_info->index - 1].action_select = &setting_action_right_analog_dpad_mode;
        (*list)[list_info->index - 1].action_start = &setting_action_start_analog_dpad_mode;
        (*list)[list_info->index - 1].get_string_representation =
                &setting_get_string_representation_uint_analog_dpad_mode;
        menu_settings_list_current_add_enum_idx(list, list_info, (enum msg_hash_enums)(MENU_ENUM_LABEL_INPUT_PLAYER_ANALOG_DPAD_MODE + user));

        CONFIG_ACTION_ALT(
                list, list_info,
                key[user],
                label[user],
                &group_info,
                &subgroup_info,
                parent_group);
        (*list)[list_info->index - 1].index = user + 1;
        (*list)[list_info->index - 1].index_offset = user;
        (*list)[list_info->index - 1].action_start  = &setting_action_start_bind_device;
        (*list)[list_info->index - 1].action_left   = &setting_action_left_bind_device;
        (*list)[list_info->index - 1].action_right  = &setting_action_right_bind_device;
        (*list)[list_info->index - 1].action_select = &setting_action_right_bind_device;
        (*list)[list_info->index - 1].get_string_representation = &get_string_representation_bind_device;

        CONFIG_ACTION_ALT(
                list, list_info,
                key_bind_all[user],
                label_bind_all[user],
                &group_info,
                &subgroup_info,
                parent_group);
        (*list)[list_info->index - 1].index          = user + 1;
        (*list)[list_info->index - 1].index_offset   = user;
        (*list)[list_info->index - 1].action_ok      = &setting_action_ok_bind_all;
        (*list)[list_info->index - 1].action_cancel  = NULL;

        CONFIG_ACTION_ALT(
                list, list_info,
                key_bind_defaults[user],
                label_bind_defaults[user],
                &group_info,
                &subgroup_info,
                parent_group);
        (*list)[list_info->index - 1].index          = user + 1;
        (*list)[list_info->index - 1].index_offset   = user;
        (*list)[list_info->index - 1].action_ok      = &setting_action_ok_bind_defaults;
        (*list)[list_info->index - 1].action_cancel  = NULL;

        CONFIG_ACTION_ALT(
                list, list_info,
                key_bind_all_save_autoconfig[user],
                label_bind_all_save_autoconfig[user],
                &group_info,
                &subgroup_info,
                parent_group);
        (*list)[list_info->index - 1].index          = user + 1;
        (*list)[list_info->index - 1].index_offset   = user;
        (*list)[list_info->index - 1].action_ok      = &setting_action_ok_bind_all_save_autoconfig;
        (*list)[list_info->index - 1].action_cancel  = NULL;
    }

    for (i = 0; i < RARCH_BIND_LIST_END; i ++)
    {
        char label[255];
        char name[255];

        if (input_config_bind_map_get_meta(i))
            continue;

        label[0] = name[0]          = '\0';

        fill_pathname_noext(label, buffer[user],
                            " ",
                            sizeof(label));

        if (
                settings->bools.input_descriptor_label_show
                && (i < RARCH_FIRST_META_KEY)
                && core_has_set_input_descriptor()
                && (i != RARCH_TURBO_ENABLE)
                )
        {
            if (system->input_desc_btn[user][i])
                strlcat(label,
                        system->input_desc_btn[user][i],
                        sizeof(label));
            else
            {
                strlcat(label, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NOT_AVAILABLE),
                        sizeof(label));

                if (settings->bools.input_descriptor_hide_unbound)
                    continue;
            }
        }
        else
            strlcat(label, input_config_bind_map_get_desc(i), sizeof(label));

        snprintf(name, sizeof(name), "p%u_%s", user + 1, input_config_bind_map_get_base(i));

        CONFIG_BIND_ALT(
                list, list_info,
                &input_config_binds[user][i],
                user + 1,
                user,
                strdup(name),
                strdup(label),
                &defaults[i],
                &group_info,
                &subgroup_info,
                parent_group);
        (*list)[list_info->index - 1].bind_type = i + MENU_SETTINGS_BIND_BEGIN;
    }

    END_SUB_GROUP(list, list_info, parent_group);
    END_GROUP(list, list_info, parent_group);

    return true;
}

/**
 * config_get_audio_resampler_driver_options:
 *
 * Get an enumerated list of all resampler driver names, separated by '|'.
 *
 * Returns: string listing of all resampler driver names, separated by '|'.
 **/
static const char* config_get_audio_resampler_driver_options(void)
{
    return char_list_new_special(STRING_LIST_AUDIO_RESAMPLER_DRIVERS, NULL);
}

static int directory_action_start_generic(void *data)
{
    rarch_setting_t *setting = (rarch_setting_t*)data;

    if (!setting)
        return -1;

    setting_set_with_string_representation(setting,
                                           setting->default_value.string);

    return 0;
}

static bool setting_append_list(
        enum settings_list_type type,
        rarch_setting_t **list,
        rarch_setting_info_t *list_info,
        const char *parent_group)
{
    unsigned user;
    rarch_setting_group_info_t group_info    = {0};
    rarch_setting_group_info_t subgroup_info = {0};
    settings_t *settings                     = config_get_ptr();
    global_t   *global                       = global_get_ptr();

    (void)settings;
    (void)global;

    switch (type)
    {
        case SETTINGS_LIST_MAIN_MENU:
            START_GROUP(list, list_info, &group_info, msg_hash_to_str(MENU_ENUM_LABEL_MAIN_MENU), parent_group);
            menu_settings_list_current_add_enum_idx(list, list_info, MENU_ENUM_LABEL_MAIN_MENU);
            START_SUB_GROUP(list, list_info, "State", &group_info, &subgroup_info, parent_group);

            CONFIG_INT(
                    list, list_info,
                    &settings->ints.state_slot,
                    MENU_ENUM_LABEL_STATE_SLOT,
                    MENU_ENUM_LABEL_VALUE_STATE_SLOT,
                    0,
                    &group_info,
                    &subgroup_info,
                    parent_group,
                    general_write_handler,
                    general_read_handler);
            menu_settings_list_current_add_range(list, list_info, -1, 0, 1, true, false);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_START_CORE,
                    MENU_ENUM_LABEL_VALUE_START_CORE,
                    &group_info,
                    &subgroup_info,
                    parent_group);


            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_CONTENT_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_CONTENT_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);

            {
                char ext_name[255];

                ext_name[0] = '\0';

                if (frontend_driver_get_core_extension(ext_name, sizeof(ext_name)))
                {
                    CONFIG_ACTION(
                            list, list_info,
                            MENU_ENUM_LABEL_CORE_LIST,
                            MENU_ENUM_LABEL_VALUE_CORE_LIST,
                            &group_info,
                            &subgroup_info,
                            parent_group);
                    (*list)[list_info->index - 1].size                = (uint32_t)path_get_realsize(RARCH_PATH_CORE);
                    (*list)[list_info->index - 1].value.target.string = path_get_ptr(RARCH_PATH_CORE);
                    (*list)[list_info->index - 1].values       = ext_name;
                    menu_settings_list_current_add_cmd(list, list_info, CMD_EVENT_LOAD_CORE);
                    settings_data_list_current_add_flags(list, list_info, SD_FLAG_BROWSER_ACTION);
                }
            }

         CONFIG_ACTION(
               list, list_info,
               MENU_ENUM_LABEL_LOAD_CONTENT_LIST,
               MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_LIST,
               &group_info,
               &subgroup_info,
               parent_group);

            if (string_is_not_equal_fast(settings->arrays.menu_driver, "xmb", 3))
            {
                CONFIG_ACTION(
                        list, list_info,
                        MENU_ENUM_LABEL_ADD_CONTENT_LIST,
                        MENU_ENUM_LABEL_VALUE_ADD_CONTENT_LIST,
                        &group_info,
                        &subgroup_info,
                        parent_group);
            }

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_INFORMATION_LIST,
                    MENU_ENUM_LABEL_VALUE_INFORMATION_LIST,
                    &group_info,
                    &subgroup_info,
                    parent_group);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_RESTART_RETROARCH,
                    MENU_ENUM_LABEL_VALUE_RESTART_RETROARCH,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            menu_settings_list_current_add_cmd(list, list_info, CMD_EVENT_RESTART_RETROARCH);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_CONFIGURATIONS,
                    MENU_ENUM_LABEL_VALUE_CONFIGURATIONS,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_SAVE_CURRENT_CONFIG,
                    MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            menu_settings_list_current_add_cmd(list, list_info, CMD_EVENT_MENU_SAVE_CURRENT_CONFIG);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_SAVE_NEW_CONFIG,
                    MENU_ENUM_LABEL_VALUE_SAVE_NEW_CONFIG,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            menu_settings_list_current_add_cmd(list, list_info, CMD_EVENT_MENU_SAVE_CONFIG);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_SAVE_CURRENT_CONFIG_OVERRIDE_CORE,
                    MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG_OVERRIDE_CORE,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            menu_settings_list_current_add_cmd(list, list_info, CMD_EVENT_MENU_SAVE_CURRENT_CONFIG_OVERRIDE_CORE);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_SAVE_CURRENT_CONFIG_OVERRIDE_GAME,
                    MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG_OVERRIDE_GAME,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            menu_settings_list_current_add_cmd(list, list_info, CMD_EVENT_MENU_SAVE_CURRENT_CONFIG_OVERRIDE_GAME);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            /* Apple rejects iOS apps that lets you forcibly quit an application. */
            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_QUIT_RETROARCH,
                    MENU_ENUM_LABEL_VALUE_QUIT_RETROARCH,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            menu_settings_list_current_add_cmd(list, list_info, CMD_EVENT_QUIT);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_CORE_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_CORE_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_ONSCREEN_OVERLAY_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_ONSCREEN_OVERLAY_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_ONSCREEN_NOTIFICATIONS_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_ONSCREEN_NOTIFICATIONS_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_MENU_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_MENU_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);

            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_MENU_FILE_BROWSER_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_MENU_FILE_BROWSER_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_UPDATER_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_UPDATER_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_ADVANCED);

            if (string_is_not_equal_fast(settings->arrays.wifi_driver, "null", 4))
            {
                CONFIG_ACTION(
                        list, list_info,
                        MENU_ENUM_LABEL_WIFI_SETTINGS,
                        MENU_ENUM_LABEL_VALUE_WIFI_SETTINGS,
                        &group_info,
                        &subgroup_info,
                        parent_group);
            }

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_NETWORK_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_NETWORK_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_DIRECTORY_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_DIRECTORY_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);
            settings_data_list_current_add_flags(list, list_info, SD_FLAG_LAKKA_ADVANCED);

            CONFIG_ACTION(
                    list, list_info,
                    MENU_ENUM_LABEL_PRIVACY_SETTINGS,
                    MENU_ENUM_LABEL_VALUE_PRIVACY_SETTINGS,
                    &group_info,
                    &subgroup_info,
                    parent_group);

//            for (user = 0; user < MAX_USERS; user++)
//                setting_append_list_input_player_options(list, list_info, parent_group, user);

            END_SUB_GROUP(list, list_info, parent_group);
            END_GROUP(list, list_info, parent_group);
            break;

        case SETTINGS_LIST_NONE:
        default:
            break;
    }

    return true;
}

bool menu_setting_free(void *data)
{
    unsigned values, n;
    rarch_setting_t *setting = (rarch_setting_t*)data;
    rarch_setting_t **list = &setting;

    if (!setting)
        return false;

    /* Free data which was previously tagged */
    for (; setting_get_type(setting) != ST_NONE; (*list = *list + 1))
        for (values = (unsigned)setting->free_flags, n = 0; values != 0; values >>= 1, n++)
            if (values & 1)
                switch (1 << n)
                {
                    case SD_FREE_FLAG_VALUES:
                        free((void*)setting->values);
                        setting->values = NULL;
                        break;
                    case SD_FREE_FLAG_NAME:
                        free((void*)setting->name);
                        setting->name = NULL;
                        break;
                    case SD_FREE_FLAG_SHORT:
                        free((void*)setting->short_description);
                        setting->short_description = NULL;
                        break;
                    default:
                        break;
                }

    free(data);

    return true;
}

static rarch_setting_t *menu_setting_new_internal(rarch_setting_info_t *list_info)
{
    unsigned i;
    rarch_setting_t* resized_list = NULL;
    enum settings_list_type list_types[] =
            {
                    SETTINGS_LIST_MAIN_MENU,
            };
    rarch_setting_t terminator           = setting_terminator_setting();
    const char *root                     = msg_hash_to_str(MENU_ENUM_LABEL_MAIN_MENU);
    rarch_setting_t *list                = (rarch_setting_t*)calloc(list_info->size, sizeof(*list));

    if (!list)
        goto error;

    for (i = 0; i < ARRAY_SIZE(list_types); i++)
    {
        if (!setting_append_list(list_types[i], &list, list_info, root))
            goto error;
    }

    if (!(settings_list_append(&list, list_info)))
        goto error;
    if (terminator.name)
        terminator.name_hash = msg_hash_calculate(terminator.name);
    (*&list)[list_info->index++] = terminator;

    /* flatten this array to save ourselves some kilobytes. */
    resized_list = (rarch_setting_t*)realloc(list, list_info->index * sizeof(rarch_setting_t));
    if (!resized_list)
        goto error;

    list = resized_list;

    return list;

error:
    if (list)
        free(list);
    return NULL;
}

/**
 * menu_setting_new:
 * @mask               : Bitmask of settings to include.
 *
 * Request a list of settings based on @mask.
 *
 * Returns: settings list composed of all requested
 * settings on success, otherwise NULL.
 **/
static rarch_setting_t *menu_setting_new(void)
{
    rarch_setting_t* list           = NULL;
    rarch_setting_info_t *list_info = (rarch_setting_info_t*)
            calloc(1, sizeof(*list_info));

    if (!list_info)
        return NULL;

    list_info->size  = 32;

    list = menu_setting_new_internal(list_info);

    menu_settings_info_list_free(list_info);
    list_info = NULL;

    return list;
}

bool menu_setting_ctl(enum menu_setting_ctl_state state, void *data)
{
    uint64_t flags;

    switch (state)
    {
        case MENU_SETTING_CTL_IS_OF_PATH_TYPE:
        {
            bool cbs_bound           = false;
            rarch_setting_t *setting = (rarch_setting_t*)data;

            if (!setting)
                return false;

            flags                    = setting->flags;

            if (setting_get_type(setting) != ST_ACTION)
                return false;

            if (!setting->change_handler)
                return false;

            cbs_bound = setting->action_right;
            cbs_bound = cbs_bound || setting->action_left;
            cbs_bound = cbs_bound || setting->action_select;

            if (!cbs_bound)
                return false;

            if (!(flags & SD_FLAG_BROWSER_ACTION))
                return false;
        }
            break;
        case MENU_SETTING_CTL_NEW:
        {
            rarch_setting_t **setting = (rarch_setting_t**)data;
            if (!setting)
                return false;
            *setting = menu_setting_new();
        }
            break;
        case MENU_SETTING_CTL_ACTION_RIGHT:
        {
            rarch_setting_t *setting = (rarch_setting_t*)data;
            if (!setting)
                return false;

            if (setting_handler(setting, MENU_ACTION_RIGHT) == -1)
                return false;
        }
            break;
        case MENU_SETTING_CTL_NONE:
        default:
            break;
    }

    return true;
}
