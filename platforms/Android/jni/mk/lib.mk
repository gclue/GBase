MY_PATH := $(call my-dir)/../
LOCAL_PATH := $(call my-dir)/../../../../engine
include $(CLEAR_VARS)

LOCAL_MODULE    := gcube
LOCAL_SRC_FILES := ApplicationController.cpp \
                   sound/SoundData.cpp \
                   sound/SoundPlayer.cpp \
                   external/stb/stb_image.c \
                   external/stb/stb_vorbis.c

LOCAL_CFLAGS    := -Werror -D__GCube_Android__                   
LOCAL_LDLIBS    := -llog -lGLESv2

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/ \
                   $(MY_PATH)/OpenAL/ \
                   $(MY_PATH)/OpenAL/include \
                   $(MY_PATH)/OpenAL/OpenAL32/Include \
                   $(LOCAL_PATH)/sound \
                   $(LOCAL_PATH)/util \
                   $(LOCAL_PATH)/external/stb/ \
				   $(LOCAL_PATH)/../main/

LOCAL_STATIC_LIBRARIES := openal

include $(BUILD_STATIC_LIBRARY)
