in vec2 v_TexCoord;
in float v_Depth;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

void main()
{
	if(texture(u_Texture, v_TexCoord).a == 0.0) discard;
	
	gl_FragColor = vec4(vec3(v_Depth), 1.0);
}