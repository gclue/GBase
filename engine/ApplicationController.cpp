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
#include "glsample.h"

namespace GCube {

static ApplicationController *_instance = NULL;

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
}

ApplicationController::~ApplicationController(void) {
	LOGD("ApplicationController::~ApplicationController()");
}

void ApplicationController::onInit(void) {
	LOGD("ApplicationController::onInit()");
	
}

void ApplicationController::onTerminate(void) {
	LOGD("ApplicationController::onTerminate()");
}

void ApplicationController::onPause(void) {
	LOGD("ApplicationController::onPause()");
}

void ApplicationController::onResume(void) {
	LOGD("ApplicationController::onResume()");
}

void ApplicationController::onContextChanged(void) {
	LOGD("ApplicationController::onContextChanged()");
}

void ApplicationController::onSizeChanged(float width, float height, GCDeviceOrientation orientation) {
	LOGD("ApplicationController::onSizeChanged(%f, %f, %d)", width, height, orientation);
	glViewport(0, 0, width, height);
}

void ApplicationController::onLowMemory(void) {
	LOGD("ApplicationController::onLowMemory()");
}

void ApplicationController::onUpdate(float dt) {
//	LOGD("ApplicationController::onUpdate(%f)", dt);
}

void ApplicationController::onDraw() {
	draw();
//	LOGD("ApplicationController::onDraw()");
}

}
