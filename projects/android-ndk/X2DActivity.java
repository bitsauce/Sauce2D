/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This is a small port of the "San Angeles Observation" demo
 * program for OpenGL ES 1.x. For more details, see:
 *
 *    http://jet.ro/visuals/san-angeles-observation/
 *
 * This program demonstrates how to use a GLSurfaceView from Java
 * along with native OpenGL calls to perform frame rendering.
 *
 * Touching the screen will start/stop the animation.
 *
 * Note that the demo runs much faster on the emulator than on
 * real devices, this is mainly due to the following facts:
 *
 * - the demo sends bazillions of polygons to OpenGL without
 *   even trying to do culling. Most of them are clearly out
 *   of view.
 *
 * - on a real device, the GPU bus is the real bottleneck
 *   that prevent the demo from getting acceptable performance.
 *
 * - the software OpenGL engine used in the emulator uses
 *   the system bus instead, and its code rocks :-)
 *
 * Fixing the program to send less polygons to the GPU is left
 * as an exercise to the reader. As always, patches welcomed :-)
 */
package com.example.x2dgame;

import android.app.Activity;
import android.os.Bundle;

import android.util.Log;

public class X2DActivity extends Activity
{
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        System.loadLibrary("x2DAndroid");

		Log.d("X2D", "onCreate");

		init();
    }
    @Override
    protected void onStart() {
        super.onStart();
        // The activity is about to become visible.
    }
    @Override
    protected void onResume() {
        super.onResume();
        // The activity has become visible (it is now "resumed").
    }
    @Override
    protected void onPause() {
        super.onPause();
        // Another activity is taking focus (this activity is about to be "paused").
    }
    @Override
    protected void onStop() {
        super.onStop();
        // The activity is no longer visible (it is now "stopped")
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        // The activity is about to be destroyed.
    }
	

	// Native function calls
	private static native void init();
	//private static native void pause();
	//private static native void resume();
}

/*public class X2DActivity extends Activity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
	{
        super.onCreate(savedInstanceState);
        m_glView = new DemoGLSurfaceView(this);
        setContentView(m_glView);

		init();
    }

    @Override
    protected void onPause()
	{
        super.onPause();
        m_glView.onPause();

		pause();
    }

    @Override
    protected void onResume()
	{
        super.onResume();
        m_glView.onResume();

		resume();
    }


    static
	{
        System.loadLibrary("san-angeles");
    }

	// GL surface
    private GLSurfaceView m_glView;

	// Native function calls
	private static native void init();
	private static native void pause();
	private static native void resume();
}

class DemoGLSurfaceView extends GLSurfaceView
{
    public DemoGLSurfaceView(Context context) {
        super(context);
        mRenderer = new DemoRenderer();
        setRenderer(mRenderer);
    }

    public boolean onTouchEvent(final MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            nativePause();
        }
        return true;
    }

    DemoRenderer mRenderer;

    private static native void nativePause();
}

class DemoRenderer implements GLSurfaceView.Renderer {
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInit();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        //gl.glViewport(0, 0, w, h);
        nativeResize(w, h);
    }

    public void onDrawFrame(GL10 gl) {
        nativeRender();
    }

    private static native void nativeInit();
    private static native void nativeResize(int w, int h);
    private static native void nativeRender();
    private static native void nativeDone();
}*/
