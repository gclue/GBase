//
//  Log.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/30.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef __GCube__Log__
#define __GCube__Log__

#include <GCubeConfig.h>


// Android
#ifdef __GCube_Android__

	#include <android/log.h>

	#ifdef __GCube_Log__
		#define LOG_TAG "GCube"
		#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
		#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
		#define LOGW(...)  __android_log_pring(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
		#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
	#endif

#endif

// iOS
#ifdef __GCube_iOS__

	#ifdef __GCube_Log__
		#define LOGI(...) _log("I", __VA_ARGS__)
		#define LOGD(...) _log("D", __VA_ARGS__)
		#define LOGW(...) _log("W", __VA_ARGS__)
		#define LOGE(...) _log("E", __VA_ARGS__)

		#include <stdio.h>
		#include <stdlib.h>
		#include <stdarg.h>
		#include <sys/time.h>
		inline void _log(const char *tag, const char *message, ...) {
			struct timeval t;
			gettimeofday(&t, NULL);
			
			time_t timer;
			struct tm *date;
			timer = time(NULL);
			date = localtime(&timer);
			
			int h = date->tm_hour;
			int m = date->tm_min;
			int s = date->tm_sec;
			int ss = t.tv_usec / 100;
			
			va_list arg;
			va_start(arg, message);
			char str[256];
			vsnprintf(str, 256, message, arg);
			printf("%02d:%02d:%02d:%04d [%s] %s\n", h, m, s, ss, tag, str);
			va_end(arg);
		}
	#endif

#endif

// Tizen
#ifdef __GCube_Tizen__

	#ifdef __GCube_Log__
		#include <FBase.h>

		#define LOGI(...) AppLog(__VA_ARGS__)
		#define LOGD(...) AppLogDebug(__VA_ARGS__)
		#define LOGW(...) AppLog(__VA_ARGS__)
		#define LOGE(...) AppLogException(__VA_ARGS__)
	#endif

#endif



#ifndef __GCube_Log__
	#define LOGI(...)
	#define LOGD(...)
	#define LOGW(...)
	#define LOGE(...)
#endif

#endif /* defined(__GCube__Log__) */
