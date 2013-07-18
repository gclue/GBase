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

#include "SoundData.h"
#include "GCube.h"

#define BUFFER_SIZE (4096)

namespace GCube {

// Oggファイル読み込み（ストリーム）
bool SoundData::openOggFileStream(const char *fileName) {
	// リソース読み込み
	ApplicationController *ctr = ApplicationController::SharedInstance();
	ctr->getResource(fileName, sourceData);
	if (sourceData.size()>0) {
		// oggオープン
		int err = 0;
		stream = stb_vorbis_open_memory((unsigned char *)&sourceData[0], sourceData.size(), &err, NULL);
		stb_vorbis_info info = stb_vorbis_get_info(stream);
		channels = info.channels;
		fileType = (info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
		sampleRate = info.sample_rate;
		return err==0;
	} else {
		stream = NULL;
		channels = 0;
		fileType = 0;
		sampleRate = 0;
		return false;
	}
}

// ストリームを閉じる
void SoundData::closeStream() {
	if (!stream) return;
	stb_vorbis_close(stream);
}

// ストリームから一定量のバッファを読み込み
bool SoundData::readStreamBuffer(ALuint buffer) {
	if (!stream) return false;
	// TODO: ファイルフォーマットに合わせてバッファサイズを調整する
	ALshort pcm[BUFFER_SIZE];
	int  size = 0;
	int  result = 0;
	while(size < BUFFER_SIZE){
		result = stb_vorbis_get_samples_short_interleaved(stream, channels, pcm+size, BUFFER_SIZE-size);
		if(result > 0) size += result*channels;
		else break;
	}
	// 最後まできたら最初に戻す
	if(result == 0) {
		stb_vorbis_seek(stream, 0);
	}
	// 読み込み失敗
	if(size == 0) {
		return false;
	}
	// ALにバインド
	alBufferData(buffer, fileType, pcm, size*sizeof(ALshort), sampleRate);
	
	return true;
}

// Oggファイル読み込み（静的）
bool SoundData::loadOggFileStatic(const char *fileName, ALuint buffer) {
	
	// リソース読み込み
	std::vector<char> buf;
	ApplicationController *ctr = ApplicationController::SharedInstance();
	ctr->getResource(fileName, buf);
 	
	// Oggデコード
	short *data;
	int size = stb_vorbis_decode_memory((unsigned char *)&buf[0], buf.size(), &channels, &sampleRate, &data);
	fileType = (channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);

	// ALにバインド
	alBufferData(buffer, fileType, data, size*sizeof(ALshort), sampleRate);
	
	// 後始末
	free(data);

	return size>0;
};

}