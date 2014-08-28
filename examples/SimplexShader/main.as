#include "gradient_noise.as"
#include "image_blend.as"

// Simplex uniforms
float frequency = 0.5f;
void incFreq() { frequency += 0.1f; }
void decFreq() { frequency -= 0.1f; }

float gain = 0.5f;
void incGain() { gain += 0.1f; }
void decGain() { gain -= 0.1f; }

float lacunarity = 2.0f;
void incLacunarity() { lacunarity += 0.1f; }
void decLacunarity() { lacunarity -= 0.1f; }

int octaves = 8;
void incOctaves() { octaves++; }
void decOctaves() { octaves--; }

bool stopTime = false;
void stop() { stopTime = true; }
void start() { stopTime = false; }

// Font drawing
Font @font = @Font("Arial Bold", 16);
Batch @fontBatch = @Batch();

void main()
{
	// Set simplex bindings
	Input.bind(KEY_S, @incFreq);
	Input.bind(KEY_A, @decFreq);
	Input.bind(KEY_X, @incGain);
	Input.bind(KEY_Z, @decGain);
	Input.bind(KEY_R, @incLacunarity);
	Input.bind(KEY_E, @decLacunarity);
	Input.bind(KEY_F, @incOctaves);
	Input.bind(KEY_D, @decOctaves);
	Input.bind(KEY_C, @stop);
	Input.bind(KEY_V, @start);
	
	Input.bind(KEY_ESCAPE, @back);
}

void draw()
{
	// Draw font
	fontBatch.clear();
	font.draw(@fontBatch, Vector2(20.0f), "Press 1 for gradient noise\nPress 2 for image blend");
	fontBatch.draw();
}

void update()
{
	if(Input.getKeyState(KEY_1))
	{
		Engine.pushScene(@GradientScene());
	}
	else if(Input.getKeyState(KEY_2))
	{
		Engine.pushScene(@ImageScene());
	}
}

void back()
{
	Engine.popScene();
}