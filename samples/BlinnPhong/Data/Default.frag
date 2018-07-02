in vec2 v_TexCoord;
			
out vec4 out_FragColor;
			
uniform sampler2D u_Texture;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
			
void main()
{
	const float ambientFactor = 0.25;

	vec3 ambientLight = u_LightColor * ambientFactor;
	vec3 surfaceColor = texture(u_Texture, v_TexCoord).rgb;

	vec3 finalColor = surfaceColor * ambientLight;

	out_FragColor = vec4(finalColor, 1.0);
}
