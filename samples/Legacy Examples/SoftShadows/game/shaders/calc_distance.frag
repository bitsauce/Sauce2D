varying vec2 v_texCoord;

uniform float u_width;
uniform sampler2D u_texture;

void main()
{
	float alpha = texture2D(u_texture, v_texCoord).a;
	if(alpha == 0.0) discard;
    //gl_FragColor = vec4(mix(1.0, alpha * length(v_texCoord - 0.5), alpha) /** u_width*/, 0.0, 0.0, 1.0);
	gl_FragColor = vec4(alpha > 0.3 ? length(v_texCoord - 0.5f) : 1.0, 0.0, 0.0, 1.0);
}