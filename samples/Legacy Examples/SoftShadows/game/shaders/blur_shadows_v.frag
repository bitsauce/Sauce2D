#version 120

varying vec2 v_texCoord;

uniform float u_width;
uniform sampler2D u_texture;

const float minBlur = 0.0;
const float maxBlur = 5.0;
const int kernelSize = 13;
const vec2 offsetAndWeight[kernelSize] = vec2[](
	vec2(-6.0, 0.002216),
	vec2(-5.0, 0.008764),
	vec2(-4.0, 0.026995),
	vec2(-3.0, 0.064759),
	vec2(-2.0, 0.120985),
	vec2(-1.0, 0.176033),
	vec2(0.0,  0.199471),
	vec2(1.0,  0.176033),
	vec2(2.0,  0.120985),
	vec2(3.0,  0.064759),
	vec2(4.0,  0.026995),
	vec2(5.0,  0.008764),
	vec2(6.0,  0.002216)
);

void main()
{
	float sum = 0.0;
	float distance = texture2D(u_texture, v_texCoord).b;
	for(int i = 0; i < kernelSize; i++)
	{    
		sum += texture2D(u_texture, v_texCoord + offsetAndWeight[i].x * mix(minBlur, maxBlur, distance)/u_width * vec2(1.0, 0.0)).r * offsetAndWeight[i].y;
	}
	float d = 2.0 * length(v_texCoord - 0.5);
	float attenuation = pow(clamp(1.0 - d, 0.0, 1.0), 1.0);
	gl_FragColor = vec4(vec3(sum * attenuation), 1.0);
}