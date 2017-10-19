#include <Sauce/Common.h>
#include <Sauce/Graphics.h>
#include <Sauce/Input.h>
#include <Sauce/Audio.h>

BEGIN_SAUCE_NAMESPACE

Exception::Exception(RetCode code, const char * msg, ...) :
	m_errorCode(code),
	m_callstack()
{
	va_list args;
	va_start(args, msg);

	// Get string length
	int size = _vscprintf(msg, args);

	// Create out string
	m_message.resize(size);

	// Parse varargs
#ifdef USE_CTR_SECURE
	vsprintf_s(&m_message[0], size + 1, msg, args);
#else
	vsprintf(out, msg, args);
#endif

	va_end(args);
}

Keycode KeyEvent::getKeycode() const
{
	return (Keycode) SDL_GetKeyFromScancode((SDL_Scancode) m_inputButton.getCode());
}

Game *Game::s_this = 0;

// TODO: Might want to do some file path validation check on the name
// and org name so that the pref path won't bug out
Game::Game(const string &name, const string &organization, const uint flags) :
	m_name(name),
	m_organization(organization),
	m_flags(flags),
	m_initialized(false),
	m_paused(false),
	m_running(false)
{
	if(s_this)
	{
		THROW("A game already exists!");
	}
	s_this = this;
}

Game::Game(const string &name, const uint flags) :
	Game(name, SAUCE_DEFAULT_ORGANIZATION, flags)
{
}

Game::~Game()
{
	// Release managers
	delete m_fileSystem;
	//delete m_audio;
	delete m_timer;
	delete m_console;
	delete m_resourceManager;
	s_this = 0;
}

//------------------------------------------------------------------------
// Run
//------------------------------------------------------------------------
int Game::run()
{
	try
	{
		// Make sure we're not running already
		if(m_running)
		{
			THROW("Game already running");
		}
		m_running = true;

		// TODO: Implement an engine config file
		// (DefaultConfig.ini for instance). It should
		// set the value of SAUCE_EXPORT_LOG and SAUCE_VERBOSE, etc.
		//
		// ConfigFile default("config:/DefaultConfig.ini");
		// default.getValue("Window/ResolutionX");
		// etc...

		for(int i = 0; i < __argc; i++)
		{
			string argType = __argv[i];
			if(argType == "-cwd")
			{
				string arg = __argv[++i];
				SetCurrentDirectory(arg.c_str());
			}
		}

		// Set game root directory
		m_binaryPath = SDL_GetBasePath();

		// Set save directory
		m_prefPath = SDL_GetPrefPath(m_organization.c_str(), m_name.c_str());

		m_console = new Console();
		m_fileSystem = new FileSystem();
		if(isEnabled(SAUCE_EXPORT_LOG))
		{
			m_console->m_output = new FileWriter(util::getAbsoluteFilePath("bin:/Console.log"));
		}

		m_timer = new Timer();
		//m_audio = new AudioManager();

		m_console->m_engine = this;

		LOG("** Initializing Engine **");

		// Initialize SDL
		if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			THROW("Unable to initialize SDL");
		}

		SDL_version sdlver;
		SDL_GetVersion(&sdlver);
		LOG("** SDL %i.%i.%i initialized **", sdlver.major, sdlver.minor, sdlver.patch);

		// Initialize resource manager
		m_resourceManager = new ResourceManager("Resources.xml");

		Uint8 windowFlags = 0;
		if(isEnabled(SAUCE_WINDOW_RESIZABLE))
		{
			windowFlags |= SDL_WINDOW_RESIZABLE;
		}

		// Initialize window
		Window *mainWindow = new Window(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
		m_windows.push_back(mainWindow);
		GraphicsContext *graphicsContext = mainWindow->getGraphicsContext();

		// TODO: Concider moving these to a function of sorts (initGraphics())
		// Initialize GL3W
		if(gl3wInit() != 0)
		{
			THROW("GL3W did not initialize!");
		}

		// Print GPU info
		LOG("** Using GPU: %s (OpenGL %s) **", glGetString(GL_VENDOR), glGetString(GL_VERSION));

		// Check OpenGL 3.1 support
		if(!gl3wIsSupported(3, 1))
		{
			THROW("OpenGL 3.1 not supported\n");
		}

		// Setup default vertex format
		VertexFormat::s_vct.set(VERTEX_POSITION, 2);
		VertexFormat::s_vct.set(VERTEX_COLOR, 4, SAUCE_UBYTE);
		VertexFormat::s_vct.set(VERTEX_TEX_COORD, 2);

		// Setup viewport
		Vector2I size;
		mainWindow->getSize(&size.x, &size.y);
		graphicsContext->resizeViewport(size.x, size.y);

		// Init graphics
		glGenVertexArrays(1, &GraphicsContext::s_vao);
		glBindVertexArray(GraphicsContext::s_vao);
		glGenBuffers(1, &GraphicsContext::s_vbo);
		glGenBuffers(1, &GraphicsContext::s_ibo);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Enable blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable alpha test // Optimization?
		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_LEQUAL, 0.0f);

		glEnable(GL_LINE_SMOOTH);
		//glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glPointSize(4);

		string vertexShader =
			"\n"
			"in vec2 in_Position;\n"
			"in vec2 in_TexCoord;\n"
			"in vec4 in_VertexColor;\n"
			"\n"
			"out vec2 v_TexCoord;\n"
			"out vec4 v_VertexColor;\n"
			"\n"
			"uniform mat4 u_ModelViewProj;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vec4(in_Position, 0.0, 1.0) * u_ModelViewProj;\n"
			"	v_TexCoord = in_TexCoord;\n"
			"	v_VertexColor = in_VertexColor;\n"
			"}\n";

		string fragmentShader =
			"\n"
			"in vec2 v_TexCoord;\n"
			"in vec4 v_VertexColor;\n"
			"\n"
			"out vec4 out_FragColor;\n"
			"\n"
			"uniform sampler2D u_Texture;"
			"\n"
			"void main()\n"
			"{\n"
			"	out_FragColor = texture(u_Texture, v_TexCoord) * v_VertexColor;\n"
			"}\n";

		GraphicsContext::s_defaultShader = shared_ptr<Shader>(new Shader(vertexShader, fragmentShader));

		uchar pixel[4];
		pixel[0] = pixel[1] = pixel[2] = pixel[3] = 255;
		GraphicsContext::s_defaultTexture = shared_ptr<Texture2D>(new Texture2D(1, 1, pixel));

		// Initialize input handler
		m_inputManager = new InputManager("InputConfig.xml");

		m_scene = new Scene(this);
		
		// Set up SDL text input
		SDL_StartTextInput();

		// Engine initialized
		m_initialized = true;

		LOG("** Engine Initialized **");

		// Call onStart event
		{
			GameEvent e(GameEvent::START);
			onEvent(&e);
		}
		
		// Fps sampling
		const uint numFpsSamples = 8;
		double fpsSamples[numFpsSamples];
		for(uint i = 0; i < numFpsSamples; ++i) fpsSamples[i] = 0.0;
		uint currFpsSample = 0;

		// Setup game loop
		m_timer->start();
		const double dt = 1.0 / 30.0;
		double accumulator = 0.0;
		double prevTime = m_timer->getElapsedTime();

		// Make sure update is called once before draw
		{
			TickEvent e(dt);
			onEvent(&e);
		}

		// Game loop
		while(m_running)
		{
			// Event handling
			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_WINDOWEVENT:
					{
						list<Window*> windows(m_windows);
						for(Window *window : windows)
						{
							if(event.window.windowID == window->getID())
							{
								if(window->handleEvent(event, this))
								{
									// The window was closed. Release its resources.
									m_windows.remove(window);
									delete window;

									// If all windows are closed, end the game.
									if(m_windows.size() == 0)
									{
										end();
										goto gameloopend;
									}
								}
							}
						}
					}
					break;

					case SDL_KEYUP: case SDL_KEYDOWN:
					{
						// Send key input event
						KeyEvent e(event.type == SDL_KEYDOWN ? (event.key.repeat == 0 ? KeyEvent::DOWN : KeyEvent::REPEAT) : KeyEvent::UP, m_inputManager, (Scancode)event.key.keysym.scancode, event.key.keysym.mod);
						onEvent(&e);
						m_inputManager->updateKeybinds(&e);
					}
					break;

					case SDL_TEXTINPUT:
					{
						// If no modifiers are pressed
						if((SDL_GetModState() & (KMOD_CTRL | KMOD_ALT)) == 0)
						{
							// Send text input event
							TextEvent e(event.text.text[0]);
							onEvent(&e);
						}
					}
					break;

					case SDL_MOUSEMOTION:
					{
						// Update mouse position
						m_inputManager->m_x = event.motion.x;
						m_inputManager->m_y = event.motion.y;

						// Send mouse move event
						MouseEvent e(MouseEvent::MOVE, m_inputManager, event.motion.x, event.motion.y, SAUCE_MOUSE_BUTTON_NONE, 0, 0);
						onEvent(&e);
					}
					break;

					case SDL_MOUSEBUTTONDOWN:
					{
						// MouseEvent
						MouseEvent mouseEvent(MouseEvent::DOWN, m_inputManager, m_inputManager->m_x, m_inputManager->m_y, (const MouseButton) event.button.button, 0, 0);
						onEvent(&mouseEvent);

						// KeyEvent
						KeyEvent keyEvent(KeyEvent::DOWN, m_inputManager, (const MouseButton) event.button.button, event.key.keysym.mod);
						onEvent(&keyEvent);
						m_inputManager->updateKeybinds(&keyEvent);
					}
					break;

					case SDL_MOUSEBUTTONUP:
					{
						// MouseEvent
						MouseEvent e(MouseEvent::UP, m_inputManager, m_inputManager->m_x, m_inputManager->m_y, (const MouseButton) event.button.button, 0, 0);
						onEvent(&e);

						// KeyEvent
						KeyEvent keyEvent(KeyEvent::UP, m_inputManager, (const MouseButton) event.button.button, event.key.keysym.mod);
						onEvent(&keyEvent);
						m_inputManager->updateKeybinds(&keyEvent);
					}
					break;

					case SDL_MOUSEWHEEL:
					{
						// Scroll event
						MouseEvent e(MouseEvent::WHEEL, m_inputManager, m_inputManager->m_x, m_inputManager->m_y, SAUCE_MOUSE_BUTTON_NONE, event.wheel.x, event.wheel.y);
						onEvent(&e);
					}
					break;

					case SDL_CONTROLLERDEVICEADDED:
					{
						m_inputManager->addController(event.cdevice.which);
						//ControllerDeviceEvent e();
						//onEvent(&e);
					}
					break;

					case SDL_CONTROLLERDEVICEREMOVED:
					{
						m_inputManager->removeController(event.cdevice.which);
						//ControllerDeviceEvent e();
						//onEvent(&e);
					}
					break;

					case SDL_CONTROLLERBUTTONDOWN:
					{
						// Send controller button event
						ControllerButtonEvent e(ControllerButtonEvent::DOWN, m_inputManager, (const ControllerButton)event.cbutton.button);// , event.cbutton.which);
						onEvent(&e);
						m_inputManager->updateKeybinds(&e);
					}
					break;

					case SDL_CONTROLLERBUTTONUP:
					{
						// Send controller button event
						ControllerButtonEvent e(ControllerButtonEvent::UP, m_inputManager, (const ControllerButton) event.cbutton.button);// , event.cbutton.which);
						onEvent(&e);
						m_inputManager->updateKeybinds(&e);
					}
					break;
  
					case SDL_CONTROLLERAXISMOTION:
					{
						// If the axis is a trigger button
						if(event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
						{
							if(!m_inputManager->getButtonState(SAUCE_CONTROLLER_BUTTON_RIGHT_TRIGGER))
							{
								// And the axis exceedes the threshold value
								if(AXIS_VALUE_TO_FLOAT(event.caxis.value) >= m_inputManager->m_triggerThreshold)
								{
									// Flag trigger as pressed and send controller button event
									m_inputManager->m_rightTrigger = true;
									ControllerButtonEvent e(ControllerButtonEvent::DOWN, m_inputManager, SAUCE_CONTROLLER_BUTTON_RIGHT_TRIGGER);// , event.cbutton.which);
									onEvent(&e);
									m_inputManager->updateKeybinds(&e);
								}
							}
							else
							{
								if(AXIS_VALUE_TO_FLOAT(event.caxis.value) < m_inputManager->m_triggerThreshold)
								{
									m_inputManager->m_rightTrigger = false;
									ControllerButtonEvent e(ControllerButtonEvent::UP, m_inputManager, SAUCE_CONTROLLER_BUTTON_RIGHT_TRIGGER);// , event.cbutton.which);
									onEvent(&e);
									m_inputManager->updateKeybinds(&e);
								}
							}
						}
						else if(event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
						{
							if(!m_inputManager->getButtonState(SAUCE_CONTROLLER_BUTTON_LEFT_TRIGGER))
							{
								if(event.caxis.value >= m_inputManager->m_triggerThreshold)
								{
									m_inputManager->m_leftTrigger = true;
									ControllerButtonEvent e(ControllerButtonEvent::DOWN, m_inputManager, SAUCE_CONTROLLER_BUTTON_LEFT_TRIGGER);// , event.cbutton.which);
									onEvent(&e);
									m_inputManager->updateKeybinds(&e);
								}
							}
							else
							{
								if(event.caxis.value < m_inputManager->m_triggerThreshold)
								{
									m_inputManager->m_leftTrigger = false;
									ControllerButtonEvent e(ControllerButtonEvent::UP, m_inputManager, SAUCE_CONTROLLER_BUTTON_LEFT_TRIGGER);// , event.cbutton.which);
									onEvent(&e);
									m_inputManager->updateKeybinds(&e);
								}
							}
						}

						// Send controller axis event
						ControllerAxisEvent e(m_inputManager, (const ControllerAxis) event.caxis.axis, event.caxis.value);// , event.cbutton.which);
						onEvent(&e);
						m_inputManager->updateKeybinds(&e);
					}
					break;
				}
			}

			// Check if game is paused or out of focus
			if(m_paused || (!isEnabled(SAUCE_RUN_IN_BACKGROUND) && !mainWindow->checkFlags(SDL_WINDOW_INPUT_FOCUS)))
			{
				continue;
			}

			// Calculate time delta
			const double currentTime = m_timer->getElapsedTime();
			double deltaTime = currentTime - prevTime;
			prevTime = currentTime;

			// Avoid spiral of death
			if(deltaTime > 0.25)
			{
				deltaTime = 0.25;
			}

			// Step begin
			{
				StepEvent e(StepEvent::BEGIN);
				onEvent(&e);
			}

			// Apply time delta to accumulator
			accumulator += deltaTime;
			while(accumulator >= dt)
			{
				// Update the game
				{
					TickEvent e(dt);
					onEvent(&e);
				}
				accumulator -= dt;
			}

			// Draw the game
			const double alpha = accumulator / dt;
			{
				DrawEvent e(alpha, graphicsContext);
				onEvent(&e);
			}
			SDL_GL_SwapWindow(mainWindow->getSDLHandle());
			glClear(GL_COLOR_BUFFER_BIT);

			// Add fps sample
			if(deltaTime != 0.0f)
			{
				fpsSamples[currFpsSample++] = 1.0 / deltaTime;
				if(currFpsSample >= numFpsSamples)
				{
					currFpsSample = 0;
				}
			}

			// Get average fps
			double fps = 0.0;
			for(uint i = 0; i < numFpsSamples; i++) fps += fpsSamples[i];
			m_framesPerSecond = fps / numFpsSamples;

			// Step end
			{
				StepEvent e(StepEvent::END);
				onEvent(&e);
			}
		}
gameloopend:

		SDL_StopTextInput();
		
		LOG("** Game Ending **");

		// Call onEnd event
		{
			GameEvent e(GameEvent::END);
			onEvent(&e);
		}
	}
	catch(Exception &e)
	{
		stringstream ss;
		ss << e.message() << endl << "------------------------------------------------------------------------------------------------" << endl;
		ss << "Callstack: " << endl << e.callstack();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "An error occured", ss.str().c_str(), m_windows.front()->getSDLHandle());
		LOG("An exception occured: %s", ss.str().c_str());
		return e.errorCode();
	}
	return SAUCE_OK;
}

void Game::end()
{
	m_running = false;
}

void Game::setPaused(const bool paused)
{
	m_paused = paused;
}

uint Game::getFlags() const
{
	return m_flags;
}

bool Game::isEnabled(const EngineFlag flag)
{
	return (m_flags & flag) != 0;
}

Window *Game::getWindow(const Sint32 id) const
{
	if(id < 0)
	{
		return m_windows.front();
	}
	for(Window *window : m_windows)
	{
		if(window->getID() == id)
		{
			return window;
		}
	}
	return 0;
}

END_SAUCE_NAMESPACE
