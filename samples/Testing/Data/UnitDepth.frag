in vec2 v_TexCoord;
in vec2 v_Position;
in float v_Depth;
out vec4 out_FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_TileDepthTexture;

#define GRID_WIDTH (1 * 120)
#define GRID_HEIGHT (1 * 60)

void main()
{
	if(texture(u_Texture, v_TexCoord).a == 0.0) discard;

	//ivec2 tilePos = ivec2(floor((v_Position.x+ GRID_WIDTH / 2) / GRID_WIDTH + v_Position.y / GRID_HEIGHT), floor((v_Position.y + GRID_HEIGHT / 2) / GRID_HEIGHT - v_Position.x / GRID_WIDTH));
	
	gl_FragColor = vec4(vec3(v_Depth), 1.0);
}