class ImageScene : Scene
{
	Shader @shader = @Shader(":/shaders/simplex3d_blend.vert", ":/shaders/simplex3d_blend.frag");
	Batch @batch = @Batch();
	float time = 0.0f;

	Texture @image0 = @Texture(":/images/koala.png");
	Texture @image1 = @Texture(":/images/bliss.png");
	Texture @gradient = @Texture(":/images/gradient.png");
	
	void show()
	{
	}
	
	void hide()
	{
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
		font.draw(@fontBatch, Vector2(20.0f), "Frequency: " + frequency + "\nGain: " + gain + "\nLacunarity: " + lacunarity + "\nOctaves: " + octaves);
		font.draw(@fontBatch, Vector2(Window.getSize().x - 250.0f, 20.0f), "Controls:\nA/S: Change Frequency\nZ/X: Change Gain\nE/R: Change Lacunarity\nD/F: Change Octaves\nC/V: Stop/Start Time\nESC: Back");
		fontBatch.draw();
	}

	void update()
	{
		// Apply time dt
		if(!stopTime) time += Graphics.dt;
		shader.setUniform1f("u_time", time);
	}

	void updateUniforms()
	{
		shader.setSampler2D("u_gradient", @gradient);
		shader.setSampler2D("u_image0", @image0);
		shader.setSampler2D("u_image1", @image1);
		shader.setUniform1f("u_frequency", frequency);
		shader.setUniform1f("u_gain", gain);
		shader.setUniform1f("u_lacunarity", lacunarity);
		shader.setUniform1i("u_octaves", octaves);
	}
}