MY_PATH := $(call my-dir)
include $(CLEAR_VARS)

#include $(MY_PATH)/mk/openal.mk
#include $(CLEAR_VARS)

include $(MY_PATH)/mk/lib.mk

LOCAL_PATH := $(MY_PATH)/../../..
include $(CLEAR_VARS)

ENGINE_PATH := $(LOCAL_PATH)/engine
MAIN_PATH := $(LOCAL_PATH)/main

APP_MODULES     := gcube_app
LOCAL_MODULE    := gcube_app
LOCAL_CFLAGS    := -Werror -D__GCube_Android__
#LOCAL_LDLIBS    := -llog -lGLESv2 -lz -landroid -L$(MY_PATH)/libs -lgcube -lopenal
LOCAL_LDLIBS    := -llog -lGLESv2 -lz -landroid -L$(MY_PATH)/libs -lopenal

LOCAL_SRC_FILES := main/Main.cpp \
                   main/glsample.cpp
                   
LOCAL_C_INCLUDES:= $(MY_PATH)/android/ \
                   $(MY_PATH)/OpenAL/ \
                   $(MY_PATH)/OpenAL/include \
                   $(MY_PATH)/OpenAL/OpenAL32/Include \
                   $(ENGINE_PATH)/ \
                   $(ENGINE_PATH)/sound \
                   $(ENGINE_PATH)/util \
                   $(ENGINE_PATH)/external/stb/ \
                   $(MAIN_PATH)
                   
LOCAL_STATIC_LIBRARIES := openal gcube

include $(BUILD_SHARED_LIBRARY)

