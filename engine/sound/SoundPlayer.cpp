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

#include <unistd.h>
#include <signal.h>
#include "SoundPlayer.h"
#include "GCube.h"

#define BUFFER_NUM (16)

// スレッド用関数
void* thread_func(void *arg) {
	GCube::SoundPlayer *instance = (GCube::SoundPlayer*)arg;
	instance->playLoop();
	return NULL;
}

namespace GCube {

// staticメンバ変数の実体
SoundPlayer* SoundPlayer::instance = NULL;

// インスタンスを取得します.
SoundPlayer* SoundPlayer::SharedInstance() {
	if (!instance) {
		instance = new SoundPlayer();
	}
	SoundPlayer::Start();
	return instance;
}

// インスタンスを破棄します.
void SoundPlayer::Dispose() {
	if (!instance) return;
	instance->cancel = true;
	delete instance;
	instance = NULL;
}

// スレッドを開始します.
void SoundPlayer::Start() {
	if (!instance) return;
	// スレッドが動いていない場合は動かす
	instance->cancel = false;
	if (instance->thread) {
		if(!pthread_kill(instance->thread, 0) == 0)
		{
			pthread_create(&(instance->thread), NULL, thread_func, instance);
		}
	} else {
		pthread_create(&(instance->thread), NULL, thread_func, instance);
	}
	// 再生中だったサウンドを再生
	std::vector<int>::iterator it = instance->playingSounds.begin();
	while(it != instance->playingSounds.end()) {
		int source = *it;
		alSourcePlay(source);
		++it;
	}
}
	
// スレッドを停止します.
void SoundPlayer::Pause() {
	if (!instance) return;
	
	// 再生中のサウンドを保持
	instance->playingSounds.clear();
	std::vector<SoundData*>::iterator it = instance->sounds.begin();
	while(it != instance->sounds.end()) {
		SoundData *data = *it;
		ALint state;
		alGetSourcei(data->sourceID, AL_SOURCE_STATE, &state);
		if (state==AL_PLAYING) {
			instance->playingSounds.push_back(data->sourceID);
			alSourceStop(data->sourceID);
		}
		++it;
	}
	// スレッド停止
	instance->cancel = true;
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
	thread = 0;
	cancel = false;
}

// デストラクタ
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

// 監視ループ
void SoundPlayer::playLoop() {
	while (!cancel) {
		//LOGD("playLoop");
		sleep(1);
		
		std::vector<SoundData*>::iterator it = sounds.begin();
		while(it != sounds.end()) {
			SoundData *data = *it;
			ALuint  popBufferNo = 0;
			ALint bufferProcessed = 0;
			int m_Source = data->sourceID;
			int type = 0;
			// ストリーミングじゃないソースは無視
			alGetSourcei( m_Source, AL_SOURCE_TYPE, &type );
			if (type!=AL_STREAMING) {
				++it;
				break;
			}
			alGetSourcei( m_Source, AL_BUFFERS_PROCESSED, &bufferProcessed );
			//  空いたバッファを埋め尽くすまでループ
			while( bufferProcessed != 0 ){
				//  キューからバッファを取り出す
				popBufferNo = 0;
				alSourceUnqueueBuffers( m_Source, 1, &popBufferNo );
				//  バッファ読み込み
				if (data->readStreamBuffer(popBufferNo)) {
					alSourceQueueBuffers( m_Source, 1, &popBufferNo );
				}
				--bufferProcessed;
			}
			++it;
		}
	}
}

// 読み込み
int SoundPlayer::loadBGM(const char *fileName) {
	
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
	
	// ファイル読み込み
	SoundData *data = new SoundData();
	data->openOggFileStream(fileName);
	
	// バッファ作成
	for (int i=0; i<BUFFER_NUM; i++) {
		alGenBuffers(1, &buffer);
		// バッファ読み込み
		if (data->readStreamBuffer(buffer)) {
			alSourceQueueBuffers(source, 1, &buffer);
		}
	}
	
	// データ保持
	data->sourceID = source;
	instance->sounds.push_back(data);
	
	// エラーチェック
	if (alGetError() != AL_NO_ERROR) {
		LOGE("*****ERROR(SoundPlayer::loadSound)");
	}
	return source;
}

	
// SE読み込み
int SoundPlayer::loadSE(const char *fileName) {
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
	
	// ファイル読み込み
	SoundData *data = new SoundData();
	data->loadOggFileStatic(fileName, buffer);
	data->sourceID = source;
	
	// ソースへバッファを関連づける
	alSourcei(source, AL_BUFFER, buffer);
	if ((error = alGetError()) != AL_NO_ERROR) {
		LOGE("Failed to set source. - %d", error);
		if (source) alDeleteSources(1, &source);
		if (buffer) alDeleteBuffers(1, &buffer);
		return 0;
	}
	
	// データ保持
	instance->sounds.push_back(data);
	
	// エラーチェック
	if (alGetError() != AL_NO_ERROR) {
		LOGE("*****ERROR(SoundPlayer::loadSound)");
	}
	return source;
}

// 再生
void SoundPlayer::play(int source, bool loop) {
	if (source == 0) return;
	int type = 0;
	alGetSourcei( source, AL_SOURCE_TYPE, &type );
	if (type!=AL_STREAMING) {
		alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}
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
