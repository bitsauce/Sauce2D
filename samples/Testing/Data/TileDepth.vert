in vec2 in_TexCoord;
in vec3 in_Position;

out vec2 v_TexCoord;
out float v_Depth;

uniform mat4 u_ModelViewProj;

void main()
{
	gl_Position = vec4(in_Position.xy, 0.0, 1.0) * u_ModelViewProj;
	v_Depth = in_Position.z;
	v_TexCoord = in_TexCoord;
}