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

#ifndef __GCube__SoundData_h
#define __GCube__SoundData_h

#include "GCDefines.h"

namespace GCube {
	
/**
 * サウンドデータクラス.
 */
class SoundData {
public:
	std::string fileName;	//!< ファイル名
	int dataSize;			//!< データサイズ
	short *data;			//!< データ
	int fileType;		//!< ファイルタイプ
	int sampleRate;		//!< サンプリングレート
	
	/**
	 * コンストラクタ.
	 */
	SoundData() : data(NULL) {};
	SoundData(const char *fileName, int dataSize, short *data, int fileType, int sampleRate) :
		fileName(std::string(fileName)), dataSize(dataSize), data(data), fileType(fileType), sampleRate(sampleRate) {};
	
	/**
	 * デストラクタ.
	 */
	~SoundData() {
		if (data) free(data);
	};
	
	
	/**
	 * Oggファイル読み込み.
	 */
	bool loadOgg(const char *fileName);
};

}

#endif
