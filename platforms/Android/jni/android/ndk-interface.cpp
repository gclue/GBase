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

#include <stddef.h>
#include <jni.h>
#include <GCube.h>
#include "GCDefines.h"

using namespace GCube;

static ApplicationController *controller = NULL;

/**
 * JNIを操作する為のデータを保持する構造体.
 */
struct JNIInterface {
	JNIEnv *env;					//!< Java環境へのポインタ
	jobject obj;					//!< NDKInterfae.javaのインスタンス

	jmethodID onGameEventMethod;
	jmethodID getStringInfoMthod;
};

/**
 * JNIインターフェイス.
 */
static JNIInterface jni = {0};


/**
 * 文字列をキーに文字列情報を取得.
 */
std::string GCGetStringInfo(const char *key) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring strKey = env->NewStringUTF(key);
		jstring strRet = (jstring)env->CallObjectMethod(jni.obj, jni.getStringInfoMthod, strKey);
		env->DeleteLocalRef(strKey);
		std::string lang = std::string(env->GetStringUTFChars(strRet, NULL));
		env->DeleteLocalRef(strRet);
		return lang;
	}
	return NULL;
}

/**
 * 言語設定を取得.
 */
std::string GCGetLanguage() {
	return GCGetStringInfo("lang");
}

extern "C" {

/**
 * ネイティブからJavaのメソッドを実行するためのインターフェースをセットします.
 *
 * @param env 環境変数.
 * @param thiz thisポインタ
 * @param ndk インターフェースクラスのオブジェクト.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_setInterface(
		JNIEnv *env, jobject thiz, jobject ndk) {
	LOGD("Java_com_gclue_gcube_setInterface");

	// インターフェースクラスのロード.
	jclass clazz = env->FindClass("com/gclue/gcube/NDKInterface");

	// クラスに環境変数を格納します.
	jni.env = env;
	jni.obj = env->NewGlobalRef(ndk);

	// 各メソッドを取得
	jni.onGameEventMethod = env->GetMethodID(clazz, "onGameEvent", "(IIJFDLjava/lang/String;)V");
	if (!jni.onGameEventMethod) {
		LOGE("Mehtod not found!! (onGameEventMethod)");
	}
	jni.getStringInfoMthod = env->GetMethodID(clazz, "getStringInfo", "(Ljava/lang/String;)Ljava/lang/String;");
	if (!jni.getStringInfoMthod) {
		LOGE("Mehtod not found!! (getStringInfoMthod)");
	}

	env->DeleteLocalRef(clazz);
}

/**
 * 初期化関数.
 *
 * @param env 環境変数
 * @param obj オブジェクト変数
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_onInit(
		JNIEnv * env, jobject obj)
{
	//LOGD("Java_com_gclue_gcube_JNDKInterface_onInit");
	// メインクラスの作成します.
	if (controller) {
		controller->onContextChanged();
	} else {
		controller = ApplicationController::SharedInstance();
		controller->onInit();
	}
}

/**
 * サイズ変更処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_step(
		JNIEnv * env, jobject obj, jfloat dt)
{
	if (controller) {
		controller->onUpdate(dt);
		controller->onDraw();
	}
}

/**
 * サイズ変更処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_onSizeChanged(
		JNIEnv * env, jobject obj, jint width, jint height, jint orientation)
{
	//LOGD("Java_com_gclue_gcube_JNDKInterface_onSizeChanged(%d, %d, %d)", width, height, orientation);
	if (controller) {
		controller->onSizeChanged(width, height, (GCDeviceOrientation)orientation);
	}
}

/**
 * 一時停止処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_onPause(
		JNIEnv* env, jobject thiz)
{
	if (controller) {
		controller->onPause();
	}
}

/**
 * 再開処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_onResume(
		JNIEnv* env, jobject thiz)
{
	if (controller) {
		controller->onResume();
	}
}

/**
 * 終了処理をします.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_onTerminate(
		JNIEnv * env, jobject obj)
{
	if (controller) {
		controller->onTerminate();
		ApplicationController::DestroyInstance();
		controller = NULL;
	}
}

/**
 * タッチイベント処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_onTouchEvent(
		JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint id, jlong time)
{
	if (controller) {
		controller->onTouch((GCTouchAction)action, x, y, id, time);
	}
}

/**
 * 傾きセンサーイベント処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_onOrientationChanged(
		JNIEnv * env, jobject obj, jfloat yaw, jfloat pitch, jfloat roll)
{
	if (controller) {
		controller->onOrientationChanged(yaw, pitch, roll);
	}
}

/**
 * フレームレートを返します.
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gcube_NDKInterface_getFrameRate(
		JNIEnv * env, jobject obj)
{
	return __GCube_FrameRate__;
}

/**
 * 傾きセンサーを使用するかを返します.
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gcube_NDKInterface_useOrientationSensor(
		JNIEnv * env, jobject obj)
{
#ifdef __GCube_OrientationSensor__
	return true;
#else
	return false;
#endif
}

/**
 * 対応画面方向を返します.
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gcube_NDKInterface_getSupportedOrientation(
		JNIEnv * env, jobject obj)
{
#ifdef __GCube_SupportedOrientation_Portrait__
	#if defined(__GCube_SupportedOrientation_LandscapeLeft__) || defined(__GCube_SupportedOrientation_LandscapeRight__)
		return 4; // SCREEN_ORIENTATION_SENSOR
	#else
		#ifdef __GCube_SupportedOrientation_PortraitUpsideDown__
			return 7; // SCREEN_ORIENTATION_SENSOR_PORTRAIT
		#else
			return 1; // SCREEN_ORIENTATION_PORTRAIT
		#endif
	#endif
#else
	#ifdef __GCube_SupportedOrientation_PortraitUpsideDown__
		#if defined(__GCube_SupportedOrientation_LandscapeLeft__) || defined(__GCube_SupportedOrientation_LandscapeRight__)
			return 4; // SCREEN_ORIENTATION_SENSOR
		#else
			return 9; // SCREEN_ORIENTATION_REVERSE_PORTRAIT
		#endif
	#else
		#ifdef __GCube_SupportedOrientation_LandscapeLeft__
			#ifdef __GCube_SupportedOrientation_LandscapeRight__
				return 6; // SCREEN_ORIENTATION_SENSOR_LANDSCAPE
			#else
				return 8; // SCREEN_ORIENTATION_REVERSE_LANDSCAPE
			#endif
		#else
			#ifdef __GCube_SupportedOrientation_LandscapeRight__
				return 0; // SCREEN_ORIENTATION_LANDSCAPE
			#endif
		#endif
	#endif
#endif

	return -1; // SCREEN_ORIENTATION_UNSPECIFIED
}

};	// end of extern "C"


