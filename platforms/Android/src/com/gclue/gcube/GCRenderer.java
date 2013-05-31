package com.gclue.gcube;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.view.Surface;

public class GCRenderer extends FixedFPSRenderer {

	private Context context;
	
	public GCRenderer(Context context) {
		super();
		this.context = context;
	}

	@Override
	public void onDrawFrame(GL10 gl, float dt) {
		NDKInterface.step(dt);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		//Log.d("GCube", "onSurfaceChanged");
		// 画面の向きを取得
		int orientation = context.getResources().getConfiguration().orientation;
        int rotation = ((Activity) context).getWindowManager().getDefaultDisplay().getRotation();
        int actual_orientation = 0;
        if (orientation == Configuration.ORIENTATION_LANDSCAPE
        &&  (rotation == Surface.ROTATION_0 
        ||  rotation == Surface.ROTATION_90)){
        	actual_orientation = 3;
        } else if (orientation == Configuration.ORIENTATION_PORTRAIT
              &&  (rotation == Surface.ROTATION_0 
               ||  rotation == Surface.ROTATION_90)) {
        	actual_orientation = 1;
        } else if (orientation == Configuration.ORIENTATION_LANDSCAPE
              &&  (rotation == Surface.ROTATION_180 
               ||  rotation == Surface.ROTATION_270)){
        	actual_orientation = 4;
        } else {
            if (orientation == Configuration.ORIENTATION_PORTRAIT
                    &&  (rotation == Surface.ROTATION_180 
                     ||  rotation == Surface.ROTATION_270)){
            	actual_orientation = 2;
            }
        }
        NDKInterface.onSizeChanged(width, height, actual_orientation);
	}

	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
		//Log.d("GCube", "onSurfaceCreated");
		NDKInterface.onInit();
	}

}
