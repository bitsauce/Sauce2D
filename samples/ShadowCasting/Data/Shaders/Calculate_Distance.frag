in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

// Calculates the distance from a shadow casting pixel to the light source
void main()
{
	out_FragColor = vec4(texture(u_Texture, v_TexCoord).a > 0.0 ? length(v_TexCoord - 0.5) : 1.0, 0.0, 0.0, 1.0);
}