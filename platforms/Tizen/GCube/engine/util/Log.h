/*
 * The MIT License (MIT)
 * Copyright (c) 2013 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __GCube__Log__
#define __GCube__Log__

#include "GCubeConfig.h"


// Android
#ifdef __GCube_Android__

	#ifdef __GCube_LogOut__
		#include <android/log.h>
		#define LOG_TAG "GCube"
		#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
		#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
		#define LOGW(...)  __android_log_pring(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
		#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
	#endif

#endif

// iOS
#ifdef __GCube_iOS__

	#ifdef __GCube_LogOut__
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

	#ifdef __GCube_LogOut__
		#include <FBase.h>

		#define LOGI(...) AppLog(__VA_ARGS__)
		#define LOGD(...) AppLogDebug(__VA_ARGS__)
		#define LOGW(...) AppLog(__VA_ARGS__)
		#define LOGE(...) AppLogException(__VA_ARGS__)
	#endif

#endif



#ifndef __GCube_LogOut__
	#define LOGI(...)
	#define LOGD(...)
	#define LOGW(...)
	#define LOGE(...)
#endif

#endif /* defined(__GCube__Log__) */
