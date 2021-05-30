///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef DIRECTIONAL_LIGHT

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

layout(binding = 1, std140) uniform LocalParams
{
	vec3 uCameraPos;
	mat4 uWorldViewProjectionMatrix;
};

out vec2 vTexCoord;
out vec3 vViewDir;

void main()
{
	vTexCoord = aTexCoord;
	vViewDir = vec3(uWorldViewProjectionMatrix * vec4(uCameraPos, 1.0));
	gl_Position = vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
in vec3 vViewDir;

layout(binding = 1, std140) uniform LightParams
{
    vec3 light_color;
    vec3 light_direction;
};          

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;

layout(location = 0) out vec4 oColor;

void main()
{
    vec3 albedo   = texture(oAlbedo, vTexCoord).rgb;
	vec3 normal   = normalize(texture(oNormal, vTexCoord).rgb);
	vec3 position = texture(oPosition, vTexCoord).rgb;
    
    vec3 lightDir = normalize(light_direction);
    vec3 viewDir = normalize(vViewDir - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    vec3 ambient = albedo * 0.4;
    vec3 diffuse = max(0.0, dot(normal, lightDir)) * albedo * light_color;
    vec3 specular = pow(max(0.0, dot(halfwayDir, normal)), 32.0) * light_color;
    
    vec3 final_color = diffuse + specular + ambient;

    // Final Color
    oColor = vec4(final_color, 1.0);
}

#endif
#endif


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef POINT_LIGHT

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
out vec4 fsPos;

uniform mat4 uViewProjection;
uniform float uLightRadius;
uniform vec3 uLightPosition;

void main()
{
	vec4 pos = uViewProjection * vec4((aPosition * uLightRadius) + uLightPosition, 1.0);
	gl_Position = pos;
    fsPos = pos;
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec4 fsPos;

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;

uniform float uLightRadius;
uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform vec3 uCameraPos;

layout(location = 0) out vec4 oColor;

void main()
{
    vec2 uv = (fsPos.xy / fsPos.w) * 0.5 + 0.5;
    
    vec3 albedo    = texture(oAlbedo, uv).xyz;
    vec3 normal    = normalize(texture(oNormal, uv).xyz);
    vec3 position  = texture(oPosition, uv).xyz;
    
    vec3 lightToPos = position - uLightPosition;
    float lightDist = length(lightToPos);
    vec3 l = -lightToPos / (lightDist);
    float ztest = step(0.0, uLightRadius - lightDist);
    
    // Attenuation
    float d = lightDist / uLightRadius;
    float attenuation = 1.0 - d;
    vec3 v = normalize(uCameraPos - position);
    vec3 h = normalize(l + v);
    
    vec3 diffuse = max(0.0, dot(normal, l)) * albedo * uLightColor;
    vec3 specular = pow(max(0.0, dot(h, normal)), 12.0) * uLightColor;
    
    vec3 lightColor = diffuse + specular;
    //lightColor *= ztest * attenuation;
    
    // Final Color
    oColor = vec4(lightColor, 1.0);

    //oColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#endif
#endif
