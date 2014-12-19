varying vec2 v_texCoord;

uniform float u_ambient;
uniform float u_lightAmbient;
uniform vec3 u_ambientColor;
uniform sampler2D u_colorMap;
uniform sampler2D u_shadingMap;

void main()
{
	vec4 color = texture2D(u_colorMap, v_texCoord);
	vec4 shading = texture2D(u_shadingMap, v_texCoord);
 
	gl_FragColor = (color * u_ambientColor * u_ambient);
	gl_FragColor += color * shading * u_lightAmbient;
}