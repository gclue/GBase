LOCAL_PATH := $(call my-dir)/../../../../engine
include $(CLEAR_VARS)

LOCAL_MODULE    := common
LOCAL_SRC_FILES := ApplicationController.cpp \
                   external/stb/stb_image.c

LOCAL_CFLAGS    := -Werror -D__GCube_Android__                   
LOCAL_LDLIBS    := -llog -lGLESv2

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/ \
                   $(LOCAL_PATH)/util \
                   $(LOCAL_PATH)/external/stb/ \
				   $(LOCAL_PATH)/../main/

include $(BUILD_STATIC_LIBRARY)
