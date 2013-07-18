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

#ifndef __GCube__SoundPlayer_h
#define __GCube__SoundPlayer_h

#include <pthread.h>
#include "SoundData.h"

namespace GCube {

/**
 * サウンド再生クラス（シングルトン）.
 */
class SoundPlayer {
private:
	// 唯一のインスタンス
	static SoundPlayer *instance;
	// 生成やコピーを禁止する
	SoundPlayer();
	SoundPlayer(const SoundPlayer& rhs);
	SoundPlayer& operator=(const SoundPlayer& rhs);

	/** デストラクタ. */
	virtual ~SoundPlayer();

	// OpenALのコンテキスト
	ALCdevice *device;
	ALCcontext *context;
	
	std::vector<SoundData*> sounds;
	std::vector<int> playingSounds;

	pthread_t thread;
	bool cancel;
	
	
public:
	
	/**
	 * 監視スレッド用メソッド.
	 */
	void playLoop();
	
	/**
	 * インスタンスを取得します.
	 * @return SoundPlayerオブジェクト
	 */
	static SoundPlayer *SharedInstance();
	
	/**
	 * インスタンスを破棄します.
	 */
	static void Dispose();
	
	/**
	 * 監視スレッドを開始します.
	 */
	static void Start();
	
	/**
	 * 監視スレッドを停止します.
	 */
	static void Pause();

	/**
	 * BGMファイルを読み込みます.
	 * @param[in] fileName ファイル名
	 * @return 登録ID
	 */
	static int loadBGM(const char *fileName);
	
	/**
	 * SEファイルを読み込みます.
	 * @param[in] fileName ファイル名
	 * @return 登録ID
	 */
	static int loadSE(const char *fileName);

	/**
	 * 登録ID指定でサウンドを再生します.
	 * @param[in] source 登録ID
	 * @param[in] loop ループ再生
	 */
	static void play(int source, bool loop=false);
	
	/**
	 * 登録ID指定でサウンドを停止します.
	 * @param[in] source 登録ID
	 */
	static void stop(int source);
	
	/**
	 * 登録ID指定で音量を調整します.
	 * @param[in] source 登録ID
	 * @param[in] volume 音量(0〜 1.0)
	 */
	static void volume(int source, float volume);

	/**
	 * 登録ID指定で再生中か確認します.
	 * @param[in] source 登録ID
	 * @return true 再生中
	 */
	static bool isPlaying(int source);

};

}

#endif
