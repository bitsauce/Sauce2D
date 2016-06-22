in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

void main()
{
	//translate u and v into [-1 , 1] domain
	float u0 = v_TexCoord.x * 2.0 - 1.0;
	float v0 = v_TexCoord.y * 2.0 - 1.0;

	//then, as u0 approaches 0 (the center), v should also approach 0 
	v0 = v0 * abs(u0);

	//convert back from [-1,1] domain to [0,1] domain
	v0 = (v0 + 1.0) / 2.0;

	//we now have the coordinates for reading from the initial image
	vec2 newCoords = vec2(v_TexCoord.x, v0);

	//read for both horizontal and vertical direction and store them in separate channels
	float horizontal = texture(u_Texture, newCoords).r;
	float vertical = texture(u_Texture, newCoords.yx).r;
	out_FragColor = vec4(horizontal, vertical, 0.0, 1.0);
}