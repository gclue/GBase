package com.gclue.gcube;

import android.content.Context;
import android.opengl.GLSurfaceView;


public class GCGLSurfaceView extends GLSurfaceView {
	
    public GCGLSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        setRenderer(new GCRenderer(context));
    }

}
