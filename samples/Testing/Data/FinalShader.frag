in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_TileColorTexture;
uniform sampler2D u_TileDepthTexture;
uniform sampler2D u_BuildingColorTexture;
uniform sampler2D u_BuildingDepthTexture;
uniform sampler2D u_UnitColorTexture;
uniform sampler2D u_UnitDepthTexture;

void main()
{
	float tileDepth = texture(u_TileDepthTexture, v_TexCoord).r, buildingDepth = texture(u_BuildingDepthTexture, v_TexCoord).r, unitDepth = texture(u_UnitDepthTexture, v_TexCoord).r;
	if(unitDepth >= buildingDepth && unitDepth >= tileDepth)
	{
		gl_FragColor = texture(u_UnitColorTexture, v_TexCoord);
	}
	else if(buildingDepth >= unitDepth && buildingDepth >= tileDepth)
	{
		gl_FragColor = texture(u_BuildingColorTexture, v_TexCoord);
	}
	else if(tileDepth >= unitDepth && tileDepth >= buildingDepth)
	{
		gl_FragColor = texture(u_TileColorTexture, v_TexCoord);
	}

	/*if(unitDepth >= buildingDepth && unitDepth >= tileDepth)
	{
		gl_FragColor = texture(u_UnitDepthTexture, v_TexCoord);
	}
	else if(buildingDepth >= unitDepth && buildingDepth >= tileDepth)
	{
		gl_FragColor = texture(u_BuildingDepthTexture, v_TexCoord);
	}
	else if(tileDepth >= unitDepth && tileDepth >= buildingDepth)
	{
		gl_FragColor = texture(u_TileDepthTexture, v_TexCoord);
	}*/
}