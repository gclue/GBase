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

using namespace GCube;
using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

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
	settings = new GCubeSettings();

	return true;
}

bool
GCubeApp::OnAppInitialized(void)
{
	// フレーム作成
	Frame *pFrame = new Frame();
	pFrame->Construct();
	pFrame->SetName(L"GCube");
	pFrame->AddOrientationEventListener(*this);

	// フォーム作成
	Form *pForm = new Form();
	pForm->Construct(FORM_STYLE_NORMAL);
	pForm->AddTouchEventListener(*this);

	pFrame->AddControl(pForm);
	pFrame->SetCurrentForm(pForm);

	// 画面の向き
	enum Orientation orientation = ORIENTATION_NONE;
	if (settings->orientationPortrait) {
		if (settings->orientationLandscapeLeft || settings->orientationLandscapeRight) {
			orientation = ORIENTATION_AUTOMATIC;
		} else {
			orientation = ORIENTATION_PORTRAIT;
		}
	} else {
		if (settings->orientationPortraitUpsideDown) {
			if (settings->orientationLandscapeLeft || settings->orientationLandscapeRight) {
				orientation = ORIENTATION_AUTOMATIC_FOUR_DIRECTION;
			} else {
				orientation = ORIENTATION_PORTRAIT_REVERSE;
			}
		} else {
			if (settings->orientationLandscapeLeft) {
				orientation = ORIENTATION_LANDSCAPE_REVERSE;
			} else {
				if (settings->orientationLandscapeRight) {
					orientation = ORIENTATION_LANDSCAPE;
				}
			}
		}
	}

	pFrame->SetOrientation(orientation);
	pForm->SetOrientation(orientation);
	pFrame->SetMultipointTouchEnabled(true);
	AddFrame(*pFrame);

	{
		__player = new Tizen::Graphics::Opengl::GlPlayer;
		__player->Construct(Tizen::Graphics::Opengl::EGL_CONTEXT_CLIENT_VERSION_2_X, pForm);

		__player->SetFps(settings->frameRate);
		__player->SetEglAttributePreset(Tizen::Graphics::Opengl::EGL_ATTRIBUTES_PRESET_RGB565);

		__player->Start();
	}

	__renderer = new GlRendererTemplate();
	__player->SetIGlRenderer(__renderer);

	int w = __renderer->GetTargetControlWidth();
	int h = __renderer->GetTargetControlHeight();

	if (settings->debugButtonPos > 0) {
		// デバッグボタン作成
		// TODO: XMLで作成
		Button* pDebugButton = new Button();
		if (settings->debugButtonPos == 1) {
			pDebugButton->Construct(Rectangle(w-80, h-80, 70, 70), L"D");
		} else if (settings->debugButtonPos == 2) {
			pDebugButton->Construct(Rectangle(10, h-80, 70, 70), L"D");
		} else if (settings->debugButtonPos == 3) {
			pDebugButton->Construct(Rectangle(w-80, 10, 70, 70), L"D");
		} else {
			pDebugButton->Construct(Rectangle(10, 10, 70, 70), L"D");
		}
		pDebugButton->SetActionId(ID_DEBUG_BUTTON);
		pDebugButton->AddActionEventListener(*this);
		pForm->AddControl(pDebugButton);

		// ポップアップ作成
		__pPopup = new Popup();
		__pPopup->Construct(true, Dimension(600, 800));
		__pPopup->SetTitleText(L"DebugConsole");

		// ポップアップを閉じるボタン作成
		Button* pCloseButton = new Button();
		pCloseButton->Construct(Rectangle(30, 600, 250, 80), L"Cancel");
		pCloseButton->SetActionId(ID_BUTTON_CLOSE_POPUP);
		pCloseButton->AddActionEventListener(*this);
		__pPopup->AddControl(pCloseButton);

		// OKボタン作成
		Button* pOKButton = new Button();
		pOKButton->Construct(Rectangle(320, 600, 250, 80), L"OK");
		pOKButton->SetActionId(ID_BUTTON_OK_POPUP);
		pOKButton->AddActionEventListener(*this);
		__pPopup->AddControl(pOKButton);

		// テキスト入力
		__pEditField = new EditField();
		__pEditField->Construct(Rectangle(30, 500, 540, 80));
		__pPopup->AddControl(__pEditField);

		// 説明テキスト
		std::vector<char> textBuff;
		GCGetResourceData("etc/debug.txt", textBuff);
		textBuff.push_back('\n');
		String text = String(&textBuff[0]);
		TextBox *pTextBox = new TextBox();
		pTextBox->Construct(Rectangle(30, 50, 540, 430));
		pTextBox->SetTextSize(18);
		pTextBox->SetText(text);
		__pPopup->AddControl(pTextBox);
	}

	// サイズを通知
	GCDeviceOrientation o = this->ConvertOrientState(pFrame->GetOrientationStatus());
	gcube->onSizeChanged(w, h, o);

	if (settings->useOrientationSensor) {
		__sensorManager.Construct();
		this->CreateSensor();
	}

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
	if (settings->useOrientationSensor) {
		this->CreateSensor();
	}
}

void
GCubeApp::OnBackground(void)
{
	if (settings->useOrientationSensor) {
		__sensorManager.RemoveSensorListener(*this);
	}
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

// IActionEventListener implementation
void GCubeApp::OnActionPerformed(const Control& source, int actionId) {
	switch (actionId) {
	case ID_DEBUG_BUTTON: {
		__pPopup->SetShowState(true);
		__pPopup->Show();
		break;
	}
	case ID_BUTTON_CLOSE_POPUP:
		 __pPopup->SetShowState(false);
		break;
	case ID_BUTTON_OK_POPUP: {
		String str = __pEditField->GetText();
		StringTokenizer strTok(str, L" ");
	    String token1, token2 = NULL;
	    strTok.GetNextToken(token1);
	    if (strTok.HasMoreTokens()) {
		    strTok.GetNextToken(token2);
	    }
		// cstrに変換
		int nLen = wcstombs( NULL, token1.GetPointer(), 0 );
		char* pstr = (char*) malloc ( nLen + 1 );
		wcstombs(pstr, token1.GetPointer(), nLen+1);
		// intに変換
		int param = 0;
		Integer::Parse(token2, param);

	    gcube->onDebugCommand(pstr, param);
		__pPopup->SetShowState(false);
		free(pstr);
		break;
	}
	default:
		break;
	}
}
