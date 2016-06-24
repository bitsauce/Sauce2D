in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform float u_Width;
uniform sampler2D u_Texture;

const float minBlur = 0.0;
const float maxBlur = 5.0;

#define KERNEL_SIZE 13
float OFFSET[KERNEL_SIZE] = float[](
	-6.0,
	-5.0,
	-4.0,
	-3.0,
	-2.0,
	-1.0,
	 0.0,
	 1.0, 
	 2.0,
	 3.0,
	 4.0,
	 5.0,
	 6.0
);

float WEIGHT[KERNEL_SIZE] = float[](
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033, 
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216
);

void main()
{
	float sum = 0.0;
	float distance = texture(u_Texture, v_TexCoord).b;
	for(int i = 0; i < KERNEL_SIZE; i++)
	{    
		sum += texture(u_Texture, v_TexCoord + OFFSET[i] * mix(minBlur, maxBlur, distance) / u_Width * vec2(1.0, 0.0)).r * WEIGHT[i];
	}
	out_FragColor = vec4(sum, sum, distance, 1.0);
}