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

using namespace GCube;

static ApplicationController *controller = NULL;

extern "C" {

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
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
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
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
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
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
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
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
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
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
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

};	// end of extern "C"


