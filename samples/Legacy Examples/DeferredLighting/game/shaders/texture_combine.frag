varying vec2 v_texCoord;

uniform sampler2D u_colorMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_lightMap;

void main()
{
	gl_FragData[0] = texture2D(u_colorMap,  vec2(v_texCoord));
	gl_FragData[1] = texture2D(u_normalMap, vec2(v_texCoord));
	gl_FragData[2] = texture2D(u_lightMap,  vec2(v_texCoord));
}