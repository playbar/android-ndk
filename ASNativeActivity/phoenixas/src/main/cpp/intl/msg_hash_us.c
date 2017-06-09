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
#include <stdint.h>
#include <stddef.h>

#include <compat/strl.h>
#include <string/stdstring.h>

#include "../src/msg_hash.h"
#include "../src/configuration.h"
#include "../src/verbosity.h"

int menu_hash_get_help_us_enum(enum msg_hash_enums msg, char *s, size_t len) {
    uint32_t driver_hash = 0;
    settings_t *settings = config_get_ptr();

    if (msg <= MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_END &&
        msg >= MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_BEGIN) {
        unsigned idx = msg - MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_BEGIN;

        switch (idx) {
            case RARCH_FAST_FORWARD_KEY:
                snprintf(s, len,
                         "Toggles between fast-forwarding and \n"
                                 "normal speed."
                );
                break;
            case RARCH_FAST_FORWARD_HOLD_KEY:
                snprintf(s, len,
                         "Hold for fast-forward. \n"
                                 " \n"
                                 "Releasing button disables fast-forward."
                );
                break;
            case RARCH_PAUSE_TOGGLE:
                snprintf(s, len,
                         "Toggle between paused and non-paused state.");
                break;
            case RARCH_FRAMEADVANCE:
                snprintf(s, len,
                         "Frame advance when content is paused.");
                break;
            case RARCH_SHADER_NEXT:
                snprintf(s, len,
                         "Applies next shader in directory.");
                break;
            case RARCH_SHADER_PREV:
                snprintf(s, len,
                         "Applies previous shader in directory.");
                break;
            case RARCH_CHEAT_INDEX_PLUS:
            case RARCH_CHEAT_INDEX_MINUS:
            case RARCH_CHEAT_TOGGLE:
                snprintf(s, len,
                         "Cheats.");
                break;
            case RARCH_RESET:
                snprintf(s, len,
                         "Reset the content.");
                break;
            case RARCH_SCREENSHOT:
                snprintf(s, len,
                         "Take screenshot.");
                break;
            case RARCH_MUTE:
                snprintf(s, len,
                         "Mute/unmute audio.");
                break;
            case RARCH_OSK:
                snprintf(s, len,
                         "Toggles onscreen keyboard.");
                break;
            case RARCH_NETPLAY_FLIP:
                snprintf(s, len,
                         "Netplay flip users.");
                break;
            case RARCH_NETPLAY_GAME_WATCH:
                snprintf(s, len,
                         "Netplay toggle play/spectate mode.");
                break;
            case RARCH_SLOWMOTION:
                snprintf(s, len,
                         "Hold for slowmotion.");
                break;
            case RARCH_ENABLE_HOTKEY:
                snprintf(s, len,
                         "Enable other hotkeys. \n"
                                 " \n"
                                 "If this hotkey is bound to either\n"
                                 "a keyboard, joybutton or joyaxis, \n"
                                 "all other hotkeys will be enabled only \n"
                                 "if this one is held at the same time. \n"
                                 " \n"
                                 "Alternatively, all hotkeys for keyboard \n"
                                 "could be disabled by the user.");
                break;
            case RARCH_VOLUME_UP:
                snprintf(s, len,
                         "Increases audio volume.");
                break;
            case RARCH_VOLUME_DOWN:
                snprintf(s, len,
                         "Decreases audio volume.");
                break;
            case RARCH_OVERLAY_NEXT:
                snprintf(s, len,
                         "Switches to next overlay. Wraps around.");
                break;
            case RARCH_DISK_EJECT_TOGGLE:
                snprintf(s, len,
                         "Toggles eject for disks. \n"
                                 " \n"
                                 "Used for multiple-disk content. ");
                break;
            case RARCH_DISK_NEXT:
            case RARCH_DISK_PREV:
                snprintf(s, len,
                         "Cycles through disk images. Use after ejecting. \n"
                                 " \n"
                                 "Complete by toggling eject again.");
                break;
            case RARCH_GRAB_MOUSE_TOGGLE:
                snprintf(s, len,
                         "Toggles mouse grab. \n"
                                 " \n"
                                 "When mouse is grabbed, RetroArch hides the \n"
                                 "mouse, and keeps the mouse pointer inside \n"
                                 "the window to allow relative mouse input to \n"
                                 "work better.");
                break;
            case RARCH_GAME_FOCUS_TOGGLE:
                snprintf(s, len,
                         "Toggles game focus.\n"
                                 " \n"
                                 "When a game has focus, RetroArch will both disable \n"
                                 "hotkeys and keep/warp the mouse pointer inside the window.");
                break;
            case RARCH_MENU_TOGGLE:
                snprintf(s, len, "Toggles menu.");
                break;
            case RARCH_LOAD_STATE_KEY:
                snprintf(s, len,
                         "Loads state.");
                break;
            case RARCH_FULLSCREEN_TOGGLE_KEY:
                snprintf(s, len,
                         "Toggles fullscreen.");
                break;
            case RARCH_QUIT_KEY:
                snprintf(s, len,
                         "Key to exit RetroArch cleanly. \n"
                                " \n"
                                "Killing it in any hard way (SIGKILL, etc.) will \n"
                                "terminate RetroArch without saving RAM, etc."
#ifdef __unix__
                        "\nOn Unix-likes, SIGINT/SIGTERM allows a clean \n"
                        "deinitialization."
#endif
                                "");
                break;
            case RARCH_STATE_SLOT_PLUS:
            case RARCH_STATE_SLOT_MINUS:
                snprintf(s, len,
                         "State slots. \n"
                                 " \n"
                                 "With slot set to 0, save state name is \n"
                                 "*.state (or whatever defined on commandline). \n"
                                 " \n"
                                 "When slot is not 0, path will be <path><d>, \n"
                                 "where <d> is slot number.");
                break;
            case RARCH_SAVE_STATE_KEY:
                snprintf(s, len,
                         "Saves state.");
                break;
            case RARCH_REWIND:
                snprintf(s, len,
                         "Hold button down to rewind. \n"
                                 " \n"
                                 "Rewinding must be enabled.");
                break;
            case RARCH_MOVIE_RECORD_TOGGLE:
                snprintf(s, len,
                         "Toggle between recording and not.");
                break;
            default:
                if (string_is_empty(s))
                    strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE), len);
                break;
        }

        return 0;
    }

    switch (msg) {
        case MENU_ENUM_LABEL_ACCOUNTS_RETRO_ACHIEVEMENTS:
            snprintf(s, len, "Login details for your \n"
                    "Retro Achievements account. \n"
                    " \n"
                    "Visit retroachievements.org and sign up \n"
                    "for a free account. \n"
                    " \n"
                    "After you are done registering, you need \n"
                    "to input the username and password into \n"
                    "RetroArch.");
            break;
        case MENU_ENUM_LABEL_CHEEVOS_USERNAME:
            snprintf(s, len, "Username for your Retro Achievements account.");
            break;
        case MENU_ENUM_LABEL_CHEEVOS_PASSWORD:
            snprintf(s, len, "Password for your Retro Achievements account.");
            break;
        case MENU_ENUM_LABEL_USER_LANGUAGE:
            snprintf(s, len, "Localizes the menu and all onscreen messages \n"
                    "according to the language you have selected \n"
                    "here. \n"
                    " \n"
                    "Requires a restart for the changes \n"
                    "to take effect. \n"
                    " \n"
                    "Note: not all languages might be currently \n"
                    "implemented. \n"
                    " \n"
                    "In case a language is not implemented, \n"
                    "we fallback to English.");
            break;
        case MENU_ENUM_LABEL_VIDEO_FONT_PATH:
            snprintf(s, len, "Change the font that is used \n"
                    "for the Onscreen Display text.");
            break;
        case MENU_ENUM_LABEL_GAME_SPECIFIC_OPTIONS:
            snprintf(s, len, "Automatically load content-specific core options.");
            break;
        case MENU_ENUM_LABEL_AUTO_OVERRIDES_ENABLE:
            snprintf(s, len, "Automatically load override configurations.");
            break;
        case MENU_ENUM_LABEL_AUTO_REMAPS_ENABLE:
            snprintf(s, len, "Automatically load input remapping files.");
            break;
        case MENU_ENUM_LABEL_SORT_SAVESTATES_ENABLE:
            snprintf(s, len, "Sort save states in folders \n"
                    "named after the libretro core used.");
            break;
        case MENU_ENUM_LABEL_SORT_SAVEFILES_ENABLE:
            snprintf(s, len, "Sort save files in folders \n"
                    "named after the libretro core used.");
            break;
        case MENU_ENUM_LABEL_RESUME_CONTENT:
            snprintf(s, len, "Exits from the menu and returns back \n"
                    "to the content.");
            break;
        case MENU_ENUM_LABEL_RESTART_CONTENT:
            snprintf(s, len, "Restarts the content from the beginning.");
            break;
        case MENU_ENUM_LABEL_CLOSE_CONTENT:
            snprintf(s, len, "Closes the content and unloads it from \n"
                    "memory.");
            break;
        case MENU_ENUM_LABEL_UNDO_LOAD_STATE:
            snprintf(s, len, "If a state was loaded, content will \n"
                    "go back to the state prior to loading.");
            break;
        case MENU_ENUM_LABEL_UNDO_SAVE_STATE:
            snprintf(s, len, "If a state was overwritten, it will \n"
                    "roll back to the previous save state.");
            break;
        case MENU_ENUM_LABEL_TAKE_SCREENSHOT:
            snprintf(s, len, "Create a screenshot. \n"
                    " \n"
                    "The screenshot will be stored inside the \n"
                    "Screenshot Directory.");
            break;
        case MENU_ENUM_LABEL_RUN:
            snprintf(s, len, "Start the content.");
            break;
        case MENU_ENUM_LABEL_INFORMATION:
            snprintf(s, len, "Show additional metadata information \n"
                    "about the content.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_CONFIG:
            snprintf(s, len, "Configuration file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_COMPRESSED_ARCHIVE:
            snprintf(s, len, "Compressed archive file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_RECORD_CONFIG:
            snprintf(s, len, "Recording configuration file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_CURSOR:
            snprintf(s, len, "Database cursor file.");
            break;
        case MENU_ENUM_LABEL_FILE_CONFIG:
            snprintf(s, len, "Configuration file.");
            break;
        case MENU_ENUM_LABEL_SCAN_THIS_DIRECTORY:
            snprintf(s, len,
                     "Select this to scan the current directory \n"
                             "for content.");
            break;
        case MENU_ENUM_LABEL_USE_THIS_DIRECTORY:
            snprintf(s, len,
                     "Select this to set this as the directory.");
            break;
        case MENU_ENUM_LABEL_CONTENT_DATABASE_DIRECTORY:
            snprintf(s, len,
                     "Content Database Directory. \n"
                             " \n"
                             "Path to content database \n"
                             "directory.");
            break;
        case MENU_ENUM_LABEL_THUMBNAILS_DIRECTORY:
            snprintf(s, len,
                     "Thumbnails Directory. \n"
                             " \n"
                             "To store thumbnail files.");
            break;
        case MENU_ENUM_LABEL_LIBRETRO_INFO_PATH:
            snprintf(s, len,
                     "Core Info Directory. \n"
                             " \n"
                             "A directory for where to search \n"
                             "for libretro core information.");
            break;
        case MENU_ENUM_LABEL_PLAYLIST_DIRECTORY:
            snprintf(s, len,
                     "Playlist Directory. \n"
                             " \n"
                             "Save all playlist files to this \n"
                             "directory.");
            break;
        case MENU_ENUM_LABEL_DUMMY_ON_CORE_SHUTDOWN:
            snprintf(s, len,
                     "Some cores might have \n"
                             "a shutdown feature. \n"
                             " \n"
                             "If this option is left disabled, \n"
                             "selecting the shutdown procedure \n"
                             "would trigger RetroArch being shut \n"
                             "down. \n"
                             " \n"
                             "Enabling this option will load a \n"
                             "dummy core instead so that we remain \n"
                             "inside the menu and RetroArch won't \n"
                             "shutdown.");
            break;
        case MENU_ENUM_LABEL_CHECK_FOR_MISSING_FIRMWARE:
            snprintf(s, len,
                     "Some cores might need \n"
                             "firmware or bios files. \n"
                             " \n"
                             "If this option is disabled, \n"
                             "it will try to load even if such \n"
                             "firmware is missing. \n"
                             "down. \n");
            break;
        case MENU_ENUM_LABEL_PARENT_DIRECTORY:
            snprintf(s, len,
                     "Go back to the parent directory.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_SHADER_PRESET:
            snprintf(s, len,
                     "Shader preset file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_SHADER:
            snprintf(s, len,
                     "Shader file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_REMAP:
            snprintf(s, len,
                     "Remap controls file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_CHEAT:
            snprintf(s, len,
                     "Cheat file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_OVERLAY:
            snprintf(s, len,
                     "Overlay file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_RDB:
            snprintf(s, len,
                     "Database file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_FONT:
            snprintf(s, len,
                     "TrueType font file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_PLAIN_FILE:
            snprintf(s, len,
                     "Plain file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_MOVIE_OPEN:
            snprintf(s, len,
                     "Video. \n"
                             " \n"
                             "Select it to open this file with the \n"
                             "video player.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_MUSIC_OPEN:
            snprintf(s, len,
                     "Music. \n"
                             " \n"
                             "Select it to open this file with the \n"
                             "music player.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_IMAGE:
            snprintf(s, len,
                     "Image file.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_IMAGE_OPEN_WITH_VIEWER:
            snprintf(s, len,
                     "Image. \n"
                             " \n"
                             "Select it to open this file with the \n"
                             "image viewer.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_CORE_SELECT_FROM_COLLECTION:
            snprintf(s, len,
                     "Libretro core. \n"
                             " \n"
                             "Selecting this will associate this core \n"
                             "to the game.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_CORE:
            snprintf(s, len,
                     "Libretro core. \n"
                             " \n"
                             "Select this file to have RetroArch load this core.");
            break;
        case MENU_ENUM_LABEL_FILE_BROWSER_DIRECTORY:
            snprintf(s, len,
                     "Directory. \n"
                             " \n"
                             "Select it to open this directory.");
            break;
        case MENU_ENUM_LABEL_CACHE_DIRECTORY:
            snprintf(s, len,
                     "Cache Directory. \n"
                             " \n"
                             "Content decompressed by RetroArch will be \n"
                             "temporarily extracted to this directory.");
            break;
        case MENU_ENUM_LABEL_HISTORY_LIST_ENABLE:
            snprintf(s, len,
                     "If enabled, every content loaded \n"
                             "in RetroArch will be automatically \n"
                             "added to the recent history list.");
            break;
        case MENU_ENUM_LABEL_RGUI_BROWSER_DIRECTORY:
            snprintf(s, len,
                     "File Browser Directory. \n"
                             " \n"
                             "Sets start directory for menu file browser.");
            break;
        case MENU_ENUM_LABEL_INPUT_POLL_TYPE_BEHAVIOR:
            snprintf(s, len,
                     "Influence how input polling is done inside \n"
                             "RetroArch. \n"
                             " \n"
                             "Early  - Input polling is performed before \n"
                             "the frame is processed. \n"
                             "Normal - Input polling is performed when \n"
                             "polling is requested. \n"
                             "Late   - Input polling is performed on \n"
                             "first input state request per frame.\n"
                             " \n"
                             "Setting it to 'Early' or 'Late' can result \n"
                             "in less latency, \n"
                             "depending on your configuration.\n\n"
                             "Will be ignored when using netplay."
            );
            break;
        case MENU_ENUM_LABEL_INPUT_DESCRIPTOR_HIDE_UNBOUND:
            snprintf(s, len,
                     "Hide input descriptors that were not set \n"
                             "by the core.");
            break;
        case MENU_ENUM_LABEL_VIDEO_REFRESH_RATE:
            snprintf(s, len,
                     "Video refresh rate of your monitor. \n"
                             "Used to calculate a suitable audio input rate.");
            break;
        case MENU_ENUM_LABEL_VIDEO_FORCE_SRGB_DISABLE:
            snprintf(s, len,
                     "Forcibly disable sRGB FBO support. Some Intel \n"
                             "OpenGL drivers on Windows have video problems \n"
                             "with sRGB FBO support enabled.");
            break;
        case MENU_ENUM_LABEL_AUDIO_ENABLE:
            snprintf(s, len,
                     "Enable audio output.");
            break;
        case MENU_ENUM_LABEL_AUDIO_SYNC:
            snprintf(s, len,
                     "Synchronize audio (recommended).");
            break;
        case MENU_ENUM_LABEL_AUDIO_LATENCY:
            snprintf(s, len,
                     "Desired audio latency in milliseconds. \n"
                             "Might not be honored if the audio driver \n"
                             "can't provide given latency.");
            break;
        case MENU_ENUM_LABEL_VIDEO_ALLOW_ROTATE:
            snprintf(s, len,
                     "Allow cores to set rotation. If false, \n"
                             "rotation requests are honored, but ignored.\n\n"
                             "Used for setups where one manually rotates \n"
                             "the monitor.");
            break;
        case MENU_ENUM_LABEL_INPUT_DESCRIPTOR_LABEL_SHOW:
            snprintf(s, len,
                     "Show the input descriptors set by the core \n"
                             "instead of the default ones.");
            break;
        case MENU_ENUM_LABEL_CONTENT_HISTORY_SIZE:
            snprintf(s, len,
                     "Number of entries that will be kept in \n"
                             "content history playlist.");
            break;
        case MENU_ENUM_LABEL_VIDEO_WINDOWED_FULLSCREEN:
            snprintf(s, len,
                     "To use windowed mode or not when going \n"
                             "fullscreen.");
            break;
        case MENU_ENUM_LABEL_VIDEO_FONT_SIZE:
            snprintf(s, len,
                     "Font size for on-screen messages.");
            break;
        case MENU_ENUM_LABEL_SAVESTATE_AUTO_INDEX:
            snprintf(s, len,
                     "Automatically increment slot index on each save, \n"
                             "generating multiple savestate files. \n"
                             "When the content is loaded, state slot will be \n"
                             "set to the highest existing value (last savestate).");
            break;
        case MENU_ENUM_LABEL_FPS_SHOW:
            snprintf(s, len,
                     "Enables displaying the current frames \n"
                             "per second.");
            break;
        case MENU_ENUM_LABEL_VIDEO_FONT_ENABLE:
            snprintf(s, len,
                     "Show and/or hide onscreen messages.");
            break;
        case MENU_ENUM_LABEL_VIDEO_MESSAGE_POS_X:
        case MENU_ENUM_LABEL_VIDEO_MESSAGE_POS_Y:
            snprintf(s, len,
                     "Offset for where messages will be placed \n"
                             "onscreen. Values are in range [0.0, 1.0].");
            break;
        case MENU_ENUM_LABEL_INPUT_OVERLAY_ENABLE:
            snprintf(s, len,
                     "Enable or disable the current overlay.");
            break;
        case MENU_ENUM_LABEL_INPUT_OVERLAY_HIDE_IN_MENU:
            snprintf(s, len,
                     "Hide the current overlay from appearing \n"
                             "inside the menu.");
            break;
        case MENU_ENUM_LABEL_OVERLAY_PRESET:
            snprintf(s, len,
                     "Path to input overlay.");
            break;
        case MENU_ENUM_LABEL_OVERLAY_OPACITY:
            snprintf(s, len,
                     "Overlay opacity.");
            break;
        case MENU_ENUM_LABEL_INPUT_BIND_TIMEOUT:
            snprintf(s, len,
                     "Input bind timer timeout (in seconds). \n"
                             "Amount of seconds to wait until proceeding \n"
                             "to the next bind.");
            break;
        case MENU_ENUM_LABEL_OVERLAY_SCALE:
            snprintf(s, len,
                     "Overlay scale.");
            break;
        case MENU_ENUM_LABEL_AUDIO_OUTPUT_RATE:
            snprintf(s, len,
                     "Audio output samplerate.");
            break;
        case MENU_ENUM_LABEL_VIDEO_SHARED_CONTEXT:
            snprintf(s, len,
                     "Set to true if hardware-rendered cores \n"
                             "should get their private context. \n"
                             "Avoids having to assume hardware state changes \n"
                             "inbetween frames."
            );
            break;
        case MENU_ENUM_LABEL_CORE_LIST:
            snprintf(s, len,
                     "Load Core. \n"
                             " \n"
                             "Browse for a libretro core \n"
                             "implementation. Where the browser \n"
                             "starts depends on your Core Directory \n"
                             "path. If blank, it will start in root. \n"
                             " \n"
                             "If Core Directory is a directory, the menu \n"
                             "will use that as top folder. If Core \n"
                             "Directory is a full path, it will start \n"
                             "in the folder where the file is.");
            break;
        case MENU_ENUM_LABEL_VALUE_MENU_ENUM_CONTROLS_PROLOG:
            snprintf(s, len,
                     "You can use the following controls below \n"
                             "on either your gamepad or keyboard in order\n"
                             "to control the menu: \n"
                             " \n"
            );
            break;
        case MENU_ENUM_LABEL_WELCOME_TO_RETROARCH:
            snprintf(s, len,
                     "Welcome to RetroArch\n"
            );
            break;
        case MENU_ENUM_LABEL_VALUE_HELP_AUDIO_VIDEO_TROUBLESHOOTING_DESC: {
            /* Work around C89 limitations */
            char u[501];
            const char *t =
                    "RetroArch relies on an unique form of\n"
                            "audio/video synchronization where it needs to be\n"
                            "calibrated against the refresh rate of your\n"
                            "display for best performance results.\n"
                            " \n"
                            "If you experience any audio crackling or video\n"
                            "tearing, usually it means that you need to\n"
                            "calibrate the settings. Some choices below:\n"
                            " \n";
            snprintf(u, sizeof(u), /* can't inline this due to the printf arguments */
                     "a) Go to '%s' -> '%s', and enable\n"
                             "'Threaded Video'. Refresh rate will not matter\n"
                             "in this mode, framerate will be higher,\n"
                             "but video might be less smooth.\n"
                             "b) Go to '%s' -> '%s', and look at\n"
                             "'%s'. Let it run for\n"
                             "2048 frames, then press 'OK'.",
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SETTINGS),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_VIDEO_SETTINGS),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SETTINGS),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_VIDEO_SETTINGS),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE_AUTO));
            strlcpy(s, t, len);
            strlcat(s, u, len);
        }
            break;
        case MENU_ENUM_LABEL_VALUE_HELP_SCANNING_CONTENT_DESC:
            snprintf(s, len,
                     "To scan for content, go to '%s' and\n"
                             "select either '%s' or %s'.\n"
                             " \n"
                             "Files will be compared to database entries.\n"
                             "If there is a match, it will add an entry\n"
                             "to a collection.\n"
                             " \n"
                             "You can then easily access this content by\n"
                             "going to '%s' ->\n"
                             "'%s'\n"
                             "instead of having to go through the\n"
                             "filebrowser everytime.\n"
                             " \n"
                             "NOTE: Content for some cores might still not be\n"
                             "scannable.",
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_ADD_CONTENT_LIST),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SCAN_DIRECTORY),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SCAN_FILE),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_LIST),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_CONTENT_COLLECTION_LIST)
            );
            break;
        case MENU_ENUM_LABEL_VALUE_EXTRACTING_PLEASE_WAIT:
            snprintf(s, len,
                     "Welcome to RetroArch\n"
                             "\n"
                             "Extracting assets, please wait.\n"
                             "This might take a while...\n"
            );
            break;
        case MENU_ENUM_LABEL_INPUT_DRIVER:
            if (settings)
                driver_hash = msg_hash_calculate(settings->arrays.input_driver);

            switch (driver_hash) {
                case MENU_LABEL_INPUT_DRIVER_UDEV:
                    snprintf(s, len,
                             "udev Input driver. \n"
                                     " \n"
                                     "It uses the recent evdev joypad API \n"
                                     "for joystick support. It supports \n"
                                     "hotplugging and force feedback. \n"
                                     " \n"
                                     "The driver reads evdev events for keyboard \n"
                                     "support. It also supports keyboard callback, \n"
                                     "mice and touchpads. \n"
                                     " \n"
                                     "By default in most distros, /dev/input nodes \n"
                                     "are root-only (mode 600). You can set up a udev \n"
                                     "rule which makes these accessible to non-root."
                    );
                    break;
                case MENU_LABEL_INPUT_DRIVER_LINUXRAW:
                    snprintf(s, len,
                             "linuxraw Input driver. \n"
                                     " \n"
                                     "This driver requires an active TTY. Keyboard \n"
                                     "events are read directly from the TTY which \n"
                                     "makes it simpler, but not as flexible as udev. \n" "Mice, etc, are not supported at all. \n"
                                     " \n"
                                     "This driver uses the older joystick API \n"
                                     "(/dev/input/js*).");
                    break;
                default:
                    snprintf(s, len,
                             "Input driver.\n"
                                     " \n"
                                     "Depending on video driver, it might \n"
                                     "force a different input driver.");
                    break;
            }
            break;
        case MENU_ENUM_LABEL_LOAD_CONTENT_LIST:
            snprintf(s, len,
                     "Load Content. \n"
                             "Browse for content. \n"
                             " \n"
                             "To load content, you need a \n"
                             "'Core' to use, and a content file. \n"
                             " \n"
                             "To control where the menu starts \n"
                             "to browse for content, set  \n"
                             "'File Browser Directory'. \n"
                             "If not set, it will start in root. \n"
                             " \n"
                             "The browser will filter out \n"
                             "extensions for the last core set \n"
                             "in 'Load Core', and use that core \n"
                             "when content is loaded."
            );
            break;
        case MENU_ENUM_LABEL_LOAD_CONTENT_HISTORY:
            snprintf(s, len,
                     "Loading content from history. \n"
                             " \n"
                             "As content is loaded, content and libretro \n"
                             "core combinations are saved to history. \n"
                             " \n"
                             "The history is saved to a file in the same \n"
                             "directory as the RetroArch config file. If \n"
                             "no config file was loaded in startup, history \n"
                             "will not be saved or loaded, and will not exist \n"
                             "in the main menu."
            );
            break;
        case MENU_ENUM_LABEL_VIDEO_DRIVER:
            snprintf(s, len,
                     "Current Video driver.");

            if (string_is_equal_fast(settings->arrays.video_driver, "gl", 2))
            {
                snprintf(s, len,
                         "OpenGL Video driver. \n"
                                 " \n"
                                 "This driver allows libretro GL cores to  \n"
                                 "be used in addition to software-rendered \n"
                                 "core implementations.\n"
                                 " \n"
                                 "Performance for software-rendered and \n"
                                 "libretro GL core implementations is \n"
                                 "dependent on your graphics card's \n"
                                 "underlying GL driver).");
            }
            else if (string_is_equal_fast(settings->arrays.video_driver, "sdl2", 4))
            {
                snprintf(s, len,
                         "SDL 2 Video driver.\n"
                                 " \n"
                                 "This is an SDL 2 software-rendered video \n"
                                 "driver.\n"
                                 " \n"
                                 "Performance for software-rendered libretro \n"
                                 "core implementations is dependent \n"
                                 "on your platform SDL implementation.");
            }
            else if (string_is_equal_fast(settings->arrays.video_driver, "sdl1", 4))
            {
                snprintf(s, len,
                         "SDL Video driver.\n"
                                 " \n"
                                 "This is an SDL 1.2 software-rendered video \n"
                                 "driver.\n"
                                 " \n"
                                 "Performance is considered to be suboptimal. \n"
                                 "Consider using it only as a last resort.");
            }
            else if (string_is_equal_fast(settings->arrays.video_driver, "d3d", 3))
            {
                snprintf(s, len,
                         "Direct3D Video driver. \n"
                                 " \n"
                                 "Performance for software-rendered cores \n"
                                 "is dependent on your graphic card's \n"
                                 "underlying D3D driver).");
            }
            else if (string_is_equal_fast(settings->arrays.video_driver, "exynos", 6))
            {
                snprintf(s, len,
                         "Exynos-G2D Video Driver. \n"
                                 " \n"
                                 "This is a low-level Exynos video driver. \n"
                                 "Uses the G2D block in Samsung Exynos SoC \n"
                                 "for blit operations. \n"
                                 " \n"
                                 "Performance for software rendered cores \n"
                                 "should be optimal.");
            }
            else if (string_is_equal_fast(settings->arrays.video_driver, "drm", 3))
            {
                snprintf(s, len,
                         "Plain DRM Video Driver. \n"
                                 " \n"
                                 "This is a low-level video driver using. \n"
                                 "libdrm for hardware scaling using \n"
                                 "GPU overlays.");
            }
            else if (string_is_equal_fast(settings->arrays.video_driver, "sunxi", 5))
            {
                snprintf(s, len,
                         "Sunxi-G2D Video Driver. \n"
                                 " \n"
                                 "This is a low-level Sunxi video driver. \n"
                                 "Uses the G2D block in Allwinner SoCs.");
            }
            break;
        case MENU_ENUM_LABEL_AUDIO_DSP_PLUGIN:
            snprintf(s, len,
                     "Audio DSP plugin.\n"
                             " Processes audio before it's sent to \n"
                             "the driver."
            );
            break;
        case MENU_ENUM_LABEL_AUDIO_RESAMPLER_DRIVER:
            if (settings)
                driver_hash = msg_hash_calculate(settings->arrays.audio_resampler);

            switch (driver_hash) {
                case MENU_LABEL_AUDIO_RESAMPLER_DRIVER_SINC:
                    snprintf(s, len,
                             "Windowed SINC implementation.");
                    break;
                case MENU_LABEL_AUDIO_RESAMPLER_DRIVER_CC:
                    snprintf(s, len,
                             "Convoluted Cosine implementation.");
                    break;
                default:
                    if (string_is_empty(s))
                        strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE), len);
                    break;
            }
            break;
        case MENU_ENUM_LABEL_VIDEO_SHADER_PRESET:
            snprintf(s, len,
                     "Load Shader Preset. \n"
                             " \n"
                             " Load a shader preset directly. \n"
                             "The menu shader menu is updated accordingly. \n"
                             " \n"
                             "If the CGP uses scaling methods which are not \n"
                             "simple, (i.e. source scaling, same scaling \n"
                             "factor for X/Y), the scaling factor displayed \n"
                             "in the menu might not be correct."
            );
            break;
        case MENU_ENUM_LABEL_VIDEO_SHADER_SCALE_PASS:
            snprintf(s, len,
                     "Scale for this pass. \n"
                             " \n"
                             "The scale factor accumulates, i.e. 2x \n"
                             "for first pass and 2x for second pass \n"
                             "will give you a 4x total scale. \n"
                             " \n"
                             "If there is a scale factor for last \n"
                             "pass, the result is stretched to \n"
                             "screen with the filter specified in \n"
                             "'Default Filter'. \n"
                             " \n"
                             "If 'Don't Care' is set, either 1x \n"
                             "scale or stretch to fullscreen will \n"
                             "be used depending if it's not the last \n"
                             "pass or not."
            );
            break;
        case MENU_ENUM_LABEL_VIDEO_SHADER_NUM_PASSES:
            snprintf(s, len,
                     "Shader Passes. \n"
                             " \n"
                             "RetroArch allows you to mix and match various \n"
                             "shaders with arbitrary shader passes, with \n"
                             "custom hardware filters and scale factors. \n"
                             " \n"
                             "This option specifies the number of shader \n"
                             "passes to use. If you set this to 0, and use \n"
                             "Apply Shader Changes, you use a 'blank' shader. \n"
                             " \n"
                             "The Default Filter option will affect the \n"
                             "stretching filter.");
            break;
        case MENU_ENUM_LABEL_VIDEO_SHADER_PARAMETERS:
            snprintf(s, len,
                     "Shader Parameters. \n"
                             " \n"
                             "Modifies current shader directly. Will not be \n"
                             "saved to CGP/GLSLP preset file.");
            break;
        case MENU_ENUM_LABEL_VIDEO_SHADER_PRESET_PARAMETERS:
            snprintf(s, len,
                     "Shader Preset Parameters. \n"
                             " \n"
                             "Modifies shader preset currently in menu."
            );
            break;
        case MENU_ENUM_LABEL_VIDEO_SHADER_PASS:
            snprintf(s, len,
                     "Path to shader. \n"
                             " \n"
                             "All shaders must be of the same \n"
                             "type (i.e. CG, GLSL or HLSL). \n"
                             " \n"
                             "Set Shader Directory to set where \n"
                             "the browser starts to look for \n"
                             "shaders."
            );
            break;
        case MENU_ENUM_LABEL_CONFIGURATION_SETTINGS:
            snprintf(s, len,
                     "Determines how configuration files \n"
                             "are loaded and prioritized.");
            break;
        case MENU_ENUM_LABEL_CONFIG_SAVE_ON_EXIT:
            snprintf(s, len,
                     "Saves config to disk on exit.\n"
                             "Useful for menu as settings can be\n"
                             "modified. Overwrites the config.\n"
                             " \n"
                             "#include's and comments are not \n"
                             "preserved. \n"
                             " \n"
                             "By design, the config file is \n"
                             "considered immutable as it is \n"
                             "likely maintained by the user, \n"
                             "and should not be overwritten \n"
                             "behind the user's back."
#if defined(RARCH_CONSOLE) || defined(RARCH_MOBILE)
            "\nThis is not not the case on \n"
            "consoles however, where \n"
            "looking at the config file \n"
            "manually isn't really an option."
#endif
            );
            break;
        case MENU_ENUM_LABEL_CONFIRM_ON_EXIT:
            snprintf(s, len, "Are you sure you want to quit?");
            break;
        case MENU_ENUM_LABEL_SHOW_HIDDEN_FILES:
            snprintf(s, len, "Show hidden files\n"
                    "and folders.");
            break;
        case MENU_ENUM_LABEL_VIDEO_SHADER_FILTER_PASS:
            snprintf(s, len,
                     "Hardware filter for this pass. \n"
                             " \n"
                             "If 'Don't Care' is set, 'Default \n"
                             "Filter' will be used."
            );
            break;
        case MENU_ENUM_LABEL_AUTOSAVE_INTERVAL:
            snprintf(s, len,
                     "Autosaves the non-volatile SRAM \n"
                             "at a regular interval.\n"
                             " \n"
                             "This is disabled by default unless set \n"
                             "otherwise. The interval is measured in \n"
                             "seconds. \n"
                             " \n"
                             "A value of 0 disables autosave.");
            break;
        case MENU_ENUM_LABEL_INPUT_BIND_DEVICE_TYPE:
            snprintf(s, len,
                     "Input Device Type. \n"
                             " \n"
                             "Picks which device type to use. This is \n"
                             "relevant for the libretro core itself."
            );
            break;
        case MENU_ENUM_LABEL_LIBRETRO_LOG_LEVEL:
            snprintf(s, len,
                     "Sets log level for libretro cores \n"
                             "(GET_LOG_INTERFACE). \n"
                             " \n"
                             " If a log level issued by a libretro \n"
                             " core is below libretro_log level, it \n"
                             " is ignored.\n"
                             " \n"
                             " DEBUG logs are always ignored unless \n"
                             " verbose mode is activated (--verbose).\n"
                             " \n"
                             " DEBUG = 0\n"
                             " INFO  = 1\n"
                             " WARN  = 2\n"
                             " ERROR = 3"
            );
            break;
        case MENU_ENUM_LABEL_STATE_SLOT_INCREASE:
        case MENU_ENUM_LABEL_STATE_SLOT_DECREASE:
            snprintf(s, len,
                     "State slots.\n"
                             " \n"
                             " With slot set to 0, save state name is *.state \n"
                             " (or whatever defined on commandline).\n"
                             "When slot is != 0, path will be (path)(d), \n"
                             "where (d) is slot number.");
            break;
        case MENU_ENUM_LABEL_SHADER_APPLY_CHANGES:
            snprintf(s, len,
                     "Apply Shader Changes. \n"
                             " \n"
                             "After changing shader settings, use this to \n"
                             "apply changes. \n"
                             " \n"
                             "Changing shader settings is a somewhat \n"
                             "expensive operation so it has to be \n"
                             "done explicitly. \n"
                             " \n"
                             "When you apply shaders, the menu shader \n"
                             "settings are saved to a temporary file (either \n"
                             "menu.cgp or menu.glslp) and loaded. The file \n"
                             "persists after RetroArch exits. The file is \n"
                             "saved to Shader Directory."
            );
            break;
        case MENU_ENUM_LABEL_MENU_TOGGLE:
            snprintf(s, len,
                     "Toggles menu.");
            break;
        case MENU_ENUM_LABEL_GRAB_MOUSE_TOGGLE:
            snprintf(s, len,
                     "Toggles mouse grab.\n"
                             " \n"
                             "When mouse is grabbed, RetroArch hides the \n"
                             "mouse, and keeps the mouse pointer inside \n"
                             "the window to allow relative mouse input to \n"
                             "work better.");
            break;
        case MENU_ENUM_LABEL_GAME_FOCUS_TOGGLE:
            snprintf(s, len,
                     "Toggles game focus.\n"
                             " \n"
                             "When a game has focus, RetroArch will both disable \n"
                             "hotkeys and keep/warp the mouse pointer inside the window.");
            break;
        case MENU_ENUM_LABEL_DISK_NEXT:
            snprintf(s, len,
                     "Cycles through disk images. Use after \n"
                             "ejecting. \n"
                             " \n"
                             " Complete by toggling eject again.");
            break;
        case MENU_ENUM_LABEL_VIDEO_FILTER:
#ifdef HAVE_FILTERS_BUILTIN
            snprintf(s, len,
                  "CPU-based video filter.");
#else
            snprintf(s, len,
                     "CPU-based video filter.\n"
                             " \n"
                             "Path to a dynamic library.");
#endif
            break;
        case MENU_ENUM_LABEL_AUDIO_DEVICE:
            snprintf(s, len,
                     "Override the default audio device \n"
                             "the audio driver uses.\n"
                             "This is driver dependent. E.g.\n"
#ifdef HAVE_ALSA
            " \n"
            "ALSA wants a PCM device."
#endif
#ifdef HAVE_OSS
            " \n"
            "OSS wants a path (e.g. /dev/dsp)."
#endif
#ifdef HAVE_JACK
            " \n"
            "JACK wants portnames (e.g. system:playback1\n"
            ",system:playback_2)."
#endif
#ifdef HAVE_RSOUND
            " \n"
            "RSound wants an IP address to an RSound \n"
            "server."
#endif
            );
            break;
        case MENU_ENUM_LABEL_DISK_EJECT_TOGGLE:
            snprintf(s, len,
                     "Toggles eject for disks.\n"
                             " \n"
                             "Used for multiple-disk content.");
            break;
        case MENU_ENUM_LABEL_ENABLE_HOTKEY:
            snprintf(s, len,
                     "Enable other hotkeys.\n"
                             " \n"
                             " If this hotkey is bound to either keyboard, \n"
                             "joybutton or joyaxis, all other hotkeys will \n"
                             "be disabled unless this hotkey is also held \n"
                             "at the same time. \n"
                             " \n"
                             "This is useful for RETRO_KEYBOARD centric \n"
                             "implementations which query a large area of \n"
                             "the keyboard, where it is not desirable that \n"
                             "hotkeys get in the way.");
            break;
        case MENU_ENUM_LABEL_REWIND_ENABLE:
            snprintf(s, len,
                     "Enable rewinding.\n"
                             " \n"
                             "This will take a performance hit, \n"
                             "so it is disabled by default.");
            break;
        case MENU_ENUM_LABEL_LIBRETRO_DIR_PATH:
            snprintf(s, len,
                     "Core Directory. \n"
                             " \n"
                             "A directory for where to search for \n"
                             "libretro core implementations.");
            break;
        case MENU_ENUM_LABEL_VIDEO_REFRESH_RATE_AUTO:
            snprintf(s, len,
                     "Refresh Rate Auto.\n"
                             " \n"
                             "The accurate refresh rate of our monitor (Hz).\n"
                             "This is used to calculate audio input rate with \n"
                             "the formula: \n"
                             " \n"
                             "audio_input_rate = game input rate * display \n"
                             "refresh rate / game refresh rate\n"
                             " \n"
                             "If the implementation does not report any \n"
                             "values, NTSC defaults will be assumed for \n"
                             "compatibility.\n"
                             " \n"
                             "This value should stay close to 60Hz to avoid \n"
                             "large pitch changes. If your monitor does \n"
                             "not run at 60Hz, or something close to it, \n"
                             "disable VSync, and leave this at its default.");
            break;
        case MENU_ENUM_LABEL_VIDEO_ROTATION:
            snprintf(s, len,
                     "Forces a certain rotation \n"
                             "of the screen.\n"
                             " \n"
                             "The rotation is added to rotations which\n"
                             "the libretro core sets (see Video Allow\n"
                             "Rotate).");
            break;
        case MENU_ENUM_LABEL_VIDEO_SCALE:
            snprintf(s, len,
                     "Fullscreen resolution.\n"
                             " \n"
                             "Resolution of 0 uses the \n"
                             "resolution of the environment.\n");
            break;
        case MENU_ENUM_LABEL_FASTFORWARD_RATIO:
            snprintf(s, len,
                     "Fastforward ratio."
                             " \n"
                             "The maximum rate at which content will\n"
                             "be run when using fast forward.\n"
                             " \n"
                             " (E.g. 5.0 for 60 fps content => 300 fps \n"
                             "cap).\n"
                             " \n"
                             "RetroArch will go to sleep to ensure that \n"
                             "the maximum rate will not be exceeded.\n"
                             "Do not rely on this cap to be perfectly \n"
                             "accurate.");
            break;
        case MENU_ENUM_LABEL_VIDEO_MONITOR_INDEX:
            snprintf(s, len,
                     "Which monitor to prefer.\n"
                             " \n"
                             "0 (default) means no particular monitor \n"
                             "is preferred, 1 and up (1 being first \n"
                             "monitor), suggests RetroArch to use that \n"
                             "particular monitor.");
            break;
        case MENU_ENUM_LABEL_VIDEO_CROP_OVERSCAN:
            snprintf(s, len,
                     "Forces cropping of overscanned \n"
                             "frames.\n"
                             " \n"
                             "Exact behavior of this option is \n"
                             "core-implementation specific.");
            break;
        case MENU_ENUM_LABEL_VIDEO_SCALE_INTEGER:
            snprintf(s, len,
                     "Only scales video in integer \n"
                             "steps.\n"
                             " \n"
                             "The base size depends on system-reported \n"
                             "geometry and aspect ratio.\n"
                             " \n"
                             "If Force Aspect is not set, X/Y will be \n"
                             "integer scaled independently.");
            break;
        case MENU_ENUM_LABEL_AUDIO_VOLUME:
            snprintf(s, len,
                     "Audio volume, expressed in dB.\n"
                             " \n"
                             " 0 dB is normal volume. No gain will be applied.\n"
                             "Gain can be controlled in runtime with Input\n"
                             "Volume Up / Input Volume Down.");
            break;
        case MENU_ENUM_LABEL_AUDIO_RATE_CONTROL_DELTA:
            snprintf(s, len,
                     "Audio rate control.\n"
                             " \n"
                             "Setting this to 0 disables rate control.\n"
                             "Any other value controls audio rate control \n"
                             "delta.\n"
                             " \n"
                             "Defines how much input rate can be adjusted \n"
                             "dynamically.\n"
                             " \n"
                             " Input rate is defined as: \n"
                             " input rate * (1.0 +/- (rate control delta))");
            break;
        case MENU_ENUM_LABEL_AUDIO_MAX_TIMING_SKEW:
            snprintf(s, len,
                     "Maximum audio timing skew.\n"
                             " \n"
                             "Defines the maximum change in input rate.\n"
                             "You may want to increase this to enable\n"
                             "very large changes in timing, for example\n"
                             "running PAL cores on NTSC displays, at the\n"
                             "cost of inaccurate audio pitch.\n"
                             " \n"
                             " Input rate is defined as: \n"
                             " input rate * (1.0 +/- (max timing skew))");
            break;
        case MENU_ENUM_LABEL_OVERLAY_NEXT:
            snprintf(s, len,
                     "Toggles to next overlay.\n"
                             " \n"
                             "Wraps around.");
            break;
        case MENU_ENUM_LABEL_LOG_VERBOSITY:
            snprintf(s, len,
                     "Enable or disable verbosity level \n"
                             "of frontend.");
            break;
        case MENU_ENUM_LABEL_VOLUME_UP:
            snprintf(s, len,
                     "Increases audio volume.");
            break;
        case MENU_ENUM_LABEL_VOLUME_DOWN:
            snprintf(s, len,
                     "Decreases audio volume.");
            break;
        case MENU_ENUM_LABEL_VIDEO_DISABLE_COMPOSITION:
            snprintf(s, len,
                     "Forcibly disable composition.\n"
                             "Only valid on Windows Vista/7 for now.");
            break;
        case MENU_ENUM_LABEL_PERFCNT_ENABLE:
            snprintf(s, len,
                     "Enable or disable frontend \n"
                             "performance counters.");
            break;
        case MENU_ENUM_LABEL_SYSTEM_DIRECTORY:
            snprintf(s, len,
                     "System Directory. \n"
                             " \n"
                             "Sets the 'system' directory.\n"
                             "Cores can query for this\n"
                             "directory to load BIOSes, \n"
                             "system-specific configs, etc.");
            break;
        case MENU_ENUM_LABEL_SAVESTATE_AUTO_SAVE:
        case MENU_ENUM_LABEL_SAVESTATE_AUTO_LOAD:
            snprintf(s, len,
                     "Automatically saves a savestate at the \n"
                             "end of RetroArch's lifetime.\n"
                             " \n"
                             "RetroArch will automatically load any savestate\n"
                             "with this path on startup if 'Auto Load State\n"
                             "is enabled.");
            break;
        case MENU_ENUM_LABEL_VIDEO_THREADED:
            snprintf(s, len,
                     "Use threaded video driver.\n"
                             " \n"
                             "Using this might improve performance at the \n"
                             "possible cost of latency and more video \n"
                             "stuttering.");
            break;
        case MENU_ENUM_LABEL_VIDEO_VSYNC:
            snprintf(s, len,
                     "Video V-Sync.\n");
            break;
        case MENU_ENUM_LABEL_VIDEO_HARD_SYNC:
            snprintf(s, len,
                     "Attempts to hard-synchronize \n"
                             "CPU and GPU.\n"
                             " \n"
                             "Can reduce latency at the cost of \n"
                             "performance.");
            break;
        case MENU_ENUM_LABEL_REWIND_GRANULARITY:
            snprintf(s, len,
                     "Rewind granularity.\n"
                             " \n"
                             " When rewinding defined number of \n"
                             "frames, you can rewind several frames \n"
                             "at a time, increasing the rewinding \n"
                             "speed.");
            break;
        case MENU_ENUM_LABEL_SCREENSHOT:
            snprintf(s, len,
                     "Take screenshot.");
            break;
        case MENU_ENUM_LABEL_VIDEO_FRAME_DELAY:
            snprintf(s, len,
                     "Sets how many milliseconds to delay\n"
                             "after VSync before running the core.\n"
                             "\n"
                             "Can reduce latency at the cost of\n"
                             "higher risk of stuttering.\n"
                             " \n"
                             "Maximum is 15.");
            break;
        case MENU_ENUM_LABEL_VIDEO_HARD_SYNC_FRAMES:
            snprintf(s, len,
                     "Sets how many frames CPU can \n"
                             "run ahead of GPU when using 'GPU \n"
                             "Hard Sync'.\n"
                             " \n"
                             "Maximum is 3.\n"
                             " \n"
                             " 0: Syncs to GPU immediately.\n"
                             " 1: Syncs to previous frame.\n"
                             " 2: Etc ...");
            break;
        case MENU_ENUM_LABEL_VIDEO_BLACK_FRAME_INSERTION:
            snprintf(s, len,
                     "Inserts a black frame inbetween \n"
                             "frames.\n"
                             " \n"
                             "Useful for 120 Hz monitors who want to \n"
                             "play 60 Hz material with eliminated \n"
                             "ghosting.\n"
                             " \n"
                             "Video refresh rate should still be \n"
                             "configured as if it is a 60 Hz monitor \n"
                             "(divide refresh rate by 2).");
            break;
        case MENU_ENUM_LABEL_RGUI_SHOW_START_SCREEN:
            snprintf(s, len,
                     "Show startup screen in menu.\n"
                             "Is automatically set to false when seen\n"
                             "for the first time.\n"
                             " \n"
                             "This is only updated in config if\n"
                             "'Save Configuration on Exit' is enabled.\n");
            break;
        case MENU_ENUM_LABEL_VIDEO_FULLSCREEN:
            snprintf(s, len, "Toggles fullscreen.");
            break;
        case MENU_ENUM_LABEL_BLOCK_SRAM_OVERWRITE:
            snprintf(s, len,
                     "Block SRAM from being overwritten \n"
                             "when loading save states.\n"
                             " \n"
                             "Might potentially lead to buggy games.");
            break;
        case MENU_ENUM_LABEL_PAUSE_NONACTIVE:
            snprintf(s, len,
                     "Pause gameplay when window focus \n"
                             "is lost.");
            break;
        case MENU_ENUM_LABEL_VIDEO_GPU_SCREENSHOT:
            snprintf(s, len,
                     "Screenshots output of GPU shaded \n"
                             "material if available.");
            break;
        case MENU_ENUM_LABEL_SCREENSHOT_DIRECTORY:
            snprintf(s, len,
                     "Screenshot Directory. \n"
                             " \n"
                             "Directory to dump screenshots to."
            );
            break;
        case MENU_ENUM_LABEL_VIDEO_SWAP_INTERVAL:
            snprintf(s, len,
                     "VSync Swap Interval.\n"
                             " \n"
                             "Uses a custom swap interval for VSync. Set this \n"
                             "to effectively halve monitor refresh rate.");
            break;
        case MENU_ENUM_LABEL_SAVEFILE_DIRECTORY:
            snprintf(s, len,
                     "Savefile Directory. \n"
                             " \n"
                             "Save all save files (*.srm) to this \n"
                             "directory. This includes related files like \n"
                             ".bsv, .rt, .psrm, etc...\n"
                             " \n"
                             "This will be overridden by explicit command line\n"
                             "options.");
            break;
        case MENU_ENUM_LABEL_SAVESTATE_DIRECTORY:
            snprintf(s, len,
                     "Savestate Directory. \n"
                             " \n"
                             "Save all save states (*.state) to this \n"
                             "directory.\n"
                             " \n"
                             "This will be overridden by explicit command line\n"
                             "options.");
            break;
        case MENU_ENUM_LABEL_ASSETS_DIRECTORY:
            snprintf(s, len,
                     "Assets Directory. \n"
                             " \n"
                             " This location is queried by default when \n"
                             "menu interfaces try to look for loadable \n"
                             "assets, etc.");
            break;
        case MENU_ENUM_LABEL_DYNAMIC_WALLPAPERS_DIRECTORY:
            snprintf(s, len,
                     "Dynamic Wallpapers Directory. \n"
                             " \n"
                             " The place to store backgrounds that will \n"
                             "be loaded dynamically by the menu depending \n"
                             "on context.");
            break;
        case MENU_ENUM_LABEL_SLOWMOTION_RATIO:
            snprintf(s, len,
                     "Slowmotion ratio."
                             " \n"
                             "When slowmotion, content will slow\n"
                             "down by factor.");
            break;
        case MENU_ENUM_LABEL_INPUT_AXIS_THRESHOLD:
            snprintf(s, len,
                     "Defines axis threshold.\n"
                             " \n"
                             "How far an axis must be tilted to result\n"
                             "in a button press.\n"
                             " Possible values are [0.0, 1.0].");
            break;
        case MENU_ENUM_LABEL_INPUT_TURBO_PERIOD:
            snprintf(s, len,
                     "Turbo period.\n"
                             " \n"
                             "Describes the period of which turbo-enabled\n"
                             "buttons toggle.\n"
                             " \n"
                             "Numbers are described in frames."
            );
            break;
        case MENU_ENUM_LABEL_INPUT_DUTY_CYCLE:
            snprintf(s, len,
                     "Duty cycle.\n"
                             " \n"
                             "Describes how long the period of a turbo-enabled\n"
                             "should be.\n"
                             " \n"
                             "Numbers are described in frames."
            );
            break;
        case MENU_ENUM_LABEL_INPUT_TOUCH_ENABLE:
            snprintf(s, len, "Enable touch support.");
            break;
        case MENU_ENUM_LABEL_INPUT_PREFER_FRONT_TOUCH:
            snprintf(s, len, "Use front instead of back touch.");
            break;
        case MENU_ENUM_LABEL_MOUSE_ENABLE:
            snprintf(s, len, "Enable mouse input inside the menu.");
            break;
        case MENU_ENUM_LABEL_POINTER_ENABLE:
            snprintf(s, len, "Enable touch input inside the menu.");
            break;
        case MENU_ENUM_LABEL_MENU_WALLPAPER:
            snprintf(s, len, "Path to an image to set as the background.");
            break;
        case MENU_ENUM_LABEL_NAVIGATION_WRAPAROUND:
            snprintf(s, len,
                     "Wrap-around to beginning and/or end \n"
                             "if boundary of list is reached \n"
                             "horizontally and/or vertically.");
            break;
        case MENU_ENUM_LABEL_PAUSE_LIBRETRO:
            snprintf(s, len,
                     "If disabled, the game will keep \n"
                             "running in the background when we are in the \n"
                             "menu.");
            break;
        case MENU_ENUM_LABEL_SUSPEND_SCREENSAVER_ENABLE:
            snprintf(s, len,
                     "Suspends the screensaver. Is a hint that \n"
                             "does not necessarily have to be \n"
                             "honored by the video driver.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_MODE:
            snprintf(s, len,
                     "Netplay client mode for the current user. \n"
                             "Will be 'Server' mode if disabled.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_DELAY_FRAMES:
            snprintf(s, len,
                     "The amount of delay frames to use for netplay. \n"
                             " \n"
                             "Increasing this value will increase \n"
                             "performance, but introduce more latency.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_PUBLIC_ANNOUNCE:
            snprintf(s, len,
                     "Whether to announce netplay games publicly. \n"
                             " \n"
                             "If set to false, clients must manually connect \n"
                             "rather than using the public lobby.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_START_AS_SPECTATOR:
            snprintf(s, len,
                     "Whether to start netplay in spectator mode. \n"
                             " \n"
                             "If set to true, netplay will be in spectator mode \n"
                             "on start. It's always possible to change mode \n"
                             "later.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_ALLOW_SLAVES:
            snprintf(s, len,
                     "Whether to allow connections in slave mode. \n"
                             " \n"
                             "Slave-mode clients require very little processing \n"
                             "power on either side, but will suffer \n"
                             "significantly from network latency.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_REQUIRE_SLAVES:
            snprintf(s, len,
                     "Whether to disallow connections not in slave mode. \n"
                             " \n"
                             "Not recommended except for very fast networks \n"
                             "with very weak machines. \n");
            break;
        case MENU_ENUM_LABEL_NETPLAY_STATELESS_MODE:
            snprintf(s, len,
                     "Whether to run netplay in a mode not requiring\n"
                             "save states. \n"
                             " \n"
                             "If set to true, a very fast network is required,\n"
                             "but no rewinding is performed, so there will be\n"
                             "no netplay jitter.\n");
            break;
        case MENU_ENUM_LABEL_NETPLAY_CHECK_FRAMES:
            snprintf(s, len,
                     "The frequency in frames with which netplay \n"
                             "will verify that the host and client are in \n"
                             "sync. \n"
                             " \n"
                             "With most cores, this value will have no \n"
                             "visible effect and can be ignored. With \n"
                             "nondeterminstic cores, this value determines \n"
                             "how often the netplay peers will be brought \n"
                             "into sync. With buggy cores, setting this \n"
                             "to any non-zero value will cause severe \n"
                             "performance issues. Set to zero to perform \n"
                             "no checks. This value is only used on the \n"
                             "netplay host. \n");
            break;
        case MENU_ENUM_LABEL_NETPLAY_INPUT_LATENCY_FRAMES_MIN:
            snprintf(s, len,
                     "The number of frames of input latency for \n"
                     "netplay to use to hide network latency. \n"
                     " \n"
                     "When in netplay, this option delays local \n"
                     "input, so that the frame being run is \n"
                     "closer to the frames being received from \n"
                     "the network. This reduces jitter and makes \n"
                     "netplay less CPU-intensive, but at the \n"
                     "price of noticeable input lag. \n");
            break;
        case MENU_ENUM_LABEL_NETPLAY_INPUT_LATENCY_FRAMES_RANGE:
            snprintf(s, len,
                     "The range of frames of input latency that \n"
                     "may be used by netplay to hide network \n"
                     "latency. \n"
                     "\n"
                     "If set, netplay will adjust the number of \n"
                     "frames of input latency dynamically to \n"
                     "balance CPU time, input latency and \n"
                     "network latency. This reduces jitter and \n"
                     "makes netplay less CPU-intensive, but at \n"
                     "the price of unpredictable input lag. \n");
            break;
        case MENU_ENUM_LABEL_NETPLAY_NAT_TRAVERSAL:
            snprintf(s, len,
                     "When hosting, attempt to listen for\n"
                             "connections from the public internet, using\n"
                             "UPnP or similar technologies to escape LANs. \n");
            break;
        case MENU_ENUM_LABEL_NETPLAY_USE_MITM_SERVER:
            snprintf(s, len,
                     "When hosting, relay connection through a\n"
                             "man-in-the-middle server\n"
                             "to get around firewalls or NAT/UPnP issues.\n");
            break;
        case MENU_ENUM_LABEL_VIDEO_MAX_SWAPCHAIN_IMAGES:
            snprintf(s, len,
                     "Maximum amount of swapchain images. This \n"
                             "can tell the video driver to use a specific \n"
                             "video buffering mode. \n"
                             " \n"
                             "Single buffering - 1\n"
                             "Double buffering - 2\n"
                             "Triple buffering - 3\n"
                             " \n"
                             "Setting the right buffering mode can have \n"
                             "a big impact on latency.");
            break;
        case MENU_ENUM_LABEL_VIDEO_SMOOTH:
            snprintf(s, len,
                     "Smoothens picture with bilinear filtering. \n"
                             "Should be disabled if using shaders.");
            break;
        case MENU_ENUM_LABEL_TIMEDATE_ENABLE:
            snprintf(s, len,
                     "Shows current date and/or time inside menu.");
            break;
        case MENU_ENUM_LABEL_BATTERY_LEVEL_ENABLE:
            snprintf(s, len,
                     "Shows current battery level inside menu.");
            break;
        case MENU_ENUM_LABEL_CORE_ENABLE:
            snprintf(s, len,
                     "Shows current core inside menu.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_ENABLE_HOST:
            snprintf(s, len,
                     "Enables Netplay in host (server) mode.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_ENABLE_CLIENT:
            snprintf(s, len,
                     "Enables Netplay in client mode.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_DISCONNECT:
            snprintf(s, len,
                     "Disconnects an active Netplay connection.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_LAN_SCAN_SETTINGS:
            snprintf(s, len,
                     "Search for and connect to netplay hosts on the local network.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_SETTINGS:
            snprintf(s, len,
                     "Setting related to Netplay.");
            break;
        case MENU_ENUM_LABEL_DYNAMIC_WALLPAPER:
            snprintf(s, len,
                     "Dynamically load a new background \n"
                             "depending on context.");
            break;
        case MENU_ENUM_LABEL_CORE_UPDATER_BUILDBOT_URL:
            snprintf(s, len,
                     "URL to core updater directory on the \n"
                             "Libretro buildbot.");
            break;
        case MENU_ENUM_LABEL_BUILDBOT_ASSETS_URL:
            snprintf(s, len,
                     "URL to assets updater directory on the \n"
                             "Libretro buildbot.");
            break;
        case MENU_ENUM_LABEL_INPUT_REMAP_BINDS_ENABLE:
            snprintf(s, len,
                     "if enabled, overrides the input binds \n"
                             "with the remapped binds set for the \n"
                             "current core.");
            break;
        case MENU_ENUM_LABEL_OVERLAY_DIRECTORY:
            snprintf(s, len,
                     "Overlay Directory. \n"
                             " \n"
                             "Defines a directory where overlays are \n"
                             "kept for easy access.");
            break;
        case MENU_ENUM_LABEL_INPUT_MAX_USERS:
            snprintf(s, len,
                     "Maximum amount of users supported by \n"
                             "RetroArch.");
            break;
        case MENU_ENUM_LABEL_CORE_UPDATER_AUTO_EXTRACT_ARCHIVE:
            snprintf(s, len,
                     "After downloading, automatically extract \n"
                             "archives that the downloads are contained \n"
                             "inside.");
            break;
        case MENU_ENUM_LABEL_NAVIGATION_BROWSER_FILTER_SUPPORTED_EXTENSIONS_ENABLE:
            snprintf(s, len,
                     "Filter files being shown by \n"
                             "supported extensions.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_NICKNAME:
            snprintf(s, len,
                     "The username of the person running RetroArch. \n"
                             "This will be used for playing online games.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_CLIENT_SWAP_INPUT:
            snprintf(s, len,
                     "When being client over netplay, use \n"
                             "keybinds for player 1.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_TCP_UDP_PORT:
            snprintf(s, len,
                     "The port of the host IP address. \n"
                             "Can be either a TCP or UDP port.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_SPECTATOR_MODE_ENABLE:
            snprintf(s, len,
                     "Enable or disable spectator mode for \n"
                             "the user during netplay.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_IP_ADDRESS:
            snprintf(s, len,
                     "The address of the host to connect to.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_PASSWORD:
            snprintf(s, len,
                     "The password for connecting to the netplay \n"
                             "host. Used only in host mode.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_SPECTATE_PASSWORD:
            snprintf(s, len,
                     "The password for connecting to the netplay \n"
                             "host with only spectator privileges. Used \n"
                             "only in host mode.");
            break;
        case MENU_ENUM_LABEL_STDIN_CMD_ENABLE:
            snprintf(s, len,
                     "Enable stdin command interface.");
            break;
        case MENU_ENUM_LABEL_UI_COMPANION_START_ON_BOOT:
            snprintf(s, len,
                     "Start User Interface companion driver \n"
                             "on boot (if available).");
            break;
        case MENU_ENUM_LABEL_MENU_DRIVER:
            snprintf(s, len, "Menu driver to use.");
            break;
        case MENU_ENUM_LABEL_INPUT_MENU_ENUM_TOGGLE_GAMEPAD_COMBO:
            snprintf(s, len,
                     "Gamepad button combination to toggle menu. \n"
                             " \n"
                             "0 - None \n"
                             "1 - Press L + R + Y + D-Pad Down \n"
                             "simultaneously. \n"
                             "2 - Press L3 + R3 simultaneously. \n"
                             "3 - Press Start + Select simultaneously.");
            break;
        case MENU_ENUM_LABEL_INPUT_ALL_USERS_CONTROL_MENU:
            snprintf(s, len, "Allows any user to control the menu. \n"
                    " \n"
                    "When disabled, only user 1 can control the menu.");
            break;
        case MENU_ENUM_LABEL_INPUT_AUTODETECT_ENABLE:
            snprintf(s, len,
                     "Enable input auto-detection.\n"
                             " \n"
                             "Will attempt to auto-configure \n"
                             "joypads, Plug-and-Play style.");
            break;
        case MENU_ENUM_LABEL_CAMERA_ALLOW:
            snprintf(s, len,
                     "Allow or disallow camera access by \n"
                             "cores.");
            break;
        case MENU_ENUM_LABEL_LOCATION_ALLOW:
            snprintf(s, len,
                     "Allow or disallow location services \n"
                             "access by cores.");
            break;
        case MENU_ENUM_LABEL_TURBO:
            snprintf(s, len,
                     "Turbo enable.\n"
                             " \n"
                             "Holding the turbo while pressing another \n"
                             "button will let the button enter a turbo \n"
                             "mode where the button state is modulated \n"
                             "with a periodic signal. \n"
                             " \n"
                             "The modulation stops when the button \n"
                             "itself (not turbo button) is released.");
            break;
        case MENU_ENUM_LABEL_OSK_ENABLE:
            snprintf(s, len,
                     "Enable/disable on-screen keyboard.");
            break;
        case MENU_ENUM_LABEL_AUDIO_MUTE:
            snprintf(s, len,
                     "Mute/unmute audio.");
            break;
        case MENU_ENUM_LABEL_REWIND:
            snprintf(s, len,
                     "Hold button down to rewind.\n"
                             " \n"
                             "Rewind must be enabled.");
            break;
        case MENU_ENUM_LABEL_EXIT_EMULATOR:
            snprintf(s, len,
                     "Key to exit RetroArch cleanly."
#if !defined(RARCH_MOBILE) && !defined(RARCH_CONSOLE)
                            "\nKilling it in any hard way (SIGKILL, \n"
                            "etc) will terminate without saving\n"
                            "RAM, etc. On Unix-likes,\n"
                            "SIGINT/SIGTERM allows\n"
                            "a clean deinitialization."
#endif
            );
            break;
        case MENU_ENUM_LABEL_LOAD_STATE:
            snprintf(s, len,
                     "Loads state.");
            break;
        case MENU_ENUM_LABEL_SAVE_STATE:
            snprintf(s, len,
                     "Saves state.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_FLIP_PLAYERS:
            snprintf(s, len,
                     "Netplay flip users.");
            break;
        case MENU_ENUM_LABEL_NETPLAY_GAME_WATCH:
            snprintf(s, len,
                     "Netplay toggle play/spectate mode.");
            break;
        case MENU_ENUM_LABEL_CHEAT_INDEX_PLUS:
            snprintf(s, len,
                     "Increment cheat index.\n");
            break;
        case MENU_ENUM_LABEL_CHEAT_INDEX_MINUS:
            snprintf(s, len,
                     "Decrement cheat index.\n");
            break;
        case MENU_ENUM_LABEL_SHADER_PREV:
            snprintf(s, len,
                     "Applies previous shader in directory.");
            break;
        case MENU_ENUM_LABEL_SHADER_NEXT:
            snprintf(s, len,
                     "Applies next shader in directory.");
            break;
        case MENU_ENUM_LABEL_RESET:
            snprintf(s, len,
                     "Reset the content.\n");
            break;
        case MENU_ENUM_LABEL_PAUSE_TOGGLE:
            snprintf(s, len,
                     "Toggle between paused and non-paused state.");
            break;
        case MENU_ENUM_LABEL_CHEAT_TOGGLE:
            snprintf(s, len,
                     "Toggle cheat index.\n");
            break;
        case MENU_ENUM_LABEL_HOLD_FAST_FORWARD:
            snprintf(s, len,
                     "Hold for fast-forward. Releasing button \n"
                             "disables fast-forward.");
            break;
        case MENU_ENUM_LABEL_SLOWMOTION:
            snprintf(s, len,
                     "Hold for slowmotion.");
            break;
        case MENU_ENUM_LABEL_FRAME_ADVANCE:
            snprintf(s, len,
                     "Frame advance when content is paused.");
            break;
        case MENU_ENUM_LABEL_MOVIE_RECORD_TOGGLE:
            snprintf(s, len,
                     "Toggle between recording and not.");
            break;
        case MENU_ENUM_LABEL_L_X_PLUS:
        case MENU_ENUM_LABEL_L_X_MINUS:
        case MENU_ENUM_LABEL_L_Y_PLUS:
        case MENU_ENUM_LABEL_L_Y_MINUS:
        case MENU_ENUM_LABEL_R_X_PLUS:
        case MENU_ENUM_LABEL_R_X_MINUS:
        case MENU_ENUM_LABEL_R_Y_PLUS:
        case MENU_ENUM_LABEL_R_Y_MINUS:
            snprintf(s, len,
                     "Axis for analog stick (DualShock-esque).\n"
                             " \n"
                             "Bound as usual, however, if a real analog \n"
                             "axis is bound, it can be read as a true analog.\n"
                             " \n"
                             "Positive X axis is right. \n"
                             "Positive Y axis is down.");
            break;
        case MENU_ENUM_LABEL_VALUE_WHAT_IS_A_CORE_DESC:
            snprintf(s, len,
                     "RetroArch by itself does nothing. \n"
                            " \n"
                            "To make it do things, you need to \n"
                            "load a program into it. \n"
                            "\n"
                            "We call such a program 'Libretro core', \n"
                            "or 'core' in short. \n"
                            " \n"
                            "To load a core, select one from\n"
                            "'Load Core'.\n"
                            " \n"
#ifdef HAVE_NETWORKING
                    "You can obtain cores in several ways: \n"
                    "* Download them by going to\n"
                    "'%s' -> '%s'.\n"
                    "* Manually move them over to\n"
                    "'%s'.",
                    msg_hash_to_str(MENU_ENUM_LABEL_VALUE_ONLINE_UPDATER),
                    msg_hash_to_str(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_LIST),
                    msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LIBRETRO_DIR_PATH)
#else
                            "You can obtain cores by\n"
                            "manually moving them over to\n"
                            "'%s'.",
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LIBRETRO_DIR_PATH)
#endif
            );
            break;
        case MENU_ENUM_LABEL_VALUE_HELP_CHANGE_VIRTUAL_GAMEPAD_DESC:
            snprintf(s, len,
                     "You can change the virtual gamepad overlay\n"
                             "by going to '%s' -> '%s'."
                             " \n"
                             "From there you can change the overlay,\n"
                             "change the size and opacity of the buttons, etc.\n"
                             " \n"
                             "NOTE: By default, virtual gamepad overlays are\n"
                             "hidden when in the menu.\n"
                             "If you'd like to change this behavior,\n"
                             "you can set '%s' to false.",
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SETTINGS),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_OVERLAY_SETTINGS),
                     msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_HIDE_IN_MENU)
            );
            break;
        default:
            if (string_is_empty(s))
                strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE), len);
            return -1;
    }

    return 0;
}

#ifdef HAVE_MENU
static const char *menu_hash_to_str_us_label_enum(enum msg_hash_enums msg)
{
   if (msg <= MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_END &&
         msg >= MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_BEGIN)
   {
      static char hotkey_lbl[128] = {0};
      unsigned idx = msg - MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_BEGIN;
      snprintf(hotkey_lbl, sizeof(hotkey_lbl), "input_hotkey_binds_%d", idx);
      return hotkey_lbl;
   }

   switch (msg)
   {
//#include "msg_hash_lbl.h"
       MSG_HASH(MENU_ENUM_LABEL_ACCOUNTS_CHEEVOS_USERNAME,
                "accounts_cheevos_username")
       MSG_HASH(MENU_ENUM_LABEL_ACCOUNTS_LIST,
                "accounts_list")
       MSG_HASH(MENU_ENUM_LABEL_ACCOUNTS_RETRO_ACHIEVEMENTS,
                "retro_achievements")
       MSG_HASH(MENU_ENUM_LABEL_ACHIEVEMENT_LIST,
                "achievement_list")
       MSG_HASH(MENU_ENUM_LABEL_ACHIEVEMENT_LIST_HARDCORE,
                "achievement_list_hardcore")
       MSG_HASH(MENU_ENUM_LABEL_ADD_CONTENT_LIST,
                "add_content")
       MSG_HASH(MENU_ENUM_LABEL_CONFIGURATIONS_LIST,
                "configurations_list")
       MSG_HASH(MENU_ENUM_LABEL_ADD_TAB,
                "add_tab")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_TAB,
                "netplay_tab")
       MSG_HASH(MENU_ENUM_LABEL_ASSETS_DIRECTORY,
                "assets_directory")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_BLOCK_FRAMES,
                "audio_block_frames")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_DEVICE,
                "audio_device")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_DRIVER,
                "audio_driver")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_DSP_PLUGIN,
                "audio_dsp_plugin")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_ENABLE,
                "audio_enable")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_FILTER_DIR,
                "audio_filter_dir")
       MSG_HASH(MENU_ENUM_LABEL_TURBO_DEADZONE_LIST,
                "turbo_deadzone_list")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_LATENCY,
                "audio_latency")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_MAX_TIMING_SKEW,
                "audio_max_timing_skew")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_MUTE,
                "audio_mute_enable")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_OUTPUT_RATE,
                "audio_output_rate")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_RATE_CONTROL_DELTA,
                "audio_rate_control_delta")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_RESAMPLER_DRIVER,
                "audio_resampler_driver")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_SETTINGS,
                "audio_settings")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_SYNC,
                "audio_sync")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_VOLUME,
                "audio_volume")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_WASAPI_EXCLUSIVE_MODE,
                "audio_wasapi_exclusive_mode")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_WASAPI_FLOAT_FORMAT,
                "audio_wasapi_float_format")
       MSG_HASH(MENU_ENUM_LABEL_AUDIO_WASAPI_SH_BUFFER_LENGTH,
                "audio_wasapi_sh_buffer_length")
       MSG_HASH(MENU_ENUM_LABEL_AUTOSAVE_INTERVAL,
                "autosave_interval")
       MSG_HASH(MENU_ENUM_LABEL_AUTO_OVERRIDES_ENABLE,
                "auto_overrides_enable")
       MSG_HASH(MENU_ENUM_LABEL_AUTO_REMAPS_ENABLE,
                "auto_remaps_enable")
       MSG_HASH(MENU_ENUM_LABEL_AUTO_SHADERS_ENABLE,
                "auto_shaders_enable")
       MSG_HASH(MENU_ENUM_LABEL_BLOCK_SRAM_OVERWRITE,
                "block_sram_overwrite")
       MSG_HASH(MENU_ENUM_LABEL_BLUETOOTH_ENABLE,
                "bluetooth_enable")
       MSG_HASH(MENU_ENUM_LABEL_BUILDBOT_ASSETS_URL,
                "buildbot_assets_url")
       MSG_HASH(MENU_ENUM_LABEL_CACHE_DIRECTORY,
                "cache_directory")
       MSG_HASH(MENU_ENUM_LABEL_CAMERA_ALLOW,
                "camera_allow")
       MSG_HASH(MENU_ENUM_LABEL_CAMERA_DRIVER,
                "camera_driver")
       MSG_HASH(MENU_ENUM_LABEL_CB_CORE_CONTENT_DIRS_LIST,
                "cb_core_content_dirs_list")
       MSG_HASH(MENU_ENUM_LABEL_CB_CORE_CONTENT_DOWNLOAD,
                "cb_core_content_download")
       MSG_HASH(MENU_ENUM_LABEL_CB_CORE_CONTENT_LIST,
                "cb_core_content_list")
       MSG_HASH(MENU_ENUM_LABEL_CB_CORE_THUMBNAILS_DOWNLOAD,
                "cb_core_thumbnails_download")
       MSG_HASH(MENU_ENUM_LABEL_CB_CORE_UPDATER_DOWNLOAD,
                "cb_core_updater_download")
       MSG_HASH(MENU_ENUM_LABEL_CB_CORE_UPDATER_LIST,
                "cb_core_updater_list")
       MSG_HASH(MENU_ENUM_LABEL_CB_DOWNLOAD_URL,
                "cb_download_url")
       MSG_HASH(MENU_ENUM_LABEL_CB_LAKKA_DOWNLOAD,
                "cb_lakka_download")
       MSG_HASH(MENU_ENUM_LABEL_CB_LAKKA_LIST,
                "cb_lakka_list")
       MSG_HASH(MENU_ENUM_LABEL_CB_MENU_THUMBNAIL,
                "cb_menu_thumbnail")
       MSG_HASH(MENU_ENUM_LABEL_CB_MENU_WALLPAPER,
                "cb_menu_wallpaper")
       MSG_HASH(MENU_ENUM_LABEL_CB_THUMBNAILS_UPDATER_DOWNLOAD,
                "cb_thumbnails_updater_download")
       MSG_HASH(MENU_ENUM_LABEL_CB_THUMBNAILS_UPDATER_LIST,
                "cb_thumbnails_updater_list")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_ASSETS,
                "cb_update_assets")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_AUTOCONFIG_PROFILES,
                "cb_update_autoconfig_profiles")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_CHEATS,
                "cb_update_cheats")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_CORE_INFO_FILES,
                "cb_update_core_info_files")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_DATABASES,
                "cb_update_databases")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_OVERLAYS,
                "cb_update_overlays")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_SHADERS_CG,
                "cb_update_shaders_cg")
       MSG_HASH(MENU_ENUM_LABEL_CB_UPDATE_SHADERS_GLSL,
                "cb_update_shaders_glsl")
       MSG_HASH(MENU_ENUM_LABEL_CHEAT_APPLY_CHANGES,
                "cheat_apply_changes")
       MSG_HASH(MENU_ENUM_LABEL_CHEAT_DATABASE_PATH,
                "cheat_database_path")
       MSG_HASH(MENU_ENUM_LABEL_CHEAT_FILE_LOAD,
                "cheat_file_load")
       MSG_HASH(MENU_ENUM_LABEL_CHEAT_FILE_SAVE_AS,
                "cheat_file_save_as")
       MSG_HASH(MENU_ENUM_LABEL_CHEAT_NUM_PASSES,
                "cheat_num_passes")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_DESCRIPTION,
                "cheevos_description")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_ENABLE,
                "cheevos_enable")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_HARDCORE_MODE_ENABLE,
                "cheevos_hardcore_mode_enable")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_LOCKED_ACHIEVEMENTS,
                "cheevos_locked_achievements")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_LOCKED_ENTRY,
                "cheevos_locked_entry")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_PASSWORD,
                "cheevos_password")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_TEST_UNOFFICIAL,
                "cheevos_test_unofficial")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_UNLOCKED_ACHIEVEMENTS,
                "cheevos_unlocked_achievements")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_UNLOCKED_ENTRY,
                "cheevos_unlocked_entry")
       MSG_HASH(MENU_ENUM_LABEL_CHEEVOS_USERNAME,
                "cheevos_username")
       MSG_HASH(MENU_ENUM_LABEL_CLOSE_CONTENT,
                "unload_core")
       MSG_HASH(MENU_ENUM_LABEL_COLLECTION,
                "collection")
       MSG_HASH(MENU_ENUM_LABEL_CONFIGURATIONS,
                "configurations")
       MSG_HASH(MENU_ENUM_LABEL_CONFIGURATION_SETTINGS,
                "Determines how configuration files \n"
                        "are loaded and prioritized.")
       MSG_HASH(MENU_ENUM_LABEL_CONFIG_SAVE_ON_EXIT,
                "config_save_on_exit")
       MSG_HASH(MENU_ENUM_LABEL_CONNECT_WIFI,
                "connect_wifi")
       MSG_HASH(MENU_ENUM_LABEL_CONNECT_NETPLAY_ROOM,
                "connect_room")
       MSG_HASH(MENU_ENUM_LABEL_CONTENT_ACTIONS,
                "content_actions")
       MSG_HASH(MENU_ENUM_LABEL_CONTENT_COLLECTION_LIST,
                "select_from_collection")
       MSG_HASH(MENU_ENUM_LABEL_BROWSE_URL_LIST,
                "browse_url_list")
       MSG_HASH(MENU_ENUM_LABEL_CONTENT_DATABASE_DIRECTORY,
                "content_database_path")
       MSG_HASH(MENU_ENUM_LABEL_CONTENT_HISTORY_SIZE,
                "content_history_size")
       MSG_HASH(MENU_ENUM_LABEL_PLAYLIST_ENTRY_REMOVE,
                "playlist_entry_remove")
       MSG_HASH(MENU_ENUM_LABEL_CONTENT_SETTINGS,
                "quick_menu")
       MSG_HASH(MENU_ENUM_LABEL_CORE_ASSETS_DIRECTORY,
                "core_assets_directory")
       MSG_HASH(MENU_ENUM_LABEL_CORE_CHEAT_OPTIONS,
                "core_cheat_options")
       MSG_HASH(MENU_ENUM_LABEL_CORE_COUNTERS,
                "core_counters")
       MSG_HASH(MENU_ENUM_LABEL_CORE_ENABLE,
                "menu_core_enable")
       MSG_HASH(MENU_ENUM_LABEL_CORE_INFORMATION,
                "core_information")
       MSG_HASH(MENU_ENUM_LABEL_CORE_INFO_ENTRY,
                "core_info_entry")
       MSG_HASH(MENU_ENUM_LABEL_CORE_INPUT_REMAPPING_OPTIONS,
                "core_input_remapping_options")
       MSG_HASH(MENU_ENUM_LABEL_CORE_LIST,
                "load_core")
       MSG_HASH(MENU_ENUM_LABEL_CORE_OPTIONS,
                "core_options")
       MSG_HASH(MENU_ENUM_LABEL_CORE_OPTION_ENTRY,
                "core_option_entry")
       MSG_HASH(MENU_ENUM_LABEL_CORE_SETTINGS,
                "core_settings")
       MSG_HASH(MENU_ENUM_LABEL_CORE_SET_SUPPORTS_NO_CONTENT_ENABLE,
                "core_set_supports_no_content_enable")
       MSG_HASH(MENU_ENUM_LABEL_CORE_UPDATER_AUTO_EXTRACT_ARCHIVE,
                "core_updater_auto_extract_archive")
       MSG_HASH(MENU_ENUM_LABEL_CORE_UPDATER_BUILDBOT_URL,
                "core_updater_buildbot_url")
       MSG_HASH(MENU_ENUM_LABEL_CORE_UPDATER_LIST,
                "core_updater_list")
       MSG_HASH(MENU_ENUM_LABEL_CPU_ARCHITECTURE,
                "system_information_cpu_architecture")
       MSG_HASH(MENU_ENUM_LABEL_CPU_CORES,
                "system_information_cpu_cores")
       MSG_HASH(MENU_ENUM_LABEL_CURSOR_DIRECTORY,
                "cursor_directory")
       MSG_HASH(MENU_ENUM_LABEL_CURSOR_MANAGER,
                "cursor_manager")
       MSG_HASH(MENU_ENUM_LABEL_CURSOR_MANAGER_LIST,
                "cursor_manager_list")
       MSG_HASH(MENU_ENUM_LABEL_CUSTOM_BIND,
                "custom_bind")
       MSG_HASH(MENU_ENUM_LABEL_CUSTOM_BIND_ALL,
                "custom_bind_all")
       MSG_HASH(MENU_ENUM_LABEL_CUSTOM_RATIO,
                "custom_ratio")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_MANAGER,
                "database_manager")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_MANAGER_LIST,
                "database_manager_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_MUSIC,
                "deferred_music")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_BROWSE_URL_START,
                "deferred_browse_url_start")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_BROWSE_URL_LIST,
                "deferred_browse_url_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ACCOUNTS_CHEEVOS_LIST,
                "deferred_accounts_cheevos_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ACCOUNTS_LIST,
                "deferred_accounts_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ARCHIVE_ACTION,
                "deferred_archive_action")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ARCHIVE_ACTION_DETECT_CORE,
                "deferred_archive_action_detect_core")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ARCHIVE_OPEN,
                "deferred_archive_open")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ARCHIVE_OPEN_DETECT_CORE,
                "deferred_archive_open_detect_core")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_AUDIO_SETTINGS_LIST,
                "deferred_audio_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CONFIGURATION_SETTINGS_LIST,
                "deferred_configuration_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CORE_CONTENT_DIRS_LIST,
                "deferred_core_content_dirs_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CORE_CONTENT_DIRS_SUBDIR_LIST,
                "deferred_core_content_dirs_subdir_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CORE_CONTENT_LIST,
                "deferred_core_content_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CORE_LIST,
                "deferred_core_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CORE_LIST_SET,
                "deferred_core_list_set")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CORE_SETTINGS_LIST,
                "deferred_core_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CORE_UPDATER_LIST,
                "core_updater")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_CURSOR_MANAGER_LIST,
                "deferred_cursor_manager_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_DATABASE_MANAGER_LIST,
                "deferred_database_manager_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_DIRECTORY_SETTINGS_LIST,
                "deferred_directory_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_DRIVER_SETTINGS_LIST,
                "deferred_driver_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_FRAME_THROTTLE_SETTINGS_LIST,
                "deferred_frame_throttle_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_INPUT_HOTKEY_BINDS_LIST,
                "deferred_input_hotkey_binds")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_INPUT_SETTINGS_LIST,
                "deferred_input_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_LAKKA_LIST,
                "deferred_lakka_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_LAKKA_SERVICES_LIST,
                "deferred_lakka_services_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_LOGGING_SETTINGS_LIST,
                "deferred_logging_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_MENU_FILE_BROWSER_SETTINGS_LIST,
                "deferred_menu_file_browser_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_MENU_SETTINGS_LIST,
                "deferred_menu_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_NETWORK_SETTINGS_LIST,
                "deferred_network_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ONSCREEN_DISPLAY_SETTINGS_LIST,
                "deferred_onscreen_display_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ONSCREEN_OVERLAY_SETTINGS_LIST,
                "deferred_onscreen_overlay_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_ONSCREEN_NOTIFICATIONS_SETTINGS_LIST,
                "deferred_onscreen_notifications_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_PLAYLIST_SETTINGS_LIST,
                "deferred_playlist_settings")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_PRIVACY_SETTINGS_LIST,
                "deferred_privacy_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_RDB_ENTRY_DETAIL,
                "deferred_rdb_entry_detail")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_RECORDING_SETTINGS_LIST,
                "deferred_recording_settings")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_RETRO_ACHIEVEMENTS_SETTINGS_LIST,
                "deferred_retro_achievements_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_REWIND_SETTINGS_LIST,
                "deferred_rewind_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_SAVING_SETTINGS_LIST,
                "deferred_saving_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_THUMBNAILS_UPDATER_LIST,
                "deferred_thumbnails_updater_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_UPDATER_SETTINGS_LIST,
                "deferred_updater_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_USER_BINDS_LIST,
                "deferred_user_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_USER_INTERFACE_SETTINGS_LIST,
                "deferred_user_interface_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_USER_SETTINGS_LIST,
                "deferred_user_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_VIDEO_FILTER,
                "deferred_video_filter")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_VIDEO_SETTINGS_LIST,
                "deferred_video_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_WIFI_SETTINGS_LIST,
                "deferred_wifi_settings_list")
       MSG_HASH(MENU_ENUM_LABEL_DELETE_ENTRY,
                "delete_entry")
       MSG_HASH(MENU_ENUM_LABEL_FAVORITES,
                "favorites")
       MSG_HASH(MENU_ENUM_LABEL_DIRECTORY_SETTINGS,
                "directory_settings")
       MSG_HASH(MENU_ENUM_LABEL_DISK_CYCLE_TRAY_STATUS,
                "disk_cycle_tray_status")
       MSG_HASH(MENU_ENUM_LABEL_DISK_IMAGE_APPEND,
                "disk_image_append")
       MSG_HASH(MENU_ENUM_LABEL_DISK_OPTIONS,
                "core_disk_options")
       MSG_HASH(MENU_ENUM_LABEL_DOWNLOADED_FILE_DETECT_CORE_LIST,
                "downloaded_file_detect_core_list")
       MSG_HASH(MENU_ENUM_LABEL_DOWNLOAD_CORE_CONTENT,
                "download_core_content")
       MSG_HASH(MENU_ENUM_LABEL_DOWNLOAD_CORE_CONTENT_DIRS,
                "download_core_content_dirs")
       MSG_HASH(MENU_ENUM_LABEL_CONTENT_DIR,
                "content_directory")
       MSG_HASH(MENU_ENUM_LABEL_DPI_OVERRIDE_ENABLE,
                "dpi_override_enable")
       MSG_HASH(MENU_ENUM_LABEL_DPI_OVERRIDE_VALUE,
                "dpi_override_value")
       MSG_HASH(MENU_ENUM_LABEL_DRIVER_SETTINGS,
                "driver_settings")
       MSG_HASH(MENU_ENUM_LABEL_CHECK_FOR_MISSING_FIRMWARE,
                "check_for_missing_firmware")
       MSG_HASH(MENU_ENUM_LABEL_DUMMY_ON_CORE_SHUTDOWN,
                "dummy_on_core_shutdown")
       MSG_HASH(MENU_ENUM_LABEL_DYNAMIC_WALLPAPER,
                "menu_dynamic_wallpaper_enable")
       MSG_HASH(MENU_ENUM_LABEL_DYNAMIC_WALLPAPERS_DIRECTORY,
                "dynamic_wallpapers_directory")
       MSG_HASH(MENU_ENUM_LABEL_ENTRY_HOVER_COLOR,
                "menu_entry_hover_color")
       MSG_HASH(MENU_ENUM_LABEL_ENTRY_NORMAL_COLOR,
                "menu_entry_normal_color")
       MSG_HASH(MENU_ENUM_LABEL_FASTFORWARD_RATIO,
                "fastforward_ratio")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_CORE,
                "file_browser_core")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_CORE_DETECTED,
                "file_browser_core_detected")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_CORE_SELECT_FROM_COLLECTION,
                "file_browser_core_select_from_collection")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_CORE_SELECT_FROM_COLLECTION_CURRENT_CORE,
                "file_browser_core_select_from_collection_current_core")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_DIRECTORY,
                "file_browser_directory")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_IMAGE,
                "file_browser_image")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_IMAGE_OPEN_WITH_VIEWER,
                "file_browser_image_open_with_viewer")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_MOVIE_OPEN,
                "file_browser_movie_open")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_MUSIC_OPEN,
                "file_browser_music_open")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_PLAIN_FILE,
                "file_browser_plain_file")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_REMAP,
                "file_browser_remap")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_SHADER,
                "file_browser_shader")
       MSG_HASH(MENU_ENUM_LABEL_FILE_BROWSER_SHADER_PRESET,
                "file_browser_shader_preset")
       MSG_HASH(MENU_ENUM_LABEL_FPS_SHOW,
                "fps_show")
       MSG_HASH(MENU_ENUM_LABEL_FRAME_THROTTLE_ENABLE,
                "fastforward_ratio_throttle_enable")
       MSG_HASH(MENU_ENUM_LABEL_FRAME_THROTTLE_SETTINGS,
                "frame_throttle_settings")
       MSG_HASH(MENU_ENUM_LABEL_FRONTEND_COUNTERS,
                "frontend_counters")
       MSG_HASH(MENU_ENUM_LABEL_GAME_SPECIFIC_OPTIONS,
                "game_specific_options")
       MSG_HASH(MENU_ENUM_LABEL_GAME_SPECIFIC_OPTIONS_CREATE,
                "game_specific_options_create")
       MSG_HASH(MENU_ENUM_LABEL_GAME_SPECIFIC_OPTIONS_IN_USE,
                "game_specific_options_in_use")
       MSG_HASH(MENU_ENUM_LABEL_HELP,
                "help")
       MSG_HASH(MENU_ENUM_LABEL_HELP_AUDIO_VIDEO_TROUBLESHOOTING,
                "help_audio_video_troubleshooting")
       MSG_HASH(MENU_ENUM_LABEL_HELP_CHANGE_VIRTUAL_GAMEPAD,
                "help_change_virtual_gamepad")
       MSG_HASH(MENU_ENUM_LABEL_HELP_CONTROLS,
                "help_controls")
       MSG_HASH(MENU_ENUM_LABEL_HELP_LIST,
                "help_list")
       MSG_HASH(MENU_ENUM_LABEL_HELP_LOADING_CONTENT,
                "help_loading_content")
       MSG_HASH(MENU_ENUM_LABEL_HELP_SCANNING_CONTENT,
                "help_scanning_content")
       MSG_HASH(MENU_ENUM_LABEL_HELP_WHAT_IS_A_CORE,
                "help_what_is_a_core")
       MSG_HASH(MENU_ENUM_LABEL_HISTORY_LIST_ENABLE,
                "history_list_enable")
       MSG_HASH(MENU_ENUM_LABEL_HISTORY_TAB,
                "history_tab")
       MSG_HASH(MENU_ENUM_LABEL_HORIZONTAL_MENU,
                "horizontal_menu")
       MSG_HASH(MENU_ENUM_LABEL_IMAGES_TAB,
                "images_tab")
       MSG_HASH(MENU_ENUM_LABEL_INFORMATION,
                "information")
       MSG_HASH(MENU_ENUM_LABEL_INFORMATION_LIST,
                "information_list")
       MSG_HASH(MENU_ENUM_LABEL_INFO_SCREEN,
                "info_screen")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_ALL_USERS_CONTROL_MENU,
                "all_users_control_menu")
       MSG_HASH(MENU_ENUM_LABEL_MENU_INPUT_SWAP_OK_CANCEL,
                "menu_swap_ok_cancel")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_AUTODETECT_ENABLE,
                "input_autodetect_enable")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_AXIS_THRESHOLD,
                "input_axis_threshold")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_BIND_MODE,
                "input_bind_mode")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_BIND_TIMEOUT,
                "input_bind_timeout")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_DESCRIPTOR_HIDE_UNBOUND,
                "input_descriptor_hide_unbound")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_DESCRIPTOR_LABEL_SHOW,
                "input_descriptor_label_show")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_DRIVER,
                "input_driver")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_DUTY_CYCLE,
                "input_duty_cycle")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_HOTKEY_BINDS,
                "input_hotkey_binds")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_HOTKEY_BINDS_BEGIN,
                "input_hotkey_binds_begin")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_ICADE_ENABLE,
                "input_icade_enable")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_KEYBOARD_GAMEPAD_MAPPING_TYPE,
                "keyboard_gamepad_mapping_type")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_LIBRETRO_DEVICE,
                "input_libretro_device_p%u")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_MAX_USERS,
                "input_max_users")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_MENU_ENUM_TOGGLE_GAMEPAD_COMBO,
                "input_menu_toggle_gamepad_combo")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_OVERLAY_ENABLE,
                "input_overlay_enable")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_OVERLAY_HIDE_IN_MENU,
                "overlay_hide_in_menu")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_PLAYER_ANALOG_DPAD_MODE,
                "input_player%u_analog_dpad_mode")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_POLL_TYPE_BEHAVIOR,
                "input_poll_type_behavior")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_PREFER_FRONT_TOUCH,
                "input_prefer_front_touch")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_REMAPPING_DIRECTORY,
                "input_remapping_directory")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_REMAP_BINDS_ENABLE,
                "input_remap_binds_enable")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_SETTINGS,
                "input_settings")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_SETTINGS_BEGIN,
                "input_settings_begin")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_SMALL_KEYBOARD_ENABLE,
                "input_small_keyboard_enable")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_TOUCH_ENABLE,
                "input_touch_enable")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_TURBO_PERIOD,
                "input_turbo_period")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_10_BINDS,
                "10_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_11_BINDS,
                "11_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_12_BINDS,
                "12_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_13_BINDS,
                "13_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_14_BINDS,
                "14_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_15_BINDS,
                "15_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_16_BINDS,
                "16_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_1_BINDS,
                "1_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_2_BINDS,
                "2_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_3_BINDS,
                "3_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_4_BINDS,
                "4_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_5_BINDS,
                "5_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_6_BINDS,
                "6_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_7_BINDS,
                "7_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_8_BINDS,
                "8_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_USER_9_BINDS,
                "9_input_binds_list")
       MSG_HASH(MENU_ENUM_LABEL_JOYPAD_AUTOCONFIG_DIR,
                "joypad_autoconfig_dir")
       MSG_HASH(MENU_ENUM_LABEL_JOYPAD_DRIVER,
                "input_joypad_driver")
       MSG_HASH(MENU_ENUM_LABEL_LAKKA_SERVICES,
                "lakka_services")
       MSG_HASH(MENU_ENUM_LABEL_LIBRETRO_DIR_PATH,
                "libretro_dir_path")
       MSG_HASH(MENU_ENUM_LABEL_LIBRETRO_INFO_PATH,
                "libretro_info_path")
       MSG_HASH(MENU_ENUM_LABEL_LIBRETRO_LOG_LEVEL,
                "libretro_log_level")
       MSG_HASH(MENU_ENUM_LABEL_LOAD_ARCHIVE,
                "load_archive")
       MSG_HASH(MENU_ENUM_LABEL_LOAD_ARCHIVE_DETECT_CORE,
                "load_archive_detect_core")
       MSG_HASH(MENU_ENUM_LABEL_LOAD_CONTENT_HISTORY,
                "load_recent")
       MSG_HASH(MENU_ENUM_LABEL_LOAD_CONTENT_LIST,
                "load_content")
       MSG_HASH(MENU_ENUM_LABEL_LOAD_STATE,
                "loadstate")
       MSG_HASH(MENU_ENUM_LABEL_LOCATION_ALLOW,
                "location_allow")
       MSG_HASH(MENU_ENUM_LABEL_LOCATION_DRIVER,
                "location_driver")
       MSG_HASH(MENU_ENUM_LABEL_LOGGING_SETTINGS,
                "logging_settings")
       MSG_HASH(MENU_ENUM_LABEL_LOG_VERBOSITY,
                "log_verbosity")
       MSG_HASH(MENU_ENUM_LABEL_MAIN_MENU,
                "main_menu")
       MSG_HASH(MENU_ENUM_LABEL_MANAGEMENT,
                "database_settings")
       MSG_HASH(MENU_ENUM_LABEL_MATERIALUI_MENU_COLOR_THEME,
                "materialui_menu_color_theme")
       MSG_HASH(MENU_ENUM_LABEL_MATERIALUI_MENU_FOOTER_OPACITY,
                "materialui_menu_footer_opacity")
       MSG_HASH(MENU_ENUM_LABEL_MATERIALUI_MENU_HEADER_OPACITY,
                "materialui_menu_header_opacity")
       MSG_HASH(MENU_ENUM_LABEL_MENU_DRIVER,
                "menu_driver")
       MSG_HASH(MENU_ENUM_LABEL_MENU_ENUM_THROTTLE_FRAMERATE,
                "menu_throttle_framerate")
       MSG_HASH(MENU_ENUM_LABEL_MENU_FILE_BROWSER_SETTINGS,
                "menu_file_browser_settings")
       MSG_HASH(MENU_ENUM_LABEL_MENU_LINEAR_FILTER,
                "menu_linear_filter")
       MSG_HASH(MENU_ENUM_LABEL_MENU_SETTINGS,
                "menu_settings")
       MSG_HASH(MENU_ENUM_LABEL_MENU_WALLPAPER,
                "menu_wallpaper")
       MSG_HASH(MENU_ENUM_LABEL_MENU_WALLPAPER_OPACITY,
                "menu_wallpaper_opacity")
       MSG_HASH(MENU_ENUM_LABEL_MOUSE_ENABLE,
                "menu_mouse_enable")
       MSG_HASH(MENU_ENUM_LABEL_MUSIC_TAB,
                "music_tab")
       MSG_HASH(MENU_ENUM_LABEL_NAVIGATION_BROWSER_FILTER_SUPPORTED_EXTENSIONS_ENABLE,
                "menu_navigation_browser_filter_supported_extensions_enable")
       MSG_HASH(MENU_ENUM_LABEL_NAVIGATION_WRAPAROUND,
                "menu_navigation_wraparound_enable")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY,
                "netplay")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_CHECK_FRAMES,
                "netplay_check_frames")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_CLIENT_SWAP_INPUT,
                "netplay_client_swap_input")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_INPUT_LATENCY_FRAMES_MIN,
                "netplay_input_latency_frames_min")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_INPUT_LATENCY_FRAMES_RANGE,
                "netplay_input_latency_frames_range")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_DISCONNECT,
                "menu_netplay_disconnect")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_ENABLE,
                "netplay_enable")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_ENABLE_CLIENT,
                "menu_netplay_enable_client")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_ENABLE_HOST,
                "menu_netplay_enable_host")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_ALLOW_SLAVES,
                "netplay_allow_slaves")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_IP_ADDRESS,
                "netplay_ip_address")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_MODE,
                "netplay_mode")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_NAT_TRAVERSAL,
                "netplay_nat_traversal")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_NICKNAME,
                "netplay_nickname")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_PASSWORD,
                "netplay_password")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_SETTINGS,
                "menu_netplay_settings")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_PUBLIC_ANNOUNCE,
                "netplay_public_announce")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_REQUIRE_SLAVES,
                "netplay_require_slaves")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_SPECTATE_PASSWORD,
                "netplay_spectate_password")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_SPECTATOR_MODE_ENABLE,
                "netplay_spectator_mode_enable")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_START_AS_SPECTATOR,
                "netplay_start_as_spectator")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_STATELESS_MODE,
                "netplay_stateless_mode")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_TCP_UDP_PORT,
                "netplay_tcp_udp_port")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_LAN_SCAN_SETTINGS,
                "Search for and connect to netplay hosts on the local network.")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_CMD_ENABLE,
                "network_cmd_enable")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_CMD_PORT,
                "network_cmd_port")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_INFORMATION,
                "network_information")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_INFO_ENTRY,
                "network_info_entry")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_REMOTE_ENABLE,
                "network_remote_enable")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_REMOTE_PORT,
                "network_remote_base_port")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_REMOTE_USER_1_ENABLE,
                "network_remote_user_1_enable")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_REMOTE_USER_LAST_ENABLE,
                "network_remote_user_last_enable")
       MSG_HASH(MENU_ENUM_LABEL_NETWORK_SETTINGS,
                "network_settings")
       MSG_HASH(MENU_ENUM_LABEL_NO_ACHIEVEMENTS_TO_DISPLAY,
                "no_achievements_to_display")
       MSG_HASH(MENU_ENUM_LABEL_NO_CORES_AVAILABLE,
                "no_cores_available")
       MSG_HASH(MENU_ENUM_LABEL_NO_CORE_INFORMATION_AVAILABLE,
                "no_core_information_available")
       MSG_HASH(MENU_ENUM_LABEL_NO_CORE_OPTIONS_AVAILABLE,
                "no_core_options_available")
       MSG_HASH(MENU_ENUM_LABEL_NO_ENTRIES_TO_DISPLAY,
                "no_entries_to_display")
       MSG_HASH(MENU_ENUM_LABEL_NO_HISTORY_AVAILABLE,
                "no_history")
       MSG_HASH(MENU_ENUM_LABEL_NO_ITEMS,
                "no_items")
       MSG_HASH(MENU_ENUM_LABEL_NO_NETPLAY_HOSTS_FOUND,
                "no_netplay_hosts_found")
       MSG_HASH(MENU_ENUM_LABEL_NO_NETWORKS_FOUND,
                "no_networks_found")
       MSG_HASH(MENU_ENUM_LABEL_NO_PERFORMANCE_COUNTERS,
                "no_performance_counters")
       MSG_HASH(MENU_ENUM_LABEL_NO_PLAYLISTS,
                "no_playlists")
       MSG_HASH(MENU_ENUM_LABEL_NO_PLAYLIST_ENTRIES_AVAILABLE,
                "no_playlist_entries_available")
       MSG_HASH(MENU_ENUM_LABEL_NO_SETTINGS_FOUND,
                "menu_label_no_settings_found")
       MSG_HASH(MENU_ENUM_LABEL_NO_SHADER_PARAMETERS,
                "no_shader_parameters.")
       MSG_HASH(MENU_ENUM_LABEL_ONLINE,
                "online")
       MSG_HASH(MENU_ENUM_LABEL_ONLINE_UPDATER,
                "online_updater")
       MSG_HASH(MENU_ENUM_LABEL_ONSCREEN_DISPLAY_SETTINGS,
                "onscreen_display_settings")
       MSG_HASH(MENU_ENUM_LABEL_ONSCREEN_OVERLAY_SETTINGS,
                "onscreen_overlay_settings")
       MSG_HASH(MENU_ENUM_LABEL_ONSCREEN_NOTIFICATIONS_SETTINGS,
                "onscreen_notifications_settings")
       MSG_HASH(MENU_ENUM_LABEL_OPEN_ARCHIVE,
                "open_archive")
       MSG_HASH(MENU_ENUM_LABEL_OPEN_ARCHIVE_DETECT_CORE,
                "open_archive_detect_core")
       MSG_HASH(MENU_ENUM_LABEL_OVERLAY_AUTOLOAD_PREFERRED,
                "overlay_autoload_preferred")
       MSG_HASH(MENU_ENUM_LABEL_OVERLAY_DIRECTORY,
                "overlay_directory")
       MSG_HASH(MENU_ENUM_LABEL_OVERLAY_OPACITY,
                "input_overlay_opacity")
       MSG_HASH(MENU_ENUM_LABEL_OVERLAY_PRESET,
                "input_overlay")
       MSG_HASH(MENU_ENUM_LABEL_OVERLAY_SCALE,
                "input_overlay_scale")
       MSG_HASH(MENU_ENUM_LABEL_PAL60_ENABLE,
                "pal60_enable")
       MSG_HASH(MENU_ENUM_LABEL_PARENT_DIRECTORY,
                "parent_directory")
       MSG_HASH(MENU_ENUM_LABEL_PAUSE_LIBRETRO,
                "menu_pause_libretro")
       MSG_HASH(MENU_ENUM_LABEL_PAUSE_NONACTIVE,
                "pause_nonactive")
       MSG_HASH(MENU_ENUM_LABEL_PERFCNT_ENABLE,
                "perfcnt_enable")
       MSG_HASH(MENU_ENUM_LABEL_PLAYLISTS_TAB,
                "playlists_tab")
       MSG_HASH(MENU_ENUM_LABEL_PLAYLIST_COLLECTION_ENTRY,
                "playlist_collection_entry")
       MSG_HASH(MENU_ENUM_LABEL_PLAYLIST_DIRECTORY,
                "playlist_directory")
       MSG_HASH(MENU_ENUM_LABEL_PLAYLIST_ENTRY,
                "playlist_entry")
       MSG_HASH(MENU_ENUM_LABEL_PLAYLIST_SETTINGS,
                "playlist_settings")
       MSG_HASH(MENU_ENUM_LABEL_PLAYLIST_SETTINGS_BEGIN,
                "playlist_settings_begin")
       MSG_HASH(MENU_ENUM_LABEL_POINTER_ENABLE,
                "menu_pointer_enable")
       MSG_HASH(MENU_ENUM_LABEL_PRIVACY_SETTINGS,
                "privacy_settings")
       MSG_HASH(MENU_ENUM_LABEL_QUIT_RETROARCH,
                "quit_retroarch")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY,
                "rdb_entry")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_ANALOG,
                "rdb_entry_analog")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_BBFC_RATING,
                "rdb_entry_bbfc_rating")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_CERO_RATING,
                "rdb_entry_cero_rating")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_CRC32,
                "rdb_entry_crc32")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_DESCRIPTION,
                "rdb_entry_description")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_DEVELOPER,
                "rdb_entry_developer")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_EDGE_MAGAZINE_ISSUE,
                "rdb_entry_edge_magazine_issue")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_EDGE_MAGAZINE_RATING,
                "rdb_entry_edge_magazine_rating")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_EDGE_MAGAZINE_REVIEW,
                "rdb_entry_edge_magazine_review")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_ELSPA_RATING,
                "rdb_entry_elspa_rating")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_ENHANCEMENT_HW,
                "rdb_entry_enhancement_hw")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_ESRB_RATING,
                "rdb_entry_esrb_rating")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_FAMITSU_MAGAZINE_RATING,
                "rdb_entry_famitsu_magazine_rating")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_FRANCHISE,
                "rdb_entry_franchise")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_GENRE,
                "rdb_entry_genre")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_MAX_USERS,
                "rdb_entry_max_users")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_MD5,
                "rdb_entry_md5")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_NAME,
                "rdb_entry_name")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_ORIGIN,
                "rdb_entry_origin")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_PEGI_RATING,
                "rdb_entry_pegi_rating")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_PUBLISHER,
                "rdb_entry_publisher")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_RELEASE_MONTH,
                "rdb_entry_releasemonth")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_RELEASE_YEAR,
                "rdb_entry_releaseyear")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_SERIAL,
                "rdb_entry_serial")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_SHA1,
                "rdb_entry_sha1")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_START_CONTENT,
                "rdb_entry_start_content")
       MSG_HASH(MENU_ENUM_LABEL_RDB_ENTRY_TGDB_RATING,
                "rdb_entry_tgdb_rating")
       MSG_HASH(MENU_ENUM_LABEL_REBOOT,
                "reboot")
       MSG_HASH(MENU_ENUM_LABEL_RECORDING_CONFIG_DIRECTORY,
                "recording_config_directory")
       MSG_HASH(MENU_ENUM_LABEL_RECORDING_OUTPUT_DIRECTORY,
                "recording_output_directory")
       MSG_HASH(MENU_ENUM_LABEL_RECORDING_SETTINGS,
                "recording_settings")
       MSG_HASH(MENU_ENUM_LABEL_RECORD_CONFIG,
                "record_config")
       MSG_HASH(MENU_ENUM_LABEL_RECORD_DRIVER,
                "record_driver")
       MSG_HASH(MENU_ENUM_LABEL_RECORD_ENABLE,
                "record_enable")
       MSG_HASH(MENU_ENUM_LABEL_RECORD_PATH,
                "record_path")
       MSG_HASH(MENU_ENUM_LABEL_RECORD_USE_OUTPUT_DIRECTORY,
                "record_use_output_directory")
       MSG_HASH(MENU_ENUM_LABEL_REMAP_FILE_LOAD,
                "remap_file_load")
       MSG_HASH(MENU_ENUM_LABEL_REMAP_FILE_SAVE_CORE,
                "remap_file_save_core")
       MSG_HASH(MENU_ENUM_LABEL_REMAP_FILE_SAVE_GAME,
                "remap_file_save_game")
       MSG_HASH(MENU_ENUM_LABEL_RESTART_CONTENT,
                "restart_content")
       MSG_HASH(MENU_ENUM_LABEL_RESTART_RETROARCH,
                "restart_retroarch")
       MSG_HASH(MENU_ENUM_LABEL_RESUME,
                "resume")
       MSG_HASH(MENU_ENUM_LABEL_RESUME_CONTENT,
                "resume_content")
       MSG_HASH(MENU_ENUM_LABEL_RETRO_ACHIEVEMENTS_SETTINGS,
                "retro_achievements_settings")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_META_REWIND,
                "input_meta_rewind")
       MSG_HASH(MENU_ENUM_LABEL_REWIND_ENABLE,
                "rewind_enable")
       MSG_HASH(MENU_ENUM_LABEL_REWIND_GRANULARITY,
                "rewind_granularity")
       MSG_HASH(MENU_ENUM_LABEL_REWIND_SETTINGS,
                "rewind_settings")
       MSG_HASH(MENU_ENUM_LABEL_RGUI_BROWSER_DIRECTORY,
                "rgui_browser_directory")
       MSG_HASH(MENU_ENUM_LABEL_RGUI_CONFIG_DIRECTORY,
                "rgui_config_directory")
       MSG_HASH(MENU_ENUM_LABEL_RGUI_SHOW_START_SCREEN,
                "rgui_show_start_screen")
       MSG_HASH(MENU_ENUM_LABEL_RUN,
                "collection")
       MSG_HASH(MENU_ENUM_LABEL_RUN_MUSIC,
                "collection_music")
       MSG_HASH(MENU_ENUM_LABEL_SAMBA_ENABLE,
                "samba_enable")
       MSG_HASH(MENU_ENUM_LABEL_SAVEFILE_DIRECTORY,
                "savefile_directory")
       MSG_HASH(MENU_ENUM_LABEL_SAVESTATE_AUTO_INDEX,
                "savestate_auto_index")
       MSG_HASH(MENU_ENUM_LABEL_SAVESTATE_AUTO_LOAD,
                "savestate_auto_load")
       MSG_HASH(MENU_ENUM_LABEL_SAVESTATE_AUTO_SAVE,
                "savestate_auto_save")
       MSG_HASH(MENU_ENUM_LABEL_SAVESTATE_DIRECTORY,
                "savestate_directory")
       MSG_HASH(MENU_ENUM_LABEL_SAVE_CURRENT_CONFIG,
                "save_current_config")
       MSG_HASH(MENU_ENUM_LABEL_SAVE_CURRENT_CONFIG_OVERRIDE_CORE,
                "save_current_config_override_core")
       MSG_HASH(MENU_ENUM_LABEL_SAVE_CURRENT_CONFIG_OVERRIDE_GAME,
                "save_current_config_override_game")
       MSG_HASH(MENU_ENUM_LABEL_SAVE_NEW_CONFIG,
                "save_new_config")
       MSG_HASH(MENU_ENUM_LABEL_SAVE_STATE,
                "savestate")
       MSG_HASH(MENU_ENUM_LABEL_SAVING_SETTINGS,
                "saving_settings")
       MSG_HASH(MENU_ENUM_LABEL_SCAN_DIRECTORY,
                "scan_directory")
       MSG_HASH(MENU_ENUM_LABEL_SCAN_FILE,
                "scan_file")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_REFRESH_ROOMS,
                "refresh_rooms")
       MSG_HASH(MENU_ENUM_LABEL_SCAN_THIS_DIRECTORY,
                "scan_this_directory")
       MSG_HASH(MENU_ENUM_LABEL_SCREENSHOT_DIRECTORY,
                "screenshot_directory")
       MSG_HASH(MENU_ENUM_LABEL_SCREEN_RESOLUTION,
                "screen_resolution")
       MSG_HASH(MENU_ENUM_LABEL_SETTINGS,
                "settings")
       MSG_HASH(MENU_ENUM_LABEL_SETTINGS_TAB,
                "settings_tab")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_APPLY_CHANGES,
                "shader_apply_changes")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_OPTIONS,
                "shader_options")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_PARAMETERS_ENTRY,
                "shader_parameters_entry")
       MSG_HASH(MENU_ENUM_LABEL_SHOW_ADVANCED_SETTINGS,
                "menu_show_advanced_settings")
       MSG_HASH(MENU_ENUM_LABEL_SHOW_HIDDEN_FILES,
                "show_hidden_files")
       MSG_HASH(MENU_ENUM_LABEL_SHUTDOWN,
                "shutdown")
       MSG_HASH(MENU_ENUM_LABEL_SLOWMOTION_RATIO,
                "slowmotion_ratio")
       MSG_HASH(MENU_ENUM_LABEL_SORT_SAVEFILES_ENABLE,
                "sort_savefiles_enable")
       MSG_HASH(MENU_ENUM_LABEL_SORT_SAVESTATES_ENABLE,
                "sort_savestates_enable")
       MSG_HASH(MENU_ENUM_LABEL_SSH_ENABLE,
                "ssh_enable")
       MSG_HASH(MENU_ENUM_LABEL_START_CORE,
                "start_core")
       MSG_HASH(MENU_ENUM_LABEL_START_NET_RETROPAD,
                "menu_start_net_retropad")
       MSG_HASH(MENU_ENUM_LABEL_START_VIDEO_PROCESSOR,
                "menu_start_video_processor")
       MSG_HASH(MENU_ENUM_LABEL_STATE_SLOT,
                "state_slot")
       MSG_HASH(MENU_ENUM_LABEL_STDIN_CMD_ENABLE,
                "stdin_commands")
       MSG_HASH(MENU_ENUM_LABEL_SUSPEND_SCREENSAVER_ENABLE,
                "suspend_screensaver_enable")
       MSG_HASH(MENU_ENUM_LABEL_SYSTEM_BGM_ENABLE,
                "system_bgm_enable")
       MSG_HASH(MENU_ENUM_LABEL_SYSTEM_DIRECTORY,
                "system_directory")
       MSG_HASH(MENU_ENUM_LABEL_SYSTEM_INFORMATION,
                "system_information")
       MSG_HASH(MENU_ENUM_LABEL_SYSTEM_INFO_ENTRY,
                "system_info_entry")
       MSG_HASH(MENU_ENUM_LABEL_TAKE_SCREENSHOT,
                "take_screenshot")
       MSG_HASH(MENU_ENUM_LABEL_THREADED_DATA_RUNLOOP_ENABLE,
                "threaded_data_runloop_enable")
       MSG_HASH(MENU_ENUM_LABEL_THUMBNAILS,
                "thumbnails")
       MSG_HASH(MENU_ENUM_LABEL_THUMBNAILS_DIRECTORY,
                "thumbnails_directory")
       MSG_HASH(MENU_ENUM_LABEL_THUMBNAILS_UPDATER_LIST,
                "thumbnails_updater_list")
       MSG_HASH(MENU_ENUM_LABEL_TIMEDATE_ENABLE,
                "menu_timedate_enable")
       MSG_HASH(MENU_ENUM_LABEL_TITLE_COLOR,
                "menu_title_color")
       MSG_HASH(MENU_ENUM_LABEL_UI_COMPANION_ENABLE,
                "ui_companion_enable")
       MSG_HASH(MENU_ENUM_LABEL_UI_COMPANION_START_ON_BOOT,
                "ui_companion_start_on_boot")
       MSG_HASH(MENU_ENUM_LABEL_UI_MENUBAR_ENABLE,
                "ui_menubar_enable")
       MSG_HASH(MENU_ENUM_LABEL_UNDO_LOAD_STATE,
                "undoloadstate")
       MSG_HASH(MENU_ENUM_LABEL_UNDO_SAVE_STATE,
                "undosavestate")
       MSG_HASH(MENU_ENUM_LABEL_UPDATER_SETTINGS,
                "updater_settings")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_ASSETS,
                "update_assets")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_AUTOCONFIG_PROFILES,
                "update_autoconfig_profiles")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_CG_SHADERS,
                "update_cg_shaders")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_CHEATS,
                "update_cheats")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_CORE_INFO_FILES,
                "update_core_info_files")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_DATABASES,
                "update_databases")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_GLSL_SHADERS,
                "update_glsl_shaders")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_LAKKA,
                "update_lakka")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_OVERLAYS,
                "update_overlays")
       MSG_HASH(MENU_ENUM_LABEL_UPDATE_SLANG_SHADERS,
                "update_slang_shaders")
       MSG_HASH(MENU_ENUM_LABEL_URL_ENTRY,
                "url_entry")
       MSG_HASH(MENU_ENUM_LABEL_USER_INTERFACE_SETTINGS,
                "user_interface_settings")
       MSG_HASH(MENU_ENUM_LABEL_USER_LANGUAGE,
                "user_language")
       MSG_HASH(MENU_ENUM_LABEL_USER_SETTINGS,
                "user_settings")
       MSG_HASH(MENU_ENUM_LABEL_USE_BUILTIN_IMAGE_VIEWER,
                "use_builtin_image_viewer")
       MSG_HASH(MENU_ENUM_LABEL_USE_BUILTIN_PLAYER,
                "use_builtin_player")
       MSG_HASH(MENU_ENUM_LABEL_USE_THIS_DIRECTORY,
                "use_this_directory")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_ALLOW_ROTATE,
                "video_allow_rotate")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_ASPECT_RATIO,
                "video_aspect_ratio")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_ASPECT_RATIO_AUTO,
                "video_aspect_ratio_auto")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_ASPECT_RATIO_INDEX,
                "aspect_ratio_index")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_BLACK_FRAME_INSERTION,
                "video_black_frame_insertion")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_CROP_OVERSCAN,
                "video_crop_overscan")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_DISABLE_COMPOSITION,
                "video_disable_composition")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_DRIVER,
                "video_driver")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FILTER,
                "video_filter")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FILTER_DIR,
                "video_filter_dir")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FILTER_FLICKER,
                "video_filter_flicker")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FONT_ENABLE,
                "video_font_enable")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FONT_PATH,
                "video_font_path")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FONT_SIZE,
                "video_font_size")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FORCE_ASPECT,
                "video_force_aspect")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FORCE_SRGB_DISABLE,
                "video_force_srgb_disable")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FRAME_DELAY,
                "video_frame_delay")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FULLSCREEN,
                "video_fullscreen")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_GAMMA,
                "video_gamma")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_GPU_RECORD,
                "video_gpu_record")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_GPU_SCREENSHOT,
                "video_gpu_screenshot")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_HARD_SYNC,
                "video_hard_sync")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_HARD_SYNC_FRAMES,
                "video_hard_sync_frames")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_MAX_SWAPCHAIN_IMAGES,
                "video_max_swapchain_images")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_MESSAGE_POS_X,
                "video_message_pos_x")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_MESSAGE_POS_Y,
                "video_message_pos_y")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_MONITOR_INDEX,
                "video_monitor_index")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_POST_FILTER_RECORD,
                "video_post_filter_record")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_REFRESH_RATE,
                "video_refresh_rate")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_REFRESH_RATE_AUTO,
                "video_refresh_rate_auto")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_ROTATION,
                "video_rotation")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SCALE,
                "video_scale")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SCALE_INTEGER,
                "video_scale_integer")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SETTINGS,
                "video_settings")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_DIR,
                "video_shader_dir")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_FILTER_PASS,
                "video_shader_filter_pass")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_NUM_PASSES,
                "video_shader_num_passes")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_PARAMETERS,
                "video_shader_parameters")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_PASS,
                "video_shader_pass")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_PRESET,
                "video_shader_preset")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_PRESET_PARAMETERS,
                "video_shader_preset_parameters")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_PRESET_SAVE_AS,
                "video_shader_preset_save_as")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_SCALE_PASS,
                "video_shader_scale_pass")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHARED_CONTEXT,
                "video_shared_context")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SMOOTH,
                "video_smooth")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SOFT_FILTER,
                "soft_filter")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SWAP_INTERVAL,
                "video_swap_interval")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_TAB,
                "video_tab")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_THREADED,
                "video_threaded")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_VFILTER,
                "video_vfilter")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_VIEWPORT_CUSTOM_HEIGHT,
                "video_viewport_custom_height")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_VIEWPORT_CUSTOM_WIDTH,
                "video_viewport_custom_width")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_VIEWPORT_CUSTOM_X,
                "video_viewport_custom_x")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_VIEWPORT_CUSTOM_Y,
                "video_viewport_custom_y")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_VI_WIDTH,
                "video_vi_width")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_VSYNC,
                "video_vsync")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_WINDOWED_FULLSCREEN,
                "video_windowed_fullscreen")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_WINDOW_WIDTH,
                "video_window_width")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_WINDOW_HEIGHT,
                "video_window_height")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FULLSCREEN_X,
                "video_fullscreen_x")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_FULLSCREEN_Y,
                "video_fullscreen_y")
       MSG_HASH(MENU_ENUM_LABEL_WIFI_DRIVER,
                "wifi_driver")
       MSG_HASH(MENU_ENUM_LABEL_WIFI_SETTINGS,
                "wifi_settings")
       MSG_HASH(MENU_ENUM_LABEL_XMB_ALPHA_FACTOR,
                "xmb_alpha_factor")
       MSG_HASH(MENU_ENUM_LABEL_XMB_FONT,
                "xmb_font")
       MSG_HASH(MENU_ENUM_LABEL_XMB_MENU_COLOR_THEME,
                "xmb_menu_color_theme")
       MSG_HASH(MENU_ENUM_LABEL_XMB_RIBBON_ENABLE,
                "xmb_ribbon_enable")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SCALE_FACTOR,
                "xmb_scale_factor")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHADOWS_ENABLE,
                "xmb_shadows_enable")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHOW_HISTORY,
                "xmb_show_history")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHOW_ADD,
                "xmb_show_add")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHOW_IMAGES,
                "xmb_show_images")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHOW_MUSIC,
                "xmb_show_music")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHOW_SETTINGS,
                "xmb_show_settings")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHOW_VIDEO,
                "xmb_show_video")
       MSG_HASH(MENU_ENUM_LABEL_XMB_SHOW_NETPLAY,
                "xmb_show_netplay")
       MSG_HASH(MENU_ENUM_LABEL_XMB_THEME,
                "xmb_theme")
       MSG_HASH(MSG_BRINGING_UP_COMMAND_INTERFACE_ON_PORT,
                "bringing_up_command_interface_at_port")
       MSG_HASH(MSG_CONNECT_DEVICE_FROM_A_VALID_PORT,
                "connect_device_from_a_valid_port")
       MSG_HASH(MSG_DEVICE_CONFIGURED_IN_PORT,
                "configured in port")
       MSG_HASH(MSG_DISCONNECTING_DEVICE_FROM_PORT,
                "disconnecting_device_from_port")
       MSG_HASH(MSG_DISCONNECT_DEVICE_FROM_A_VALID_PORT,
                "disconnect_device_from_a_valid_port")
       MSG_HASH(MSG_FAILED_TO_SET_DISK,
                "Failed to set disk")
       MSG_HASH(MSG_FAILED_TO_START_AUDIO_DRIVER,
                "failed_to_start_audio_driver")
       MSG_HASH(MSG_FOUND_LAST_STATE_SLOT,
                "found_last_state_slot")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_SELECTION,
                "database_selection")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST,
                "database_cursor_list")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_DEVELOPER,
                "database_cursor_list_entry_developer")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_PUBLISHER,
                "database_cursor_list_entry_publisher")
       MSG_HASH(MENU_ENUM_LABEL_OFF, "off")
       MSG_HASH(MENU_ENUM_LABEL_ON,  "on")
       MSG_HASH(MENU_ENUM_LABEL_DISABLED, "disabled")
       MSG_HASH(MENU_ENUM_LABEL_ENABLED,  "enabled")
       MSG_HASH(MENU_ENUM_LABEL_SECONDS,  "seconds")
       MSG_HASH(MENU_ENUM_LABEL_STATUS,   "status")
       MSG_HASH(MENU_ENUM_LABEL_DETECT_CORE_LIST_OK,
                "detect_core_list_ok")
       MSG_HASH(MENU_ENUM_LABEL_DETECT_CORE_LIST_OK_CURRENT_CORE,
                "detect_core_list_ok_current_core")
       MSG_HASH(MENU_ENUM_LABEL_NO_DISK,
                "no_disk")
       MSG_HASH(MENU_ENUM_LABEL_DEFERRED_RPL_ENTRY_ACTIONS,
                "deferred_rpl_entry_actions")
       MSG_HASH(MENU_ENUM_LABEL_MENU_THROTTLE_FRAMERATE,
                "menu_throttle_framerate")
       MSG_HASH(MENU_ENUM_LABEL_OVERLAY_SETTINGS,
                "overlay_settings")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_ORIGIN,
                "database_cursor_list_entry_origin")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_FRANCHISE,
                "database_cursor_list_entry_franchise")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_ESRB_RATING,
                "database_cursor_list_entry_esrb_rating")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_ELSPA_RATING,
                "database_cursor_list_entry_elspa_rating")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_PEGI_RATING,
                "database_cursor_list_entry_pegi_rating")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_CERO_RATING,
                "database_cursor_list_entry_cero_rating")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_BBFC_RATING,
                "database_cursor_list_entry_bbfc_rating")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_MAX_USERS,
                "database_cursor_list_entry_max_users")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_RELEASEDATE_BY_MONTH,
                "database_cursor_list_releasedate_by_month")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_RELEASEDATE_BY_YEAR,
                "database_cursor_list_releasedate_by_year")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_EDGE_MAGAZINE_ISSUE,
                "database_cursor_list_edge_magazine_issue")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_EDGE_MAGAZINE_RATING,
                "database_cursor_list_edge_magazine_rating")
       MSG_HASH(MENU_ENUM_LABEL_DATABASE_CURSOR_LIST_ENTRY_DATABASE_INFO,
                "database_info")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_PIPELINE_RIBBON,
                "shader_pipeline_ribbon")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_PIPELINE_RIBBON_SIMPLIFIED,
                "shader_pipeline_ribbon_simplified")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_PIPELINE_SIMPLE_SNOW,
                "shader_pipeline_simple_snow")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_PIPELINE_SNOW,
                "shader_pipeline_snow")
       MSG_HASH(MENU_ENUM_LABEL_INPUT_UNIFIED_MENU_CONTROLS,
                "unified_menu_controls")
       MSG_HASH(MENU_ENUM_LABEL_FILE_DETECT_CORE_LIST_PUSH_DIR,
                "detect_core_list_push_dir")
       MSG_HASH(MENU_ENUM_LABEL_BATTERY_LEVEL_ENABLE,
                "menu_battery_level_enable")
       MSG_HASH(MENU_ENUM_LABEL_BROWSE_URL,
                "browse_url")
       MSG_HASH(MENU_ENUM_LABEL_BROWSE_START,
                "browse_start")
       MSG_HASH(MENU_ENUM_LABEL_SHADER_PIPELINE_BOKEH,
                "shader_pipeline_bokeh")
       MSG_HASH(MENU_ENUM_LABEL_NETPLAY_USE_MITM_SERVER,
                "netplay_use_mitm_server")
       MSG_HASH(MENU_ENUM_LABEL_ADD_TO_MIXER,
                "audio_add_to_mixer")
       MSG_HASH(MENU_ENUM_LABEL_ADD_TO_MIXER_AND_COLLECTION,
                "audio_add_to_mixer_and_collection")
       MSG_HASH(MENU_ENUM_LABEL_FILTER_BY_CURRENT_CORE,
                "filter_by_current_Core")

       default:
#if 0
         RARCH_LOG("Unimplemented: [%d]\n", msg);
#endif
         break;
   }

   return "null";
}
#endif

const char *msg_hash_to_str_us(enum msg_hash_enums msg) {
#ifdef HAVE_MENU
    const char *ret = menu_hash_to_str_us_label_enum(msg);

    if (ret && !string_is_equal(ret, "null"))
       return ret;
#endif

    switch (msg) {
//#include "msg_hash_us.h"
       MSG_HASH(
          MSG_COMPILER,
          "Compiler"
       )
       MSG_HASH(
          MSG_UNKNOWN_COMPILER,
          "Unknown compiler"
       )
       MSG_HASH(
          MSG_DEVICE_DISCONNECTED_FROM_PORT,
          "Device disconnected from port"
       )
       MSG_HASH(
          MSG_UNKNOWN_NETPLAY_COMMAND_RECEIVED,
          "Unknown netplay command received"
       )
       MSG_HASH(
          MSG_FILE_ALREADY_EXISTS_SAVING_TO_BACKUP_BUFFER,
          "File already exists. Saving to backup buffer"
       )
       MSG_HASH(
          MSG_GOT_CONNECTION_FROM,
          "Got connection from: \"%s\""
       )
       MSG_HASH(
          MSG_GOT_CONNECTION_FROM_NAME,
          "Got connection from: \"%s (%s)\""
       )
       MSG_HASH(
          MSG_PUBLIC_ADDRESS,
          "Public address"
       )
       MSG_HASH(
          MSG_NO_ARGUMENTS_SUPPLIED_AND_NO_MENU_BUILTIN,
          "No arguments supplied and no menu builtin, displaying help..."
       )
       MSG_HASH(
          MSG_NETPLAY_USERS_HAS_FLIPPED,
          "Netplay users have flipped"
       )
       MSG_HASH(
          MSG_SETTING_DISK_IN_TRAY,
          "Setting disk in tray"
       )
       MSG_HASH(
          MSG_WAITING_FOR_CLIENT,
          "Waiting for client ..."
       )
       MSG_HASH(
          MSG_NETPLAY_YOU_HAVE_LEFT_THE_GAME,
          "You have left the game"
       )
       MSG_HASH(
          MSG_NETPLAY_YOU_HAVE_JOINED_AS_PLAYER_N,
          "You have joined as player %d"
       )
       MSG_HASH(
          MSG_NETPLAY_IMPLEMENTATIONS_DIFFER,
          "Implementations differ. Make sure you're using the exact same versions of RetroArch and the core."
       )
       MSG_HASH(
          MSG_NETPLAY_ENDIAN_DEPENDENT,
          "This core does not support inter-architecture netplay between these systems"
       )
       MSG_HASH(
          MSG_NETPLAY_PLATFORM_DEPENDENT,
          "This core does not support inter-architecture netplay"
       )
       MSG_HASH(
          MSG_NETPLAY_ENTER_PASSWORD,
          "Enter netplay server password:"
       )
       MSG_HASH(
          MSG_NETPLAY_INCORRECT_PASSWORD,
          "Incorrect password"
       )
       MSG_HASH(
          MSG_NETPLAY_SERVER_NAMED_HANGUP,
          "\"%s\" has disconnected"
       )
       MSG_HASH(
          MSG_NETPLAY_SERVER_HANGUP,
          "A netplay client has disconnected"
       )
       MSG_HASH(
          MSG_NETPLAY_CLIENT_HANGUP,
          "Netplay disconnected"
       )
       MSG_HASH(
          MSG_NETPLAY_CANNOT_PLAY_UNPRIVILEGED,
          "You do not have permission to play"
       )
       MSG_HASH(
          MSG_NETPLAY_CANNOT_PLAY_NO_SLOTS,
          "There are no free player slots"
       )
       MSG_HASH(
          MSG_NETPLAY_CANNOT_PLAY,
          "Cannot switch to play mode"
       )
       MSG_HASH(
          MSG_NETPLAY_PEER_PAUSED,
          "Netplay peer \"%s\" paused"
       )
       MSG_HASH(
          MSG_NETPLAY_CHANGED_NICK,
          "Your nickname changed to \"%s\""
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_SHARED_CONTEXT,
          "Give hardware-rendered cores their own private context. Avoids having to assume hardware state changes inbetween frames."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_MENU_SETTINGS,
          "Adjusts menu screen appearance settings."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_HARD_SYNC,
          "Hard-synchronize the CPU and GPU. Reduces latency at the cost of performance."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_THREADED,
          "Improves performance at the cost of latency and more video stuttering. Use only if you cannot obtain full speed otherwise."
       )
       MSG_HASH(
          MSG_AUDIO_VOLUME,
          "Audio volume"
       )
       MSG_HASH(
          MSG_AUTODETECT,
          "Autodetect"
       )
       MSG_HASH(
          MSG_AUTOLOADING_SAVESTATE_FROM,
          "Auto-loading savestate from"
       )
       MSG_HASH(
          MSG_CAPABILITIES,
          "Capabilities"
       )
       MSG_HASH(
          MSG_CONNECTING_TO_NETPLAY_HOST,
          "Connecting to netplay host"
       )
       MSG_HASH(
          MSG_CONNECTING_TO_PORT,
          "Connecting to port"
       )
       MSG_HASH(
          MSG_CONNECTION_SLOT,
          "Connection slot"
       )
       MSG_HASH(
          MSG_SORRY_UNIMPLEMENTED_CORES_DONT_DEMAND_CONTENT_NETPLAY,
          "Sorry, unimplemented: cores that don't demand content cannot participate in netplay."
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACCOUNTS_CHEEVOS_PASSWORD,
          "Password"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACCOUNTS_CHEEVOS_SETTINGS,
          "Accounts Cheevos"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACCOUNTS_CHEEVOS_USERNAME,
          "Username"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACCOUNTS_LIST,
          "Accounts"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACCOUNTS_LIST_END,
          "Accounts List Endpoint"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACCOUNTS_RETRO_ACHIEVEMENTS,
          "Retro Achievements"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACHIEVEMENT_LIST,
          "Achievement List"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ACHIEVEMENT_LIST_HARDCORE,
          "Achievement List (Hardcore)"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ADD_CONTENT_LIST,
          "Scan Content"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONFIGURATIONS_LIST,
          "Configurations"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ADD_TAB,
          "Import content"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_NETPLAY_TAB,
          "Netplay Rooms"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ASK_ARCHIVE,
          "Ask"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ASSETS_DIRECTORY,
          "Assets"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_BLOCK_FRAMES,
          "Block Frames"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_DEVICE,
          "Audio Device"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_DRIVER,
          "Audio Driver"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_DSP_PLUGIN,
          "Audio DSP Plugin"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_ENABLE,
          "Audio Enable"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_FILTER_DIR,
          "Audio Filter"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_TURBO_DEADZONE_LIST,
          "Turbo/Deadzone"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_LATENCY,
          "Audio Latency (ms)"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_MAX_TIMING_SKEW,
          "Audio Maximum Timing Skew"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_MUTE,
          "Audio Mute"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_OUTPUT_RATE,
          "Audio Output Rate (Hz)"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_RATE_CONTROL_DELTA,
          "Dynamic Audio Rate Control"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_RESAMPLER_DRIVER,
          "Audio Resampler Driver"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_SETTINGS,
          "Audio"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_SYNC,
          "Audio Sync"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_VOLUME,
          "Audio Volume Level (dB)"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_WASAPI_EXCLUSIVE_MODE,
          "WASAPI Exclusive Mode"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_WASAPI_FLOAT_FORMAT,
          "WASAPI Float Format"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUDIO_WASAPI_SH_BUFFER_LENGTH,
          "WASAPI Shared Buffer Length"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUTOSAVE_INTERVAL,
          "SaveRAM Autosave Interval"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUTO_OVERRIDES_ENABLE,
          "Load Override Files Automatically"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUTO_REMAPS_ENABLE,
          "Load Remap Files Automatically"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_AUTO_SHADERS_ENABLE,
          "Load Shader Presets Automatically"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_BACK,
          "Back"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_CONFIRM,
          "Confirm"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_INFO,
          "Info"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_QUIT,
          "Quit"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_SCROLL_DOWN,
          "Scroll Down"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_SCROLL_UP,
          "Scroll Up"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_START,
          "Start"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_TOGGLE_KEYBOARD,
          "Toggle Keyboard"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_TOGGLE_MENU,
          "Toggle Menu"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS,
          "Basic menu controls"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_CONFIRM,
          "Confirm/OK"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_INFO,
          "Info"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_QUIT,
          "Quit"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_SCROLL_UP,
          "Scroll Up"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_START,
          "Defaults"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_TOGGLE_KEYBOARD,
          "Toggle Keyboard"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_TOGGLE_MENU,
          "Toggle Menu"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BLOCK_SRAM_OVERWRITE,
          "Don't overwrite SaveRAM on loading savestate"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BLUETOOTH_ENABLE,
          "Bluetooth Enable"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BUILDBOT_ASSETS_URL,
          "Buildbot Assets URL"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CACHE_DIRECTORY,
          "Cache"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CAMERA_ALLOW,
          "Allow Camera"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CAMERA_DRIVER,
          "Camera Driver"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEAT,
          "Cheat"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEAT_APPLY_CHANGES,
          "Apply Changes"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEAT_DATABASE_PATH,
          "Cheat File"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEAT_FILE,
          "Cheat File"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEAT_FILE_LOAD,
          "Load Cheat File"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEAT_FILE_SAVE_AS,
          "Save Cheat File As"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEAT_NUM_PASSES,
          "Cheat Passes"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_DESCRIPTION,
          "Description"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_HARDCORE_MODE_ENABLE,
          "Achievements Hardcore Mode"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_LOCKED_ACHIEVEMENTS,
          "Locked Achievements:"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_LOCKED_ENTRY,
          "Locked"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_SETTINGS,
          "Retro Achievements"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_TEST_UNOFFICIAL,
          "Test Unofficial Achievements"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_UNLOCKED_ACHIEVEMENTS,
          "Unlocked Achievements:"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CHEEVOS_UNLOCKED_ENTRY,
          "Unlocked"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CLOSE_CONTENT,
          "Close Content"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONFIG,
          "Config"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONFIGURATIONS,
          "Load Configuration"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONFIGURATION_SETTINGS,
          "Configuration"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONFIG_SAVE_ON_EXIT,
          "Save Configuration on Exit"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONTENT_COLLECTION_LIST,
          "Collections"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONTENT_DATABASE_DIRECTORY,
          "Database"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONTENT_DIR,
          "Content"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONTENT_HISTORY_SIZE,
          "History List Size")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLIST_ENTRY_REMOVE,
                "Allow to remove entries")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CONTENT_SETTINGS,
                "Quick Menu")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_ASSETS_DIR,
                "Core Assets")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_ASSETS_DIRECTORY,
                "Downloads")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_CHEAT_OPTIONS,
                "Cheats")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_COUNTERS,
                "Core Counters")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_ENABLE,
                "Display core name")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFORMATION,
                "Core Information")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_AUTHORS,
                "Authors")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_CATEGORIES,
                "Categories")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_CORE_LABEL,
                "Core label")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_CORE_NAME,
                "Core name")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_FIRMWARE,
                "Firmware(s)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_LICENSES,
                "License(s)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_PERMISSIONS,
                "Permissions")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_SUPPORTED_EXTENSIONS,
                "Supported extensions")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_SYSTEM_MANUFACTURER,
                "System manufacturer")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_SYSTEM_NAME,
                "System name")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INPUT_REMAPPING_OPTIONS,
                "Controls")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_LIST,
                "Load Core")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_OPTIONS,
                "Options")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_SETTINGS,
                "Core")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_SET_SUPPORTS_NO_CONTENT_ENABLE,
                "Start a Core Automatically")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_AUTO_EXTRACT_ARCHIVE,
                "Automatically extract downloaded archive")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_BUILDBOT_URL,
                "Buildbot Cores URL")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_LIST,
                "Core Updater")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_SETTINGS,
                "Updater")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CPU_ARCHITECTURE,
                "CPU Architecture:")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CPU_CORES,
                "CPU Cores:")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CURSOR_DIRECTORY,
                "Cursor")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CURSOR_MANAGER,
                "Cursor Manager")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CUSTOM_RATIO,
                "Custom Ratio")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_MANAGER,
                "Database Manager")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_SELECTION,
                "Database Selection")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DELETE_ENTRY,
                "Remove")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FAVORITES,
                "Start directory")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_CONTENT,
                "<Content dir>")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_DEFAULT,
                "<Default>")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_NONE,
                "<None>")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_NOT_FOUND,
                "Directory not found.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_SETTINGS,
                "Directory")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_CYCLE_TRAY_STATUS,
                "Disk Cycle Tray Status")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_IMAGE_APPEND,
                "Disk Image Append")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_INDEX,
                "Disk Index")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_OPTIONS,
                "Disk Control")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DONT_CARE,
                "Don't care")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DOWNLOADED_FILE_DETECT_CORE_LIST,
                "Downloads")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DOWNLOAD_CORE,
                "Download Core...")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DOWNLOAD_CORE_CONTENT,
                "Content Downloader")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DPI_OVERRIDE_ENABLE,
                "DPI Override Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DPI_OVERRIDE_VALUE,
                "DPI Override")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DRIVER_SETTINGS,
                "Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DUMMY_ON_CORE_SHUTDOWN,
                "Load Dummy on Core Shutdown")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CHECK_FOR_MISSING_FIRMWARE,
                "Check for Missing Firmware Before Loading")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DYNAMIC_WALLPAPER,
                "Dynamic Background")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DYNAMIC_WALLPAPERS_DIRECTORY,
                "Dynamic Backgrounds")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_CHEEVOS_ENABLE,
                "Enable Achievements")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ENTRY_HOVER_COLOR,
                "Menu entry hover color")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ENTRY_NORMAL_COLOR,
                "Menu entry normal color")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FALSE,
                "False")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FASTFORWARD_RATIO,
                "Maximum Run Speed")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FPS_SHOW,
                "Display Framerate")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FRAME_THROTTLE_ENABLE,
                "Limit Maximum Run Speed")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FRAME_THROTTLE_SETTINGS,
                "Frame Throttle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FRONTEND_COUNTERS,
                "Frontend Counters")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_GAME_SPECIFIC_OPTIONS,
                "Load Content-Specific Core Options Automatically")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_GAME_SPECIFIC_OPTIONS_CREATE,
                "Create game-options file")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_GAME_SPECIFIC_OPTIONS_IN_USE,
                "Game-options file")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP,
                "Help")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_AUDIO_VIDEO_TROUBLESHOOTING,
                "Audio/Video Troubleshooting")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_CHANGE_VIRTUAL_GAMEPAD,
                "Changing Virtual Gamepad Overlay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_CONTROLS,
                "Basic Menu Controls")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_LIST,
                "Help")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_LOADING_CONTENT,
                "Loading Content")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_SCANNING_CONTENT,
                "Scanning For Content")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_WHAT_IS_A_CORE,
                "What Is A Core?")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HISTORY_LIST_ENABLE,
                "History List Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HISTORY_TAB,
                "History")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_HORIZONTAL_MENU,
                "Horizontal Menu")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_IMAGES_TAB,
                "Image")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INFORMATION,
                "Information")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INFORMATION_LIST,
                "Information")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ADC_TYPE,
                "Analog To Digital Type")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ALL_USERS_CONTROL_MENU,
                "All Users Control Menu")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_X,
                "Left Analog X")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_X_MINUS,
                "Left analog X- (left)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_X_PLUS,
                "Left analog X+ (right)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_Y,
                "Left Analog Y")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_Y_MINUS,
                "Left analog Y- (up)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_Y_PLUS,
                "Left analog Y+ (down)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_X,
                "Right Analog X")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_X_MINUS,
                "Right analog X- (left)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_X_PLUS,
                "Right analog X+ (right)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_Y,
                "Right Analog Y")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_Y_MINUS,
                "Right analog Y- (up)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_Y_PLUS,
                "Right analog Y+ (down)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_AUTODETECT_ENABLE,
                "Autoconfig Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_AXIS_THRESHOLD,
                "Analog Stick Deadzone")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_INPUT_SWAP_OK_CANCEL,
                "Menu Swap OK & Cancel Buttons")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_BIND_ALL,
                "Bind All")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_BIND_DEFAULT_ALL,
                "Bind Default All")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_BIND_TIMEOUT,
                "Bind Timeout")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DESCRIPTOR_HIDE_UNBOUND,
                "Hide Unbound Core Input Descriptors")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DESCRIPTOR_LABEL_SHOW,
                "Display Input Descriptor Labels")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DEVICE_INDEX,
                "Device Index")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DEVICE_TYPE,
                "Device Type")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DRIVER,
                "Input Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DUTY_CYCLE,
                "Duty Cycle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_HOTKEY_BINDS,
                "Input Hotkey Binds")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ICADE_ENABLE,
                "Keyboard Gamepad Mapping Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_A,
                "A button (right)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_B,
                "B button (down)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_DOWN,
                "Down D-pad")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_L2,
                "L2 button (trigger)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_L3,
                "L3 button (thumb)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_L,
                "L button (shoulder)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_LEFT,
                "Left D-pad")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_R2,
                "R2 button (trigger)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_R3,
                "R3 button (thumb)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_R,
                "R button (shoulder)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_RIGHT,
                "Right D-pad")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_SELECT,
                "Select button")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_START,
                "Start button")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_UP,
                "Up D-pad")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_X,
                "X button (top)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_Y,
                "Y button (left)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_KEY,
                "(Key: %s)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_KEYBOARD_GAMEPAD_MAPPING_TYPE,
                "Keyboard Gamepad Mapping Type")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_MAX_USERS,
                "Max Users")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_MENU_ENUM_TOGGLE_GAMEPAD_COMBO,
                "Menu Toggle Gamepad Combo")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_CHEAT_INDEX_MINUS,
                "Cheat index -")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_CHEAT_INDEX_PLUS,
                "Cheat index +")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_CHEAT_TOGGLE,
                "Cheat toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_DISK_EJECT_TOGGLE,
                "Disk eject toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_DISK_NEXT,
                "Disk next")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_DISK_PREV,
                "Disk prev")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_ENABLE_HOTKEY,
                "Enable hotkeys")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FAST_FORWARD_HOLD_KEY,
                "Fast forward hold")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FAST_FORWARD_KEY,
                "Fast forward toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FRAMEADVANCE,
                "Frameadvance")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FULLSCREEN_TOGGLE_KEY,
                "Fullscreen toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_GRAB_MOUSE_TOGGLE,
                "Grab mouse toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_GAME_FOCUS_TOGGLE,
                "Game focus toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_LOAD_STATE_KEY,
                "Load state")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_MENU_TOGGLE,
                "Menu toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_MOVIE_RECORD_TOGGLE,
                "Movie record toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_MUTE,
                "Audio mute toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_NETPLAY_FLIP,
                "Netplay flip users")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_NETPLAY_GAME_WATCH,
                "Netplay toggle play/spectate mode")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_OSK,
                "On-screen keyboard toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_OVERLAY_NEXT,
                "Overlay next")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_PAUSE_TOGGLE,
                "Pause toggle")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_QUIT_KEY,
                "Quit RetroArch")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_RESET,
                "Reset game")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_REWIND,
                "Rewind")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SAVE_STATE_KEY,
                "Save state")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SCREENSHOT,
                "Take screenshot")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SHADER_NEXT,
                "Next shader")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SHADER_PREV,
                "Previous shader")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SLOWMOTION,
                "Slow motion")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_STATE_SLOT_MINUS,
                "Savestate slot -")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_STATE_SLOT_PLUS,
                "Savestate slot +")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_VOLUME_DOWN,
                "Volume -")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_VOLUME_UP,
                "Volume +")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_ENABLE,
                "Display Overlay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_HIDE_IN_MENU,
                "Hide Overlay In Menu")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR,
                "Poll Type Behavior")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_EARLY,
                "Early")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_LATE,
                "Late")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_NORMAL,
                "Normal")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_PREFER_FRONT_TOUCH,
                "Prefer Front Touch")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_REMAPPING_DIRECTORY,
                "Input Remapping")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_REMAP_BINDS_ENABLE,
                "Remap Binds Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_SAVE_AUTOCONFIG,
                "Save Autoconfig")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_SETTINGS,
                "Input")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_SMALL_KEYBOARD_ENABLE,
                "Small Keyboard Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_TOUCH_ENABLE,
                "Touch Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_TURBO_ENABLE,
                "Turbo enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_TURBO_PERIOD,
                "Turbo Period")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_USER_BINDS,
                "Input User %u Binds")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INTERNAL_STORAGE_STATUS,
                "Internal storage status")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_JOYPAD_AUTOCONFIG_DIR,
                "Input Autoconfig")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_JOYPAD_DRIVER,
                "Joypad Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LAKKA_SERVICES,
                "Services")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_CHINESE_SIMPLIFIED,
                "Chinese (Simplified)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_CHINESE_TRADITIONAL,
                "Chinese (Traditional)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_DUTCH,
                "Dutch")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_ENGLISH,
                "English")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_ESPERANTO,
                "Esperanto")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_FRENCH,
                "French")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_GERMAN,
                "German")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_ITALIAN,
                "Italian")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_JAPANESE,
                "Japanese")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_KOREAN,
                "Korean")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_POLISH,
                "Polish")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_PORTUGUESE_BRAZIL,
                "Portuguese (Brazil)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_PORTUGUESE_PORTUGAL,
                "Portuguese (Portugal)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_RUSSIAN,
                "Russian")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_SPANISH,
                "Spanish")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_VIETNAMESE,
                "Vietnamese")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LEFT_ANALOG,
                "Left Analog")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LIBRETRO_DIR_PATH,
                "Core")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LIBRETRO_INFO_PATH,
                "Core Info")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LIBRETRO_LOG_LEVEL,
                "Core Logging Level")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LINEAR,
                "Linear")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_ARCHIVE,
                "Load Archive")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_HISTORY,
                "Load Recent")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_LIST,
                "Load Content")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_STATE,
                "Load State")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOCATION_ALLOW,
                "Allow Location")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOCATION_DRIVER,
                "Location Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOGGING_SETTINGS,
                "Logging")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_LOG_VERBOSITY,
                "Logging Verbosity")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MAIN_MENU,
                "Main Menu")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MANAGEMENT,
                "Database Settings")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME,
                "Menu Color Theme")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_BLUE,
                "Blue")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_BLUE_GREY,
                "Blue Grey")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_DARK_BLUE,
                "Dark Blue")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_GREEN,
                "Green")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_NVIDIA_SHIELD,
                "Shield")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_RED,
                "Red")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_YELLOW,
                "Yellow")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_FOOTER_OPACITY,
                "Footer Opacity")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_HEADER_OPACITY,
                "Header Opacity")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_DRIVER,
                "Menu Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_ENUM_THROTTLE_FRAMERATE,
                "Throttle Menu Framerate")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_FILE_BROWSER_SETTINGS,
                "Settings")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_LINEAR_FILTER,
                "Menu Linear Filter")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_SETTINGS,
                "Menu")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_WALLPAPER,
                "Background")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_WALLPAPER_OPACITY,
                "Background opacity")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MISSING,
                "Missing")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MORE,
                "...")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MOUSE_ENABLE,
                "Mouse Support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MULTIMEDIA_SETTINGS,
                "Multimedia")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_MUSIC_TAB,
                "Music")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NAVIGATION_BROWSER_FILTER_SUPPORTED_EXTENSIONS_ENABLE,
                "Filter unknown extensions")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NAVIGATION_WRAPAROUND,
                "Navigation Wrap-Around")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NEAREST,
                "Nearest")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY,
                "Netplay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ALLOW_SLAVES,
                "Allow Slave-Mode Clients")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_CHECK_FRAMES,
                "Netplay Check Frames")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_INPUT_LATENCY_FRAMES_MIN,
                "Input Latency Frames")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_INPUT_LATENCY_FRAMES_RANGE,
                "Input Latency Frames Range")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_CLIENT_SWAP_INPUT,
                "Netplay P2 Uses C1")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_DELAY_FRAMES,
                "Netplay Delay Frames")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_DISCONNECT,
                "Disconnect from netplay host")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ENABLE,
                "Netplay Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ENABLE_CLIENT,
                "Connect to netplay host")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ENABLE_HOST,
                "Start netplay host")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_DISABLE_HOST,
                "Stop netplay host")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_IP_ADDRESS,
                "Server Address")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_LAN_SCAN_SETTINGS,
                "Scan local network")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_MODE,
                "Netplay Client Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_NICKNAME,
                "Username")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_PASSWORD,
                "Server Password")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_PUBLIC_ANNOUNCE,
                "Publicly Announce Netplay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_REQUIRE_SLAVES,
                "Disallow Non-Slave-Mode Clients")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_SETTINGS,
                "Netplay settings")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_START_AS_SPECTATOR,
                "Netplay Spectator Mode")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_STATELESS_MODE,
                "Netplay Stateless Mode")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_SPECTATE_PASSWORD,
                "Server Spectate-Only Password")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_SPECTATOR_MODE_ENABLE,
                "Netplay Spectator Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_TCP_UDP_PORT,
                "Netplay TCP Port")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_NAT_TRAVERSAL,
                "Netplay NAT Traversal")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_CMD_ENABLE,
                "Network Commands")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_CMD_PORT,
                "Network Command Port")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_INFORMATION,
                "Network Information")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_REMOTE_ENABLE,
                "Network Gamepad")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_REMOTE_PORT,
                "Network Remote Base Port")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_SETTINGS,
                "Network")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO,
                "No")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NONE,
                "None")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NOT_AVAILABLE,
                "N/A")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_ACHIEVEMENTS_TO_DISPLAY,
                "No achievements to display.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORE,
                "No Core")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORES_AVAILABLE,
                "No cores available.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORE_INFORMATION_AVAILABLE,
                "No core information available.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORE_OPTIONS_AVAILABLE,
                "No core options available.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_ENTRIES_TO_DISPLAY,
                "No entries to display.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_HISTORY_AVAILABLE,
                "No history available.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE,
                "No information is available.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_ITEMS,
                "No items.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_NETPLAY_HOSTS_FOUND,
                "No netplay hosts found.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_NETWORKS_FOUND,
                "No networks found.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_PERFORMANCE_COUNTERS,
                "No performance counters.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_PLAYLISTS,
                "No playlists.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_PLAYLIST_ENTRIES_AVAILABLE,
                "No playlist entries available.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_SETTINGS_FOUND,
                "No settings found.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_SHADER_PARAMETERS,
                "No shader parameters.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OFF,
                "OFF")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ON,
                "ON")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ONLINE,
                "Online")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ONLINE_UPDATER,
                "Online Updater")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ONSCREEN_DISPLAY_SETTINGS,
                "Onscreen Display")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ONSCREEN_OVERLAY_SETTINGS,
                "Onscreen Overlay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ONSCREEN_NOTIFICATIONS_SETTINGS,
                "Onscreen Notifications")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OPEN_ARCHIVE,
                "Browse Archive")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OPTIONAL,
                "Optional")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY,
                "Overlay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_AUTOLOAD_PREFERRED,
                "Autoload Preferred Overlay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_DIRECTORY,
                "Overlay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_OPACITY,
                "Overlay Opacity")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_PRESET,
                "Overlay Preset")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_SCALE,
                "Overlay Scale")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_SETTINGS,
                "Onscreen Overlay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PAL60_ENABLE,
                "Use PAL60 Mode")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PARENT_DIRECTORY,
                "Parent directory")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PAUSE_LIBRETRO,
                "Pause when menu activated")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PAUSE_NONACTIVE,
                "Don't run in background")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PERFCNT_ENABLE,
                "Performance Counters")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLISTS_TAB,
                "Playlists")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLIST_DIRECTORY,
                "Playlist")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLIST_SETTINGS,
                "Playlists")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_POINTER_ENABLE,
                "Touch Support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PORT,
                "Port")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PRESENT,
                "Present")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_PRIVACY_SETTINGS,
                "Privacy")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_QUIT_RETROARCH,
                "Quit RetroArch")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ANALOG,
                "Analog supported")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_BBFC_RATING,
                "BBFC Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_CERO_RATING,
                "CERO Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_COOP,
                "Co-op supported")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_CRC32,
                "CRC32")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_DESCRIPTION,
                "Description")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_DEVELOPER,
                "Developer")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_ISSUE,
                "Edge Magazine Issue")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_RATING,
                "Edge Magazine Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_REVIEW,
                "Edge Magazine Review")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ELSPA_RATING,
                "ELSPA Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ENHANCEMENT_HW,
                "Enhancement Hardware")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ESRB_RATING,
                "ESRB Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_FAMITSU_MAGAZINE_RATING,
                "Famitsu Magazine Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_FRANCHISE,
                "Franchise")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_GENRE,
                "Genre")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_MD5,
                "MD5")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_NAME,
                "Name")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ORIGIN,
                "Origin")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_PEGI_RATING,
                "PEGI Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_PUBLISHER,
                "Publisher")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RELEASE_MONTH,
                "Releasedate Month")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RELEASE_YEAR,
                "Releasedate Year")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RUMBLE,
                "Rumble supported")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_SERIAL,
                "Serial")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_SHA1,
                "SHA1")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_START_CONTENT,
                "Start Content")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_TGDB_RATING,
                "TGDB Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REBOOT,
                "Reboot")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORDING_CONFIG_DIRECTORY,
                "Recording Config")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORDING_OUTPUT_DIRECTORY,
                "Recording Output")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORDING_SETTINGS,
                "Recording")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_CONFIG,
                "Load Recording Config...")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_DRIVER,
                "Record Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_ENABLE,
                "Enable Recording")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_PATH,
                "Save Output Recording as...")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_USE_OUTPUT_DIRECTORY,
                "Save Recordings in Output Dir")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE,
                "Remap File")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE_LOAD,
                "Load Remap File")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE_SAVE_CORE,
                "Save Core Remap File")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE_SAVE_GAME,
                "Save Game Remap File")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REQUIRED,
                "Required")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RESTART_CONTENT,
                "Restart")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RESTART_RETROARCH,
                "Restart RetroArch")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RESUME,
                "Resume")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RESUME_CONTENT,
                "Resume")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RETROKEYBOARD,
                "RetroKeyboard")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RETROPAD,
                "RetroPad")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RETROPAD_WITH_ANALOG,
                "RetroPad w/ Analog")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RETRO_ACHIEVEMENTS_SETTINGS,
                "Achievements")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REWIND_ENABLE,
                "Rewind Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REWIND_GRANULARITY,
                "Rewind Granularity")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_REWIND_SETTINGS,
                "Rewind")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RGUI_BROWSER_DIRECTORY,
                "File Browser")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RGUI_CONFIG_DIRECTORY,
                "Config")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RGUI_SHOW_START_SCREEN,
                "Display Start Screen")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RIGHT_ANALOG,
                "Right Analog")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RUN,
                "Run")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_RUN_MUSIC,
                "Run")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAMBA_ENABLE,
                "SAMBA Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVEFILE_DIRECTORY,
                "Savefile")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_AUTO_INDEX,
                "Save State Auto Index")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_AUTO_LOAD,
                "Auto Load State")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_AUTO_SAVE,
                "Auto Save State")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_DIRECTORY,
                "Savestate")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_THUMBNAIL_ENABLE,
                "Savestate Thumbnails")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG,
                "Save Current Configuration")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG_OVERRIDE_CORE,
                "Save Core Overrides")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG_OVERRIDE_GAME,
                "Save Game Overrides")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_NEW_CONFIG,
                "Save New Configuration")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_STATE,
                "Save State")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVING_SETTINGS,
                "Saving")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SCAN_DIRECTORY,
                "Scan Directory")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SCAN_FILE,
                "Scan File")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SCAN_THIS_DIRECTORY,
                "<Scan This Directory>")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SCREENSHOT_DIRECTORY,
                "Screenshot")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SCREEN_RESOLUTION,
                "Screen Resolution")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SEARCH,
                "Search")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SECONDS,
                "seconds")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SETTINGS,
                "Settings")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SETTINGS_TAB,
                "Settings")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER,
                "Shader")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_APPLY_CHANGES,
                "Apply Changes")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_OPTIONS,
                "Shaders")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_RIBBON,
                "Ribbon")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_RIBBON_SIMPLIFIED,
                "Ribbon (simplified)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_SIMPLE_SNOW,
                "Simple Snow")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_SNOW,
                "Snow")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHOW_ADVANCED_SETTINGS,
                "Show Advanced Settings")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHOW_HIDDEN_FILES,
                "Show Hidden Files and Folders")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHUTDOWN,
                "Shutdown")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SLOWMOTION_RATIO,
                "Slow-Motion Ratio")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SORT_SAVEFILES_ENABLE,
                "Sort Saves In Folders")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SORT_SAVESTATES_ENABLE,
                "Sort Savestates In Folders")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SSH_ENABLE,
                "SSH Enable")
//       MSG_HASH(MENU_ENUM_LABEL_VALUE_START_CORE,
//                "Start Core")
        case MENU_ENUM_LABEL_VALUE_START_CORE:
            return "Start Core";
       MSG_HASH(MENU_ENUM_LABEL_VALUE_START_NET_RETROPAD,
                "Start Remote RetroPad")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_START_VIDEO_PROCESSOR,
                "Start Video Processor")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_STATE_SLOT,
                "State Slot")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_STATUS,
                "Status")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_STDIN_CMD_ENABLE,
                "stdin Commands")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SUPPORTED_CORES,
                "Suggested cores")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SUSPEND_SCREENSAVER_ENABLE,
                "Suspend Screensaver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_BGM_ENABLE,
                "System BGM Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_DIRECTORY,
                "System/BIOS")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFORMATION,
                "System Information")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_7ZIP_SUPPORT,
                "7zip support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ALSA_SUPPORT,
                "ALSA support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_BUILD_DATE,
                "Build date")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CG_SUPPORT,
                "Cg support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COCOA_SUPPORT,
                "Cocoa support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COMMAND_IFACE_SUPPORT,
                "Command interface support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CORETEXT_SUPPORT,
                "CoreText support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CPU_FEATURES,
                "CPU Features")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_DPI,
                "Display metric DPI")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_HEIGHT,
                "Display metric height (mm)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_WIDTH,
                "Display metric width (mm)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DSOUND_SUPPORT,
                "DirectSound support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_WASAPI_SUPPORT,
                "WASAPI support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DYLIB_SUPPORT,
                "Dynamic library support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DYNAMIC_SUPPORT,
                "Dynamic run-time loading of libretro library")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_EGL_SUPPORT,
                "EGL support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FBO_SUPPORT,
                "OpenGL/Direct3D render-to-texture (multi-pass shaders) support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FFMPEG_SUPPORT,
                "FFmpeg support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FREETYPE_SUPPORT,
                "FreeType support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_IDENTIFIER,
                "Frontend identifier")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_NAME,
                "Frontend name")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_OS,
                "Frontend OS")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_GIT_VERSION,
                "Git version")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_GLSL_SUPPORT,
                "GLSL support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_HLSL_SUPPORT,
                "HLSL support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_JACK_SUPPORT,
                "JACK support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_KMS_SUPPORT,
                "KMS/EGL support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LAKKA_VERSION,
                "Lakka Version")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBRETRODB_SUPPORT,
                "LibretroDB support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBUSB_SUPPORT,
                "Libusb support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBXML2_SUPPORT,
                "libxml2 XML parsing support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETPLAY_SUPPORT,
                "Netplay (peer-to-peer) support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETWORK_COMMAND_IFACE_SUPPORT,
                "Network Command interface support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETWORK_REMOTE_SUPPORT,
                "Network Gamepad support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENAL_SUPPORT,
                "OpenAL support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENGLES_SUPPORT,
                "OpenGL ES support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENGL_SUPPORT,
                "OpenGL support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENSL_SUPPORT,
                "OpenSL support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENVG_SUPPORT,
                "OpenVG support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OSS_SUPPORT,
                "OSS support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OVERLAY_SUPPORT,
                "Overlay support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE,
                "Power source")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_CHARGED,
                "Charged")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_CHARGING,
                "Charging")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_DISCHARGING,
                "Discharging")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_NO_SOURCE,
                "No source")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_PULSEAUDIO_SUPPORT,
                "PulseAudio support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_PYTHON_SUPPORT,
                "Python (script support in shaders) support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RBMP_SUPPORT,
                "BMP support (RBMP)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RETRORATING_LEVEL,
                "RetroRating level")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RJPEG_SUPPORT,
                "JPEG support (RJPEG)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ROARAUDIO_SUPPORT,
                "RoarAudio support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RPNG_SUPPORT,
                "PNG support (RPNG)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RSOUND_SUPPORT,
                "RSound support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RTGA_SUPPORT,
                "TGA support (RTGA)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL2_SUPPORT,
                "SDL2 support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL_IMAGE_SUPPORT,
                "SDL image support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL_SUPPORT,
                "SDL1.2 support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SLANG_SUPPORT,
                "Slang support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_THREADING_SUPPORT,
                "Threading support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_UDEV_SUPPORT,
                "Udev support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_V4L2_SUPPORT,
                "Video4Linux2 support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_VIDEO_CONTEXT_DRIVER,
                "Video context driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_VULKAN_SUPPORT,
                "Vulkan support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_WAYLAND_SUPPORT,
                "Wayland support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_X11_SUPPORT,
                "X11 support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_XAUDIO2_SUPPORT,
                "XAudio2 support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_XVIDEO_SUPPORT,
                "XVideo support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ZLIB_SUPPORT,
                "Zlib support")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_TAKE_SCREENSHOT,
                "Take Screenshot")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_THREADED_DATA_RUNLOOP_ENABLE,
                "Threaded tasks")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAILS,
                "Thumbnails")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAILS_DIRECTORY,
                "Thumbnails")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAILS_UPDATER_LIST,
                "Thumbnails Updater")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAIL_MODE_BOXARTS,
                "Boxarts")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAIL_MODE_SCREENSHOTS,
                "Screenshots")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAIL_MODE_TITLE_SCREENS,
                "Title Screens")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_TIMEDATE_ENABLE,
                "Display time / date")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_TITLE_COLOR,
                "Menu title color")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_TRUE,
                "True")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UI_COMPANION_ENABLE,
                "UI Companion Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UI_COMPANION_START_ON_BOOT,
                "UI Companion Start On Boot")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UI_MENUBAR_ENABLE,
                "Menubar")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UNABLE_TO_READ_COMPRESSED_FILE,
                "Unable to read compressed file.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UNDO_LOAD_STATE,
                "Undo Load State")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UNDO_SAVE_STATE,
                "Undo Save State")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UNKNOWN,
                "Unknown")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATER_SETTINGS,
                "Updater")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_ASSETS,
                "Update Assets")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_AUTOCONFIG_PROFILES,
                "Update Autoconfig Profiles")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_CG_SHADERS,
                "Update Cg Shaders")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_CHEATS,
                "Update Cheats")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_CORE_INFO_FILES,
                "Update Core Info Files")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_DATABASES,
                "Update Databases")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_GLSL_SHADERS,
                "Update GLSL Shaders")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_LAKKA,
                "Update Lakka")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_OVERLAYS,
                "Update Overlays")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_SLANG_SHADERS,
                "Update Slang Shaders")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_USER,
                "User")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_USER_INTERFACE_SETTINGS,
                "User Interface")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_USER_LANGUAGE,
                "Language")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_USER_SETTINGS,
                "User")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_USE_BUILTIN_IMAGE_VIEWER,
                "Use Builtin Image Viewer")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_USE_BUILTIN_PLAYER,
                "Use Builtin Media Player")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_USE_THIS_DIRECTORY,
                "<Use this directory>")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ALLOW_ROTATE,
                "Allow rotation")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ASPECT_RATIO,
                "Config Aspect Ratio")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ASPECT_RATIO_AUTO,
                "Auto Aspect Ratio")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ASPECT_RATIO_INDEX,
                "Aspect Ratio")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_BLACK_FRAME_INSERTION,
                "Black Frame Insertion")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_CROP_OVERSCAN,
                "Crop Overscan (Reload)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_DISABLE_COMPOSITION,
                "Disable Desktop Composition")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_DRIVER,
                "Video Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FILTER,
                "Video Filter")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FILTER_DIR,
                "Video Filter")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FILTER_FLICKER,
                "Flicker filter")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FONT_ENABLE,
                "Enable Onscreen Notifications")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FONT_PATH,
                "Onscreen Notification Font")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FONT_SIZE,
                "Onscreen Notification Size")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FORCE_ASPECT,
                "Force aspect ratio")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FORCE_SRGB_DISABLE,
                "Force-disable sRGB FBO")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FRAME_DELAY,
                "Frame Delay")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN,
                "Use Fullscreen Mode")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_GAMMA,
                "Video Gamma")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_GPU_RECORD,
                "Use GPU Recording")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_GPU_SCREENSHOT,
                "GPU Screenshot Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_HARD_SYNC,
                "Hard GPU Sync")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_HARD_SYNC_FRAMES,
                "Hard GPU Sync Frames")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MAX_SWAPCHAIN_IMAGES,
                "Max swapchain images")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MESSAGE_POS_X,
                "Onscreen Notification X Position")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MESSAGE_POS_Y,
                "Onscreen Notification Y Position")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MONITOR_INDEX,
                "Monitor Index")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_POST_FILTER_RECORD,
                "Use Post Filter Recording")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE,
                "Vertical Refresh Rate")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE_AUTO,
                "Estimated Screen Framerate")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ROTATION,
                "Rotation")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SCALE,
                "Windowed Scale")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SCALE_INTEGER,
                "Integer Scale")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SETTINGS,
                "Video")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_DIR,
                "Video Shader")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_NUM_PASSES,
                "Shader Passes")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PARAMETERS,
                "Preview Shader Parameters")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET,
                "Load Shader Preset")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_PARAMETERS,
                "Menu Shader Parameters")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE_AS,
                "Save Shader Preset As")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE_CORE,
                "Save Core Preset")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE_GAME,
                "Save Game Preset")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHARED_CONTEXT,
                "Enable Hardware Shared Context")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SMOOTH,
                "Bilinear Filtering")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SOFT_FILTER,
                "Soft Filter Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SWAP_INTERVAL,
                "Vertical Sync (Vsync) Swap Interval")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_TAB,
                "Video")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_THREADED,
                "Threaded Video")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VFILTER,
                "Deflicker")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_HEIGHT,
                "Custom Aspect Ratio Height")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_WIDTH,
                "Custom Aspect Ratio Width")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_X,
                "Custom Aspect Ratio X Pos.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_Y,
                "Custom Aspect Ratio Y Pos.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VI_WIDTH,
                "Set VI Screen Width")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VSYNC,
                "Vertical Sync (Vsync)")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_WINDOWED_FULLSCREEN,
                "Windowed Fullscreen Mode")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_WIDTH,
                "Window Width")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_HEIGHT,
                "Window Height")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN_X,
                "Fullscreen Width")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN_Y,
                "Fullscreen Height")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_WIFI_DRIVER,
                "Wi-Fi Driver")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_WIFI_SETTINGS,
                "Wi-Fi")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ALPHA_FACTOR,
                "Menu Alpha Factor")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_FONT,
                "Menu Font")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_CUSTOM,
                "Custom")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_FLATUI,
                "FlatUI")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_MONOCHROME,
                "Monochrome")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_SYSTEMATIC,
                "Systematic")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_NEOACTIVE,
                "NeoActive")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_PIXEL,
                "Pixel")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_RETROACTIVE,
                "RetroActive")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_DOTART,
                "Dot-Art")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME,
                "Menu Color Theme")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_APPLE_GREEN,
                "Apple Green")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_DARK,
                "Dark")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_DARK_PURPLE,
                "Dark Purple")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_ELECTRIC_BLUE,
                "Electric Blue")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_GOLDEN,
                "Golden")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_LEGACY_RED,
                "Legacy Red")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_MIDNIGHT_BLUE,
                "Midnight Blue")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_PLAIN,
                "Plain")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_UNDERSEA,
                "Undersea")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_VOLCANIC_RED,
                "Volcanic Red")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_RIBBON_ENABLE,
                "Menu Shader Pipeline")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SCALE_FACTOR,
                "Menu Scale Factor")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHADOWS_ENABLE,
                "Icon Shadows Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_HISTORY,
                "Display History Tab")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_ADD,
                "Display Import content Tab")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_IMAGES,
                "Display Image Tab")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_MUSIC,
                "Display Music Tab")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_SETTINGS,
                "Display Settings Tab")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_VIDEO,
                "Display Video Tab")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_NETPLAY,
                "Display Netplay Tab")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_THEME,
                "Menu Icon Theme")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_YES,
                "Yes")
       MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_PRESET_TWO,
                "Shader Preset")
       MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_ENABLE,
                "Enable or disable achievements. For more information, visit http://retroachievements.org")
       MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_TEST_UNOFFICIAL,
                "Enable or disable unofficial achievements and/or beta features for testing purposes.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_HARDCORE_MODE_ENABLE,
                "Enable or disable savestates, cheats, rewind, fast-forward, pause, and slow-motion for all games.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DRIVER_SETTINGS,
                "Change drivers used by the system.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RETRO_ACHIEVEMENTS_SETTINGS,
                "Change achievement settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_SETTINGS,
                "Change core settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RECORDING_SETTINGS,
                "Change recording settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_ONSCREEN_DISPLAY_SETTINGS,
                "Change display overlay and keyboard overlay, and onscreen notification settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_FRAME_THROTTLE_SETTINGS,
                "Change rewind, fast-forward, and slow-motion settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SAVING_SETTINGS,
                "Change saving settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_LOGGING_SETTINGS,
                "Change logging settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_USER_INTERFACE_SETTINGS,
                "Change user interface settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_USER_SETTINGS,
                "Change account, username, and language settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_PRIVACY_SETTINGS,
                "Change your privacy settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DIRECTORY_SETTINGS,
                "Change default directories where files are located.")
       MSG_HASH(MENU_ENUM_SUBLABEL_PLAYLIST_SETTINGS,
                "Change playlist settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETWORK_SETTINGS,
                "Configure server and network settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_ADD_CONTENT_LIST,
                "Scan content and add to the database.")
       MSG_HASH(MENU_ENUM_SUBLABEL_AUDIO_SETTINGS,
                "Change audio output settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_BLUETOOTH_ENABLE,
                "Enable or disable bluetooth.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CONFIG_SAVE_ON_EXIT,
                "Saves changes to the configuration file on exit.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CONFIGURATION_SETTINGS,
                "Change default settings for configuration files.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CONFIGURATIONS_LIST,
                "Manage and create configuration files.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CPU_CORES,
                "Amount of cores that the CPU has.")
       MSG_HASH(MENU_ENUM_SUBLABEL_FPS_SHOW,
                "Displays the current framerate per second onscreen.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_HOTKEY_BINDS,
                "Configure hotkey settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_MENU_ENUM_TOGGLE_GAMEPAD_COMBO,
                "Gamepad button combination to toggle menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_SETTINGS,
                "Change joypad, keyboard, and mouse settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_USER_BINDS,
                "Configure controls for this user.")
       MSG_HASH(MENU_ENUM_SUBLABEL_LOG_VERBOSITY,
                "Enable or disable logging to the terminal.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY,
                "Join or host a netplay session.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_LAN_SCAN_SETTINGS,
                "Search for and connect to netplay hosts on the local network.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INFORMATION_LIST_LIST,
                "Display core, network, and system information.")
       MSG_HASH(MENU_ENUM_SUBLABEL_ONLINE_UPDATER,
                "Download add-ons, components, and content for RetroArch.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SAMBA_ENABLE,
                "Enable or disable network sharing of your folders.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SERVICES_SETTINGS,
                "Manage operating system level services.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SHOW_HIDDEN_FILES,
                "Show hidden files/directories inside the file browser.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SSH_ENABLE,
                "Enable or disable remote command line access.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SUSPEND_SCREENSAVER_ENABLE,
                "Prevents your system's screensaver from becoming active.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_WINDOW_SCALE,
                "Sets the window size relative to the core viewport size. Alternatively, you can set a window width and height below for a fixed window size.")
       MSG_HASH(MENU_ENUM_SUBLABEL_USER_LANGUAGE,
                "Sets the language of the interface.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_BLACK_FRAME_INSERTION,
                "Inserts a black frame inbetween frames. Useful for users with 120Hz screens who want to play 60Hz content to eliminate ghosting.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FRAME_DELAY,
                "Reduces latency at the cost of a higher risk of video stuttering. Adds a delay after V-Sync (in ms).")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_HARD_SYNC_FRAMES,
                "Sets how many frames the CPU can run ahead of the GPU when using 'Hard GPU Sync'.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_MAX_SWAPCHAIN_IMAGES,
                "Tells the video driver to explicitly use a specified buffering mode.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_MONITOR_INDEX,
                "Selects which display screen to use.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_REFRESH_RATE_AUTO,
                "The accurate estimated refresh rate of the screen in Hz.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SETTINGS,
                "Change video output settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_WIFI_SETTINGS,
                "Scans for wireless networks and establishes connection.")
       MSG_HASH(MENU_ENUM_SUBLABEL_HELP_LIST,
                "Learn more about how the program works.")
       MSG_HASH(MSG_APPENDED_DISK,
                "Appended disk")
       MSG_HASH(MSG_APPLICATION_DIR,
                "Application Dir")
       MSG_HASH(MSG_APPLYING_CHEAT,
                "Applying cheat changes.")
       MSG_HASH(MSG_APPLYING_SHADER,
                "Applying shader")
       MSG_HASH(MSG_AUDIO_MUTED,
                "Audio muted.")
       MSG_HASH(MSG_AUDIO_UNMUTED,
                "Audio unmuted.")
       MSG_HASH(MSG_AUTOCONFIG_FILE_ERROR_SAVING,
                "Error saving autoconf file.")
       MSG_HASH(MSG_AUTOCONFIG_FILE_SAVED_SUCCESSFULLY,
                "Autoconfig file saved successfully.")
       MSG_HASH(MSG_AUTOSAVE_FAILED,
                "Could not initialize autosave.")
       MSG_HASH(MSG_AUTO_SAVE_STATE_TO,
                "Auto save state to")
       MSG_HASH(MSG_BLOCKING_SRAM_OVERWRITE,
                "Blocking SRAM Overwrite")
       MSG_HASH(MSG_BRINGING_UP_COMMAND_INTERFACE_ON_PORT,
                "Bringing up command interface on port")
       MSG_HASH(MSG_BYTES,
                "bytes")
       MSG_HASH(MSG_CANNOT_INFER_NEW_CONFIG_PATH,
                "Cannot infer new config path. Use current time.")
       MSG_HASH(MSG_CHEEVOS_HARDCORE_MODE_ENABLE,
                "Hardcore Mode Enabled, savestate & rewind were disabled.")
       MSG_HASH(MSG_COMPARING_WITH_KNOWN_MAGIC_NUMBERS,
                "Comparing with known magic numbers...")
       MSG_HASH(MSG_COMPILED_AGAINST_API,
                "Compiled against API")
       MSG_HASH(MSG_CONFIG_DIRECTORY_NOT_SET,
                "Config directory not set. Cannot save new config.")
       MSG_HASH(MSG_CONNECTED_TO,
                "Connected to")
       MSG_HASH(MSG_CONTENT_CRC32S_DIFFER,
                "Content CRC32s differ. Cannot use different games.")
       MSG_HASH(MSG_CONTENT_LOADING_SKIPPED_IMPLEMENTATION_WILL_DO_IT,
                "Content loading skipped. Implementation will load it on its own.")
       MSG_HASH(MSG_CORE_DOES_NOT_SUPPORT_SAVESTATES,
                "Core does not support save states.")
       MSG_HASH(MSG_CORE_OPTIONS_FILE_CREATED_SUCCESSFULLY,
                "Core options file created successfully.")
       MSG_HASH(MSG_COULD_NOT_FIND_ANY_NEXT_DRIVER,
                "Could not find any next driver")
       MSG_HASH(MSG_COULD_NOT_FIND_COMPATIBLE_SYSTEM,
                "Could not find compatible system.")
       MSG_HASH(MSG_COULD_NOT_FIND_VALID_DATA_TRACK,
                "Could not find valid data track")
       MSG_HASH(MSG_COULD_NOT_OPEN_DATA_TRACK,
                "could not open data track")
       MSG_HASH(MSG_COULD_NOT_READ_CONTENT_FILE,
                "Could not read content file")
       MSG_HASH(MSG_COULD_NOT_READ_MOVIE_HEADER,
                "Could not read movie header.")
       MSG_HASH(MSG_COULD_NOT_READ_STATE_FROM_MOVIE,
                "Could not read state from movie.")
       MSG_HASH(MSG_CRC32_CHECKSUM_MISMATCH,
                "CRC32 checksum mismatch between content file and saved content checksum in replay file header. Replay highly likely to desync on playback.")
       MSG_HASH(MSG_CUSTOM_TIMING_GIVEN,
                "Custom timing given")
       MSG_HASH(MSG_DECOMPRESSION_ALREADY_IN_PROGRESS,
                "Decompression already in progress.")
       MSG_HASH(MSG_DECOMPRESSION_FAILED,
                "Decompression failed.")
       MSG_HASH(MSG_DETECTED_VIEWPORT_OF,
                "Detected viewport of")
       MSG_HASH(MSG_DID_NOT_FIND_A_VALID_CONTENT_PATCH,
                "Did not find a valid content patch.")
       MSG_HASH(MSG_DISCONNECT_DEVICE_FROM_A_VALID_PORT,
                "Disconnect device from a valid port.")
       MSG_HASH(MSG_DISK_CLOSED,
                "Closed")
       MSG_HASH(MSG_DISK_EJECTED,
                "Ejected")
       MSG_HASH(MSG_DOWNLOADING,
                "Downloading")
       MSG_HASH(MSG_DOWNLOAD_FAILED,
                "Download failed")
       MSG_HASH(MSG_ERROR,
                "Error")
       MSG_HASH(MSG_ERROR_LIBRETRO_CORE_REQUIRES_CONTENT,
                "Libretro core requires content, but nothing was provided.")
       MSG_HASH(MSG_ERROR_LIBRETRO_CORE_REQUIRES_SPECIAL_CONTENT,
                "Libretro core requires special content, but none were provided.")
       MSG_HASH(MSG_ERROR_PARSING_ARGUMENTS,
                "Error parsing arguments.")
       MSG_HASH(MSG_ERROR_SAVING_CORE_OPTIONS_FILE,
                "Error saving core options file.")
       MSG_HASH(MSG_ERROR_SAVING_REMAP_FILE,
                "Error saving remap file.")
       MSG_HASH(MSG_ERROR_SAVING_SHADER_PRESET,
                "Error saving shader preset.")
       MSG_HASH(MSG_EXTERNAL_APPLICATION_DIR,
                "External Application Dir")
       MSG_HASH(MSG_EXTRACTING,
                "Extracting")
       MSG_HASH(MSG_EXTRACTING_FILE,
                "Extracting file")
       MSG_HASH(MSG_FAILED_SAVING_CONFIG_TO,
                "Failed saving config to")
       MSG_HASH(MSG_FAILED_TO,
                "Failed to")
       MSG_HASH(MSG_FAILED_TO_ACCEPT_INCOMING_SPECTATOR,
                "Failed to accept incoming spectator.")
       MSG_HASH(MSG_FAILED_TO_ALLOCATE_MEMORY_FOR_PATCHED_CONTENT,
                "Failed to allocate memory for patched content...")
       MSG_HASH(MSG_FAILED_TO_APPLY_SHADER,
                "Failed to apply shader.")
       MSG_HASH(MSG_FAILED_TO_BIND_SOCKET,
                "Failed to bind socket.")
       MSG_HASH(MSG_FAILED_TO_CREATE_THE_DIRECTORY,
                "Failed to create the directory.")
       MSG_HASH(MSG_FAILED_TO_EXTRACT_CONTENT_FROM_COMPRESSED_FILE,
                "Failed to extract content from compressed file")
       MSG_HASH(MSG_FAILED_TO_GET_NICKNAME_FROM_CLIENT,
                "Failed to get nickname from client.")
       MSG_HASH(MSG_FAILED_TO_LOAD,
                "Failed to load")
       MSG_HASH(MSG_FAILED_TO_LOAD_CONTENT,
                "Failed to load content")
       MSG_HASH(MSG_FAILED_TO_LOAD_MOVIE_FILE,
                "Failed to load movie file")
       MSG_HASH(MSG_FAILED_TO_LOAD_OVERLAY,
                "Failed to load overlay.")
       MSG_HASH(MSG_FAILED_TO_LOAD_STATE,
                "Failed to load state from")
       MSG_HASH(MSG_FAILED_TO_OPEN_LIBRETRO_CORE,
                "Failed to open libretro core")
       MSG_HASH(MSG_FAILED_TO_PATCH,
                "Failed to patch")
       MSG_HASH(MSG_FAILED_TO_RECEIVE_HEADER_FROM_CLIENT,
                "Failed to receive header from client.")
       MSG_HASH(MSG_FAILED_TO_RECEIVE_NICKNAME,
                "Failed to receive nickname.")
       MSG_HASH(MSG_FAILED_TO_RECEIVE_NICKNAME_FROM_HOST,
                "Failed to receive nickname from host.")
       MSG_HASH(MSG_FAILED_TO_RECEIVE_NICKNAME_SIZE_FROM_HOST,
                "Failed to receive nickname size from host.")
       MSG_HASH(MSG_FAILED_TO_RECEIVE_SRAM_DATA_FROM_HOST,
                "Failed to receive SRAM data from host.")
       MSG_HASH(MSG_FAILED_TO_REMOVE_DISK_FROM_TRAY,
                "Failed to remove disk from tray.")
       MSG_HASH(MSG_FAILED_TO_REMOVE_TEMPORARY_FILE,
                "Failed to remove temporary file")
       MSG_HASH(MSG_FAILED_TO_SAVE_SRAM,
                "Failed to save SRAM")
       MSG_HASH(MSG_FAILED_TO_SAVE_STATE_TO,
                "Failed to save state to")
       MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME,
                "Failed to send nickname.")
       MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME_SIZE,
                "Failed to send nickname size.")
       MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME_TO_CLIENT,
                "Failed to send nickname to client.")
       MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME_TO_HOST,
                "Failed to send nickname to host.")
       MSG_HASH(MSG_FAILED_TO_SEND_SRAM_DATA_TO_CLIENT,
                "Failed to send SRAM data to client.")
       MSG_HASH(MSG_FAILED_TO_START_AUDIO_DRIVER,
                "Failed to start audio driver. Will continue without audio.")
       MSG_HASH(MSG_FAILED_TO_START_MOVIE_RECORD,
                "Failed to start movie record.")
       MSG_HASH(MSG_FAILED_TO_START_RECORDING,
                "Failed to start recording.")
       MSG_HASH(MSG_FAILED_TO_TAKE_SCREENSHOT,
                "Failed to take screenshot.")
       MSG_HASH(MSG_FAILED_TO_UNDO_LOAD_STATE,
                "Failed to undo load state.")
       MSG_HASH(MSG_FAILED_TO_UNDO_SAVE_STATE,
                "Failed to undo save state.")
       MSG_HASH(MSG_FAILED_TO_UNMUTE_AUDIO,
                "Failed to unmute audio.")
       MSG_HASH(MSG_FATAL_ERROR_RECEIVED_IN,
                "Fatal error received in")
       MSG_HASH(MSG_FILE_NOT_FOUND,
                "File not found")
       MSG_HASH(MSG_FOUND_AUTO_SAVESTATE_IN,
                "Found auto savestate in")
       MSG_HASH(MSG_FOUND_DISK_LABEL,
                "Found disk label")
       MSG_HASH(MSG_FOUND_FIRST_DATA_TRACK_ON_FILE,
                "Found first data track on file")
       MSG_HASH(MSG_FOUND_LAST_STATE_SLOT,
                "Found last state slot")
       MSG_HASH(MSG_FOUND_SHADER,
                "Found shader")
       MSG_HASH(MSG_FRAMES,
                "Frames")
       MSG_HASH(MSG_GAME_SPECIFIC_CORE_OPTIONS_FOUND_AT,
                "Per-Game Options: game-specific core options found at")
       MSG_HASH(MSG_GOT_INVALID_DISK_INDEX,
                "Got invalid disk index.")
       MSG_HASH(MSG_GRAB_MOUSE_STATE,
                "Grab mouse state")
       MSG_HASH(MSG_GAME_FOCUS_ON,
                "Game focus on")
       MSG_HASH(MSG_GAME_FOCUS_OFF,
                "Game focus off")
       MSG_HASH(MSG_HW_RENDERED_MUST_USE_POSTSHADED_RECORDING,
                "Libretro core is hardware rendered. Must use post-shaded recording as well.")
       MSG_HASH(MSG_INFLATED_CHECKSUM_DID_NOT_MATCH_CRC32,
                "Inflated checksum did not match CRC32.")
       MSG_HASH(MSG_INPUT_CHEAT,
                "Input Cheat")
       MSG_HASH(MSG_INPUT_CHEAT_FILENAME,
                "Cheat Filename")
       MSG_HASH(MSG_INPUT_PRESET_FILENAME,
                "Preset Filename")
       MSG_HASH(MSG_INTERFACE,
                "Interface")
       MSG_HASH(MSG_INTERNAL_STORAGE,
                "Internal Storage")
       MSG_HASH(MSG_REMOVABLE_STORAGE,
                "Removable Storage")
       MSG_HASH(MSG_INVALID_NICKNAME_SIZE,
                "Invalid nickname size.")
       MSG_HASH(MSG_IN_BYTES,
                "in bytes")
       MSG_HASH(MSG_IN_GIGABYTES,
                "in gigabytes")
       MSG_HASH(MSG_IN_MEGABYTES,
                "in megabytes")
       MSG_HASH(MSG_LIBRETRO_ABI_BREAK,
                "is compiled against a different version of libretro than this libretro implementation.")
       MSG_HASH(MSG_LIBRETRO_FRONTEND,
                "Frontend for libretro")
       MSG_HASH(MSG_LOADED_STATE_FROM_SLOT,
                "Loaded state from slot #%d.")
       MSG_HASH(MSG_LOADED_STATE_FROM_SLOT_AUTO,
                "Loaded state from slot #-1 (auto).")
       MSG_HASH(MSG_LOADING,
                "Loading")
       MSG_HASH(MSG_FIRMWARE,
                "One or more firmware files are missing")
       MSG_HASH(MSG_LOADING_CONTENT_FILE,
                "Loading content file")
       MSG_HASH(MSG_LOADING_HISTORY_FILE,
                "Loading history file")
       MSG_HASH(MSG_LOADING_STATE,
                "Loading state")
       MSG_HASH(MSG_MEMORY,
                "Memory")
       MSG_HASH(MSG_MOVIE_FILE_IS_NOT_A_VALID_BSV1_FILE,
                "Movie file is not a valid BSV1 file.")
       MSG_HASH(MSG_MOVIE_FORMAT_DIFFERENT_SERIALIZER_VERSION,
                "Movie format seems to have a different serializer version. Will most likely fail.")
       MSG_HASH(MSG_MOVIE_PLAYBACK_ENDED,
                "Movie playback ended.")
       MSG_HASH(MSG_MOVIE_RECORD_STOPPED,
                "Stopping movie record.")
       MSG_HASH(MSG_NETPLAY_FAILED,
                "Failed to initialize netplay.")
       MSG_HASH(MSG_NO_CONTENT_STARTING_DUMMY_CORE,
                "No content, starting dummy core.")
       MSG_HASH(MSG_NO_SAVE_STATE_HAS_BEEN_OVERWRITTEN_YET,
                "No save state has been overwritten yet.")
       MSG_HASH(MSG_NO_STATE_HAS_BEEN_LOADED_YET,
                "No state has been loaded yet.")
       MSG_HASH(MSG_OVERRIDES_ERROR_SAVING,
                "Error saving overrides.")
       MSG_HASH(MSG_OVERRIDES_SAVED_SUCCESSFULLY,
                "Overrides saved successfully.")
       MSG_HASH(MSG_PAUSED,
                "Paused.")
       MSG_HASH(MSG_PROGRAM,
                "RetroArch")
       MSG_HASH(MSG_READING_FIRST_DATA_TRACK,
                "Reading first data track...")
       MSG_HASH(MSG_RECEIVED,
                "received")
       MSG_HASH(MSG_RECORDING_TERMINATED_DUE_TO_RESIZE,
                "Recording terminated due to resize.")
       MSG_HASH(MSG_RECORDING_TO,
                "Recording to")
       MSG_HASH(MSG_REDIRECTING_CHEATFILE_TO,
                "Redirecting cheat file to")
       MSG_HASH(MSG_REDIRECTING_SAVEFILE_TO,
                "Redirecting save file to")
       MSG_HASH(MSG_REDIRECTING_SAVESTATE_TO,
                "Redirecting savestate to")
       MSG_HASH(MSG_REMAP_FILE_SAVED_SUCCESSFULLY,
                "Remap file saved successfully.")
       MSG_HASH(MSG_REMOVED_DISK_FROM_TRAY,
                "Removed disk from tray.")
       MSG_HASH(MSG_REMOVING_TEMPORARY_CONTENT_FILE,
                "Removing temporary content file")
       MSG_HASH(MSG_RESET,
                "Reset")
       MSG_HASH(MSG_RESTARTING_RECORDING_DUE_TO_DRIVER_REINIT,
                "Restarting recording due to driver reinit.")
       MSG_HASH(MSG_RESTORED_OLD_SAVE_STATE,
                "Restored old save state.")
       MSG_HASH(MSG_RESTORING_DEFAULT_SHADER_PRESET_TO,
                "Shaders: restoring default shader preset to")
       MSG_HASH(MSG_REVERTING_SAVEFILE_DIRECTORY_TO,
                "Reverting savefile directory to")
       MSG_HASH(MSG_REVERTING_SAVESTATE_DIRECTORY_TO,
                "Reverting savestate directory to")
       MSG_HASH(MSG_REWINDING,
                "Rewinding.")
       MSG_HASH(MSG_REWIND_INIT,
                "Initializing rewind buffer with size")
       MSG_HASH(MSG_REWIND_INIT_FAILED,
                "Failed to initialize rewind buffer. Rewinding will be disabled.")
       MSG_HASH(MSG_REWIND_INIT_FAILED_THREADED_AUDIO,
                "Implementation uses threaded audio. Cannot use rewind.")
       MSG_HASH(MSG_REWIND_REACHED_END,
                "Reached end of rewind buffer.")
       MSG_HASH(MSG_SAVED_NEW_CONFIG_TO,
                "Saved new config to")
       MSG_HASH(MSG_SAVED_STATE_TO_SLOT,
                "Saved state to slot #%d.")
       MSG_HASH(MSG_SAVED_STATE_TO_SLOT_AUTO,
                "Saved state to slot #-1 (auto).")
       MSG_HASH(MSG_SAVED_SUCCESSFULLY_TO,
                "Saved successfully to")
       MSG_HASH(MSG_SAVING_RAM_TYPE,
                "Saving RAM type")
       MSG_HASH(MSG_SAVING_STATE,
                "Saving state")
       MSG_HASH(MSG_SCANNING,
                "Scanning")
       MSG_HASH(MSG_SCANNING_OF_DIRECTORY_FINISHED,
                "Scanning of directory finished")
       MSG_HASH(MSG_SENDING_COMMAND,
                "Sending command")
       MSG_HASH(MSG_SEVERAL_PATCHES_ARE_EXPLICITLY_DEFINED,
                "Several patches are explicitly defined, ignoring all...")
       MSG_HASH(MSG_SHADER,
                "Shader")
       MSG_HASH(MSG_SHADER_PRESET_SAVED_SUCCESSFULLY,
                "Shader preset saved successfully.")
       MSG_HASH(MSG_SKIPPING_SRAM_LOAD,
                "Skipping SRAM load.")
       MSG_HASH(MSG_SLOW_MOTION,
                "Slow motion.")
       MSG_HASH(MSG_SLOW_MOTION_REWIND,
                "Slow motion rewind.")
       MSG_HASH(MSG_SRAM_WILL_NOT_BE_SAVED,
                "SRAM will not be saved.")
       MSG_HASH(MSG_STARTING_MOVIE_PLAYBACK,
                "Starting movie playback.")
       MSG_HASH(MSG_STARTING_MOVIE_RECORD_TO,
                "Starting movie record to")
       MSG_HASH(MSG_STATE_SIZE,
                "State size")
       MSG_HASH(MSG_STATE_SLOT,
                "State slot")
       MSG_HASH(MSG_TAKING_SCREENSHOT,
                "Taking screenshot.")
       MSG_HASH(MSG_TO,
                "to")
       MSG_HASH(MSG_UNDID_LOAD_STATE,
                "Undid load state.")
       MSG_HASH(MSG_UNDOING_SAVE_STATE,
                "Undoing save state")
       MSG_HASH(MSG_UNKNOWN,
                "Unknown")
       MSG_HASH(MSG_UNPAUSED,
                "Unpaused.")
       MSG_HASH(MSG_UNRECOGNIZED_COMMAND,
                "Unrecognized command")
       MSG_HASH(MSG_USING_CORE_NAME_FOR_NEW_CONFIG,
                "Using core name for new config.")
       MSG_HASH(MSG_USING_LIBRETRO_DUMMY_CORE_RECORDING_SKIPPED,
                "Using libretro dummy core. Skipping recording.")
       MSG_HASH(MSG_VALUE_CONNECT_DEVICE_FROM_A_VALID_PORT,
                "Connect device from a valid port.")
       MSG_HASH(MSG_VALUE_DISCONNECTING_DEVICE_FROM_PORT,
                "Disconnecting device from port")
       MSG_HASH(MSG_VALUE_REBOOTING,
                "Rebooting...")
       MSG_HASH(MSG_VALUE_SHUTTING_DOWN,
                "Shutting down...")
       MSG_HASH(MSG_VERSION_OF_LIBRETRO_API,
                "Version of libretro API")
       MSG_HASH(MSG_VIEWPORT_SIZE_CALCULATION_FAILED,
                "Viewport size calculation failed! Will continue using raw data. This will probably not work right ...")
       MSG_HASH(MSG_VIRTUAL_DISK_TRAY,
                "virtual disk tray.")
       MSG_HASH(MENU_ENUM_SUBLABEL_AUDIO_LATENCY,
                "Desired audio latency in milliseconds. Might not be honored if the audio driver can't provide given latency.")
       MSG_HASH(MENU_ENUM_SUBLABEL_AUDIO_MUTE,
                "Mute/unmute audio.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_RATE_CONTROL_DELTA,
          "Helps smooth out imperfections in timing when synchronizing audio and video. Be aware that if disabled, proper synchronization is nearly impossible to obtain."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CAMERA_ALLOW,
          "Allow or disallow camera access by cores."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_LOCATION_ALLOW,
          "Allow or disallow location services access by cores."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_MAX_USERS,
          "Maximum amount of users supported by RetroArch."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_POLL_TYPE_BEHAVIOR,
          "Influence how input polling is done inside RetroArch. Setting it to 'Early' or 'Late' can result in less latency, depending on your configuration."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_ALL_USERS_CONTROL_MENU,
          "Allows any user to control the menu. If disabled, only User 1 can control the menu."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_VOLUME,
          "Audio volume (in dB). 0 dB is normal volume, and no gain is applied."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_WASAPI_EXCLUSIVE_MODE,
          "Allow the WASAPI driver to take exclusive control of the audio device. If disabled, it will use shared mode instead."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_WASAPI_FLOAT_FORMAT,
          "Use float format for the WASAPI driver, if supported by your audio device."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_WASAPI_SH_BUFFER_LENGTH,
          "The intermediate buffer length (in frames) when using the WASAPI driver in shared mode."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_SYNC,
          "Synchronize audio. Recommended."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_AXIS_THRESHOLD,
          "How far an axis must be tilted to result in a button press."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_BIND_TIMEOUT,
          "Amount of seconds to wait until proceeding to the next bind."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_TURBO_PERIOD,
          "Describes the period when turbo-enabled buttons are toggled. Numbers are described in frames."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_DUTY_CYCLE,
          "Describes how long the period of a turbo-enabled button should be. Numbers are described in frames."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_VSYNC,
          "Synchronizes the output video of the graphics card to the refresh rate of the screen. Recommended."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_ALLOW_ROTATE,
          "Allow cores to set rotation. When disabled, rotation requests are ignored. Useful for setups where one manually rotates the screen."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_DUMMY_ON_CORE_SHUTDOWN,
          "Some cores might have a shutdown feature. If enabled, it will prevent the core from shutting RetroArch down. Instead, it loads a dummy core."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CHECK_FOR_MISSING_FIRMWARE,
          "Check if all the required firmware is present before attempting to load content."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_REFRESH_RATE,
          "Vertical refresh rate of your screen. Used to calculate a suitable audio input rate. NOTE: This will be ignored if 'Threaded Video' is enabled."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_ENABLE,
          "Enable audio output."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_MAX_TIMING_SKEW,
          "The maximum change in audio input rate. Increasing this enables very large changes in timing at the cost of an inaccurate audio pitch (e.g., running PAL cores on NTSC displays)."
       )
       MSG_HASH(
          MSG_FAILED,
          "failed"
       )
       MSG_HASH(
          MSG_SUCCEEDED,
          "succeeded"
       )
       MSG_HASH(
          MSG_DEVICE_NOT_CONFIGURED,
          "not configured"
       )
       MSG_HASH(
          MSG_DEVICE_NOT_CONFIGURED_FALLBACK,
          "not configured, using fallback"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST,
          "Database Cursor List"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_DEVELOPER,
          "Database - Filter : Developer"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_PUBLISHER,
          "Database - Filter : Publisher"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_DISABLED,
          "Disabled"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_ENABLED,
          "Enabled"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_CONTENT_HISTORY_PATH,
          "Content History Path"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_ORIGIN,
          "Database - Filter : Origin")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_FRANCHISE,
                "Database - Filter : Franchise")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_ESRB_RATING,
                "Database - Filter : ESRB Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_ELSPA_RATING,
                "Database - Filter : ELSPA Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_PEGI_RATING,
                "Database - Filter : PEGI Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_CERO_RATING,
                "Database - Filter : CERO Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_BBFC_RATING,
                "Database - Filter : BBFC Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_MAX_USERS,
                "Database - Filter : Max Users")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_RELEASEDATE_BY_MONTH,
                "Database - Filter : Releasedate By Month")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_RELEASEDATE_BY_YEAR,
                "Database - Filter : Releasedate By Year")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_EDGE_MAGAZINE_ISSUE,
                "Database - Filter : Edge Magazine Issue")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_EDGE_MAGAZINE_RATING,
                "Database - Filter : Edge Magazine Rating")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_DATABASE_INFO,
                "Database Info")
       MSG_HASH(MSG_WIFI_SCAN_COMPLETE,
                "Wi-Fi scan complete.")
       MSG_HASH(MSG_SCANNING_WIRELESS_NETWORKS,
                "Scanning wireless networks...")
       MSG_HASH(MSG_NETPLAY_LAN_SCAN_COMPLETE,
                "Netplay scan complete.")
       MSG_HASH(MSG_NETPLAY_LAN_SCANNING,
                "Scanning for netplay hosts...")
       MSG_HASH(MENU_ENUM_SUBLABEL_PAUSE_NONACTIVE,
                "Pause gameplay when RetroArch is not the active window.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_DISABLE_COMPOSITION,
                "Enable or disable composition (Windows only).")
       MSG_HASH(MENU_ENUM_SUBLABEL_HISTORY_LIST_ENABLE,
                "Enable or disable recent playlist for games, images, music, and videos.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CONTENT_HISTORY_SIZE,
                "Limit the number of entries in recent playlist for games, images, music, and videos.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_UNIFIED_MENU_CONTROLS,
                "Unified Menu Controls")
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_UNIFIED_MENU_CONTROLS,
                "Use the same controls for both the menu and the game. Applies to the keyboard.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FONT_ENABLE,
                "Show onscreen messages.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_USER_REMOTE_ENABLE,
                "User %d Remote Enable")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_BATTERY_LEVEL_ENABLE,
                "Display battery level")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SELECT_FILE,
                "Select File")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SELECT_FROM_COLLECTION,
                "Select From Collection")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FILTER,
                "Filter")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SCALE,
                "Scale")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_START_WHEN_LOADED,
                "Netplay will start when content is loaded.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_LOAD_CONTENT_MANUALLY,
                "Couldn't find a suitable core or content file, load manually.")
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BROWSE_URL_LIST,
          "Browse URL"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BROWSE_URL,
          "URL Path"
       )
       MSG_HASH(
          MENU_ENUM_LABEL_VALUE_BROWSE_START,
          "Start"
       )
       MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_BOKEH,
                "Bokeh")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_REFRESH_ROOMS,
                "Refresh Room List")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ROOM_NICKNAME,
                "Nickname: %s")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ROOM_NICKNAME_LAN,
                "Nickname (lan): %s")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_COMPAT_CONTENT_FOUND,
                "Compatible content found")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_CROP_OVERSCAN,
                "Cuts off a few pixels around the edges of the image customarily left blank by developers which sometimes also contain garbage pixels.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SMOOTH,
                "Adds a slight blur to the image to take the edge off of the hard pixel edges. This option has very little impact on performance.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FILTER,
                "Apply a CPU-powered video filter. NOTE: Might come at a high performance cost. Some video filters might only work for cores that use 32bit or 16bit color.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_USERNAME,
                "Input the username of your Retro Achievements account.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_PASSWORD,
                "Input the password of your Retro Achievements account.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_NICKNAME,
                "Input your user name here. This will be used for netplay sessions, among other things.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_POST_FILTER_RECORD,
                "Capture the image after filters (but not shaders) are applied. Your video will look as fancy as what you see on your screen.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_LIST,
                "Select which core to use.")
       MSG_HASH(MENU_ENUM_SUBLABEL_LOAD_CONTENT_LIST,
                "Select which content to start.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETWORK_INFORMATION,
                "Show network interface(s) and associated IP addresses.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SYSTEM_INFORMATION,
                "Show information specific to the device.")
       MSG_HASH(MENU_ENUM_SUBLABEL_QUIT_RETROARCH,
                "Quit the program.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_WINDOW_WIDTH,
                "Set the custom width size for the display window. Leaving it at 0 will attempt to scale the window as large as possible.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_WINDOW_HEIGHT,
                "Set the custom height size for the display window. Leaving it at 0 will attempt to scale the window as large as possible.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FULLSCREEN_X,
                "Set the custom width size for the non-windowed fullscreen mode. Leaving it at 0 will use the desktop resolution.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FULLSCREEN_Y,
                "Set the custom height size for the non-windowed fullscreen mode. Leaving it at 0 will use the desktop resolution")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_MESSAGE_POS_X,
                "Specify custom X axis position for onscreen text.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_MESSAGE_POS_Y,
                "Specify custom Y axis position for onscreen text.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FONT_SIZE,
                "Specify the font size in points.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_OVERLAY_HIDE_IN_MENU,
                "Hide the overlay while inside the menu, and show it again when exiting the menu.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CONTENT_COLLECTION_LIST,
          "Scanned content will appear here."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_SCALE_INTEGER,
          "Only scales video in integer steps. The base size depends on system-reported geometry and aspect ratio. If 'Force Aspect' is not set, X/Y will be integer scaled independently."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_GPU_SCREENSHOT,
          "Screenshots output of GPU shaded material if available."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_ROTATION,
          "Forces a certain rotation of the screen. The rotation is added to rotations which the core sets."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_FORCE_SRGB_DISABLE,
          "Forcibly disable sRGB FBO support. Some Intel OpenGL drivers on Windows have video problems with sRGB FBO support if this is enabled. Enabling this can work around it."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_FULLSCREEN,
          "Start in fullscreen. Can be changed at runtime."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_WINDOWED_FULLSCREEN,
          "If fullscreen, prefer using a windowed fullscreen mode."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_GPU_RECORD,
          "Records output of GPU shaded material if available."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_SAVESTATE_AUTO_INDEX,
          "When making a savestate, save state index is automatically increased before it is saved. When loading content, the index will be set to the highest existing index."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_BLOCK_SRAM_OVERWRITE,
          "Block Save RAM from being overwritten when loading save states. Might potentially lead to buggy games."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_FASTFORWARD_RATIO,
          "The maximum rate at which content will be run when using fast forward (e.g., 5.0x for 60 fps content = 300 fps cap). If set to 0.0x, fastforward ratio is unlimited (no FPS cap)."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_SLOWMOTION_RATIO,
          "When in slow motion, content will slow down by the factor specified/set."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_REWIND_ENABLE,
          "Enable rewinding. This will take a performance hit when playing."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_REWIND_GRANULARITY,
          "When rewinding a defined number of frames, you can rewind several frames at a time, increasing the rewind speed."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_LIBRETRO_LOG_LEVEL,
          "Sets log level for cores. If a log level issued by a core is below this value, it is ignored."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_PERFCNT_ENABLE,
          "Enable performance counters for RetroArch (and cores)."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_SAVESTATE_AUTO_SAVE,
          "Automatically makes a savestate at the end of RetroArch's runtime. RetroArch will automatically load this savestate if 'Auto Load State' is enabled."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_SAVESTATE_AUTO_LOAD,
          "Automatically load the auto save state on startup."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_SAVESTATE_THUMBNAIL_ENABLE,
          "Show thumbnails of save states inside the menu."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUTOSAVE_INTERVAL,
          "Autosaves the non-volatile Save RAM at a regular interval. This is disabled by default unless set otherwise. The interval is measured in seconds. A value of 0 disables autosave."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_REMAP_BINDS_ENABLE,
          "If enabled, overrides the input binds with the remapped binds set for the current core."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_AUTODETECT_ENABLE,
          "Enable input auto-detection. Will attempt to autoconfigure joypads, Plug-and-Play style."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_MENU_INPUT_SWAP_OK_CANCEL,
          "Swap buttons for OK/Cancel. Disabled is the Japanese button orientation, enabled is the western orientation."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_PAUSE_LIBRETRO,
          "If disabled, the content will keep running in the background when RetroArch's menu is toggled."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_DRIVER,
          "Video driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_DRIVER,
          "Audio driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_DRIVER,
          "Input driver to use. Depending on the video driver, it might force a different input driver."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_JOYPAD_DRIVER,
          "Joypad driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_RESAMPLER_DRIVER,
          "Audio resampler driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CAMERA_DRIVER,
          "Camera driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_LOCATION_DRIVER,
          "Location driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_MENU_DRIVER,
          "Menu driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_RECORD_DRIVER,
          "Record driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_WIFI_DRIVER,
          "WiFi driver to use."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NAVIGATION_BROWSER_FILTER_SUPPORTED_EXTENSIONS_ENABLE,
          "Filter files being shown in filebrowser by supported extensions."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_MENU_WALLPAPER,
          "Select an image to set as menu wallpaper."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_DYNAMIC_WALLPAPER,
          "Dynamically load a new wallpaper depending on context."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_DEVICE,
          "Override the default audio device the audio driver uses. This is driver dependent."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_DSP_PLUGIN,
          "Audio DSP plugin that processes audio before it's sent to the driver."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_OUTPUT_RATE,
          "Audio output sample rate."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_OVERLAY_OPACITY,
          "Opacity of all UI elements of the overlay."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_OVERLAY_SCALE,
          "Scale of all UI elements of the overlay."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_INPUT_OVERLAY_ENABLE,
          "Enable the overlay."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_OVERLAY_PRESET,
          "Select an overlay from the file browser."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_IP_ADDRESS,
          "The address of the host to connect to."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_TCP_UDP_PORT,
          "The port of the host IP address. Can be either a TCP or UDP port."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_PASSWORD,
          "The password for connecting to the netplay host. Used only in host mode."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_PUBLIC_ANNOUNCE,
          "Whether to announce netplay games publicly. If unset, clients must manually connect rather than using the public lobby."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_SPECTATE_PASSWORD,
          "The password for connecting to the netplay host with only spectator privileges. Used only in host mode."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_START_AS_SPECTATOR,
          "Whether to start netplay in spectator mode."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_ALLOW_SLAVES,
          "Whether to allow connections in slave mode. Slave-mode clients require very little processing power on either side, but will suffer significantly from network latency."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_REQUIRE_SLAVES,
          "Whether to disallow connections not in slave mode. Not recommended except for very fast networks with very weak machines."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_STATELESS_MODE,
          "Whether to run netplay in a mode not requiring save states. If set to true, a very fast network is required, but no rewinding is performed, so there will be no netplay jitter."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_CHECK_FRAMES,
          "The frequency in frames with which netplay will verify that the host and client are in sync."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_NAT_TRAVERSAL,
          "When hosting, attempt to listen for connections from the public Internet, using UPnP or similar technologies to escape LANs."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_STDIN_CMD_ENABLE,
          "Enable stdin command interface."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_MOUSE_ENABLE,
          "Enable mouse controls inside the menu."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_POINTER_ENABLE,
          "Enable touch controls inside the menu."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_THUMBNAILS,
          "Type of thumbnail to display."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_TIMEDATE_ENABLE,
          "Shows current date and/or time inside the menu."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_BATTERY_LEVEL_ENABLE,
          "Shows current battery level inside the menu."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NAVIGATION_WRAPAROUND,
          "Wrap-around to beginning and/or end if boundary of list is reached horizontally or vertically."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_NETPLAY_ENABLE_HOST,
          "Enables netplay in host (server) mode."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_ENABLE_CLIENT,
                "Enables netplay in client mode.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_DISCONNECT,
                "Disconnects an active Netplay connection.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SCAN_DIRECTORY,
                "Scans a directory for compatible files and add them to the collection.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SCAN_FILE,
                "Scans a compatible file and add it to the collection.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SWAP_INTERVAL,
                "Uses a custom swap interval for Vsync. Set this to effectively halve monitor refresh rate."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_SORT_SAVEFILES_ENABLE,
                "Sort save files in folders named after the core used."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_SORT_SAVESTATES_ENABLE,
                "Sort save states in folders named after the core used."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_CLIENT_SWAP_INPUT,
                "When being client over netplay, use keybinds for Player 1.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_UPDATER_BUILDBOT_URL,
                "URL to core updater directory on the Libretro buildbot.")
       MSG_HASH(MENU_ENUM_SUBLABEL_BUILDBOT_ASSETS_URL,
                "URL to assets updater directory on the Libretro buildbot.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_UPDATER_AUTO_EXTRACT_ARCHIVE,
                "After downloading, automatically extract files contained in the downloaded archives."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_REFRESH_ROOMS,
                "Scan for new rooms.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DELETE_ENTRY,
                "Remove this entry from the collection.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INFORMATION,
                "View more information about the content.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RUN,
                "Start the content.")
       MSG_HASH(MENU_ENUM_SUBLABEL_MENU_FILE_BROWSER_SETTINGS,
                "Adjusts filebrowser settings.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUTO_REMAPS_ENABLE,
          "Enable customized controls by default at startup."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUTO_OVERRIDES_ENABLE,
          "Enable customized configuration by default at startup."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_GAME_SPECIFIC_OPTIONS,
                "Enable customized core options by default at startup.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_ENABLE,
                "Shows current core name inside menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DATABASE_MANAGER,
                "View databases.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CURSOR_MANAGER,
                "View previous searches.")
       MSG_HASH(MENU_ENUM_SUBLABEL_TAKE_SCREENSHOT,
                "Captures an image of the screen.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CLOSE_CONTENT,
          "Closes the current content. Any unsaved changes might be lost."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_LOAD_STATE,
                "Load a saved state from the currently selected slot.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SAVE_STATE,
                "Save a state to the currently selected slot.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RESUME,
                "Resume the currently running content and leave the Quick Menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RESUME_CONTENT,
                "Resume the currently running content and leave the Quick Menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_STATE_SLOT,
                "Changes the currently selected state slot.")
       MSG_HASH(MENU_ENUM_SUBLABEL_UNDO_LOAD_STATE,
                "If a state was loaded, content will go back to the state prior to loading.")
       MSG_HASH(MENU_ENUM_SUBLABEL_UNDO_SAVE_STATE,
                "If a state was overwritten, it will roll back to the previous save state.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_ACCOUNTS_RETRO_ACHIEVEMENTS,
          "Retro Achievements service. For more information, visit http://retroachievements.org"
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_ACCOUNTS_LIST,
          "Manages currently configured accounts."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_META_REWIND,
                "Manages rewind settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RESTART_CONTENT,
                "Restarts the content from the beginning.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SAVE_CURRENT_CONFIG_OVERRIDE_CORE,
                "Saves an override configuration file which will apply for all content loaded with this core. Will take precedence over the main configuration.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SAVE_CURRENT_CONFIG_OVERRIDE_GAME,
                "Saves an override configuration file which will apply for the current content only. Will take precedence over the main configuration.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_CHEAT_OPTIONS,
                "Set up cheat codes.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SHADER_OPTIONS,
                "Set up shaders to visually augment the image.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_INPUT_REMAPPING_OPTIONS,
                "Change the controls for the currently running content.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_OPTIONS,
                "Change the options for the currently running content.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SHOW_ADVANCED_SETTINGS,
                "Show advanced settings for power users (hidden by default).")
       MSG_HASH(MENU_ENUM_SUBLABEL_THREADED_DATA_RUNLOOP_ENABLE,
                "Perform tasks on a separate thread.")
       MSG_HASH(MENU_ENUM_SUBLABEL_PLAYLIST_ENTRY_REMOVE,
                "Allow the user to remove entries from collections.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SYSTEM_DIRECTORY,
                "Sets the System directory. Cores can query for this directory to load BIOSes, system-specific configs, etc.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RGUI_BROWSER_DIRECTORY,
                "Sets start directory for the filebrowser.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CONTENT_DIR,
          "Usually set by developers who bundle libretro/RetroArch apps to point to assets."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_DYNAMIC_WALLPAPERS_DIRECTORY,
                "Directory to store wallpapers dynamically loaded by the menu depending on context.")
       MSG_HASH(MENU_ENUM_SUBLABEL_THUMBNAILS_DIRECTORY,
                "Supplementary thumbnails (boxarts/misc. images, etc.) are stored here."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_RGUI_CONFIG_DIRECTORY,
                "Sets start directory for menu configuration browser.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_INPUT_LATENCY_FRAMES_MIN,
                "The number of frames of input latency for netplay to use to hide network latency. Reduces jitter and makes netplay less CPU-intensive, at the expense of noticeable input lag.")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_INPUT_LATENCY_FRAMES_RANGE,
                "The range of frames of input latency that may be used to hide network latency. Reduces jitter and makes netplay less CPU-intensive, at the expense of unpredictable input lag.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DISK_CYCLE_TRAY_STATUS,
                "Cycle the current disk. If the disk is inserted, it will eject the disk. If the disk has not been inserted, it will be inserted. ")
       MSG_HASH(MENU_ENUM_SUBLABEL_DISK_INDEX,
                "Change the disk index.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DISK_OPTIONS,
                "Disk image management.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DISK_IMAGE_APPEND,
                "Select a disk image to insert.")
       MSG_HASH(MENU_ENUM_SUBLABEL_MENU_ENUM_THROTTLE_FRAMERATE,
                "Makes sure the framerate is capped while inside the menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_THEME,
                "Select a different theme for the icon. Changes will take effect after you restart the program.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHADOWS_ENABLE,
                "Enable drop shadows for all icons. This will have a minor performance hit.")
       MSG_HASH(MENU_ENUM_SUBLABEL_MATERIALUI_MENU_COLOR_THEME,
                "Select a different background color gradient theme.")
       MSG_HASH(MENU_ENUM_SUBLABEL_MENU_WALLPAPER_OPACITY,
                "Modify the opacity of the background wallpaper.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_MENU_COLOR_THEME,
                "Select a different background color gradient theme.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_RIBBON_ENABLE,
                "Select an animated background effect. Can be GPU-intensive depending on the effect. If performance is unsatisfactory, either turn this off or revert to a simpler effect.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_FONT,
                "Select a different main font to be used by the menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHOW_IMAGES,
                "Show the image tab inside the main menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHOW_MUSIC,
                "Show the music tab inside the main menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHOW_VIDEO,
                "Show the video tab inside the main menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHOW_NETPLAY,
                "Show the netplay tab inside the main menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHOW_SETTINGS,
                "Show the settings tab inside the main menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHOW_HISTORY,
                "Show the recent history tab inside the main menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_XMB_SHOW_ADD,
                "Show the import content tab inside the main menu.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RGUI_SHOW_START_SCREEN,
                "Show startup screen in menu. This is automatically set to false after the program starts for the first time.")
       MSG_HASH(MENU_ENUM_SUBLABEL_MATERIALUI_MENU_HEADER_OPACITY,
                "Modify the opacity of the header graphic.")
       MSG_HASH(MENU_ENUM_SUBLABEL_MATERIALUI_MENU_FOOTER_OPACITY,
                "Modify the opacity of the footer graphic.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DPI_OVERRIDE_ENABLE,
                "The menu normally scales itself dynamically. If you want to set a specific scaling size instead, enable this.")
       MSG_HASH(MENU_ENUM_SUBLABEL_DPI_OVERRIDE_VALUE,
                "Set the custom scaling size here. NOTE: You have to enable 'DPI Override' for this scaling size to take effect.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_ASSETS_DIRECTORY,
                "Save all downloaded files to this directory.")
       MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_REMAPPING_DIRECTORY,
                "Save all remapped controls to this directory.")
       MSG_HASH(MENU_ENUM_SUBLABEL_LIBRETRO_DIR_PATH,
                "Directory where the program searches for content/cores.")
       MSG_HASH(MENU_ENUM_SUBLABEL_LIBRETRO_INFO_PATH,
                "Application/core information files are stored here.")
       MSG_HASH(MENU_ENUM_SUBLABEL_JOYPAD_AUTOCONFIG_DIR,
                "If a joypad is plugged in, that joypad will be autoconfigured if a config file corresponding to it is present inside this directory.")
       MSG_HASH(MENU_ENUM_SUBLABEL_PLAYLIST_DIRECTORY,
                "Save all collections to this directory.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CACHE_DIRECTORY,
          "If set to a directory, content which is temporarily extracted (e.g. from archives) will be extracted to this directory."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_CURSOR_DIRECTORY,
                "Saved queries are stored to this directory.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CONTENT_DATABASE_DIRECTORY,
          "Databases are stored to this directory."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_ASSETS_DIRECTORY,
          "This location is queried by default when menu interfaces try to look for loadable assets, etc."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_SAVEFILE_DIRECTORY,
                "Save all save files to this directory. If not set, will try to save inside the content file's working directory.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SAVESTATE_DIRECTORY,
                "Save all save states to this directory. If not set, will try to save inside the content file's working directory.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SCREENSHOT_DIRECTORY,
                "Directory to dump screenshots to.")
       MSG_HASH(MENU_ENUM_SUBLABEL_OVERLAY_DIRECTORY,
                "Defines a directory where overlays are kept for easy access.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CHEAT_DATABASE_PATH,
          "Cheat files are kept here."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_AUDIO_FILTER_DIR,
          "Directory where audio DSP filter files are kept."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_FILTER_DIR,
          "Directory where CPU-based video filter files are kept."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SHADER_DIR,
                "Defines a directory where GPU-based video shader files are kept for easy access.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RECORDING_OUTPUT_DIRECTORY,
                "Recordings will be dumped to this directory.")
       MSG_HASH(MENU_ENUM_SUBLABEL_RECORDING_CONFIG_DIRECTORY,
                "Recording configurations will be kept here.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FONT_PATH,
                "Select a different font for onscreen notifications.")
       MSG_HASH(MENU_ENUM_SUBLABEL_SHADER_APPLY_CHANGES,
                "Changes to the shader configuration will take effect immediately. Use this if you changed the amount of shader passes, filtering, FBO scale, etc.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_VIDEO_SHADER_NUM_PASSES,
          "Increase or decrease the amount of shader pipeline passes. You can bind a separate shader to each pipeline pass and configure its scale and filtering."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SHADER_PRESET,
                "Load a shader preset. The shader pipeline will be automatically set-up.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SHADER_PRESET_SAVE_AS,
                "Save the current shader settings as a new shader preset.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SHADER_PRESET_SAVE_CORE,
                "Save the current shader settings as the default settings for this application/core.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SHADER_PRESET_SAVE_GAME,
                "Save the current shader settings as the default settings for the content.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SHADER_PARAMETERS,
                "Modifies the current shader directly. Changes will not be saved to the preset file.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SHADER_PRESET_PARAMETERS,
                "Modifies the shader preset itself currently used in the menu.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CHEAT_NUM_PASSES,
          "Increase or decrease the amount of cheats."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_CHEAT_APPLY_CHANGES,
                "Cheat changes will take effect immediately.")
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CHEAT_FILE_LOAD,
          "Load a cheat file."
       )
       MSG_HASH(
          MENU_ENUM_SUBLABEL_CHEAT_FILE_SAVE_AS,
          "Save current cheats as a save file."
       )
       MSG_HASH(MENU_ENUM_SUBLABEL_CONTENT_SETTINGS,
                "Quickly access all relevant in-game settings.")
       MSG_HASH(MENU_ENUM_SUBLABEL_CORE_INFORMATION,
                "View information pertaining to the application/core.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_ASPECT_RATIO,
                "Floating point value for video aspect ratio (width / height), used if the Aspect Ratio is set to 'Config'.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_VIEWPORT_CUSTOM_HEIGHT,
                "Custom viewport height that is used if the Aspect Ratio is set to 'Custom'.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_VIEWPORT_CUSTOM_WIDTH,
                "Custom viewport width that is used if the Aspect Ratio is set to 'Custom'.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_VIEWPORT_CUSTOM_X,
                "Custom viewport offset used for defining the X-axis position of the viewport. These are ignored if 'Integer Scale' is enabled. It will be automatically centered then.")
       MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_VIEWPORT_CUSTOM_Y,
                "Custom viewport offset used for defining the Y-axis position of the viewport. These are ignored if 'Integer Scale' is enabled. It will be automatically centered then.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_USE_MITM_SERVER,
                "Use MITM Server")
       MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_USE_MITM_SERVER,
                "Forward netplay connections through a man-in-the-middle server. Useful if the host is behind a firewall or has NAT/UPnP problems.")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ADD_TO_MIXER,
                "Add to mixer")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_ADD_TO_MIXER_AND_COLLECTION,
                "Add to mixer")
       MSG_HASH(MENU_ENUM_LABEL_VALUE_FILTER_BY_CURRENT_CORE,
                "Filter by current core")

       default:
#if 0
            RARCH_LOG("Unimplemented: [%d]\n", msg);
            {
               RARCH_LOG("[%d] : %s\n", msg - 1, msg_hash_to_str(((enum msg_hash_enums)(msg - 1))));
            }
#endif
            break;
    }

    return "null";
}
