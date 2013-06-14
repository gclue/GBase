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


#include "GCubeApp.h"
#include "GCubeFrame.h"

using namespace GCube;
using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

GCubeApp::GCubeApp(void)
{
}

GCubeApp::~GCubeApp(void)
{
}

UiApp*
GCubeApp::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new GCubeApp();
}

bool
GCubeApp::OnAppInitializing(AppRegistry& appRegistry)
{
	gcube = ApplicationController::SharedInstance();

	return true;
}

bool
GCubeApp::OnAppInitialized(void)
{
	// Create a Frame
	GCubeFrame* pGCubeFrame = new GCubeFrame();
	pGCubeFrame->Construct();
	pGCubeFrame->SetName(L"GCube");
	pGCubeFrame->AddOrientationEventListener(*this);
	pGCubeFrame->AddTouchEventListener(*this);

	// 画面の向き
	enum Orientation orientation = ORIENTATION_NONE;
#ifdef __GCube_SupportedOrientation_Portrait__
	#if defined(__GCube_SupportedOrientation_LandscapeLeft__) || defined(__GCube_SupportedOrientation_LandscapeRight__)
		orientation = ORIENTATION_AUTOMATIC;
	#else
		orientation = ORIENTATION_PORTRAIT;
	#endif
#else
	#ifdef __GCube_SupportedOrientation_PortraitUpsideDown__
		#if defined(__GCube_SupportedOrientation_LandscapeLeft__) || defined(__GCube_SupportedOrientation_LandscapeRight__)
			orientation = ORIENTATION_AUTOMATIC_FOUR_DIRECTION;
		#else
			orientation = ORIENTATION_PORTRAIT_REVERSE;
		#endif
	#else
		#ifdef __GCube_SupportedOrientation_LandscapeLeft__
			orientation = ORIENTATION_LANDSCAPE_REVERSE;
		#else
			#ifdef __GCube_SupportedOrientation_LandscapeRight__
				orientation = ORIENTATION_LANDSCAPE;
			#endif
		#endif
	#endif
#endif
	pGCubeFrame->SetOrientation(orientation);
	pGCubeFrame->SetMultipointTouchEnabled(true);
	AddFrame(*pGCubeFrame);

	{
		__player = new Tizen::Graphics::Opengl::GlPlayer;
		__player->Construct(Tizen::Graphics::Opengl::EGL_CONTEXT_CLIENT_VERSION_2_X, pGCubeFrame);

		__player->SetFps(__GCube_FrameRate__);
		__player->SetEglAttributePreset(Tizen::Graphics::Opengl::EGL_ATTRIBUTES_PRESET_RGB565);

		__player->Start();
	}

	__renderer = new GlRendererTemplate();
	__player->SetIGlRenderer(__renderer);

	// サイズを通知
	GCDeviceOrientation o = this->ConvertOrientState(pGCubeFrame->GetOrientationStatus());
	gcube->onSizeChanged(__renderer->GetTargetControlWidth(), __renderer->GetTargetControlHeight(), o);

#ifdef __GCube_OrientationSensor__
	__sensorManager.Construct();
	this->CreateSensor();
#endif

	return true;
}

bool
GCubeApp::OnAppWillTerminate(void)
{
	return true;
}


bool
GCubeApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// 終了を通知
	gcube->onTerminate();

	__player->Stop();

	if(__renderer != null)
	{
		delete __renderer;
	}
	delete __player;

	return true;
}

void
GCubeApp::OnForeground(void)
{
	__player->Resume();
#ifdef __GCube_OrientationSensor__
	this->CreateSensor();
#endif
}

void
GCubeApp::OnBackground(void)
{
#ifdef __GCube_OrientationSensor__
	__sensorManager.RemoveSensorListener(*this);
#endif
	__player->Pause();
}

void
GCubeApp::OnLowMemory(void)
{
	gcube->onLowMemory();
}

void
GCubeApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
}

void
GCubeApp::OnScreenOn(void)
{
}

void
GCubeApp::OnScreenOff(void)
{
}

void
GCubeApp::OnOrientationChanged (const Tizen::Ui::Control &source, Tizen::Ui::OrientationStatus orientationStatus) {
	// 画面の回転を通知
	__renderer->SetTargetControlHeight(source.GetHeight());
	__renderer->SetTargetControlWidth(source.GetWidth());
	GCDeviceOrientation o = this->ConvertOrientState(orientationStatus);
	gcube->onSizeChanged(source.GetWidthF(), source.GetHeightF(), o);
}

GCDeviceOrientation
GCubeApp::ConvertOrientState (Tizen::Ui::OrientationStatus orientationStatus) {
	// 画面の回転定数の変換
	GCDeviceOrientation o;
	switch (orientationStatus) {
		case ORIENTATION_STATUS_PORTRAIT:
			o = GCDeviceOrientationPortrait;
			break;
		case ORIENTATION_STATUS_LANDSCAPE:
			o = GCDeviceOrientationLandscapeLeft;
			break;
		case ORIENTATION_STATUS_PORTRAIT_REVERSE:
			o = GCDeviceOrientationPortraitUpsideDown;
			break;
		case ORIENTATION_STATUS_LANDSCAPE_REVERSE:
			o = GCDeviceOrientationLandscapeRight;
			break;
		default:
			o = GCDeviceOrientationUnknown;
			break;
	}
	return o;
}

void GCubeApp::OnTouch (GCTouchAction type, const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo) {
	long long tick = 0;
	Tizen::System::SystemTime::GetTicks(tick);
	ApplicationController *controller = ApplicationController::SharedInstance();
	controller->onTouch(type, currentPosition.x, currentPosition.y, touchInfo.GetPointId(), tick);
}

void GCubeApp::OnTouchCanceled (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo) {
	this->OnTouch(GCTouchActionCancel, source, currentPosition, touchInfo);
}

void GCubeApp::OnTouchFocusIn (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo) {

}

void GCubeApp::OnTouchFocusOut (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo) {

}

void GCubeApp::OnTouchMoved (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo) {
	this->OnTouch(GCTouchActionMove, source, currentPosition, touchInfo);
}

void GCubeApp::OnTouchPressed (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo) {
	this->OnTouch(GCTouchActionDown, source, currentPosition, touchInfo);
}

void GCubeApp::OnTouchReleased (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo) {
	this->OnTouch(GCTouchActionUp, source, currentPosition, touchInfo);
}

void GCubeApp::OnDataReceived(SensorType sensorType, SensorData& sensorData, result r)
{
	// TODO: データをAndroidを基準に合わせる
	DeviceOrientationSensorData& data = static_cast<DeviceOrientationSensorData&>(sensorData);
	gcube->onOrientationChanged(DEGREES_TO_RADIANS(data.yaw), DEGREES_TO_RADIANS(data.pitch), DEGREES_TO_RADIANS(data.roll));
}

bool GCubeApp::CreateSensor(void)
 {
     result r = E_SUCCESS;

     if (__sensorManager.IsAvailable(SENSOR_TYPE_DEVICE_ORIENTATION))
     {
         r = __sensorManager.AddSensorListener(*this, SENSOR_TYPE_DEVICE_ORIENTATION, 50, true);
         if (IsFailed(r))
         {
             return false;
       }
     }
     else
     {
         AppLogException("Acceleration sensor is not available.");
         return false;
     }

     return true;
 }
