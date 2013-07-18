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

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import com.gclue.gcube.R;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {
	static {
		System.loadLibrary("gcube_app");
	}

	private GCGLSurfaceView glview = null;
	private GCSensorEventListener sensor = null;
	
	/** デバッグメニュー用のID */
	private static final int MENU_ID_DEBUG = 9999;
	
	// アクティビティ作成
	@SuppressLint("SdCardPath")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// filesフォルダを作成
		new File("/data/data/"+getPackageName()+"/files/").mkdir();
		// ステータスバーの表示設定
		if (!NDKInterface.showStatusBar()) {
			getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		}
	    requestWindowFeature(Window.FEATURE_NO_TITLE);
		// 画面回転方向を設定
		this.setRequestedOrientation(NDKInterface.getSupportedOrientation());
		// GLViewを作成
		glview = new GCGLSurfaceView(this);
		setContentView(glview);
		if (NDKInterface.useOrientationSensor()) {
			sensor = new GCSensorEventListener(this);
		}
	}

	// メニュー作成
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		if (NDKInterface.useDebugConsole()) {
			menu.add(Menu.NONE, MENU_ID_DEBUG, Menu.NONE, "Debug");
		}
		return true;
	}

	// メニュー選択時
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    switch (item.getItemId()) {
	    case MENU_ID_DEBUG:
	    	// 描画一時停止
	    	glview.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	    	// デバッグ解説用テキスト読み込み
	    	AssetManager as = getResources().getAssets();   
	        InputStream is = null;  
	        BufferedReader br = null;  
	        StringBuilder sb = new StringBuilder();   
	        try{  
	            try {  
	                is = as.open("etc/debug.txt");  
	                br = new BufferedReader(new InputStreamReader(is));   
	                String str;     
	                while((str = br.readLine()) != null){     
	                    sb.append(str +"\n");    
	                }      
	            } finally {  
	                if (br != null) br.close();  
	            }  
	        } catch (IOException e) {}  
	        
	        // デバックコンソールダイアログ表示
	        View layout = this.getLayoutInflater().inflate(R.layout.debug_dialog, null);
	        TextView text = (TextView) layout.findViewById(R.id.textView1);
	        text.setText(sb.toString());
	        final EditText editText = (EditText) layout.findViewById(R.id.editText1);
	        new AlertDialog.Builder(MainActivity.this)
	            .setIcon(android.R.drawable.ic_dialog_info)
	            .setTitle("DebugConsole")
	            .setView(layout)
	            .setPositiveButton("OK", new DialogInterface.OnClickListener() {
	                public void onClick(DialogInterface dialog, int whichButton) {
	                	// コマンドとパラメータに分割
	                	String[] commands = editText.getText().toString().split(" ", 0);
	                	String command = null;
	                	int param = 0;
	                	if (commands.length>0) {
	                		command = commands[0];
	                	}
	                	if (commands.length>1) {
	                		param = Integer.parseInt(commands[1]);
	                	}
	                	// コマンドをネイティブに渡す
	                	NDKInterface.sendDebugCommand(command, param);
	        	    	// 描画再開
            	    	glview.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	                }
	            })
	            .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
	        	    	// 描画再開
            	    	glview.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
                    }
	            })
	            .show();
	        return true;
	    }
	    return false;
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		glview.onResume();
		NDKInterface.onResume();
		if (sensor!=null) sensor.start();
	}
	
	@Override
	protected void onPause() {
		if (sensor!=null) sensor.stop();
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
