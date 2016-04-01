in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform bool u_ShowNoise;
uniform vec2 u_Position;
uniform vec2 u_Resolution;
uniform float u_Time;
uniform float u_Scale;

uint hash(uint x)
{
    x += (x << 10U);
    x ^= (x >>  6U);
    x += (x <<  3U);
    x ^= (x >> 11U);
    x += (x << 15U);
    return x;
}

float rand(float x)
{
	return float(hash(uint(abs(x * 1280.241223)))) * (1.0 / 4294967296.0);
}

float rand(vec2 pos)
{
	return float(hash((uint(abs(pos.x * 1280.241223)) << 16U) + uint(abs(pos.y * 720.241223)))) * (1.0 / 4294967296.0);
}

float fractalNoise1D(int octaves, float x)
{
	float value = 0.0;
	float amplitude = 0.5 / (1.0 - pow(2.0, -float(octaves)));
	for(int i = 0; i < octaves; i++)
	{
		float j = floor(x);
		float f = smoothstep(0.0, 1.0, fract(x));
		value += amplitude * mix(rand(j), rand(j + 1.0), f);
		x *= 2.0;
		amplitude *= 0.5;
	}
	return value;
}

float fractalNoise2D(int octaves, vec2 pos)
{
	float value = 0.0;
	float amplitude = 0.5 / (1.0 - pow(2.0, -float(octaves)));
	for(int i = 0; i < octaves; i++)
	{
		vec2 j = floor(pos);
		vec2 f = smoothstep(0.0, 1.0, fract(pos));
		value += amplitude * mix(
								mix(rand(j + vec2(0.0, 0.0)),
									rand(j + vec2(1.0, 0.0)), f.x),
								mix(rand(j + vec2(0.0, 1.0)),
									rand(j + vec2(1.0, 1.0)), f.x),
								f.y);
		pos *= 2.0;
		amplitude *= 0.5;
	}
	return value;
}

#define MOUNTAIN_TOP 100
#define MOUNTAIN_BOTTOM 500

const vec4 BLOCK_EMPTY = vec4(0.78, 0.97, 0.96, 1.0);
const vec4 BLOCK_GRASS = vec4(0.1, 0.8, 0.2, 1.0);
const vec4 BLOCK_DIRT = vec4(0.3, 0.25, 0.19, 1.0);
const vec4 BLOCK_STONE = vec4(0.5, 0.5, 0.5, 1.0);

void main()
{
	vec2 blockPos = floor(v_TexCoord * u_Resolution * u_Scale + u_Position);
	
	// Get noise value
	float value = fractalNoise1D(4, 5.0 * blockPos.x / u_Resolution.x);

	// Apply gradient
	value += 2.0 * (MOUNTAIN_BOTTOM - blockPos.y) / (MOUNTAIN_BOTTOM - MOUNTAIN_TOP) - 1.0;

	// Get block color
	vec4 block;
	if(value > 0.5)
	{
		block = BLOCK_EMPTY;
	}
	else if(value > 0.25)
	{
		block = BLOCK_DIRT;
	}
	else
	{
		block = BLOCK_STONE;
	}

	// Set block color
	if(u_ShowNoise)
	{
		out_FragColor = vec4(vec3(value), 1.0);
	}
	else 
	{
		out_FragColor = block;
	}
}