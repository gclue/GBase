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

#include "ApplicationController.h"
#include "GCube.h"
#include "Main.h"

namespace GCube {

static ApplicationController *_instance = NULL;

/////////////////////////////////////////////////////////////////////////
#pragma mark - Life cycle
	
ApplicationController* ApplicationController::SharedInstance() {
//	LOGD("ApplicationController::SharedInstance()");
	if (!_instance) _instance = new ApplicationController();
	return _instance;
}

void ApplicationController::DestroyInstance() {
	LOGD("ApplicationController::DestroyInstance()");
	delete _instance;
	_instance = NULL;
}

ApplicationController::ApplicationController(void) {
	LOGD("ApplicationController::ApplicationController()");
	main = new Main();
}

ApplicationController::~ApplicationController(void) {
	LOGD("ApplicationController::~ApplicationController()");
	delete main;
}


/////////////////////////////////////////////////////////////////////////
#pragma mark - Public method

// 言語取得
std::string ApplicationController::getLanguage() {
	 return GCGetLanguage();
}

// リソース取得
void ApplicationController::getResource(const char *fileName, std::vector<char>& outData) {
	GCGetResourceData(fileName, outData);
}

// ストレージパスを取得
std::string ApplicationController::getStoragePath(GCStorageType type) {
	return GCGetStoragePath(type);
}

// ユーザーイベント
int ApplicationController::sendUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	LOGD("sendUserEvent(t: %d, 1: %d, 2: %lld, 3:%f, 4:%f, 5:%s)", type, param1, param2, param3, param4, param5);
	return GCSendUserEvent(type, param1, param2, param3, param4, param5);
}


/////////////////////////////////////////////////////////////////////////
#pragma mark - IApplicationEventListener

// 初期化
void ApplicationController::onInit(void) {
	LOGD("ApplicationController::onInit()");
	main->onInit();
}

void ApplicationController::onTerminate(void) {
	LOGD("ApplicationController::onTerminate()");
	main->onTerminate();
}

void ApplicationController::onPause(void) {
	LOGD("ApplicationController::onPause()");
	main->onPause();
}

void ApplicationController::onResume(void) {
	LOGD("ApplicationController::onResume()");
	main->onResume();
}

void ApplicationController::onContextChanged(void) {
	LOGD("ApplicationController::onContextChanged()");
	main->onContextChanged();
}

void ApplicationController::onSizeChanged(float width, float height, GCDeviceOrientation orientation) {
	LOGD("ApplicationController::onSizeChanged(%f, %f, %d)", width, height, orientation);
	main->onSizeChanged(width, height, orientation);
}

void ApplicationController::onLowMemory(void) {
	LOGD("ApplicationController::onLowMemory()");
	main->onLowMemory();
}

void ApplicationController::onUpdate(float dt) {
//	LOGD("ApplicationController::onUpdate(%f)", dt);
	main->onUpdate(dt);
}

void ApplicationController::onDraw() {
//	LOGD("ApplicationController::onDraw()");
	main->onDraw();
}
	
// タッチイベント
void ApplicationController::onTouch(GCTouchAction action, float x, float y, long id, long time) {
	LOGD("***********onTouch[%d](%f,%f)[%d] %u", action, x, y, id, time);
	main->onTouch(action, x, y, id, time);
}

// 傾きセンサイベント
void ApplicationController::onOrientationChanged(float yaw, float pitch, float roll) {
	LOGD("***********onOrientationChanged(%f, %f, %f)", RADIANS_TO_DEGREES(yaw), RADIANS_TO_DEGREES(pitch), RADIANS_TO_DEGREES(roll));
	main->onOrientationChanged(yaw, pitch, roll);
}

// ユーザイベント
int ApplicationController::onUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	//LOGD("onUserEvent(t: %d, 1: %d, 2: %lld, 3:%f, 4:%f, 5:%s)", type, param1, param2, param3, param4, param5);
	return main->onUserEvent(type, param1, param2, param3, param4, param5);
}

}
