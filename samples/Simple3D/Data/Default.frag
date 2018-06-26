//in vec2 v_TexCoord;
in vec4 v_VertexColor;
			
out vec4 out_FragColor;
			
//uniform sampler2D u_Texture;
			
void main()
{
	out_FragColor = v_VertexColor;
	//out_FragColor = texture(u_Texture, v_TexCoord) * v_VertexColor;
}
