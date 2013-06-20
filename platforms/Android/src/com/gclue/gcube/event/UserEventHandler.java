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

package com.gclue.gcube.event;

import com.gclue.gcube.NDKInterface;

import android.app.Activity;
import android.util.Log;

/**
 * ユーザーイベント処理.
 * デベロッパはこのクラスを変更してAndroid向けの処理を記述してください.
 */
public class UserEventHandler {
	
	private Activity activity;
	
	/**
	 * コンストラクタ
	 * @param activity 
	 */
	public UserEventHandler(Activity activity) {
		this.activity = activity;
	}

	/**
	 * ユーザーイベントを受け取る. <br>
	 * 
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	public int onUserEvent(int type, int param1, long param2, float param3, double param4, String param5) {
		// なにか処理をする。
		Log.d(activity.getLocalClassName(), "処理");
		// ユーザイベントを送信。
		NDKInterface.sendUserEvent(10, 11, 12, 13, 14, "b");
		// なにか値を返す。
		return type;
	}

}
