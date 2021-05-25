///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef LIGHTING

struct Light
{
    int type;
    float range;
    vec3 color;
    vec3 direction;
    vec3 position;
};

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

layout(binding = 0, std140) uniform GlobalParams
{
    vec3   uCameraPos;
    int    uLightCount;
    Light  uLight[16];
};

layout(binding = 1, std140) uniform LocalParams
{
	mat4 uWorldMatrix;
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

layout(binding = 0, std140) uniform GlobalParams
{
    vec3   uCameraPos;
    int    uLightCount;
    Light  uLight[16];
};          

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;
uniform sampler2D oDepth;

layout(location = 0) out vec4 oColor;

void main()
{
    vec3 iAlbedo   = texture(oAlbedo, vTexCoord).rgb;
	vec3 iNormal   = texture(oNormal, vTexCoord).rgb;
	vec3 iPosition = texture(oPosition, vTexCoord).rgb;
    
    vec3 normal = normalize(iNormal);
    float ambient = 0.4;
    vec3 lightColor = iAlbedo * ambient;
    vec3 viewDir = normalize(vViewDir - iPosition);
    
    for(int i = 0; i < uLightCount; ++i)
    {
        // Diffuse
        vec3 lightDir = normalize(uLight[i].position - iPosition);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * iAlbedo * uLight[i].color;
    
        // Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec_factor = pow(max(dot(normal, halfwayDir), 0.0), 60.0);
        vec3 specular = uLight[i].color * spec_factor * vec3(1.0);
    
        // Attenuation
        float attenuation = 1.0f;
        float dist = length(uLight[i].position - iPosition);
        attenuation = 1.0 / (1.0 + 0.1 * dist + 0.02 * dist * dist);
        
        lightColor += attenuation * (diffuse + specular);   
    }
    
    // Final Color
    oColor = vec4(lightColor, 1.0);

    //oColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#endif
#endif


// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.
