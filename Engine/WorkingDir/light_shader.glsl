///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef DIRECTIONAL_LIGHT

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform vec3 uCameraPos;
uniform mat4 uViewProjection;

out vec2 vTexCoord;
out vec3 vViewDir;

void main()
{
	vTexCoord = aTexCoord;
	vViewDir = vec3(uViewProjection * vec4(uCameraPos, 1.0));
	gl_Position = vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
in vec3 vViewDir;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;

layout(location = 0) out vec4 oColor;

void main()
{
    vec3 albedo   = texture(oAlbedo, vTexCoord).rgb;
	vec3 normal   = normalize(texture(oNormal, vTexCoord).rgb);
	vec3 position = texture(oPosition, vTexCoord).rgb;
    
    vec3 lightDir = normalize(uLightDirection);
    vec3 viewDir = normalize(vViewDir - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    vec3 ambient = albedo * 0.4;
    vec3 diffuse = max(0.0, dot(normal, lightDir)) * albedo * uLightColor;
    vec3 specular = pow(max(0.0, dot(halfwayDir, normal)), 32.0) * uLightColor;
    
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
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uModel;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = uViewProjection * vec4(vec3(uModel * vec4(aPosition, 1.0)), 1.0);}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;

uniform vec3 uCameraPos;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uLightK;
uniform float uLightL;
uniform float uLightQ;

layout(location = 0) out vec4 oColor;

void main()
{
    vec3 Albedo   = texture(oAlbedo, vTexCoord).rgb;
	vec3 Normal   = normalize(texture(oNormal, vTexCoord).rgb);
    vec3 Position = texture(oPosition, vTexCoord).rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(uLightPosition - Position);
    vec3 diffuse  = max(dot(Normal, lightDir), 0.0) * Albedo * uLightColor;  
    
    // specular
    vec3 viewDir    = normalize(uCameraPos - Position);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    vec3 specular   = uLightColor * pow(max(dot(viewDir, reflectDir), 0.0), 10.0);
    
    // attenuation
    float distance    = length(uLightPosition - Position);
    float attenuation = 1.0 / (uLightK + uLightL * distance + uLightQ * (distance * distance));    
        
    vec3 result = attenuation * (diffuse + specular);
    oColor = vec4(result, 1.0);}

#endif
#endif
