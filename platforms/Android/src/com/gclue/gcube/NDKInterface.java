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
