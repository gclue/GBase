package com.gclue.gcube;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MainActivity extends Activity {
	static {
		System.loadLibrary("gcube");
	}

	private GCGLSurfaceView glview;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		glview = new GCGLSurfaceView(this);
		setContentView(glview);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	protected void onResume() {
		super.onResume();
		glview.onResume();
		NDKInterface.onResume();
	}
	
	@Override
	protected void onPause() {
		NDKInterface.onPause();
		glview.onPause();
		super.onPause();
	}

	@Override
	public void onDestroy() {
		NDKInterface.onTerminate();
		glview = null;
		super.onDestroy();
	}

}
