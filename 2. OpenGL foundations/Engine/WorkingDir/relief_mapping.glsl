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

layout(binding = 1, std140) uniform LocalParams
{
    mat4 uWorldMatrix;
    mat4 uWorldViewProjectionMatrix;
};

uniform vec3 uCameraPos;

out vec2 vTexCoord;
out vec3 vPosition;
out vec3 vNormal;
out vec3 vViewDir;
out mat3 vTBN;

void main()
{
	vTexCoord = aTexCoord;
    vPosition = vec3(uWorldMatrix * vec4(aPosition, 1.0));
    vNormal = vec3(uWorldMatrix * vec4(aNormal, 0.0));
    vViewDir = normalize(uCameraPos - vPosition);

    vec3 T = normalize(vec3(uWorldMatrix * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(uWorldMatrix * vec4(aNormal, 0.0)));
    vec3 B = cross(N, T);

    vTBN = mat3(T,B,N);
    gl_Position = uWorldViewProjectionMatrix * vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT)

in vec2 vTexCoord;
in vec3 vPosition; 
in vec3 vNormal; 
in vec3 vViewDir;
in mat3 vTBN;

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

vec2 reliefMapping(vec2 texCoords, vec3 viewDir)
{
    int numSteps = 15;
    float bumpiness = 1.0;

    //Compute the view ray in texture space
    vec3 viewRay = transpose(vTBN) * vViewDir;

    ivec2 textureSize2D = textureSize(uBumpTexture,0);


    // Increment
    vec3 rayIncrement;
    rayIncrement.xy = bumpiness * viewRay.xy / abs(viewRay.z * textureSize2D.y);
    rayIncrement.z = 1.0 / numSteps;

    //Sampling state
    vec3 samplePos = vec3(texCoords, 0.0);
    float sampleDepth = 1.0 - texture(uBumpTexture, samplePos.xy).r;

    //Linear search
    while(samplePos.z < sampleDepth)
    {
        samplePos += rayIncrement;
        sampleDepth = 1.0 - texture(uBumpTexture, samplePos.xy).r;
    }

    return samplePos.xy;
}

vec2 parallaxMapping(vec2 T, vec3 V)
{
   float numLayers = 30;

   V = transpose(vTBN) * V;

   float layerHeight = 1.0 / numLayers;         // height of each layer
   float currentLayerHeight = 0;                // depth of current layer
   vec2 dtex = 0.1 * V.xy / V.z / numLayers;    // shift of texture coordinates for each iteration

   
   vec2 currentTextureCoords = T;                                           // current texture coordinates
   float heightFromTexture = texture(uBumpTexture, currentTextureCoords).r;     // get first depth from heightmap

   // while point is above surface
   while(heightFromTexture > currentLayerHeight)
   {
      currentLayerHeight += layerHeight;                                // to the next layer
      currentTextureCoords -= dtex;                                     // shift texture coordinates along vector V
      heightFromTexture = texture(uBumpTexture, currentTextureCoords).r;    // get new depth from heightmap
   }
    
   // --- Start of Relief Mapping ---

   // decrease shift and height of layer by half
   vec2 deltaTexCoord = dtex / 2;
   float deltaHeight = layerHeight / 2;

   // return to the mid point of previous layer
   currentTextureCoords += deltaTexCoord;
   currentLayerHeight -= deltaHeight;

   // binary search to increase precision of Steep Paralax Mapping
   const int numSearches = 5;
   for(int i=0; i<numSearches; i++)
   {
      // decrease shift and height of layer by half
      deltaTexCoord /= 2;
      deltaHeight /= 2;

      // new depth from heightmap
      heightFromTexture = texture(uBumpTexture, currentTextureCoords).r;

      // shift along or agains vector V
      if(heightFromTexture > currentLayerHeight) // below the surface
      {
         currentTextureCoords -= deltaTexCoord;
         currentLayerHeight += deltaHeight;
      }
      else // above the surface
      {
         currentTextureCoords += deltaTexCoord;
         currentLayerHeight -= deltaHeight;
      }
   }

   return currentTextureCoords;
}


void main()
{
    vec2 texCoords = parallaxMapping(vTexCoord, vViewDir);

    vec3 normal = texture(uNormalMap, texCoords).xyz;
    normal = normal * 2.0 - 1.0;
    normal = normalize(vTBN * normal);    

    oAlbedo = texture(uTexture, texCoords);
    oNormal = vec4(normal, 1.0);
    oPosition = vec4(vPosition, 1.0);
	oDepth = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);
}

#endif
#endif

// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.
