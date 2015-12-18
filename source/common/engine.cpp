//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>
#include <x2d/audio.h>

BEGIN_CG_NAMESPACE

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

// TODO: Might want to do some validation check on the name
// and org name so that the pref path won't bug out
Game::Game(const string &name, const string &organization) :
	m_name(name),
	m_organization(organization),
	m_flags(0),
	m_rootDir(""),
	m_saveDir(""),
	m_initialized(false),
	m_paused(false),
	m_running(false)
{
}

Game::~Game()
{
	// Release managers
	delete m_fileSystem;
	//delete m_audio;
	delete m_timer;
	delete m_console;
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
		// set the value of XD_EXPORT_LOG and XD_VERBOSE, etc.
		//
		// ConfigFile default("config:/DefaultConfig.ini");
		// default.getValue("Window/ResolutionX");
		// etc...

		// Set game root directory
		m_rootDir = SDL_GetBasePath();
#ifdef __WINDOWS__
		m_rootDir += "..\\..\\..\\";
#else
		m_rootDir += "../../../";
#endif

		// Set save directory
		m_saveDir = SDL_GetPrefPath(m_organization.c_str(), m_name.c_str());

		m_console = new Console();
		m_fileSystem = new FileSystem();
		if(isEnabled(XD_EXPORT_LOG))
		{
			m_console->m_output = new FileWriter(util::getAbsoluteFilePath("binary:/Console.log"));
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

		m_window = new Window(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
		GraphicsContext *graphicsContext = m_window->getGraphicsContext();

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
		VertexFormat::s_vct.set(VERTEX_COLOR, 4, XD_UBYTE);
		VertexFormat::s_vct.set(VERTEX_TEX_COORD, 2);

		// Setup viewport
		Vector2i size;
		m_window->getSize(&size.x, &size.y);
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
		Input::init("config:/InputDefault.ini");//(m_inputConfig);

		// Engine initialized
		m_initialized = true;

		LOG("** Engine Initialized **");

		// Call onStart event
		onStart(*graphicsContext);

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
		onUpdate(dt);

		// Game loop
		while(m_running)
		{
			// Event
			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_WINDOWEVENT:
					{
						if(event.window.type == SDL_WINDOWEVENT_SIZE_CHANGED)
						{
							// Resize viewport
							int width = event.window.data1, height = event.window.data2;
							graphicsContext->resizeViewport(width, height);

							// Call onSizeChanged event
							onSizeChanged(width, height);
						}
					}
					break;

					case SDL_KEYDOWN:
					{
						// Call onKeyDown event
						onKeyDown(event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.scancode);
					}
					break;

					case SDL_KEYUP:
					{
						// Call onKeyUp event
						onKeyUp(event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.scancode);
					}
					break;

					case SDL_MOUSEMOTION:
					{
						Input::s_position.set((float) event.motion.x, (float) event.motion.y);
						onMouseMove((float) event.motion.x, (float) event.motion.y);
					}
					break;

					case SDL_MOUSEBUTTONDOWN:
					{
						onMouseDown(event.button.button);
					}
					break;

					case SDL_MOUSEBUTTONUP:
					{
						onMouseUp(event.button.button);
					}
					break;

					case SDL_MOUSEWHEEL:
					{
						onMouseWheel(event.wheel.x, event.wheel.y);
					}
					break;
				}
			}

			// Check if game is paused or out of focus
			if(m_paused || (!isEnabled(XD_RUN_IN_BACKGROUND) && !m_window->checkFlags(SDL_WINDOW_INPUT_FOCUS)))
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
			onStepBegin();

			// Apply time delta to accumulator
			accumulator += deltaTime;
			while(accumulator >= dt)
			{
				// Update the game
				Input::updateBindings();
				onUpdate(dt);
				accumulator -= dt;
			}

			// Draw the game
			const double alpha = accumulator / dt;
			onDraw(*graphicsContext, alpha);
			SDL_GL_SwapWindow(m_window->getSDLHandle());
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
			onStepEnd();
		}


		LOG("** Game Ending **");

		// Call onEnd event
		onEnd();
	}
	catch(Exception e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "An error occured", e.message().c_str(), m_window->getSDLHandle());
		LOG("An exception occured: %s", e.message().c_str());
		return e.errorCode();
	}
	catch(...)
	{
		LOG("Unknown exception occured.");
		return X2D_UNKNOWN_EXCEPTION;
	}
	return X2D_OK;
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

END_CG_NAMESPACE