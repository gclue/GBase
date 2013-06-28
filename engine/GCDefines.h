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

#ifndef GCube_GCDefines_h
#define GCube_GCDefines_h

#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>

// Android
#ifdef __GCube_Android__
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
# include <AL/al.h>
# include <AL/alc.h>
#endif

// iOS
#ifdef __GCube_iOS__
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# include <OpenAL/al.h>
# include <OpenAL/alc.h>
#endif

// Tizen
#ifdef __GCube_Tizen__
# include <gl2.h>
# include <gl2ext.h>
# include <AL/al.h>
# include <AL/alc.h>
#endif


#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0 * M_PI)
#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / M_PI))

// デバイスの向き
typedef enum GCDeviceOrientation
{
	GCDeviceOrientationUnknown,
	GCDeviceOrientationPortrait,            // Device oriented vertically, home button on the bottom
	GCDeviceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
	GCDeviceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
	GCDeviceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
	GCDeviceOrientationFaceUp,              // Device oriented flat, face up
	GCDeviceOrientationFaceDown             // Device oriented flat, face down
}
GCDeviceOrientation;

// タッチイベント
typedef enum GCTouchAction
{
	GCTouchActionDown,
	GCTouchActionUp,
	GCTouchActionMove,
	GCTouchActionCancel,
}
GCTouchAction;

// ストレージタイプ
typedef enum GCStorageType
{
	GCStorageTypeDocument,
	GCStorageTypeCache,
}
GCStorageType;

// クラス
namespace GCube {
	class Main;
}


/////////////////////////////////////////////////////////////////////////
#pragma mark - API glue

// 言語コードを取得
std::string GCGetLanguage();
// リソースを取得
void GCGetResourceData(const char *fileName, std::vector<char>& outData);
// ストレージパスを取得
std::string GCGetStoragePath(GCStorageType type);

// ユーザーイベントを送信
int GCSendUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5);


#endif
