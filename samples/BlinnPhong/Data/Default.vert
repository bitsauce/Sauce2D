in vec3 in_Position;
in vec2 in_TexCoord;

out vec2 v_TexCoord;
uniform mat4 u_ModelViewProj;
			
void main()
{
	gl_Position = vec4(in_Position, 1.0) * u_ModelViewProj;
	v_TexCoord = in_TexCoord;
}
