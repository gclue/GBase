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

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.SparseArray;
import android.view.MotionEvent;

/**
 * GLを描画するためのView.
 * @author GClue, Inc.
 */
public class GCGLSurfaceView extends GLSurfaceView {
	
	/** タッチイベントを保持するリスト. */
	private SparseArray<TouchEvent> eventArray = new SparseArray<TouchEvent>();
	
	/**
	 * コンストラクタ.
	 * @param context コンテキスト
	 */
    public GCGLSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        setRenderer(new GCRenderer(context));
    }

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		int count = event.getPointerCount();
		int action = event.getActionMasked();
		int idx = event.getActionIndex();
		final long time = event.getEventTime();
		
		for(int i=0; i<count; i++) {
			if (action!=MotionEvent.ACTION_MOVE && idx!=i) {
				continue;
			}
			final int id = event.getPointerId(i);
			final int x = (int) event.getX(i);
			final int y = (int) event.getY(i);
			
			TouchEvent evt = getTouchEvent(action, x, y, id, time);
			if (evt==null) continue;
			
			switch (action) {
			case MotionEvent.ACTION_MOVE:
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL:
				queueEvent(evt.clone());
				break;
			}
		}
		return true;
	}
	
	/**
	 * 使用していないタッチイベント用のオブジェクトを取得します.
	 * もし、使用していないタッチイベントがない場合には新たに作成します。
	 * @param action アクション
	 * @param x x座標
	 * @param y y座標
	 * @param time タッチイベントの発生した時間
	 * @return タッチイベント
	 */
	private TouchEvent getTouchEvent(int action, int x, int y, int id, long time) {
		TouchEvent evt = eventArray.get(id);
		// 同一座標のMoveイベントは無視
		if (evt!=null && evt.x == x && evt.y == y && action==MotionEvent.ACTION_MOVE) {
			return null;
		}
		if (evt==null) {
			evt = new TouchEvent();
		}
		// データ詰め替え
		evt.action = action;
		evt.x = x;
		evt.y = y;
		evt.time = time;
		evt.id = id;
		eventArray.put(id, evt);
		
		return evt;
	}
	
	/**
	 * タッチイベント.
	 * @author GClue, Inc.
	 */
	private static class TouchEvent implements Runnable, Cloneable {
		/** タッチイベントのアクション. */
		private int action;
		/** タッチされたx座標. */
		private int x;
		/** タッチされたy座標. */
		private int y;
		/** タッチされた時間. */
		private long time;
		/** ID. */
		private int id;
		@Override
		public void run() {
			int sendaction = action;
			if (sendaction == MotionEvent.ACTION_POINTER_DOWN) sendaction = MotionEvent.ACTION_DOWN;
			if (sendaction == MotionEvent.ACTION_POINTER_UP) sendaction = MotionEvent.ACTION_UP;
			NDKInterface.onTouchEvent(sendaction, x, y, id, time);
		}
		public TouchEvent clone(){
			TouchEvent t;
			try {
				t = (TouchEvent)super.clone();
			} catch (CloneNotSupportedException ce) {
				throw new RuntimeException();
			}
			return t;
		}
	}
}
