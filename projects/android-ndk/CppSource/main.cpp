#include "android_engine.h"
#include "android_debug.h"
#include "android_app.h"
#include "android_render.h"
#include "android_sound.h"
#include "android_asset_loader.h"

extern "C"
{

/**
 * Just the current frame in the display.
 */
int floatSize = sizeof(float);

float vertexData[3 * 8] = {
	0,0   ,1,0,0,1, 0,0,
	50,0  ,0,1,0,1, 0,0,
	50,50 ,0,0,1,1, 0,0
};

static void engine_draw_frame(AppManager* manager)
{
    // No display
    if(manager->display == NULL)
        return;

	AndroidRender *render = (AndroidRender*)manager->engine->gfx;
	render->beginDraw();
	render->setColor(vec4(0.5, 0.5, 0.0, 1.0));
	render->begin(X2D_DrawTriangles);
	render->addVertex(0, 0);
	render->addVertex(20, 0);
	render->addVertex(20, 20);
	render->end();
	render->endDraw();
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(AppManager* engine)
{
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
	// Get app manager
    AppManager* manager = (AppManager*)app->userData;
    if(AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		// Move "mouse"
		if(AKeyEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN)
			((AndroidApp*)manager->engine->app)->moveMouseBegin(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0));
		else
			((AndroidApp*)manager->engine->app)->moveMouse(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0));
        return 1;
    }

    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    AppManager* appManager = (AppManager*)app->userData;
    switch(cmd)
	{
		case APP_CMD_START:
			// We initialize the engine now since we have access to android->activity
			if(appManager->engine->init() != X2D_OK && appManager->engine->app)
			{
				appManager->engine->app->exception(X2D_OK, "Failed initialize the x2D Framework");
			}else{
				// Initialize OS event hooks
				((AndroidApp*)appManager->engine->app)->initEvents();
			}
			break;

        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            //appManager->android->savedState = malloc(sizeof(SaveState));
            //*((SaveState*)appManager->android->savedState) = appManager->state;
            //appManager->android->savedStateSize = sizeof(SaveState);
            break;


        case APP_CMD_INIT_WINDOW:
            // Initialize the window (OpenGL ES)
            if(appManager->android->window != NULL) {
				AndroidRender *render = (AndroidRender*)appManager->engine->gfx;
				if(render->initGL() < 0)
					appManager->engine->app->exception(X2D_OK, "Failed to initialize OpenGL ES");
            }
            break;

        case APP_CMD_TERM_WINDOW:
			{
            // The window is being hidden or closed, clean it up
			AndroidRender *render = (AndroidRender*)appManager->engine->gfx;
			render->destroyGL();
			}
            break;

        case APP_CMD_GAINED_FOCUS:
			iosystem::print("Gain focus called");
            // When our app gains focus, we start monitoring the accelerometer.
            if (appManager->accelerometerSensor != NULL)
			{
                ASensorEventQueue_enableSensor(appManager->sensorEventQueue, appManager->accelerometerSensor);

                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(appManager->sensorEventQueue,
                        appManager->accelerometerSensor, (1000L/60)*1000);
            }
            appManager->animating = 1;
            break;

        case APP_CMD_LOST_FOCUS:
			iosystem::print("Lost focus called");
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (appManager->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(appManager->sensorEventQueue,
                        appManager->accelerometerSensor);
            }

            // Also stop animating.
            appManager->animating = 0;
            engine_draw_frame(appManager);
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */

void android_main(struct android_app* app)
{
    // Make sure glue isn't stripped
    app_dummy();

	// Create application manager
	AppManager manager;
    app->userData = &manager;
    app->onAppCmd = engine_handle_cmd;
    app->onInputEvent = engine_handle_input;

	// Create x2d engine
	X2DEngine *engine = CreateEngine(0);
	engine->app  = new AndroidApp(&manager);
	engine->gfx = new AndroidRender(&manager);
	engine->sfx = new AndroidSound;
	engine->debug = new AndroidDebug;
	engine->assetLoader = new AndroidAssetLoader(&manager);
	// NOTE: engine->init() can be found in engine_handle_cmd

    manager.android = app;
	manager.engine = engine;
	manager.display = NULL;

    // Prepare to monitor accelerometer
    manager.sensorManager = ASensorManager_getInstance();
    manager.accelerometerSensor = ASensorManager_getDefaultSensor(manager.sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    manager.sensorEventQueue = ASensorManager_createEventQueue(manager.sensorManager, app->looper, LOOPER_ID_USER, NULL, NULL);

    if(app->savedState != NULL)
	{
        // We are starting from a previous saved state; restore from it.
        manager.state = *(SaveState*)app->savedState;
    }

    // Loop waiting for stuff to do.
    while(true)
	{
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while((ident = ALooper_pollAll(manager.animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
		{
            // Process this event.
            if (source != NULL)
                source->process(app, source);

            // If a sensor has data, process it now.
            if(ident == LOOPER_ID_USER)
			{
                if(manager.accelerometerSensor != NULL)
				{
                    ASensorEvent event;
                    while(ASensorEventQueue_getEvents(manager.sensorEventQueue, &event, 1) > 0) {
                        iosystem::print("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y,  event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if(app->destroyRequested != 0)
			{
                engine_term_display(&manager);
                return;
            }
        }

        if(manager.display != NULL && manager.animating)
		{
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            //engine_draw_frame(&manager);
			engine->draw();
        }
    }
}
}