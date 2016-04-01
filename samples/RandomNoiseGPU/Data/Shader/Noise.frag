// Source: http://www.reedbeta.com/blog/2013/01/12/quick-and-easy-gpu-random-numbers-in-d3d11/

in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform float u_Time;

uint hash(uint seed)
{
    seed = (seed ^ 61U) ^ (seed >> 16U);
    seed *= 9U;
    seed = seed ^ (seed >> 4U);
    seed *= 0x27D4EB2DU;
    seed = seed ^ (seed >> 15U);
    return seed;
}

uint hash(uvec2 v)
{
	return hash(v.x ^ hash(v.y));
}

float rand(vec2 n)
{ 
	n = abs(n);
    return float(hash(uvec2(n.x * 1280.0, n.y * 720.0))) * (1.0 / 4294967296.0);
}

void main(void)
{
	out_FragColor = vec4(vec3(rand(v_TexCoord - 0.5)), 1.0);
}