varying vec2 v_texCoord;

uniform vec2 u_textureDimentions;
uniform sampler2D u_texture;

void main()
{
	vec2 color = texture2D(u_texture, v_texCoord).xy;
	vec2 colorR = texture2D(u_texture, v_texCoord + vec2(u_textureDimentions.x, 0.0)).xy;
	vec2 result = min(color, colorR);
    gl_FragColor = vec4(result, 0.0, 1.0);
}