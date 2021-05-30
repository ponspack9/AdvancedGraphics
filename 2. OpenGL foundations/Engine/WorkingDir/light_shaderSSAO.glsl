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
out mat4 invertProj;

void main()
{
	vTexCoord = aTexCoord;
	vViewDir = vec3(uWorldViewProjectionMatrix * vec4(uCameraPos, 1.0));
    invertProj = inverse(uWorldViewProjectionMatrix);
	gl_Position = vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
in vec3 vViewDir;
in mat4 invertProj;

layout(binding = 1, std140) uniform LightParams
{
    vec3 light_color;
    vec3 light_direction;
};          

uniform sampler2D oAlbedo;
uniform sampler2D oNormal;
uniform sampler2D oPosition;
uniform sampler2D oDepth;

layout(location = 0) out vec4 oColor;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float unpackFloat(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);
    return dot(rgbaDepth, bitShift);
}

vec3 normal_from_depth(float depth, vec2 texcoords) {
    const vec2 offset1 = vec2(0.0, 0.001);
    const vec2 offset2 = vec2(0.001, 0.0);

    float depth1 = texture2D(oDepth, texcoords + offset1).r;
    float depth2 = texture2D(oDepth, texcoords + offset2).r;

    vec3 p1 = vec3(offset1, depth1 - depth);
    vec3 p2 = vec3(offset2, depth2 - depth);

    vec3 normal = cross(p1, p2);
    normal.z = -normal.z;
    return normalize(normal);
}

vec3 adjustContrast(vec3 color, float value) {
    return 0.5 + value * (color - 0.5);
}

mat4 contrastMatrix(float contrast)
{
    float t = (1.0 - contrast) / 2.0;

    return mat4(contrast, 0, 0, 0,
        0, contrast, 0, 0,
        0, 0, contrast, 0,
        t, t, t, 1);

}

vec3 adjustExposure(vec3 color, float value) {
    return (1.0 + value) * color;
}

vec3 VSPositionFromDepth(vec2 uv)
{
    // Get the depth value for this pixel
    float z = texture2D(oDepth, uv).r;
    // Get x/w and y/w from the viewport position
    float x = uv.x * 2.0 - 1.0;
    float y = (1.0 - uv.y) * 2.0 - 1.0;
    vec4 vProjectedPos = vec4(x, y, z, 1.0);
    // Transform by the inverse projection matrix
    vec4 vPositionVS = invertProj * vProjectedPos;
    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;
}


void main(void) {
    // Light
    vec3 albedo = texture(oAlbedo, vTexCoord).rgb;
    vec3 normal = normalize(texture(oNormal, vTexCoord).rgb);
    vec3 position = texture(oPosition, vTexCoord).rgb;

    vec3 lightDir = normalize(light_direction);
    vec3 viewDir = normalize(vViewDir - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 ambient = albedo * 0.4;
    vec3 diffuse = max(0.0, dot(normal, lightDir)) * albedo * light_color;
    vec3 specular = pow(max(0.0, dot(halfwayDir, normal)), 32.0) * light_color;

    vec3 final_color = diffuse + specular + ambient;

    // SSAO

    const float base = 0.2;
    vec2 screen = vec2(800.0, 600.0);
    const float total_strength = 20.0;
    const float area = 0.0075;
    const float radius = 0.005;

    //radius = textureSize(oAlbedo, 0);
    const float falloff = 0.0001;

    float contrast = total_strength;
    float depth = adjustContrast(texture2D(oDepth, vTexCoord).xyz, contrast).r;
    float radius_depth = radius / depth;

    const int uSampleKernelSize = 16;
    vec3 sample_sphere[uSampleKernelSize];
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
    sample_sphere[15] = normalize(vec3(-0.4776, 0.2847, 0.0271)) * 0.90;

    vec3 random = normalize(vec3(rand(vTexCoord), rand(vTexCoord + vec2(10.0, 1.0)), rand(vTexCoord) + vec2(10.0, 50.0)));

    vec2 ndc = vTexCoord * 2.0 - 1.0;
    float fov = 45.0;
    float aspect = screen.x / screen.y;
    float thfov = tan(fov / 2.0); // can do this on the CPU
    vec3 viewray = vec3(
        ndc.x * thfov * aspect,
        ndc.y * thfov,
        1.0
    );


    vec2 uNoiseScale = vec2(screen.x / 8.0, screen.y / 8.0);
    vec3 origin = viewray * texture2D(oDepth, vTexCoord).r;
    //  origin.z *= texture2D(oDepth, vTexCoord).r;
    //vec3 normal = normal_from_depth(texture2D(oDepth, vTexCoord).r, vTexCoord);

    // TODO noiseTexture
    vec3 noiseVec = texture2D(oNormal, vTexCoord * uNoiseScale).xyz;
    noiseVec.xy -= 0.5;
    noiseVec.xy /= 0.5;

    vec3 rvec = noiseVec * 2.0 - 1.0;
    rvec = random;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);


    float uRadius = 3.0;

    float occlusion = 0.0;
    for (int i = 0; i < uSampleKernelSize; i++)
    {
        // get sample position:
        vec3 sampleSphere = tbn * sample_sphere[i];
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
        occlusion += (sampleDepth <= sampleSphere.z ? 1.0 : 0.0);
    }

    float ao = 1.0 - (occlusion / 16.0);

    oColor = mix(vec4(final_color, 1.0), vec4(0.0, 0.0, 0.0, 1.0), ao);
    //oColor = mix(texture2D(oAlbedo, vTexCoord), vec4(0.0, 0.0, 0.0, 1.0), ao);
    //gl_FragColor.xyz = vec3(ao);

    //gl_FragColor.a = 1.0;
    // Final Color
    //oColor = vec4(final_color, 1.0);
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
