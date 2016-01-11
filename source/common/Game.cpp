//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>
#include <CGF/Graphics.h>
#include <CGF/Input.h>
#include <CGF/Audio.h>

BEGIN_CGF_NAMESPACE

Exception::Exception(RetCode code, const char * msg, ...) :
	m_errorCode(code)
{
	va_list args;
	va_start(args, msg);

	int size = _scprintf(msg, args) + 1;
	m_message.resize(size);
	vsprintf_s(&m_message[0], size, msg, args);

	va_end(args);
}

Game *Game::s_game = 0;

// TODO: Might want to do some validation check on the name
// and org name so that the pref path won't bug out
Game::Game(const string &name, const string &organization) :
	m_name(name),
	m_organization(organization),
	m_flags(0),
	m_initialized(false),
	m_paused(false),
	m_running(false)
{
	if(s_game)
	{
		THROW("A game already exists!");
	}
	s_game = this;
}

Game::~Game()
{
	// Release managers
	delete m_fileSystem;
	//delete m_audio;
	delete m_timer;
	delete m_console;
	s_game = 0;
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
		// set the value of CGF_EXPORT_LOG and CGF_VERBOSE, etc.
		//
		// ConfigFile default("config:/DefaultConfig.ini");
		// default.getValue("Window/ResolutionX");
		// etc...

		// Set game root directory
		m_binaryPath = SDL_GetBasePath();

		// Set save directory
		m_prefPath = SDL_GetPrefPath(m_organization.c_str(), m_name.c_str());

		m_console = new Console();
		m_fileSystem = new FileSystem();
		if(true)//isEnabled(CGF_EXPORT_LOG))
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

		// Initialize window
		Window *mainWindow = new Window(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
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
		VertexFormat::s_vct.set(VERTEX_COLOR, 4, CGF_UBYTE);
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

		GraphicsContext::s_defaultShader = ShaderPtr(new Shader(vertexShader, fragmentShader));

		uchar pixel[4];
		pixel[0] = pixel[1] = pixel[2] = pixel[3] = 255;
		GraphicsContext::s_defaultTexture = Texture2DPtr(new Texture2D(1, 1, pixel));

		// Initialize input handler
		m_inputManager = new InputManager("config:/InputDefault.ini");//(m_inputConfig);

		m_scene = new Scene(this);

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

					case SDL_KEYDOWN:
					{
						if(event.key.repeat == 0)
						{
							KeyEvent e(KeyEvent::DOWN, (Keycode) event.key.keysym.sym, (Scancode) event.key.keysym.scancode, event.key.keysym.mod);
							onEvent(&e);
							m_inputManager->updateKeybinds(&e);
						}
						else
						{
							KeyEvent e(KeyEvent::REPEAT, (Keycode) event.key.keysym.sym, (Scancode) event.key.keysym.scancode, event.key.keysym.mod);
							onEvent(&e);
							m_inputManager->updateKeybinds(&e);
						}
					}
					break;

					case SDL_KEYUP:
					{
						KeyEvent e(KeyEvent::UP, (Keycode) event.key.keysym.sym, (Scancode) event.key.keysym.scancode, event.key.keysym.mod);
						onEvent(&e);
						m_inputManager->updateKeybinds(&e);
					}
					break;

					case SDL_TEXTINPUT:
					{
						if((SDL_GetModState() & (KMOD_CTRL | KMOD_ALT)) == 0)
						{
							TextEvent e(event.text.text[0]);
							onEvent(&e);
						}
					}
					break;

					case SDL_MOUSEMOTION:
					{
						m_inputManager->m_x = event.motion.x;
						m_inputManager->m_y = event.motion.y;
						MouseEvent e(MouseEvent::MOVE, event.motion.x, event.motion.y, MOUSE_BUTTON_NONE, 0, 0);
						onEvent(&e);
					}
					break;

					case SDL_MOUSEBUTTONDOWN:
					{
						MouseEvent e(MouseEvent::DOWN, m_inputManager->m_x, m_inputManager->m_y, (const MouseButton) event.button.button, 0, 0);
						onEvent(&e);
					}
					break;

					case SDL_MOUSEBUTTONUP:
					{
						MouseEvent e(MouseEvent::UP, m_inputManager->m_x, m_inputManager->m_y, (const MouseButton) event.button.button, 0, 0);
						onEvent(&e);
					}
					break;

					case SDL_MOUSEWHEEL:
					{
						MouseEvent e(MouseEvent::WHEEL, m_inputManager->m_x, m_inputManager->m_y, MOUSE_BUTTON_NONE, event.wheel.x, event.wheel.y);
						onEvent(&e);
					}
					break;
				}
			}

			// Check if game is paused or out of focus
			if(m_paused || (!isEnabled(CGF_RUN_IN_BACKGROUND) && !mainWindow->checkFlags(SDL_WINDOW_INPUT_FOCUS)))
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
	catch(Exception e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "An error occured", e.message().c_str(), m_windows.front()->getSDLHandle());
		LOG("An exception occured: %s", e.message().c_str());
		return e.errorCode();
	}
	catch(...)
	{
		LOG("Unknown exception occured.");
		return CGF_UNKNOWN_EXCEPTION;
	}
	return CGF_OK;
}

void Game::end()
{
	m_running = false;
}

void Game::setPaused(const bool paused)
{
	m_paused = paused;
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

END_CGF_NAMESPACE
