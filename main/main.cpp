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

#include "Main.h"
#include "glsample.h"

namespace GCube {

// 初期化
void Main::onInit() {
	LOGD("Main::onInit()");
	
	ApplicationController *ctr = ApplicationController::SharedInstance();
	// 言語設定を取得
	LOGD("lang:%s", ctr->getLanguage().c_str());
	// UserEventを送信
	LOGD("send user event:%d", ctr->sendUserEvent(1, 2, 3, 4, 5, "a"));
	
	this->onContextChanged();
}

// サイズ変更
void Main::onSizeChanged(float width, float height, GCDeviceOrientation orientation) {
	LOGD("Main::onSizeChanged(%f, %f, %d)", width, height, orientation);
	glViewport(0, 0, width, height);
}

// コンテキスト切り替え
void Main::onContextChanged() {
	// リソース読み込み
	ApplicationController *ctr = ApplicationController::SharedInstance();
	std::vector<char> buf;
	ctr->getResource("texture/gclue_logo.png", buf);
	
	// ファイル書き込み
	std::string filePath = ctr->getStoragePath(GCStorageTypeDocument);
	filePath += "hello.txt";
	LOGD("file:%s", filePath.c_str());
	FILE* file = fopen(filePath.c_str(), "w+");
	if (file != NULL) {
		fputs("HELLO WORLD!\n", file);
		fflush(file);
		fclose(file);
	}
	
	// サンプル初期化
	initProgram(buf);
}

// 描画
void Main::onDraw() {
	draw();
}

// ユーザイベント
int Main::onUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	LOGD("Main::onUserEvent(t: %d, 1: %d, 2: %lld, 3:%f, 4:%f, 5:%s)", type, param1, param2, param3, param4, param5);
	return type;
}

}
