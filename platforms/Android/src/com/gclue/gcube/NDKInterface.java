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

package com.gclue.gcube;

public class NDKInterface {

	/**
	 * ステップ実行.
	 * @param dt 経過時間
	 */
	public static native void step(float dt);

	/**
	 */
	public static native void onInit();
	
	/**
	 * 終了処理.
	 */
	public static native void onTerminate();
	
	/**
	 * 停止処理.
	 */
	public synchronized static native void onPause();

	/**
	 * 開始処理.
	 */
	public synchronized static native void onResume();

	/**
	 * GLのコンテキスト変更処理.
	 */
	public synchronized static native void onContextChanged();
	
	/**
	 * 画面サイズ変更処理.
	 * @param width 幅
	 * @param height 高さ
	 * @param orientation 画面の向き
	 */
	public static native void onSizeChanged(int width, int height, int orientation);
	
	/**
	 * バックボタンをクリックした時のイベント
	 * @return 処理をした場合はtrue,しないでシステムに返す場合はfalse
	 */
	public synchronized static native boolean onPressBackKey();
	
	/**
	 * JNI側にタッチイベントを通知します.
	 * @param action アクション
	 * @param x x座標
	 * @param y y座標
	 * @param time イベント発生時間
	 */
	public static native void touchEvent(int action, float x, float y, long time);
	
	/**
	 * JNIにイベントを配送します.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	public synchronized static native void sendGameEvent(int type, int param1, int param2, int param3, int param4, String param5);
}
