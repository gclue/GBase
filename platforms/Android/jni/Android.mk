MY_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(MY_PATH)/mk/lib.mk

LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

ENGINE_PATH := $(LOCAL_PATH)/../../../engine
MAIN_PATH := $(LOCAL_PATH)/../../../main

APP_MODULES     := gcube
LOCAL_MODULE    := gcube
LOCAL_CFLAGS    := -Werror -D__GCube_Android__
LOCAL_SRC_FILES := android/ndk-interface.cpp
                   
LOCAL_LDLIBS    := -llog -lGLESv2 -lz
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/android/ \
                   $(ENGINE_PATH)/ \
                   $(MAIN_PATH)
                   
LOCAL_STATIC_LIBRARIES := common

include $(BUILD_SHARED_LIBRARY)
