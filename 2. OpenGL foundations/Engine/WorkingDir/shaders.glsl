///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef TEXTURED_GEOMETRY

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aTexCoord;
layout(binding = 1, std140) uniform LocalParams
{
	mat4 uWorldMatrix;
	mat4 uWorldViewProjectionMatrix;
};
out vec2 vTexCoord;
out vec3 vPosition;
out vec3 vNormal;
out vec3 vViewDir;


void main()
{
	vTexCoord = aTexCoord;
	float clippingScale = 5.0;

	vPosition = vec3(uWorldMatrix * vec4(aPosition, 1.0));
	//vNormal		= vec3(uWorldMatrix * vec4(aNormal, 0.0));

	gl_Position = uWorldViewProjectionMatrix * vec4(vPosition, 1.0);
	//gl_Position = vec4(aPosition, clippingScale);
	//gl_Position.z = -gl_Position.z;
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;
in vec3 vViewDir;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uDepth;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec4 oColor2;
layout(location = 2) out vec4 oColor3;
layout(location = 3) out vec4 oDepth;

void main()
{
	oColor = texture(uTexture, vTexCoord);
	oColor2 = texture(uTexture2, vTexCoord) * 0.75;
	oColor3 = texture(uDepth, vTexCoord);
	oDepth = texture(uDepth, vTexCoord);
	//oColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#endif
#endif


// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.
