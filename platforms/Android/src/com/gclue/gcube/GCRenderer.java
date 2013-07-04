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
		NDKInterface.initInterface((Activity)context);
		NDKInterface.onInit();
	}

}
