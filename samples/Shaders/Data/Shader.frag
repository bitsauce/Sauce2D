in vec2 v_TexCoord;
out vec4 out_FragColor;

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	const vec2 offset = vec2(-1.0, -0.5), resolution = vec2(1280.0, 720.0);
	float time = 0.0, scale = 1.0;

	vec2 pos = vec2(v_TexCoord.x * (resolution.x / resolution.y), v_TexCoord.y) * scale + offset;

	vec2 z = vec2(0.0, 0.0);
	int r = 100;
	const int iterations = 100;
	for (int i = 1; i <= iterations; i++)
	{
		if(length(z) >= 2.0) { r = i; break; }
		z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + pos;
	}
	float smoothResult = (float(r) - log(log(length(z))) / log(2.0)) / float(iterations);
	gl_FragColor = vec4(hsv2rgb(vec3(mod(time * 0.1 + float(smoothResult), 1.0), 1.0, 1.0)), 1.0);
}