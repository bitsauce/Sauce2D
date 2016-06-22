in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform vec2 u_TextureDimentions;
uniform sampler2D u_Texture;

void main()
{
	vec2 color = texture(u_Texture, v_TexCoord).xy;
	vec2 colorR = texture(u_Texture, v_TexCoord + vec2(u_TextureDimentions.x, 0.0)).xy;
	vec2 result = min(color, colorR);
    out_FragColor = vec4(result, 0.0, 1.0);
}