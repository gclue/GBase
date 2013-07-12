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

#import "GCViewController.h"
#import <CoreMotion/CoreMotion.h>
#import "GCube.h"
#import "GCubeConfig.h"
#import "GCAppDelegate.h"

#define kDebugButtonPadding 15

using namespace GCube;

@interface GCViewController () {
	ApplicationController *gcube;
	CMMotionManager *motionMgr;
	UITouch *touchArray[10];
	GCubeSettings *_settings;
}
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation GCViewController

// 初期化時に設定を読み込む
- (id)initWithCoder:(NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
		GCAppDelegate *delegate = (GCAppDelegate*)([UIApplication sharedApplication].delegate);
		_settings = delegate.settings;
    }
    return self;
}

// view読み込み
- (void)viewDidLoad
{
    [super viewDidLoad];
	
	// ステータスバー表示
	[[UIApplication sharedApplication] setStatusBarHidden:!_settings->showStatusBar withAnimation:UIStatusBarAnimationNone];
	
	
	gcube = ApplicationController::SharedInstance();
	
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	self.preferredFramesPerSecond = _settings->frameRate;

	NSAssert(self.context, @"Failed to create ES context");
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	view.multipleTouchEnabled = YES;
    
    [EAGLContext setCurrentContext:self.context];
	
	float scale = [UIScreen mainScreen].scale;
	gcube->onInit();
	gcube->onSizeChanged(view.bounds.size.width*scale, view.bounds.size.height*scale, (GCDeviceOrientation)self.interfaceOrientation);
	
	if (_settings->useOrientationSensor) {
		// 傾きセンサー開始
		[self startMotionSensor];
	}
	
	// デバッグボタン追加
	if (_settings->debugButtonPos > 0) {
		UIButton *button = [UIButton buttonWithType:UIButtonTypeDetailDisclosure];
		[button addTarget:self action:@selector(debugButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
		[self.view addSubview:button];
		if (_settings->debugButtonPos == 1) {
			button.center = CGPointMake(view.bounds.size.width-kDebugButtonPadding, view.bounds.size.height-kDebugButtonPadding);
			[button setAutoresizingMask:UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleLeftMargin];
		} else if (_settings->debugButtonPos == 2) {
			button.center = CGPointMake(kDebugButtonPadding, view.bounds.size.height-kDebugButtonPadding);
			[button setAutoresizingMask:UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleRightMargin];
		} else if (_settings->debugButtonPos == 3) {
			button.center = CGPointMake(view.bounds.size.width-kDebugButtonPadding, kDebugButtonPadding);
			[button setAutoresizingMask:UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleLeftMargin];
		} else {
			button.center = CGPointMake(kDebugButtonPadding, kDebugButtonPadding);
			[button setAutoresizingMask:UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleRightMargin];
		}
	}
}

// 後処理
- (void)dealloc
{

	if (_settings->useOrientationSensor) {
		[self stopMotionSensor];
	}
	
    [EAGLContext setCurrentContext:self.context];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

// メモリ不足
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
	gcube->onLowMemory();
}


#pragma mark - GLKView and GLKViewController delegate methods

// 処理
- (void)update
{
	
	if (_settings->useOrientationSensor) {
		// 傾き取得（Androidを基準に値を変換）
		CMAttitude *currentAttitude = motionMgr.deviceMotion.attitude;
		double roll = currentAttitude.roll;  // Y軸中心のラジアン角: -π〜π(-180度〜180度)
		double pitch = -currentAttitude.pitch; // X軸中心のラジアン角: -π/2〜π/2(-90度〜90度)
		double yaw = -M_PI_2-currentAttitude.yaw;  // Z軸中心のラジアン角: -π〜π(-180度〜180度)
		if (yaw < -M_PI) yaw += (2.0*M_PI);
		if (!(pitch==0 && roll==0)) {
			gcube->onOrientationChanged(yaw, pitch, roll);
		}
	}
	
	gcube->onUpdate(self.timeSinceLastUpdate);
}

// 描画
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	gcube->onDraw();
}

// 回転方向（over iOS6）
- (NSUInteger)supportedInterfaceOrientations
{
	NSUInteger orient = 0;
	if (_settings->orientationPortrait) {
		orient |= UIInterfaceOrientationMaskPortrait;
	}
	if (_settings->orientationPortraitUpsideDown) {
		orient |= UIInterfaceOrientationMaskPortraitUpsideDown;
	}
	if (_settings->orientationLandscapeLeft) {
		orient |= UIInterfaceOrientationMaskLandscapeLeft;
	}
	if (_settings->orientationLandscapeRight) {
		orient |= UIInterfaceOrientationMaskLandscapeRight;
	}
	return orient;
}

// 回転方向（iOS5）
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	if (_settings->orientationPortrait) {
		if (interfaceOrientation == UIInterfaceOrientationPortrait) return YES;
	}
	if (_settings->orientationPortraitUpsideDown) {
		if (interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) return YES;
	}
	if (_settings->orientationLandscapeLeft) {
		if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft) return YES;
	}
	if (_settings->orientationLandscapeRight) {
		if (interfaceOrientation == UIInterfaceOrientationLandscapeRight) return YES;
	}
	return NO;
}

// 画面回転
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	float scale = [UIScreen mainScreen].scale;
	gcube->onSizeChanged(self.view.bounds.size.width*scale, self.view.bounds.size.height*scale, (GCDeviceOrientation)self.interfaceOrientation);
}


#pragma mark - Events

- (int)findTouch:(UITouch*)touch
{
	int idx = NSNotFound;
	for (int i=0; i<10; i++) {
		if (touchArray[i]==touch) {
			idx = i;
			break;
		}
	}
	return idx;
}

// タッチイベント
- (void)toucheEvent:(NSSet *)touches withEvent:(UIEvent *)event withType:(GCTouchAction)type {
	float scale = [UIScreen mainScreen].scale;
	for (UITouch *touch in touches) {
		// 何番目のタッチかを探す
		int idx = [self findTouch:touch];
		// 見つからない時は空欄に入れる
		if (idx == NSNotFound) {
			idx = [self findTouch:nil];
			touchArray[idx] = touch;
		} else {
			// タッチアップのタイミングで空欄にする
			if (type == GCTouchActionUp || type == GCTouchActionCancel) {
				touchArray[idx] = nil;
			}
		}
		// 通知
        CGPoint location = [touch locationInView:self.view];
		gcube->onTouch(type, location.x*scale, location.y*scale, idx, touch.timestamp*1000);
	}
}

// タッチ開始イベント
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[self toucheEvent:touches withEvent:event withType:GCTouchActionDown];
}

// タッチ移動イベント
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	[self toucheEvent:touches withEvent:event withType:GCTouchActionMove];
}

// タッチ終了イベント
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	[self toucheEvent:touches withEvent:event withType:GCTouchActionUp];
}

// タッチキャンセルイベント
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	[self toucheEvent:touches withEvent:event withType:GCTouchActionCancel];
}


#pragma mark - MotionSensor

// モーションセンサー開始
- (void)startMotionSensor
{
	// インスタンスの生成
	if (!motionMgr) {
		motionMgr = [[CMMotionManager alloc] init];
	}
	
	// 実質、ジャイロスコープの有無を確認
	if (motionMgr.deviceMotionAvailable) {
		// センサーの利用開始
		[motionMgr startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXTrueNorthZVertical];
	}
}

// モーションセンサー停止
- (void)stopMotionSensor
{
	if (motionMgr.deviceMotionActive) {
        [motionMgr stopMagnetometerUpdates];
    }
}


#pragma mark - For debug

// デバッグボタンイベント
- (void)debugButtonPressed:(id)sender {
	self.paused = YES;
    NSString *path = [[NSBundle mainBundle] pathForResource:@"debug" ofType:@"txt"];
	NSString *msg = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
	UIAlertView *alert = [[UIAlertView alloc]initWithTitle:nil message:msg delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
    [alert setAlertViewStyle:UIAlertViewStylePlainTextInput];
    [alert show];
}

// デバッグアラートイベント
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	self.paused = NO;
	if (buttonIndex==1 && gcube) {
		UITextField *txt = [alertView textFieldAtIndex:0];
		NSArray *cmds = [[txt text] componentsSeparatedByString:@" "];
		if ([cmds count]>0) {
			NSString *str = [cmds objectAtIndex:0];
			int p = 0;
			if ([cmds count]>1) {
				p = [[cmds objectAtIndex:1] intValue];
			}
			const char* c = [str cStringUsingEncoding:NSUTF8StringEncoding];
			gcube->onDebugCommand(c, p);
		}
	}
}

@end
