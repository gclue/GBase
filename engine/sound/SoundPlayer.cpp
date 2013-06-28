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

#include "SoundPlayer.h"
#include "GCube.h"

namespace GCube {

// staticメンバ変数の実体
SoundPlayer* SoundPlayer::instance = NULL;

// インスタンスを取得します.
SoundPlayer* SoundPlayer::SharedInstance() {
	if (!instance) {
		instance = new SoundPlayer();
	}
	return instance;
}

// インスタンスを破棄します.
void SoundPlayer::dispose() {
	delete instance;
	instance = NULL;
}


// コンストラクタ
SoundPlayer::SoundPlayer() {
	// デバイスオープン
	device = alcOpenDevice(NULL);
	// コンテキスト取得
	context = alcCreateContext(device, NULL);
	// コンテキストをアクティブにする
	alcMakeContextCurrent(context);
	// エラーチェック
	if (alGetError() != AL_NO_ERROR) {
		LOGE("*****ERROR(SoundPlayer::SoundPlayer)**********");
	}
}

SoundPlayer::~SoundPlayer() {
	if (context) {
		alcMakeContextCurrent( NULL);
		alcDestroyContext(context);
	}
	context = NULL;

	if (device) {
		alcCloseDevice(device);
	}
	device = NULL;
}

// 読み込み
int SoundPlayer::loadSound(SoundData *data) {
	
	SoundPlayer::SharedInstance();
	
	// バッファとソース
	ALuint buffer, source;
	ALenum error;
	
	// ソース作成
	alGenSources(1, &source);
	if ((error = alGetError()) != AL_NO_ERROR) {
		LOGE("Failed to gen sources. - %d", error);
		return 0;
	}
	
	// バッファ作成
	alGenBuffers(1, &buffer);
	if ((error = alGetError()) != AL_NO_ERROR) {
		LOGE("Failed to gen buffers. - %d", error);
		if (source) alDeleteSources(1, &source);
		return 0;
	}
	
	// データをバッファへ格納
	alBufferData(buffer, data->fileType, data->data, data->dataSize, data->sampleRate);
	
	// ソースへバッファを関連づける
	alSourcei(source, AL_BUFFER, buffer);
	if ((error = alGetError()) != AL_NO_ERROR) {
		LOGE("Failed to set source. - %d", error);
		if (source) alDeleteSources(1, &source);
		if (buffer) alDeleteBuffers(1, &buffer);
		return 0;
	}

	// source,bufferをキャッシュ
	instance->sounds.insert(std::map<ALuint, ALuint>::value_type(source, buffer));

	// エラーチェック
	if (alGetError() != AL_NO_ERROR) {
		LOGE("*****ERROR(SoundPlayer::loadSound)");
	}
	return source;
}

// 削除
void SoundPlayer::removeSound(int source) {
	if (source) {
		// ソース削除
		alDeleteSources(1, (ALuint*) &source);
		// バッファ削除
		std::map<ALuint, ALuint>::iterator it = instance->sounds.find(source);
		if (it != instance->sounds.end()) {
			ALuint buffer = (*it).second;
			if (buffer) {
				alDeleteBuffers(1, (ALuint*) &buffer);
			}
			// ハッシュから削除
			instance->sounds.erase(it);
		}
	}
}

// リセット
void SoundPlayer::resetAllSounds() {
	std::map<ALuint, ALuint>::iterator it = instance->sounds.begin();
	while (it != instance->sounds.end()) {
		ALuint source = (*it).first;
		ALuint buffer = (*it).second;
		if (source) alDeleteSources(1, (ALuint*) &source);
		if (buffer) alDeleteBuffers(1, (ALuint*) &buffer);
		it++;
	}
	instance->sounds.clear();
}

// 再生
void SoundPlayer::play(int source, bool loop) {
	if (source == 0) return;
	alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	alSourcePlay(source);
}

// 停止
void SoundPlayer::stop(int source) {
	if (source == 0) return;
	alSourceStop(source);
}

// 音量
void SoundPlayer::volume(int source, float volume) {
	if (source == 0) return;
	if (volume < 0) volume = 0;
	if (volume > 1) volume = 1;
	alSourcef(source, AL_GAIN, volume);
}

// 再生チェック
bool SoundPlayer::isPlaying(int source) {
	ALint state;
	if (source == 0) return false;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}
	
}
