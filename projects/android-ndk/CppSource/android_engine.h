#ifndef ANDROID_ENGINE_H
#define ANDROID_ENGINE_H

#include "x2d/platform.h"
#include "x2d/engine.h"
#include "x2d/iosystem.h"

#include <jni.h>
#include <errno.h>

#include <android/sensor.h>
#include <android/log.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
// NOTE TO SELF: Do not mix GLES 1.1 and GLES 2.0 calls. It will fail miserably.
// For GLES 2.0, include system library: GLESv2
//#include <GLES2/gl2.h>

#include "android_native_app_glue.h"

/**
 * Our saved state data.
 */
struct SaveState
{
};

/**
 * Shared state for our app.
 */
struct AppManager
{
	// Native app glue
    android_app *android;

	// X2D Engine
	X2DEngine *engine;

	// Sensor classes
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

	// State
    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    SaveState state;
};

#endif // ANDROID_ENGINE_H