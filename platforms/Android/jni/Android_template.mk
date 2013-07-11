MY_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_PATH := $(MY_PATH)/../../..
include $(CLEAR_VARS)

MAIN_PATH := $(LOCAL_PATH)/main

APP_MODULES     := gcube_app
LOCAL_MODULE    := gcube_app
LOCAL_CFLAGS    := -Werror -D__GCube_Android__
LOCAL_LDLIBS    := -llog -lGLESv2 -lz -landroid -L$(MY_PATH)/libs -lgcube -lopenal

LOCAL_SRC_FILES := main/Main.cpp \
                   main/glsample.cpp
                   
LOCAL_C_INCLUDES:= $(MY_PATH)/android/ \
                   $(MY_PATH)/OpenAL/ \
                   $(MY_PATH)/OpenAL/include \
                   $(MY_PATH)/OpenAL/OpenAL32/Include \
                   $(MY_PATH)/Headers/ \
                   $(MY_PATH)/Headers/sound \
                   $(MY_PATH)/Headers/util \
                   $(MY_PATH)/Headers/external/stb/ \
                   $(MAIN_PATH)
                   
LOCAL_STATIC_LIBRARIES := openal gcube

include $(BUILD_SHARED_LIBRARY)

