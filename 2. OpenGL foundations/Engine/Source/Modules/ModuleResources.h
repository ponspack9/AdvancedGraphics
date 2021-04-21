#pragma once
#include "Module.h"

class ModuleResources : public Module
{
public:

	ModuleResources();
	~ModuleResources();

	bool Init() override;
	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;


public:
	// Static
	static std::string GetDirectoryPart(std::string path);

	/**
	 * Reads a whole file and returns a string with its contents. The returned string
	 * is temporary and should be copied if it needs to persist for several frames.
	 */
	static std::string ReadTextFile(const char* filepath);

	/**
	 * It retrieves a timestamp indicating the last time the file was modified.
	 * Can be useful in order to check for file modifications to implement hot reloads.
	 */
	static u64 GetFileLastWriteTimestamp(const char* filepath);

	static GLuint CreateProgramFromSource(std::string programSource, const char* shaderName);


	static void FreeImage(Image image);
	static Image ReadImage(const char* filename);

	static GLuint CreateTexture2DFromImage(Image image);

public:
	
	u32 LoadProgram(const char* filepath, const char* programName);
	u32 LoadTexture2D(const char* filepath);

	std::vector<Texture>	textures;
	std::vector<Material>	materials;
	std::vector<Mesh>		meshes;
	std::vector<Model>		models;
	std::vector<Program>	programs;

};

extern ModuleResources* M_Resources;



//void LogString(const char* str)
//{
//#ifdef _WIN32
//    OutputDebugStringA(str);
//    OutputDebugStringA("\n");
//#else
//    fprintf(stderr, "%s\n", str);
//#endif
//}

