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
out mat4 invertProj;

void main()
{
	vTexCoord = aTexCoord;
	vViewDir = vec3(uViewProjection * vec4(uCameraPos, 1.0));
    invertProj = inverse(uViewProjection);
	gl_Position = vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
in vec3 vViewDir;
in mat4 invertProj;

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

    vec3 final_color = diffuse + specular + ambient;

    // SSAO
    vec2 screen = vec2(800.0, 600.0);

    const int kernelSize = 64;
    vec3 kernel[kernelSize];
    
    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] = normalize(vec3(rand(-1.0, 1.0), rand(-1.0, 1.0), rand(0.0, 1.0)));
        float scale = float(i) / float(kernelSize);
        scale = mix(0.1f, 1.0f, scale * scale);
        kernel[i] *= scale;
    }

    //vec3 random = normalize(vec3(rand(vec2(time, time * 3)), rand(vec2(time) + vec2(10.0, 1.0)), rand(vec2(time, time * 5)) + vec2(10.0, 50.0)));
    vec3 random = normalize( vec3( rand(time, time*3), rand(time*2, time+10), rand(time, time + 50) ));
    
    vec2 ndc = vTexCoord * 2.0 - 1.0;
    float fov = 60.0;
    float aspect = screen.x / screen.y;
    float thfov = tan(fov / 2.0); // can do this on the CPU
    vec3 viewray = vec3(ndc.x * thfov * aspect, ndc.y * thfov, 1.0);


    vec3 origin = viewray * texture2D(oDepth, vTexCoord).r;

    // TODO noiseTexture
    vec3 noiseVec = texture2D(oNormal, vTexCoord).xyz;
    noiseVec.xy -= 0.5;
    noiseVec.xy /= 0.5;

    vec3 rvec = noiseVec * 2.0 - 1.0;
    rvec = random;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);


    float uRadius = 1.0;

    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++)
    {
        // get sample position:
        vec3 sampleSphere = tbn * kernel[i];
        sampleSphere = sampleSphere * uRadius + origin;

        // project sample position:
        vec4 offset = vec4(sampleSphere, 1.0);
        offset = invertProj * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        // get sample depth:
        float sampleDepth = adjustContrast(texture2D(oDepth, offset.xy).xyz, 0.75).r;
        float originDepth = adjustContrast(texture2D(oDepth, vTexCoord).xyz, 0.75).r;

        // range check & accumulate:
        float rangeCheck = abs(originDepth - sampleDepth) < uRadius ? 1.0 : 0.0;
        occlusion += (sampleDepth < sampleSphere.z ? 1.0 : 0.0);
    }

    float ao = intensity - (occlusion / kernelSize);

    if (showOnlySSAO)
    {
        //oColor = mix(vec4(final_color, 1.0), vec4(0.0, 0.0, 0.0, 1.0), ao);
        oColor = vec4(1.0-ao);
    }
    else
    {
        oColor = mix(vec4(final_color, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 1.0 - ao);
    }
}

#endif
#endif
/*
sample_sphere[0] = normalize(vec3(0.5381, 0.1856, 0.4319)) * 0.1;
sample_sphere[1] = normalize(vec3(0.1379, 0.2486, 0.4430)) * 0.15;
sample_sphere[2] = normalize(vec3(0.3371, 0.5679, 0.0057)) * 0.2;
sample_sphere[3] = normalize(vec3(-0.6999, -0.0451, 0.0019)) * 0.25;
sample_sphere[4] = normalize(vec3(0.0689, -0.1598, 0.8547)) * 0.30;
sample_sphere[5] = normalize(vec3(0.0560, 0.0069, 0.1843)) * 0.35;
sample_sphere[6] = normalize(vec3(-0.0146, 0.1402, 0.0762)) * 0.40;
sample_sphere[7] = normalize(vec3(0.0100, -0.1924, 0.0344)) * 0.45;
sample_sphere[8] = normalize(vec3(-0.3577, -0.5301, 0.4358)) * 0.50;
sample_sphere[9] = normalize(vec3(-0.3169, 0.1063, 0.0158)) * 0.55;
sample_sphere[10] = normalize(vec3(0.0103, -0.5869, 0.0046)) * 0.60;
sample_sphere[11] = normalize(vec3(-0.0897, -0.4940, 0.3287)) * 0.65;
sample_sphere[12] = normalize(vec3(0.7119, -0.0154, 0.0918)) * 0.75;
sample_sphere[13] = normalize(vec3(-0.0533, 0.0596, 0.5411)) * 0.80;
sample_sphere[14] = normalize(vec3(0.0352, -0.0631, 0.5460)) * 0.85;
sample_sphere[15] = normalize(vec3(-0.4776, 0.2847, 0.0271)) * 0.90;*/