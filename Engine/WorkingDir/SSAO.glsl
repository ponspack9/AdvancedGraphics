///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef SSAO

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform vec3 uCameraPos;

out vec2 vTexCoord;
out vec3 vViewDir;
out mat4 _uViewProjection;

void main()
{
    _uViewProjection = uViewProjection;

	vTexCoord = aTexCoord;
	vViewDir = vec3(uViewProjection * vec4(uCameraPos, 1.0));
	gl_Position = vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
in vec3 vViewDir;
in mat4 _uViewProjection;

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;
uniform sampler2D oDepth;

layout(location = 0) out vec4 oColor;
uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform float intensity;
uniform bool showOnlySSAO;
uniform float time;

float rand(float from, float to) {
    vec2 co = vec2(from, to);
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 adjustContrast(vec3 color, float value) {
    return 0.5 + value * (color - 0.5);
}


void main(void) {
    // Light
    vec3 albedo = texture(oAlbedo, vTexCoord).rgb;
    vec3 normal = normalize(texture(oNormal, vTexCoord).rgb);
    vec3 position = texture(oPosition, vTexCoord).rgb;

    vec3 lightDir = normalize(uLightDirection);
    vec3 viewDir = normalize(vViewDir - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 ambient = albedo * 0.4;
    vec3 diffuse = max(0.0, dot(normal, lightDir)) * albedo * uLightColor;
    vec3 specular = pow(max(0.0, dot(halfwayDir, normal)), 32.0) * uLightColor;

    vec3 final_color = diffuse  + specular + ambient;
    

    // SSAO
    vec2 screen = vec2(800.0, 600.0);

    // KERNEL
    const int kernelSize = 64;
    vec3 kernel[kernelSize];
    
    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] = normalize(vec3(rand(-1.0, 1.0), rand(-1.0, 1.0), rand(0.0, 1.0)));

        float scale = float(i) / float(kernelSize);
        scale = mix(0.1f, 1.0f, scale * scale);
        kernel[i] *= scale;
    }

    // VIEW RAY
    vec2 ndc = vTexCoord * 2.0 - 1.0;
    float fov = 60.0;
    float aspect = screen.x / screen.y;
    float thfov = tan(fov / 2.0);
    vec3 viewray = vec3(ndc.x * thfov * aspect, ndc.y * thfov, 1.0);

    vec3 origin = viewray * texture2D(oDepth, vTexCoord).r;

    // TODO noiseTexture
    vec3 noiseVec = texture2D(oDepth, vTexCoord).xyz;
    noiseVec.xy *= 2.0;
    noiseVec.xy -= 1.0;

    vec3 rvec = noiseVec * 2.0 - 1.0;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);


    float uRadius = 1.0;

    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++)
    {
        // get sample position:
        vec3 sampleKernel = tbn * kernel[i];
        sampleKernel = sampleKernel * uRadius + origin;

        // project sample position:
        vec4 offset = vec4(sampleKernel, 1.0);
        offset = _uViewProjection * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        // get sample depth:
        float sampleDepth = adjustContrast(texture2D(oDepth, offset.xy).xyz, 0.75).r;
        float originDepth = adjustContrast(texture2D(oDepth, vTexCoord).xyz, 0.75).r;

        // range check & accumulate:
        float r = rand(0.0, 0.05);
        float rangeCheck = abs(originDepth - sampleDepth) < uRadius ? intensity : r;
        occlusion += (sampleDepth < sampleKernel.z ? 1.0 : r) * rangeCheck;
    }

    float ao = 1.0 - (occlusion / kernelSize);


    if (showOnlySSAO)
    {
        //oColor = mix(vec4(final_color, 1.0), vec4(0.0, 0.0, 0.0, 1.0), ao);
        oColor = vec4(1.0-ao);
    }
    else
    {
        //oColor = mix(vec4(final_color, 1.0), vec4(0.0, 0.0, 0.0, 1.0), ao);
        //oColor = mix(vec4(final_color, 1.0), vec4(final_color*0.25, 1.0),  ao);
        oColor = vec4(final_color,1.0)* (1.0 -ao);
    }
}

#endif
#endif