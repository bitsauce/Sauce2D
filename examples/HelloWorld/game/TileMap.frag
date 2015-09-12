in vec2 v_TexCoord;

out vec4 out_FragColor;

uniform usampler2D u_TileMap;

void main()
{
	//out_fragColor = vec4(vec3(float(texture(u_tileMap, v_texCoord).r) / 255.0), 1.0);
	out_FragColor = vec4(float(texture(u_TileMap, v_TexCoord)) / 255.0);
}