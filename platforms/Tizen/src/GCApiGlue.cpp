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

#include "GCDefines.h"
#include <FBase.h>
#include <FLocales.h>

using namespace Tizen::Base;
using namespace Tizen::Locales;

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
