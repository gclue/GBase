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

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.res.AssetManager;

import com.gclue.gcube.event.UserEventHandler;

public class NDKInterface {

	private Activity activity;
	private UserEventHandler userEventHandler;
	
	/**
	 * コンストラクタ
	 * @param activity 
	 */
	private NDKInterface(Activity activity) {
		this.activity = activity;
		userEventHandler = new UserEventHandler(activity);
	}
	
	/**
	 * 初期化処理を行います.
	 */
	public static void initInterface(Activity activity) {
		NDKInterface.setInterface(new NDKInterface(activity), activity.getResources().getAssets());
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////// send event to native
	
	/**
	 * ネイティブとの橋渡しオブジェクトを設定します.
	 * @param ndkInterface
	 */
	private synchronized static native void setInterface(NDKInterface ndkInterface, AssetManager manager);
	
	/**
	 * ステップ実行.
	 * @param dt 経過時間
	 */
	public static native void step(float dt);

	/**
	 * 初期化処理
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
	 * タッチイベントを通知.
	 * @param action アクション
	 * @param x x座標
	 * @param y y座標
	 * @param time イベント発生時間
	 */
	public static native void onTouchEvent(int action, float x, float y, int id, long time);
	
	/**
	 * 傾きセンサーイベントを通知.
	 * @param yaw ヨー
	 * @param pitch ピッチ
	 * @param roll ロール
	 */
	public static native void onOrientationChanged(float yaw, float pitch, float roll);
	
	/**
	 * ユーザーイベントを配送.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	public synchronized static native void sendUserEvent(int type, int param1, long param2, float param3, double param4, String param5);
	
	/**
	 * フレームレート取得
	 */
	public static native int getFrameRate();
	
	/**
	 * 傾きセンサーを使用するかを取得
	 */
	public static native boolean useOrientationSensor();
	
	/**
	 * 対応画面方向取得
	 */
	public static native int getSupportedOrientation();
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////// received event from native

	/**
	 * 文字列をキーに文字列情報を返す.
	 * @param key キー
	 * @return 情報
	 */
	@SuppressLint("SdCardPath")
	public String getStringInfo(String key, String opt) {
		String ret = "";
		// 言語設定
		if (key.equals("Lang")) {
			return activity.getResources().getConfiguration().locale.getLanguage();
		}
		// ストレージパス取得
		if (key.equals("StoragePath")) {
			if (opt.equals("Cache")) {
				return activity.getCacheDir().getAbsolutePath();
			} else {
				return "/data/data/" + activity.getPackageName() + "/files/";
			}
		}
		return ret;
	}
	
	/**
	 * ネイティブからのイベントを受け取る. <br>
	 * 
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	public int onUserEvent(int type, int param1, long param2, float param3, double param4, final String param5) {
		return userEventHandler.onUserEvent(type, param1, param2, param3, param4, param5);
	}
	
}
