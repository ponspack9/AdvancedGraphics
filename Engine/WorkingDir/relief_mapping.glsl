///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef RELIEF_MAPPING

#if defined(VERTEX)  ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 uModel;
uniform mat4 uViewProjection;
uniform vec3 uCameraPos;

out vec2 vTexCoord;
out vec3 vPosition;
out vec3 vNormal;
out vec3 vViewDir;
out mat3 vTBN;

void main()
{
	vTexCoord = aTexCoord;
    vPosition = vec3(uModel * vec4(aPosition, 1.0));
    vNormal =   vec3(uModel * vec4(aNormal, 0.0));
    vViewDir =  normalize(uCameraPos - vPosition);

    vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(uModel * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(vNormal));
    vTBN = mat3(T,B,N);
    
    gl_Position = uViewProjection * vec4(vPosition, 1.0);
}

#elif defined(FRAGMENT)

in vec2 vTexCoord;
in vec3 vPosition; 
in vec3 vNormal; 
in vec3 vViewDir;
in mat3 vTBN;

uniform int hasNormalMap;
uniform int hasBumpTexture;
uniform float bumpiness;

uniform sampler2D uTexture;
uniform sampler2D uNormalMap;
uniform sampler2D uBumpTexture;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec4 oAlbedo;
layout(location = 2) out vec4 oNormal;
layout(location = 3) out vec4 oPosition;
layout(location = 4) out vec4 oDepth;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
   float numLayers = 30;

   float height = 1.0 / numLayers;        
   float currentHeight = 0.0;               
   vec2 dtex = (viewDir.xy / viewDir.z * bumpiness) / numLayers;

   
   vec2 currentTexCoords = texCoords;                                       // current texture coordinates
   float texHeight = texture(uBumpTexture, currentTexCoords).r;     // get first depth from heightmap

   while(currentHeight < texHeight)
   {
      currentTexCoords -= dtex;                               // shift texture coordinates along vector V
      texHeight = texture(uBumpTexture, currentTexCoords).r;  // get new depth from heightmap
      currentHeight += height;                                // to the next layer
   }

   vec2 prevTexCoords = currentTexCoords + dtex;
   float after = texHeight - currentHeight;
   float before = texture(uBumpTexture, prevTexCoords).r - currentHeight + height;
   float weight = after / (after - before);
   
   vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
   return finalTexCoords;
}

//---------------
void main()
{
    vec2 texCoords = vTexCoord;
    if (hasBumpTexture == 1)
    {
        texCoords = parallaxMapping(vTexCoord, transpose(vTBN) * vViewDir);
    }

    vec3 normal = vNormal;
    if(hasNormalMap == 1)
    {
        normal = texture(uNormalMap, texCoords).xyz;
        normal = normal * 2.0 - 1.0;
        normal = normalize(vTBN * normal);
    }
    
    oAlbedo =   texture(uTexture, texCoords);
    oNormal =   vec4(normal, 1.0);
    oPosition = vec4(vPosition, 1.0);
	oDepth =    vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);
}

#endif
#endif

// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.
