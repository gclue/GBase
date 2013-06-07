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

import java.util.List;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class GCSensorEventListener implements SensorEventListener {

	private SensorManager mSensorManager;
	
	private boolean mIsMagSensor;
    private boolean mIsAccSensor;
    private static final int MATRIX_SIZE = 16;
    float[]  inR = new float[MATRIX_SIZE];
    float[] outR = new float[MATRIX_SIZE];
    float[]    I = new float[MATRIX_SIZE];
	float[] orientationValues   = new float[3];
    float[] magneticValues      = new float[3];
    float[] accelerometerValues = new float[3];
    long lastTimeStamp = 0;

	
	public GCSensorEventListener(Activity activity) {
	    mSensorManager = (SensorManager) activity.getSystemService(Activity.SENSOR_SERVICE);
	}
	
	public void start() {
		// センサの取得
        List<Sensor> sensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
        for (Sensor sensor : sensors) {
            if( sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD){
                mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_GAME);
                mIsMagSensor = true;
            }
            if( sensor.getType() == Sensor.TYPE_ACCELEROMETER){
                mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_GAME);
                mIsAccSensor = true;
            }
        }
	}
	
	public void stop() {
		//センサーマネージャのリスナ登録破棄
	    if (mIsMagSensor || mIsAccSensor) {
	        mSensorManager.unregisterListener(this);
	        mIsMagSensor = false;
	        mIsAccSensor = false;
	    }
	}
	
	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		if (event.accuracy == SensorManager.SENSOR_STATUS_UNRELIABLE) return;
		 
	    switch (event.sensor.getType()) {
	        case Sensor.TYPE_MAGNETIC_FIELD:
	            magneticValues = event.values.clone();
	            break;
	        case Sensor.TYPE_ACCELEROMETER:
	            accelerometerValues = event.values.clone();
	            break;
	    }
	 
	    if (magneticValues != null && accelerometerValues != null && event.timestamp > lastTimeStamp) {
	        SensorManager.getRotationMatrix(inR, I, accelerometerValues, magneticValues);
	 
	        // TODO: Activityの表示が縦固定の場合。横向きになる場合、修正が必要です
	        SensorManager.remapCoordinateSystem(inR, SensorManager.AXIS_X, SensorManager.AXIS_Y, outR);
	        SensorManager.getOrientation(outR, orientationValues);
	        
	        // jni側に通知
	        float yaw = orientationValues[0];
	        float pitch = orientationValues[1];
	        float roll = orientationValues[2];
	        NDKInterface.onOrientationChanged(yaw, pitch, roll);
	    }
	    
	    lastTimeStamp = event.timestamp;
	}
}
