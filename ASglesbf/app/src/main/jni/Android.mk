# Copyright 2013 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libgles3jni
LOCAL_CFLAGS    := -Werror -DDYNAMIC_ES3
LOCAL_SRC_FILES := gles3jni.cpp \
				   gl3stub.c \
				   RendererES2.cpp \
				   RendererES3.cpp
LOCAL_LDLIBS    := -llog -lGLESv3 -lEGL

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE :=mdklog
LOCAL_SRC_FILES := libmdklog.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE :=mojing_gles3
LOCAL_SRC_FILES := libmojing_gles3.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := MGD
LOCAL_SRC_FILES := libMGD.so
include $(PREBUILT_SHARED_LIBRARY)

#include $(LOCAL_PATH)/../android-non-root/MGD.mk

