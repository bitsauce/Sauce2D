in vec3 in_Position;
in vec2 in_TexCoord;
in vec4 in_VertexColor;
			
out vec2 v_TexCoord;
out vec4 v_VertexColor;
			
uniform mat4 u_ModelViewProj;
			
void main()
{
	gl_Position = vec4(in_Position, 1.0) * u_ModelViewProj;
	v_TexCoord = in_TexCoord;
	v_VertexColor = in_VertexColor;
}
