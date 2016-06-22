in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

float getShadowDistanceH(vec2 texCoord, float displacementV)
{
	float u = texCoord.x;
	float v = texCoord.y;

	u = abs(u - 0.5) * 2.0;
	v = v * 2.0 - 1.0;
	float v0 = v/u;
	v0 += displacementV;
	v0 = (v0 + 1.0) / 2.0;

	vec2 newCoords = vec2(texCoord.x, v0);
	//horizontal info was stored in the Red component
	return texture(u_Texture, newCoords).r;
}

float getShadowDistanceV(vec2 texCoord, float displacementV)
{
	float u = texCoord.y;
	float v = texCoord.x;

	u = abs(u - 0.5) * 2.0;
	v = v * 2.0 - 1.0;
	float v0 = v/u;
	v0 += displacementV;
	v0 = (v0 + 1.0) / 2.0;

	vec2 newCoords = vec2(texCoord.y, v0);
	// vertical info was stored in the Green component
	return texture(u_Texture, newCoords).g;
}

void main()
{
	// distance of this pixel from the center
	float distance = length(v_TexCoord - 0.5);
	//distance *= u_width;
	//distance -= 2.0; // apply a 2-pixel bias

	// distance stored in the shadow map
	float shadowMapDistance;

	// coords in [-1,1]
	float nY = 2.0 *(v_TexCoord.y - 0.5);
	float nX = 2.0 *(v_TexCoord.x - 0.5);

	// we use these to determine which quadrant we are in
	if(abs(nY) < abs(nX))
	{
		shadowMapDistance = getShadowDistanceH(v_TexCoord, 0.0);
	}
	else
	{
		shadowMapDistance = getShadowDistanceV(v_TexCoord, 0.0);
	}

	// if distance to this pixel is lower than distance from shadowMap, 
	// then we are not in shadow
	float light = distance < shadowMapDistance ? 1.0 : 0.0;
	light += 0.5;

	out_FragColor = vec4(light, light, length(v_TexCoord - 0.5), 1.0);
}