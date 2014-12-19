varying vec2 v_texCoord;

uniform float u_lightStrength;
uniform float u_lightRadius;
uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;

uniform float u_screenWidth;
uniform float u_screenHeight;

uniform sampler2D u_normalMap;
uniform sampler2D u_lightMap;

void main()
{
    vec3 normal = texture2D(u_normalMap, v_texCoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(normal);
 
    float depth = texture2D(u_lightMap, v_texCoord).r;
 
    vec3 pixelPosition;
    pixelPosition.x = u_screenWidth * v_texCoord.x;
    pixelPosition.y = u_screenHeight * v_texCoord.y;
    pixelPosition.z = depth;
    //pixelPosition.w = 1.0f;
 
	vec3 lightDirection = u_lightPosition - pixelPosition;
	float distance = u_lightStrength / length(u_lightPosition - pixelPosition);
	float amount = max(dot(normal + vec3(depth), vec3(normalize(distance))), 0.0);

	float coneAttenuation = clamp(1.0 - length(lightDirection) / u_lightRadius, 0.0, 1.0);

	vec3 shading = distance * amount * coneAttenuation * u_lightColor;
    gl_FragColor = vec4(shading.r, shading.g, shading.b, 1.0);
}