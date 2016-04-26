in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

void main()
{
	out_FragColor = vec4(vec3(float(texture(u_Texture, v_TexCoord).a == 1.0) * 127.0 / 255.5), 1.0);
}