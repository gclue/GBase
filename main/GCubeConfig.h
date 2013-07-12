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

#ifndef GCube_GCubeConfig_h
#define GCube_GCubeConfig_h

// ログ出力
#define __GCube_LogOut__

#ifdef __cplusplus
struct GCubeSettings{
	
	unsigned int debugButtonPos;
	bool useOrientationSensor;
	bool showStatusBar;
	unsigned int frameRate;
	bool orientationPortrait;
	bool orientationPortraitUpsideDown;
	bool orientationLandscapeLeft;
	bool orientationLandscapeRight;
	
	GCubeSettings() {
		// デバッグボタン表示（Androidの場合はメニュー内に表示）
		//  0:非表示
		//  1:右下に表示
		//  2:左下に表示
		//  3:右上に表示
		//  4:左上に表示
		debugButtonPos = 1;
		
		// フレームレート
		frameRate = 60;
		
		// 傾きセンサー使用
		useOrientationSensor = false;

		// 対応画面方向
		orientationPortrait = true;
		orientationPortraitUpsideDown = false;
		orientationLandscapeLeft = true;
		orientationLandscapeRight = true;
		
		// ステータスバー表示
		showStatusBar = false;
	}

};
#endif

#endif
