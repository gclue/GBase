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

#include "GCube.h"
#include <FBase.h>
#include <FApp.h>
#include <FLocales.h>
#include <FIo.h>

using namespace GCube;

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Locales;
using namespace Tizen::Io;

// 言語コードを取得
std::string GCGetLanguage() {
	// locale取得
	LocaleManager localeManager;
	localeManager.Construct();
	String lang = localeManager.GetSelectedLanguage();
	// cstrに変換
	int nLen = wcstombs( NULL, lang.GetPointer(), 0 );
	char* pstr = (char*) malloc ( nLen + 1 );
	wcstombs(pstr, lang.GetPointer(), nLen+1);
	std::string ret = std::string(pstr);
	free(pstr);
	// 返る値がiOS/Androidと少し違うので注意
	return ret;
}

// リソースを取得
void GCGetResourceData(const char *fileName, std::vector<char>& outData) {
	AppLogDebug("GCGetResourceData... %s", fileName);
	File file;
	static const int bufflen = 1024;
	char buffer[bufflen];
	result r = E_SUCCESS;

	// Open the file
	r = file.Construct(App::GetInstance()->GetAppResourcePath() + "assets/" + fileName, "r");
	TryCatch(!IsFailed(r), , "[GCGetResourceData] file open failed.");

	// Reads
	while(int len = file.Read(buffer, bufflen)) {
		TryCatch(!IsFailed(GetLastResult()), , "[GCGetResourceData] file read failed.");
		std::vector<float> tmp(buffer, buffer+len);
		outData.insert(outData.end(), tmp.begin(), tmp.end());
	}

	//AppLog("GCGetResourceData Succeeded!");
	return;
CATCH:
	AppLog("GCGetResourceData Failed...");
}

// ストレージパスを取得
std::string GCGetStoragePath(GCStorageType type) {
	String path;
	switch (type) {
		case GCStorageTypeDocument:
			path = App::GetInstance()->GetAppDataPath();
			break;
		case GCStorageTypeCache:
			// TODO: キャッシュの概念を考える
			path = App::GetInstance()->GetAppDataPath();
			break;
	}
	// cstrに変換
	int nLen = wcstombs( NULL, path.GetPointer(), 0 );
	char* pstr = (char*) malloc ( nLen + 1 );
	wcstombs(pstr, path.GetPointer(), nLen+1);
	std::string ret = std::string(pstr);
	free(pstr);
	return ret;
}

// TODO: デベロッパーが記述しやすい場所を準備する
// ユーザーイベントを受け取る
int GCSendUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	// なにか処理をする。
	AppLog("処理");
	// ユーザイベントを送信。
	ApplicationController *ctr = ApplicationController::SharedInstance();
	ctr->onUserEvent(10, 11, 12, 13, 14, "b");
	// なにか値を返す。
	return 1;
}
