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
    return float(hash(uvec2(n.x * 2394.241223, n.y * 2394.241223))) * (1.0 / 4294967296.0);
}

float noise( in vec2 p )
{
    vec2 i = floor( p );
    vec2 f = fract( p );
	f = f * f * (3.0 - 2.0 * f);
    return mix(mix(rand(i + vec2(0.0, 0.0)),
                   rand(i + vec2(1.0, 0.0)), f.x),
               mix(rand(i + vec2(0.0, 1.0)),
                   rand(i + vec2(1.0, 1.0)), f.x), f.y);
}

void main()
{
	vec2 uv = v_TexCoord * 50.0;
    mat2 m = mat2(1.6,  1.2, -1.2,  1.6);

	float f = 0.0;
	f += 0.5000 * noise( uv ); uv = m*uv;
	f += 0.2500 * noise( uv ); uv = m*uv;
	f += 0.1250 * noise( uv ); uv = m*uv;
	f += 0.0625 * noise( uv ); uv = m*uv;

	out_FragColor = vec4(vec3(f), 1.0);
}