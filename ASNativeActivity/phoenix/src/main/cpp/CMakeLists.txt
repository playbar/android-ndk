cmake_minimum_required(VERSION 3.4.1)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
            -Wall -std=gnu99 -pthread -Wno-unused-function -fno-stack-protector -funroll-loops -Wno-invalid-source-encoding")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fpermissive -std=c++11 -fno-rtti -Wno-reorder -Wno-invalid-source-encoding")

ADD_DEFINITIONS(-DANDROID_ARM)
ADD_DEFINITIONS( -DHAVE_OPENGLES3)
ADD_DEFINITIONS(-DSINC_LOWER_QUALITY -DANDROID_ARM_V7)
ADD_DEFINITIONS(-DHAVE_ZLIB -DRARCH_MOBILE -DHAVE_GRIFFIN -DHAVE_STB_VORBIS -DANDROID -DHAVE_DYNAMIC -DHAVE_OPENGL)
ADD_DEFINITIONS(-DHAVE_FBO -DHAVE_OVERLAY -DHAVE_OPENGLES -DGLSL_DEBUG -DHAVE_DYLIB -DHAVE_EGL -DHAVE_GLSL -DHAVE_MENU -DHAVE_RGUI)
ADD_DEFINITIONS(-DHAVE_RPNG -DHAVE_RJPEG -DHAVE_RBMP -DHAVE_RTGA -DINLINE=inline -D__LIBRETRO__ -DHAVE_RSOUND -DHAVE_NETWORKGAMEPAD -DHAVE_NETWORKING)
ADD_DEFINITIONS(-DRARCH_INTERNAL -DHAVE_FILTERS_BUILTIN -DHAVE_MATERIALUI -DHAVE_XMB -DHAVE_SHADERPIPELINE -DHAVE_LIBRETRODB -DHAVE_STB_FONT)
ADD_DEFINITIONS( -DHAVE_IMAGEVIEWER -DHAVE_UPDATE_ASSETS -DHAVE_CC_RESAMPLER -DHAVE_MINIUPNPC -DHAVE_BUILTINMINIUPNPC -DMINIUPNPC_SET_SOCKET_TIMEOUT)
ADD_DEFINITIONS(-DMINIUPNPC_GET_SRC_ADDR -DWANT_IFADDRS -DHAVE_SHADERS -DHAVE_COMPRESSION -DJSON_STATIC )
ADD_DEFINITIONS(-DHAVE_VULKAN)
ADD_DEFINITIONS(-DHAVE_7ZIP -DHAVE_CHEEVOS -DHAVE_SL)


set(RARCH_DIR ${CMAKE_SOURCE_DIR})
set(LIBRETRO_COMM_DIR ${RARCH_DIR}/libretro-common)
set(DEPS_DIR ${RARCH_DIR}/deps)

SET(HAVE_ZLIB true)

add_library(utils_lib SHARED IMPORTED)
set_target_properties(utils_lib  PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libutils.so)


##################################################
#retroarch-activity so
MESSAGE(STATUS "Project: retroarch-activity")

#make retroarch-activity

add_library(retroarch-activity SHARED
            ${LIBRETRO_COMM_DIR}/audio/conversion/s16_to_float_neon.S
            ${LIBRETRO_COMM_DIR}/audio/conversion/float_to_s16_neon.S
            ${LIBRETRO_COMM_DIR}/audio/resampler/drivers/sinc_resampler_neon.S
            ${RARCH_DIR}/audio/drivers_resampler/cc_resampler_neon.S
                ${RARCH_DIR}/src/verbosity.c
                ${RARCH_DIR}/libretro-common/file/archive_file.c
                ${RARCH_DIR}/libretro-common/file/archive_file_zlib.c
                ${RARCH_DIR}/libretro-common/file/archive_file_7z.c
                ${RARCH_DIR}/libretro-common/streams/stdin_stream.c
                ${RARCH_DIR}/libretro-common/streams/trans_stream.c
                ${RARCH_DIR}/libretro-common/streams/trans_stream_pipe.c
                ${RARCH_DIR}/libretro-common/streams/trans_stream_zlib.c
                ${RARCH_DIR}/libretro-common/encodings/encoding_utf.c
                ${RARCH_DIR}/libretro-common/encodings/encoding_crc32.c
                ${RARCH_DIR}/libretro-common/features/features_cpu.c
                ${RARCH_DIR}/src/performance_counters.c
                ${LIBRETRO_COMM_DIR}/compat/compat_getopt.c
                ${LIBRETRO_COMM_DIR}/compat/compat_strcasestr.c
                ${LIBRETRO_COMM_DIR}/compat/compat_strl.c
                ${LIBRETRO_COMM_DIR}/compat/compat_ifaddrs.c
                ${RARCH_DIR}/libretro-common/compat/compat_fnmatch.c
                ${RARCH_DIR}/libretro-common/memmap/memalign.c
                ${RARCH_DIR}/libretro-common/file/config_file.c
                ${RARCH_DIR}/libretro-common/file/config_file_userdata.c
                ${RARCH_DIR}/managers/core_manager.c
                ${RARCH_DIR}/managers/core_option_manager.c
                ${RARCH_DIR}/libretro-common/formats/json/jsonsax.c
                ${RARCH_DIR}/network/net_http_special.c
                ${RARCH_DIR}/cheevos/cheevos.c
                ${RARCH_DIR}/libretro-common/utils/md5.c
                ${RARCH_DIR}/managers/cheat_manager.c
                ${RARCH_DIR}/libretro-common/hash/rhash.c
                ${RARCH_DIR}/gfx/drivers_context/android_ctx.c
                ${RARCH_DIR}/gfx/common/vulkan_common.c
                ${RARCH_DIR}/libretro-common/vulkan/vulkan_symbol_wrapper.c
                ${RARCH_DIR}/gfx/common/egl_common.c
                ${RARCH_DIR}/gfx/video_shader_parse.c
                ${RARCH_DIR}/gfx/drivers_shader/shader_null.c
                ${RARCH_DIR}/gfx/drivers_shader/shader_glsl.c
                ${RARCH_DIR}/libretro-common/formats/image_texture.c
                ${RARCH_DIR}/libretro-common/formats/tga/rtga.c
                ${RARCH_DIR}/cores/libretro-imageviewer/image_core.c
                ${RARCH_DIR}/libretro-common/formats/image_transfer.c
                ${RARCH_DIR}/libretro-common/formats/png/rpng.c
                ${RARCH_DIR}/libretro-common/formats/png/rpng_encode.c
                ${RARCH_DIR}/libretro-common/formats/jpeg/rjpeg.c
                ${RARCH_DIR}/libretro-common/formats/bmp/rbmp.c
                ${RARCH_DIR}/libretro-common/formats/bmp/rbmp_encode.c
                ${RARCH_DIR}/libretro-common/formats/wav/rwav.c
                ${RARCH_DIR}/gfx/drivers/vulkan.c
                ${RARCH_DIR}/gfx/video_renderchain_driver.c
                ${RARCH_DIR}/gfx/drivers_renderchain/null_renderchain.c
                ${RARCH_DIR}/gfx/common/gl_common.c
                ${RARCH_DIR}/gfx/drivers/gl.c
                ${RARCH_DIR}/libretro-common/gfx/gl_capabilities.c
                ${RARCH_DIR}/gfx/drivers_renderchain/gl_legacy_renderchain.c
                ${RARCH_DIR}/libretro-common/glsym/rglgen.c
                ${RARCH_DIR}/libretro-common/glsym/glsym_es3.c
                ${RARCH_DIR}/gfx/drivers/nullgfx.c
                ${RARCH_DIR}/gfx/drivers_font_renderer/bitmapfont.c
                ${RARCH_DIR}/gfx/font_driver.c
                ${RARCH_DIR}/gfx/drivers_font_renderer/stb_unicode.c
                ${RARCH_DIR}/gfx/drivers_font_renderer/stb.c
                ${RARCH_DIR}/gfx/drivers_font/gl_raster_font.c
                ${RARCH_DIR}/gfx/drivers_font/vulkan_raster_font.c
                ${RARCH_DIR}/tasks/task_autodetect.c
                ${RARCH_DIR}/tasks/task_audio_mixer.c
                ${RARCH_DIR}/input/input_config.c
                ${RARCH_DIR}/input/input_keymaps.c
                ${RARCH_DIR}/input/input_remapping.c
                ${RARCH_DIR}/input/input_keyboard.c
                ${RARCH_DIR}/input/input_overlay.c
                ${RARCH_DIR}/tasks/task_overlay.c
                ${RARCH_DIR}/input/input_autodetect_builtin.c
                ${RARCH_DIR}/input/drivers/android_input.c
                ${RARCH_DIR}/input/drivers_keyboard/keyboard_event_android.c
                ${RARCH_DIR}/input/drivers_joypad/android_joypad.c
                ${RARCH_DIR}/input/drivers/nullinput.c
                ${RARCH_DIR}/input/drivers_joypad/null_joypad.c
                ${RARCH_DIR}/gfx/video_state_tracker.c
                ${RARCH_DIR}/libretro-common/queues/fifo_queue.c
                ${RARCH_DIR}/libretro-common/audio/resampler/audio_resampler.c
                ${RARCH_DIR}/libretro-common/audio/resampler/drivers/sinc_resampler.c
                ${RARCH_DIR}/libretro-common/audio/resampler/drivers/nearest_resampler.c
                ${RARCH_DIR}/libretro-common/audio/resampler/drivers/null_resampler.c
                ${RARCH_DIR}/audio/drivers_resampler/cc_resampler.c
                ${RARCH_DIR}/camera/drivers/android.c
                ${RARCH_DIR}/camera/drivers/nullcamera.c
                ${RARCH_DIR}/location/drivers/android.c
                ${RARCH_DIR}/location/drivers/nulllocation.c
                ${RARCH_DIR}/audio/librsound.c
                ${RARCH_DIR}/audio/drivers/rsound.c
                ${RARCH_DIR}/audio/drivers/opensl.c
                ${RARCH_DIR}/audio/drivers/nullaudio.c
                ${RARCH_DIR}/gfx/video_driver.c
                ${RARCH_DIR}/gfx/video_coord_array.c
                ${RARCH_DIR}/input/input_driver.c
                ${RARCH_DIR}/audio/audio_driver.c
                ${RARCH_DIR}/libretro-common/audio/audio_mixer.c
                ${RARCH_DIR}/camera/camera_driver.c
                ${RARCH_DIR}/location/location_driver.c
                ${RARCH_DIR}/src/driver.c
                ${RARCH_DIR}/libretro-common/gfx/scaler/scaler_filter.c
                ${RARCH_DIR}/libretro-common/gfx/scaler/pixconv.c
                ${RARCH_DIR}/libretro-common/gfx/scaler/scaler.c
                ${RARCH_DIR}/libretro-common/gfx/scaler/scaler_int.c
                ${RARCH_DIR}/gfx/video_filters/2xsai.c
                ${RARCH_DIR}/gfx/video_filters/super2xsai.c
                ${RARCH_DIR}/gfx/video_filters/supereagle.c
                ${RARCH_DIR}/gfx/video_filters/2xbr.c
                ${RARCH_DIR}/gfx/video_filters/darken.c
                ${RARCH_DIR}/gfx/video_filters/epx.c
                ${RARCH_DIR}/gfx/video_filters/scale2x.c
                ${RARCH_DIR}/gfx/video_filters/blargg_ntsc_snes.c
                ${RARCH_DIR}/gfx/video_filters/lq2x.c
                ${RARCH_DIR}/gfx/video_filters/phosphor2x.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/echo.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/eq.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/chorus.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/iir.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/panning.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/phaser.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/reverb.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filters/wahwah.c
                ${RARCH_DIR}/libretro-common/dynamic/dylib.c
                ${RARCH_DIR}/src/dynamic.c
                ${RARCH_DIR}/gfx/video_filter.c
                ${RARCH_DIR}/libretro-common/audio/dsp_filter.c
                ${RARCH_DIR}/cores/dynamic_dummy.c
                ${RARCH_DIR}/libretro-common/file/file_path.c
                ${RARCH_DIR}/src/file_path_special.c
                ${RARCH_DIR}/src/file_path_str.c
                ${RARCH_DIR}/libretro-common/lists/dir_list.c
                ${RARCH_DIR}/libretro-common/lists/string_list.c
                ${RARCH_DIR}/libretro-common/lists/file_list.c
                ${RARCH_DIR}/src/setting_list.c
                ${RARCH_DIR}/libretro-common/file/retro_dirent.c
                ${RARCH_DIR}/libretro-common/streams/file_stream.c
                ${RARCH_DIR}/libretro-common/streams/interface_stream.c
                ${RARCH_DIR}/libretro-common/streams/memory_stream.c
                ${RARCH_DIR}/libretro-common/file/retro_stat.c
                ${RARCH_DIR}/src/list_special.c
                ${RARCH_DIR}/libretro-common/string/stdstring.c
                ${RARCH_DIR}/libretro-common/file/nbio/nbio_stdio.c
                ${RARCH_DIR}/libretro-common/queues/message_queue.c
                ${RARCH_DIR}/src/configuration.c
                ${RARCH_DIR}/managers/state_manager.c
                ${RARCH_DIR}/frontend/frontend_driver.c
                ${RARCH_DIR}/frontend/drivers/platform_linux.c
                ${RARCH_DIR}/frontend/drivers/platform_null.c
                ${RARCH_DIR}/src/core_info.c
                ${RARCH_DIR}/ui/ui_companion_driver.c
                ${RARCH_DIR}/ui/drivers/ui_null.c
                ${RARCH_DIR}/ui/drivers/null/ui_null_window.c
                ${RARCH_DIR}/ui/drivers/null/ui_null_browser_window.c
                ${RARCH_DIR}/ui/drivers/null/ui_null_msg_window.c
                ${RARCH_DIR}/ui/drivers/null/ui_null_application.c
                ${RARCH_DIR}/frontend/frontend.c
                ${RARCH_DIR}/src/core_impl.c
                ${RARCH_DIR}/src/retroarch.c
                ${RARCH_DIR}/src/dirs.c
                ${RARCH_DIR}/src/paths.c
                ${RARCH_DIR}/libretro-common/queues/task_queue.c
                ${RARCH_DIR}/src/msg_hash.c
                ${RARCH_DIR}/intl/msg_hash_us.c
                ${RARCH_DIR}/wifi/wifi_driver.c
                ${RARCH_DIR}/wifi/drivers/nullwifi.c
                ${RARCH_DIR}/src/movie.c
                ${RARCH_DIR}/record/record_driver.c
                ${RARCH_DIR}/record/drivers/record_null.c
                ${RARCH_DIR}/libretro-common/rthreads/rthreads.c
                ${RARCH_DIR}/gfx/video_thread_wrapper.c
                ${RARCH_DIR}/audio/audio_thread_wrapper.c
                ${RARCH_DIR}/network/netplay/netplay_delta.c
                ${RARCH_DIR}/network/netplay/netplay_frontend.c
                ${RARCH_DIR}/network/netplay/netplay_handshake.c
                ${RARCH_DIR}/network/netplay/netplay_init.c
                ${RARCH_DIR}/network/netplay/netplay_io.c
                ${RARCH_DIR}/network/netplay/netplay_sync.c
                ${RARCH_DIR}/network/netplay/netplay_discovery.c
                ${RARCH_DIR}/network/netplay/netplay_buf.c
                ${RARCH_DIR}/network/netplay/netplay_room_parse.c
                ${RARCH_DIR}/libretro-common/net/net_compat.c
                ${RARCH_DIR}/libretro-common/net/net_socket.c
                ${RARCH_DIR}/libretro-common/net/net_http.c
                ${RARCH_DIR}/libretro-common/net/net_natt.c
                ${RARCH_DIR}/libretro-common/formats/json/jsonsax_full.c
                ${RARCH_DIR}/libretro-common/net/net_ifinfo.c
                ${RARCH_DIR}/tasks/task_http.c
                ${RARCH_DIR}/tasks/task_netplay_lan_scan.c
                ${RARCH_DIR}/tasks/task_netplay_nat_traversal.c
                ${RARCH_DIR}/tasks/task_wifi.c
                ${RARCH_DIR}/tasks/task_netplay_find_content.c
                ${RARCH_DIR}/tasks/task_powerstate.c
                ${RARCH_DIR}/tasks/task_content.c
                ${RARCH_DIR}/tasks/task_save.c
                ${RARCH_DIR}/tasks/task_image.c
                ${RARCH_DIR}/tasks/task_file_transfer.c
                ${RARCH_DIR}/tasks/task_decompress.c
                ${RARCH_DIR}/tasks/task_database.c
                ${RARCH_DIR}/tasks/task_database_cue.c
                ${RARCH_DIR}/tasks/task_screenshot.c
                ${RARCH_DIR}/src/playlist.c
                ${RARCH_DIR}/src/callstack.c
                ${RARCH_DIR}/menu/menu_driver.c
                ${RARCH_DIR}/menu/menu_input.c
                ${RARCH_DIR}/menu/menu_event.c
                ${RARCH_DIR}/menu/menu_entries.c
                ${RARCH_DIR}/menu/menu_setting.c
                ${RARCH_DIR}/menu/menu_cbs.c
                ${RARCH_DIR}/menu/menu_content.c
                ${RARCH_DIR}/menu/widgets/menu_entry.c
                ${RARCH_DIR}/menu/widgets/menu_filebrowser.c
                ${RARCH_DIR}/menu/widgets/menu_dialog.c
                ${RARCH_DIR}/menu/widgets/menu_input_dialog.c
                ${RARCH_DIR}/menu/widgets/menu_input_bind_dialog.c
                ${RARCH_DIR}/menu/widgets/menu_list.c
                ${RARCH_DIR}/menu/widgets/menu_osk.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_ok.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_cancel.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_select.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_start.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_info.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_refresh.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_left.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_right.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_title.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_deferred_push.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_scan.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_get_value.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_label.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_sublabel.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_up.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_down.c
                ${RARCH_DIR}/menu/cbs/menu_cbs_contentlist_switch.c
                ${RARCH_DIR}/menu/menu_shader.c
                ${RARCH_DIR}/menu/menu_displaylist.c
                ${RARCH_DIR}/menu/menu_animation.c
                ${RARCH_DIR}/menu/drivers/null.c
                ${RARCH_DIR}/menu/drivers/menu_generic.c
                ${RARCH_DIR}/menu/drivers_display/menu_display_null.c
                ${RARCH_DIR}/menu/drivers_display/menu_display_gl.c
                ${RARCH_DIR}/menu/drivers_display/menu_display_vulkan.c
                ${RARCH_DIR}/menu/drivers/rgui.c
                ${RARCH_DIR}/menu/drivers/xmb.c
                ${RARCH_DIR}/menu/drivers/materialui.c
                ${RARCH_DIR}/input/input_remote.c
                ${RARCH_DIR}/cores/libretro-net-retropad/net_retropad_core.c
                ${RARCH_DIR}/src/command.c
                ${RARCH_DIR}/libretro-common/net/net_http_parse.c
                ${RARCH_DIR}/deps/7zip/7zIn.c
                ${RARCH_DIR}/deps/7zip/7zAlloc.c
                ${RARCH_DIR}/deps/7zip/Bra86.c
                ${RARCH_DIR}/deps/7zip/7zFile.c
                ${RARCH_DIR}/deps/7zip/7zStream.c
                ${RARCH_DIR}/deps/7zip/7zBuf2.c
                ${RARCH_DIR}/deps/7zip/LzmaDec.c
                ${RARCH_DIR}/deps/7zip/7zCrcOpt.c
                ${RARCH_DIR}/deps/7zip/Bra.c
                ${RARCH_DIR}/deps/7zip/7zDec.c
                ${RARCH_DIR}/deps/7zip/Bcj2.c
                ${RARCH_DIR}/deps/7zip/7zCrc.c
                ${RARCH_DIR}/deps/7zip/Lzma2Dec.c
                ${RARCH_DIR}/deps/7zip/7zBuf.c
                ${RARCH_DIR}/libretro-common/audio/conversion/s16_to_float.c
                ${RARCH_DIR}/libretro-common/audio/conversion/float_to_s16.c
                ${RARCH_DIR}/libretro-common/audio/audio_mix.c
                ${RARCH_DIR}/libretro-db/bintree.c
                ${RARCH_DIR}/libretro-db/libretrodb.c
                ${RARCH_DIR}/libretro-db/rmsgpack.c
                ${RARCH_DIR}/libretro-db/rmsgpack_dom.c
                ${RARCH_DIR}/libretro-db/query.c
                ${RARCH_DIR}/src/database_info.c
                ${RARCH_DIR}/deps/miniupnpc/igd_desc_parse.c
                ${RARCH_DIR}/deps/miniupnpc/upnpreplyparse.c
                ${RARCH_DIR}/deps/miniupnpc/upnpcommands.c
                ${RARCH_DIR}/deps/miniupnpc/upnperrors.c
                ${RARCH_DIR}/deps/miniupnpc/connecthostport.c
                ${RARCH_DIR}/deps/miniupnpc/portlistingparse.c
                ${RARCH_DIR}/deps/miniupnpc/receivedata.c
                ${RARCH_DIR}/deps/miniupnpc/upnpdev.c
                ${RARCH_DIR}/deps/miniupnpc/minissdpc.c
                ${RARCH_DIR}/deps/miniupnpc/miniwget.c
                ${RARCH_DIR}/deps/miniupnpc/miniupnpc.c
                ${RARCH_DIR}/deps/miniupnpc/minixml.c
                ${RARCH_DIR}/deps/miniupnpc/minisoap.c
            ${RARCH_DIR}/src/CallStackPP.cpp
            ${RARCH_DIR}/gfx/drivers_shader/shader_vulkan.cpp
            ${RARCH_DIR}/gfx/drivers_shader/glslang_util.cpp
            ${RARCH_DIR}/gfx/drivers_shader/slang_reflection.cpp
            ${RARCH_DIR}/deps/SPIRV-Cross/spirv_cross.cpp
            ${RARCH_DIR}/deps/SPIRV-Cross/spirv_cfg.cpp
            ${DEPS_DIR}/glslang/glslang.cpp
            ${DEPS_DIR}/glslang/glslang/SPIRV/SpvBuilder.cpp
            ${DEPS_DIR}/glslang/glslang/SPIRV/Logger.cpp
            ${DEPS_DIR}/glslang/glslang/SPIRV/SPVRemapper.cpp
            ${DEPS_DIR}/glslang/glslang/SPIRV/InReadableOrder.cpp
            ${DEPS_DIR}/glslang/glslang/SPIRV/doc.cpp
            ${DEPS_DIR}/glslang/glslang/SPIRV/GlslangToSpv.cpp
            ${DEPS_DIR}/glslang/glslang/SPIRV/disassemble.cpp
            ${DEPS_DIR}/glslang/glslang/OGLCompilersDLL/InitializeDll.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/GenericCodeGen/Link.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/GenericCodeGen/CodeGen.cpp
            ${DEPS_DIR}/glslang/glslang/hlsl/hlslAttributes.cpp
            ${DEPS_DIR}/glslang/glslang/hlsl/hlslGrammar.cpp
            ${DEPS_DIR}/glslang/glslang/hlsl/hlslOpMap.cpp
            ${DEPS_DIR}/glslang/glslang/hlsl/hlslTokenStream.cpp
            ${DEPS_DIR}/glslang/glslang/hlsl/hlslScanContext.cpp
            ${DEPS_DIR}/glslang/glslang/hlsl/hlslParseHelper.cpp
            ${DEPS_DIR}/glslang/glslang/hlsl/hlslParseables.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/Intermediate.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/propagateNoContraction.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/glslang_tab.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/Versions.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/RemoveTree.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/limits.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/intermOut.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/Initialize.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/SymbolTable.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/parseConst.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/ParseContextBase.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/ParseHelper.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/ShaderLang.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/IntermTraverse.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/iomapper.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/InfoSink.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/Constant.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/Scan.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/reflection.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/linkValidate.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/PoolAlloc.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/preprocessor/PpMemory.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/preprocessor/PpSymbols.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/preprocessor/Pp.cpp
            ${DEPS_DIR}/glslang/glslang/glslang/OSDependent/Unix/ossource.cpp
            )

target_include_directories(retroarch-activity PRIVATE
                           ${CMAKE_SOURCE_DIR}/
                           ${RARCH_DIR}/
                           ${RARCH_DIR}/src
                           ${RARCH_DIR}/libretro-common/include/
                           ${RARCH_DIR}/gfx/include/
                           ${RARCH_DIR}/deps/
                           ${RARCH_DIR}/frontend/
                           ${RARCH_DIR}/input/
                           ${DEPS_DIR}/glslang/
                           ${DEPS_DIR}/glslang/glslang/glslang/Public/
                           ${DEPS_DIR}/glslang/glslang/glslang/MachineIndependent/
                           ${DEPS_DIR}/glslang/glslang/SPIRV/
                           ${DEPS_DIR}/spir2cross/
                           ${DEPS_DIR}/SPIRV-Cross/
                           )

target_link_libraries(retroarch-activity
                      log
                      android
                      EGL
                      GLESv3
                      OpenSLES
                      z)


