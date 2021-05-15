#pragma once
#include "Module.h"

class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;

class ModuleResources : public Module
{
public:

	ModuleResources();
	~ModuleResources();

	bool Init() override;
	bool Update(float dt) override;
	bool CleanUp() override;

public:
	// Static
	static std::string GetDirectoryPart(std::string path);

	static std::string GetFileNamePart(std::string path);

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

	void ProcessAssimpMesh(const aiScene* scene, aiMesh* mesh, Mesh* myMesh, u32 baseMeshMaterialIndex, std::vector<u32>& submeshMaterialIndices);

	void ProcessAssimpMaterial(aiMaterial* material, Material* myMaterial, std::string directory);

	void ProcessAssimpNode(const aiScene* scene, aiNode* node, Mesh* myMesh, u32 baseMeshMaterialIndex, std::vector<u32>& submeshMaterialIndices);


public:
	
	Model* LoadModel(const char* filename);
	u32 LoadProgram(const char* filepath, const char* programName);
	Texture* LoadTexture2D(const char* filepath);

	std::vector<Texture*>	textures;
	std::vector<Material*>	materials;
	std::vector<Mesh*>		meshes;
	std::vector<Program*>	programs;

	Model* plane;
	Model* cube;
	Model* sphereLow;
	Model* sphereHigh;

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

