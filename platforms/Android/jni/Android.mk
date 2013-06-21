MY_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(MY_PATH)/mk/lib.mk

LOCAL_PATH := $(MY_PATH)/../../..
include $(CLEAR_VARS)

ENGINE_PATH := $(LOCAL_PATH)/engine
MAIN_PATH := $(LOCAL_PATH)/main

APP_MODULES     := gcube
LOCAL_MODULE    := gcube
LOCAL_CFLAGS    := -Werror -D__GCube_Android__
LOCAL_LDLIBS    := -llog -lGLESv2 -lz -landroid

LOCAL_SRC_FILES := platforms/Android/jni/android/ndk-interface.cpp \
                   main/Main.cpp \
                   main/glsample.cpp
                   
LOCAL_C_INCLUDES:= $(MY_PATH)/android/ \
                   $(ENGINE_PATH)/ \
                   $(ENGINE_PATH)/external/stb/ \
                   $(MAIN_PATH)
                   
LOCAL_STATIC_LIBRARIES := common

include $(BUILD_SHARED_LIBRARY)

