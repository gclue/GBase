/**
 * Name        : GCube
 * Version     :
 * Vendor      :
 * Description :
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
	pGCubeFrame->SetOrientation(ORIENTATION_AUTOMATIC);
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
}

void
GCubeApp::OnBackground(void)
{
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
