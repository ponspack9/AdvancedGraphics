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
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;
	gl_Position = vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;

layout(binding = 1, std140) uniform LightParams
{
    vec3 uCameraPos;
    vec3 light_color;
    vec3 light_position;
    float light_radius;
};          

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;

layout(location = 0) out vec4 oColor;

void main()
{
    //vec3 albedo   = texture(oAlbedo, vTexCoord).rgb;
	//vec3 normal   = normalize(texture(oNormal, vTexCoord).rgb);
	//vec3 position = texture(oPosition, vTexCoord).rgb;
    //
    //vec3 lightToPos = position - light_position;
    //float lightDist = length(lightToPos);
    //vec3 l = -lightToPos / (lightDist);
    //float ztest = step(0.0, light_radius - lightDist);
    //
    //// Attenuation
    //float d = lightDist / light_radius;
    //float attenuation = 1.0 - d;
    //vec3 v = normalize(uCameraPos - position);
    //vec3 h = normalize(l + v);
    //
    //vec3 diffuse = max(0.0, dot(normal, l)) * albedo * light_color;
    //vec3 specular = pow(max(0.0, dot(h, normal)), 12.0) * light_color;
    //
    //vec3 lightColor = diffuse + specular;
    //lightColor *= ztest * attenuation;
    //
    //// Final Color
    //oColor = vec4(lightColor, 1.0);

    oColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#endif
#endif
