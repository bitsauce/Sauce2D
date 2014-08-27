Shader @shader = @Shader(":/simplex3d.vert", ":/simplex3d.frag");
Batch @batch = @Batch();
float time = 0.0f;
Font @font = @Font("Arial Bold", 16);
Batch @fontBatch = @Batch();

float frequency = 0.5f;
void incFreq() { frequency += 0.1f; updateUniforms(); }
void decFreq() { frequency -= 0.1f; updateUniforms(); }

float gain = 0.5f;
void incGain() { gain += 0.1f; updateUniforms(); }
void decGain() { gain -= 0.1f; updateUniforms(); }

float lacunarity = 2.0f;
void incLacunarity() { lacunarity += 0.1f; updateUniforms(); }
void decLacunarity() { lacunarity -= 0.1f; updateUniforms(); }

int octaves = 8;
void incOctaves() { octaves++; updateUniforms(); }
void decOctaves() { octaves--; updateUniforms(); }

void main()
{
	// Bind input
	Input.bind(KEY_Q, @incGradient);
	Input.bind(KEY_W, @decGradient);
	Input.bind(KEY_S, @incFreq);
	Input.bind(KEY_A, @decFreq);
	Input.bind(KEY_X, @incGain);
	Input.bind(KEY_Z, @decGain);
	Input.bind(KEY_R, @incLacunarity);
	Input.bind(KEY_E, @decLacunarity);
	Input.bind(KEY_F, @incOctaves);
	Input.bind(KEY_D, @decOctaves);
	updateUniforms();
	// Set shader
	batch.setShader(@shader);
	
	// Draw rect to batch
	Shape @shape = @Shape(Rect(Vector2(0.0f), Vector2(Window.getSize())));
	shape.draw(@batch);
}

void draw()
{
	// Draw batch
	batch.draw();
	
	// Draw font
	fontBatch.clear();
	font.draw(@fontBatch, Vector2(20.0f), "Colors: " + GRADIENT_NAME[gradient] + "\nFrequency: " + frequency + "\nGain: " + gain + "\nLacunarity: " + lacunarity + "\nOctaves: " + octaves);
	font.draw(@fontBatch, Vector2(Window.getSize().x - 250.0f, 20.0f), "Controls:\nQ/W: Change Colors\nA/S: Change Frequency\nZ/X: Change Gain\nE/R: Change Lacunarity\nD/F: Change Octaves");
	fontBatch.draw();
}

void update()
{
	// Apply time dt
	time += Graphics.dt;
	shader.setUniform1f("u_time", time);
}
enum Gradient
{
	DEFAULT = 0,
	WATER,
	LAVA,
	GRADIENT_MAX
}

array<string> GRADIENT_NAME = {
	"Default",
	"Water",
	"Lava"
};
array<Texture@> GRADIENT_TEXTURES = {
	@Texture(":/gradients/default.png"),
	@Texture(":/gradients/water.png"),
	@Texture(":/gradients/lava.png")
};
Gradient gradient = DEFAULT;
void incGradient()
{
	gradient = Gradient(gradient+1);
	if(gradient >= GRADIENT_MAX)
	{
		gradient = DEFAULT;
	}
	updateUniforms();
}
void decGradient()
{
	gradient = Gradient(gradient-1);
	if(gradient < DEFAULT)
	{
		gradient = Gradient(GRADIENT_MAX-1);
	}
	updateUniforms();}

void updateUniforms()
{
	shader.setSampler2D("u_gradient", @GRADIENT_TEXTURES[gradient]);
	shader.setUniform1f("u_frequency", frequency);
	shader.setUniform1f("u_gain", gain);
	shader.setUniform1f("u_lacunarity", lacunarity);
	shader.setUniform1i("u_octaves", octaves);
}