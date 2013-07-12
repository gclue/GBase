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
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GCube.h>

using namespace GCube;

static ApplicationController *controller = NULL;
static GCubeSettings *settings = new GCubeSettings();

/**
 * JNIを操作する為のデータを保持する構造体.
 */
struct JNIInterface {
	JNIEnv *env;					//!< Java環境へのポインタ
	jobject obj;					//!< NDKInterfae.javaのインスタンス
	jobject assetManager;			//!< AssetManagerのインスタンス

	jmethodID onUserEventMethod;
	jmethodID getStringInfoMthod;
};

/**
 * JNIインターフェイス.
 */
static JNIInterface jni = {0};


/**
 * 文字列をキーに文字列情報を取得.
 */
std::string GCGetStringInfo(const char *key, const char *opt) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring strKey = env->NewStringUTF(key);
		jstring strOpt = opt?env->NewStringUTF(opt):NULL;
		jstring strRet = (jstring)env->CallObjectMethod(jni.obj, jni.getStringInfoMthod, strKey, strOpt);
		env->DeleteLocalRef(strKey);
		env->DeleteLocalRef(strOpt);
		std::string ret = std::string(env->GetStringUTFChars(strRet, NULL));
		env->DeleteLocalRef(strRet);
		return ret;
	}
	return NULL;
}

/**
 * 言語設定を取得.
 */
std::string GCGetLanguage() {
	return GCGetStringInfo("Lang", NULL);
}

/**
 * リソースを取得.
 */
void GCGetResourceData(const char *fileName, std::vector<char>& outData){
	AAssetManager* mgr = AAssetManager_fromJava(jni.env, jni.assetManager);
	AAsset* asset = AAssetManager_open(mgr, fileName, AASSET_MODE_BUFFER);
	const char* assetData = (const char*)AAsset_getBuffer(asset);
	const off_t assetLen = AAsset_getLength(asset);
	outData.assign(assetData, assetData+assetLen);
}

/**
 * ストレージパスを取得.
 */
std::string GCGetStoragePath(GCStorageType type) {
	switch (type) {
		case GCStorageTypeDocument:
			return GCGetStringInfo("StoragePath", "Document");
			break;
		case GCStorageTypeCache:
			return GCGetStringInfo("StoragePath", "Cache");
			break;
	}
	return NULL;
}


/**
 * ユーザーイベントをJava側に送信.
 */
int GCSendUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	JNIEnv* env = jni.env;
	jint ret = -1;
	if (env) {
		jstring str = NULL;
		if (param5)	str = env->NewStringUTF(param5);
		ret = env->CallIntMethod(jni.obj, jni.onUserEventMethod, type, param1, param2, param3, param4, str);
		if (str) env->DeleteLocalRef(str);
	}
	return ret;
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
		JNIEnv *env, jobject thiz, jobject ndk, jobject assetManager) {
	LOGD("Java_com_gclue_gcube_setInterface");

	// インターフェースクラスのロード.
	jclass clazz = env->FindClass("com/gclue/gcube/NDKInterface");

	// クラスに環境変数を格納します.
	jni.env = env;
	jni.obj = env->NewGlobalRef(ndk);
	jni.assetManager = env->NewGlobalRef(assetManager);

	// 各メソッドを取得
	jni.onUserEventMethod = env->GetMethodID(clazz, "onUserEvent", "(IIJFDLjava/lang/String;)I");
	if (!jni.onUserEventMethod) {
		LOGE("Mehtod not found!! (onUserEventMethod)");
	}
	jni.getStringInfoMthod = env->GetMethodID(clazz, "getStringInfo", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
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
	return settings->frameRate;
}

/**
 * 傾きセンサーを使用するかを返します.
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gcube_NDKInterface_useOrientationSensor(
		JNIEnv * env, jobject obj)
{
	return settings->useOrientationSensor;
}

/**
 * デバッグコンソールを使用するかを返します.
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gcube_NDKInterface_useDebugConsole (
		JNIEnv * env, jobject obj)
{
	return settings->debugButtonPos > 0;
}

/**
 * ステータスバーを表示するかを返します.
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gcube_NDKInterface_showStatusBar (
		JNIEnv * env, jobject obj)
{
	return settings->showStatusBar;
}

/**
 * 対応画面方向を返します.
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gcube_NDKInterface_getSupportedOrientation(
		JNIEnv * env, jobject obj)
{
	if (settings->orientationPortrait) {
		if (settings->orientationLandscapeLeft || settings->orientationLandscapeRight) {
			return 4; // SCREEN_ORIENTATION_SENSOR
		} else {
			if (settings->orientationPortraitUpsideDown) {
				return 7; // SCREEN_ORIENTATION_SENSOR_PORTRAIT
			} else {
				return 1; // SCREEN_ORIENTATION_PORTRAIT
			}
		}
	} else {
		if (settings->orientationPortraitUpsideDown) {
			if (settings->orientationLandscapeLeft || settings->orientationLandscapeRight) {
				return 4; // SCREEN_ORIENTATION_SENSOR
			} else {
				return 8; // SCREEN_ORIENTATION_REVERSE_LANDSCAPE
			}
		} else {
			if (settings->orientationLandscapeLeft) {
				if (settings->orientationLandscapeRight) {
					return 6; // SCREEN_ORIENTATION_SENSOR_LANDSCAPE
				} else {
					return 8; // SCREEN_ORIENTATION_REVERSE_LANDSCAPE
				}
			} else {
				if (settings->orientationLandscapeRight) {
					return 0; // SCREEN_ORIENTATION_LANDSCAPE
				}
			}
		}
	}

	return -1; // SCREEN_ORIENTATION_UNSPECIFIED
}

/**
 * ユーザーイベント処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_sendUserEvent (
		JNIEnv * env, jobject obj, jint type, jint param1, jlong param2, jfloat param3, jdouble param4, jstring param5)
{
	if (controller) {
		const char* str = env->GetStringUTFChars(param5, NULL);
		controller->onUserEvent(type, param1, param2, param3, param4, str);
		if (str) {
			env->ReleaseStringUTFChars(param5, str);
		}
	}
}

/**
 * デバッグコマンド処理を行います.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gcube_NDKInterface_sendDebugCommand (
		JNIEnv * env, jobject obj, jstring command, jint param)
{
	if (controller) {
		const char* str = env->GetStringUTFChars(command, NULL);
		controller->onDebugCommand(str, param);
		if (str) {
			env->ReleaseStringUTFChars(command, str);
		}
	}
}

};	// end of extern "C"


