//
//  ApplicationController.cpp
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/05/23.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#include "ApplicationController.h"
#include "util/Log.h"

namespace GCube {

ApplicationController* ApplicationController::SharedInstance() {
	LOGD("ApplicationController::SharedInstance()");
	static ApplicationController *_instance = NULL;
	if (!_instance) _instance = new ApplicationController();
	return _instance;
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

void ApplicationController::onSizeChanged(float width, float height, int orientation) {
	LOGD("ApplicationController::onSizeChanged(%f, %f, %d)", width, height, orientation);
}

void ApplicationController::onLowMemory(void) {
	LOGD("ApplicationController::onLowMemory()");
}

void ApplicationController::onUpdate(float dt) {
//	LOGD("ApplicationController::onUpdate(%f)", dt);
}

void ApplicationController::onDraw() {
//	LOGD("ApplicationController::onDraw()");
}

}
