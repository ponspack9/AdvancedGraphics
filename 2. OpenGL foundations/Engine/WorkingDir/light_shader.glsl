///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef LIGHTING

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(binding = 0, std140) uniform LocalParams
{
	mat4 uWorldMatrix;
	mat4 uWorldViewProjectionMatrix;
};

out vec2 vTexCoord;
out vec3 vPosition;
out vec3 vNormal;

void main()
{
	vTexCoord = aTexCoord;
	vPosition = vec3(uWorldMatrix * vec4(aPosition, 1.0));
	vNormal	  = vec3(uWorldMatrix * vec4(aNormal, 0.0));

	gl_Position = uWorldViewProjectionMatrix * vec4(vPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;

layout(binding = 1, std140) uniform GlobalParams
{
	vec3 uCameraPos;

	unsigned int uNumDirectional;
	DirectionalLight uDirLights[];

	unsigned int uNumPoint;
	PointLight uPointLights[];
};

uniform sampler2D uTexture;

layout(location = 0) out vec4 oColor;

//------------- STRUCTS -------------
// Directional Light
struct DirectionalLight
{
	vec4 color;
	vec4 dir;
	float intensity;	
};

// Point Light
struct PointLight
{
	vec4 color;
	vec4 pos;
	float intensity;
	float att_const;
	float att_linear;
	float att_quad;
};

//------------- FUNCTIONS -------------
vec4 CalcDirLight(DirectionalLight light, vec3 pos, vec3 normal)
{
	vec3 dir = normalize(light.dir);
	vec3 halfway_dir = normalize(dir + pos);

	float diffuse = max(dot(normal, dir), 0.0);
	float specular = pow(max(dot(normal, halfway_dir), 0.0), 256.0);

	vec3 color = light.color * light.intensity * (diffuse + specular);
	return vec4(color, 1.0);
}

vec4 CalcPointLight(PointLight light, vec3 pos, vec3 normal)
{
	vec3 position = light.pos.xyz - vPosition;
	float dist = length(position);
	vec3 dir = normalize(position);
	vec3 halfway_dir = normalize(dir + pos);

	float diffuse = max(dot(normal, dir), 0.0);
	float specular = pow(max(dot(normal, halfway_dir), 0.0), 256.0);

	float attenuation = 1.0 / (light.att_const + light.att_linear * dist + light.att_quad * dist * dist);
	vec4 color = light.color * light.intensity * attenuation * (diffuse + specular);
	
	return vec4(color.rgb, 1.0);
}


//------------- MAIN -------------
void main()
{
	vec4 light_color = 0.0;
	vec3 albedo = texture(uTexture, vTexCoord).xyz;
	vec3 pos	  = texture(vPosition, vTexCoord).xyz;
	vec3 normal = texture(vNormal, vTexCoord).xyz;
	normal = normalize(normal);
	
	// Calculate Directional Lights
	for (int i = 0; i < uNumDirectional; ++i)
		light_color += CalcDirLight(uDirLights[i], pos, normal);
	
	// Calculate Point Lights
	for (int j = 0; j < uNumPoint; ++j)
		light_color += CalcPointLight(uPointLights[j], pos, normal);
	
	// Get Final Color
    oColor = vec4(albedo, 1.0) + light_color;

	//oColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#endif
#endif


// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.
