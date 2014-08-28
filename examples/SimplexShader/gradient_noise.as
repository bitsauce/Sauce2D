class GradientScene : Scene
{
	// Load shader
	Shader @shader = @Shader(":/shaders/simplex3d.vert", ":/shaders/simplex3d.frag");
	Batch @batch = @Batch();
	float time = 0.0f;

	void show()
	{
		// Bind input
		Input.bind(KEY_W, @incGradient);
		Input.bind(KEY_Q, @decGradient);
	}
	
	void hide()
	{
		Input.bind(KEY_W, null);
		Input.bind(KEY_Q, null);
	}

	void draw()
	{
		// Clear batch
		batch.clear();
		
		// Update uniforms
		updateUniforms();

		// Set shader
		batch.setShader(@shader);

		// Draw fullscreen rect to batch
		Shape @shape = @Shape(Rect(Vector2(0.0f), Vector2(Window.getSize())));
		shape.draw(@batch);
		
		// Draw batch
		batch.draw();
		
		// Draw font
		fontBatch.clear();
		font.draw(@fontBatch, Vector2(20.0f), "Gradient: " + GRADIENT_NAME[gradient] + "\nFrequency: " + frequency + "\nGain: " + gain + "\nLacunarity: " + lacunarity + "\nOctaves: " + octaves);
		font.draw(@fontBatch, Vector2(Window.getSize().x - 250.0f, 20.0f), "Controls:\nQ/W: Change Colors\nA/S: Change Frequency\nZ/X: Change Gain\nE/R: Change Lacunarity\nD/F: Change Octaves\nC/V: Stop/Start Time\nESC: Back");
		fontBatch.draw();
	}

	void update()
	{
		// Apply time dt
		if(!stopTime)
			time += Graphics.dt;
	}
	
	void updateUniforms()
	{
		shader.setSampler2D("u_gradient", @GRADIENT_TEXTURES[gradient]);
		shader.setUniform1f("u_frequency", frequency);
		shader.setUniform1f("u_gain", gain);
		shader.setUniform1f("u_lacunarity", lacunarity);
		shader.setUniform1i("u_octaves", octaves);
		shader.setUniform1f("u_time", time);
	}
}
enum Gradient
{
	DEFAULT = 0,
	WATER,
	ICE,
	LAVA,
	DISCRETE,
	GRADIENT_MAX
}

array<string> GRADIENT_NAME = {
	"Default",
	"Water",
	"Ice",
	"Lava",
	"Discrete"
};
array<Texture@> GRADIENT_TEXTURES = {
	@Texture(":/gradients/default.png"),
	@Texture(":/gradients/water.png"),
	@Texture(":/gradients/ice.png"),
	@Texture(":/gradients/lava.png"),
	@Texture(":/gradients/discrete.png")
};
Gradient gradient = DEFAULT;
void incGradient()
{
	gradient = Gradient(gradient+1);
	if(gradient >= GRADIENT_MAX)
	{
		gradient = DEFAULT;
	}
}
void decGradient()
{
	gradient = Gradient(gradient-1);
	if(gradient < DEFAULT)
	{
		gradient = Gradient(GRADIENT_MAX-1);
	}}