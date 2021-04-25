#include "pch.h"
#include "ModuleResources.h"



#pragma region Module overrides
ModuleResources* M_Resources = nullptr;

ModuleResources::ModuleResources()
{
    this->name = "ModuleResources";
    LOG_DEBUG("Created module [{0}]", name);

}

ModuleResources::~ModuleResources()
{
    LOG_DEBUG("Deleted module [{0}]", name);

}

bool ModuleResources::Init()
{
    if (GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 3))
    {
        glDebugMessageCallback(Log::OnGlError, App);
    }
    //panels.push_back(new PanelInfo());
    // TODO: Initialize your resources here!
    // - programs (and retrieve uniform indices)
    // - textures

    // Programs
    App->texturedGeometryProgramIdx = M_Resources->LoadProgram("shaders.glsl", "TEXTURED_GEOMETRY");

    //Submesh quadSubmesh;
    //quadSubmesh.vertices = {
    //     -0.5f, -0.5f, 0.0f,   0.0f,0.0f,
    //      0.5f, -0.5f, 0.0f,   1.0f,0.0f,
    //      0.5f,  0.5f, 0.0f,   1.0f,1.0f,
    //     -0.5f,  0.5f, 0.0f,   0.0f,1.0f };
    //quadSubmesh.indices = {
    //    0, 1, 2,
    //    0, 2, 3
    //};
    //quadSubmesh.vertexOffset = 0;
    //quadSubmesh.indexOffset = 0;
    //quadSubmesh.vertexBufferLayout.attributes = { { 0,3,0 },{ 1,2,3 * sizeof(float) } };
    //quadSubmesh.vertexBufferLayout.stride = 20;

    //Mesh quadMesh;
    //quadMesh.submeshes.push_back(quadSubmesh);
    //

    //// VBO
    //glGenBuffers(1, &quadMesh.vertexBufferHandle);
    //glBindBuffer(GL_ARRAY_BUFFER, quadMesh.vertexBufferHandle);
    //glBufferData(GL_ARRAY_BUFFER, quadMesh.submeshes[0].vertices.size() * sizeof(float), &quadMesh.submeshes[0].vertices[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //// EBO
    //glGenBuffers(1, &quadMesh.indexBufferHandle);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh.indexBufferHandle);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadMesh.submeshes[0].indices.size() * sizeof(u32), &quadMesh.submeshes[0].indices[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    //// VAO
    //// Element 0, has 3 components, that are floats, no need to normalize, 
    ////GLuint vao = 0;
    //// TODO: VAO
    //glGenVertexArrays(1, &App->vao);
    //glBindVertexArray(App->vao);
    //glBindBuffer(GL_ARRAY_BUFFER, quadMesh.vertexBufferHandle);

    //for (u32 i = 0; i < quadMesh.submeshes.size(); ++i)
    //{
    //    Submesh* submesh = &quadMesh.submeshes[i];
    //    u8 stride = submesh->vertexBufferLayout.stride;

    //    submesh->vaos.push_back({ App->vao,App->texturedGeometryProgramIdx });
    //    for (VertexBufferAttribute attribute : submesh->vertexBufferLayout.attributes)
    //    {
    //        glVertexAttribPointer(attribute.location, attribute.componentCount, GL_FLOAT, GL_FALSE, stride, (void*)attribute.offset);
    //        glEnableVertexAttribArray(attribute.location);

    //    }
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh.indexBufferHandle);
    //    glBindVertexArray(0);
    //}

    //meshes.push_back(quadMesh);
    //
    //Program& texturedGeometryProgram = M_Resources->programs[App->texturedGeometryProgramIdx];

    //App->programUniformTexture = glGetUniformLocation(texturedGeometryProgram.handle, "uTexture");

    //// Textures
    //App->diceTexIdx = M_Resources->LoadTexture2D("dice.png");
    //App->whiteTexIdx = M_Resources->LoadTexture2D("color_white.png");
    //App->blackTexIdx = M_Resources->LoadTexture2D("color_black.png");
    //App->normalTexIdx = M_Resources->LoadTexture2D("color_normal.png");
    //App->magentaTexIdx = M_Resources->LoadTexture2D("color_magenta.png");

    //Model quad;
    //quad.meshIdx = 0;
    //quad.materialIdx.push_back(0);

    //models.push_back(quad);

    //Material material;
    //material.albedoTextureIdx = App->diceTexIdx;

    //materials.push_back(material);
    App->mode = Mode::Mode_TexturedQuad;


    LoadModel("Patrick/Patrick.obj");

    return true;;
}

bool ModuleResources::Start()
{
    return true;;
}

bool ModuleResources::PreUpdate(float dt)
{
    return true;;
}

bool ModuleResources::Update(float dt)
{
    // hot reload
    for (int i = 0; i < M_Resources->programs.size(); ++i)
    {
        Program& program = M_Resources->programs[i];
        u64 currentTimeStamp = ModuleResources::GetFileLastWriteTimestamp(program.filepath.c_str());

        if (currentTimeStamp > program.lastWriteTimestamp)
        {
            glDeleteProgram(program.handle);
            std::string programSource = ModuleResources::ReadTextFile(program.filepath.c_str());

            program.handle = ModuleResources::CreateProgramFromSource(programSource, program.programName.c_str());
            program.lastWriteTimestamp = currentTimeStamp;
        }
    }
    return true;;
}

bool ModuleResources::PostUpdate(float dt)
{
    return true;;
}

bool ModuleResources::CleanUp()
{
    return true;;
}

#pragma endregion

#pragma region Static
std::string ModuleResources::GetDirectoryPart(std::string path)
{
    size_t pos = path.rfind('/');
    if (pos == std::string::npos)
        pos = path.rfind('\\');

    if (pos != std::string::npos)
        return path.substr(0, path.length() - pos);

    LOG_ERROR("Could not get directory part from {0}", path);
    return path; // not found
}

std::string ModuleResources::ReadTextFile(const char* filepath)
{
    std::ifstream ifs(filepath);

    if (ifs)
    {
        std::string fileText((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return  fileText;
    }

    LOG_ERROR("fopen() failed reading file {0}", filepath);
    return "ERROR";
}

u64 ModuleResources::GetFileLastWriteTimestamp(const char* filepath)
{
#ifdef _WIN32
    union Filetime2u64 {
        FILETIME filetime;
        u64      u64time;
    } conversor;

    WIN32_FILE_ATTRIBUTE_DATA Data;
    if (GetFileAttributesExA(filepath, GetFileExInfoStandard, &Data)) {
        conversor.filetime = Data.ftLastWriteTime;
        return(conversor.u64time);
    }
#else
    // NOTE: This has not been tested in unix-like systems
    struct stat attrib;
    if (stat(filepath, &attrib) == 0) {
        return attrib.st_mtime;
    }
#endif

    return 0;
}

GLuint ModuleResources::CreateProgramFromSource(std::string programSource, const char* shaderName)
{
    GLchar  infoLogBuffer[1024] = {};
    GLsizei infoLogBufferSize = sizeof(infoLogBuffer);
    GLsizei infoLogSize;
    GLint   success;

    char versionString[] = "#version 430\n";
    char shaderNameDefine[128];
    sprintf(shaderNameDefine, "#define %s\n", shaderName);
    char vertexShaderDefine[] = "#define VERTEX\n";
    char fragmentShaderDefine[] = "#define FRAGMENT\n";

    const GLchar* vertexShaderSource[] = {
        versionString,
        shaderNameDefine,
        vertexShaderDefine,
        programSource.c_str()
    };
    const GLint vertexShaderLengths[] = {
        (GLint)strlen(versionString),
        (GLint)strlen(shaderNameDefine),
        (GLint)strlen(vertexShaderDefine),
        (GLint)programSource.length()
    };
    const GLchar* fragmentShaderSource[] = {
        versionString,
        shaderNameDefine,
        fragmentShaderDefine,
        programSource.c_str()
    };
    const GLint fragmentShaderLengths[] = {
        (GLint)strlen(versionString),
        (GLint)strlen(shaderNameDefine),
        (GLint)strlen(fragmentShaderDefine),
        (GLint)programSource.length()
    };

    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, ARRAY_COUNT(vertexShaderSource), vertexShaderSource, vertexShaderLengths);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vshader, infoLogBufferSize, &infoLogSize, infoLogBuffer);
        LOG_ERROR("glCompileShader() failed with vertex shader %s\nReported message:\n%s\n", shaderName, infoLogBuffer);
    }

    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, ARRAY_COUNT(fragmentShaderSource), fragmentShaderSource, fragmentShaderLengths);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fshader, infoLogBufferSize, &infoLogSize, infoLogBuffer);
        LOG_ERROR("glCompileShader() failed with fragment shader %s\nReported message:\n%s\n", shaderName, infoLogBuffer);
    }

    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vshader);
    glAttachShader(programHandle, fshader);
    glLinkProgram(programHandle);
    glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programHandle, infoLogBufferSize, &infoLogSize, infoLogBuffer);
        LOG_ERROR("glLinkProgram() failed with program %s\nReported message:\n%s\n", shaderName, infoLogBuffer);
    }

    glUseProgram(0);

    glDetachShader(programHandle, vshader);
    glDetachShader(programHandle, fshader);
    glDeleteShader(vshader);
    glDeleteShader(fshader);

    return programHandle;
}

Image ModuleResources::ReadImage(const char* filename)
{
    Image img = {};
    stbi_set_flip_vertically_on_load(true);
    img.pixels = stbi_load(filename, &img.size.x, &img.size.y, &img.nchannels, 0);
    if (img.pixels)
    {
        img.stride = img.size.x * img.nchannels;
    }
    else
    {
        LOG_ERROR("Could not open file {0}", filename);
    }
    return img;
}

void ModuleResources::FreeImage(Image image)
{
    stbi_image_free(image.pixels);
}

GLuint ModuleResources::CreateTexture2DFromImage(Image image)
{
    GLenum internalFormat = GL_RGB8;
    GLenum dataFormat = GL_RGB;
    GLenum dataType = GL_UNSIGNED_BYTE;

    switch (image.nchannels)
    {
    case 3: dataFormat = GL_RGB; internalFormat = GL_RGB8; break;
    case 4: dataFormat = GL_RGBA; internalFormat = GL_RGBA8; break;
    default: LOG_ERROR("LoadTexture2D() - Unsupported number of channels");
    }

    GLuint texHandle;
    glGenTextures(1, &texHandle);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.size.x, image.size.y, 0, dataFormat, dataType, image.pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texHandle;
}


void ModuleResources::ProcessAssimpMesh(const aiScene* scene, aiMesh* mesh, Mesh* myMesh, u32 baseMeshMaterialIndex, std::vector<u32>& submeshMaterialIndices)
{
    std::vector<float> vertices;
    std::vector<u32> indices;

    bool hasTexCoords = false;
    bool hasTangentSpace = false;

    // process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            hasTexCoords = true;
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }

        if (mesh->mTangents != nullptr && mesh->mBitangents)
        {
            hasTangentSpace = true;
            vertices.push_back(mesh->mTangents[i].x);
            vertices.push_back(mesh->mTangents[i].y);
            vertices.push_back(mesh->mTangents[i].z);

            // For some reason ASSIMP gives me the bitangents flipped.
            // Maybe it's my fault, but when I generate my own geometry
            // in other files (see the generation of standard assets)
            // and all the bitangents have the orientation I expect,
            // everything works ok.
            // I think that (even if the documentation says the opposite)
            // it returns a left-handed tangent space matrix.
            // SOLUTION: I invert the components of the bitangent here.
            vertices.push_back(-mesh->mBitangents[i].x);
            vertices.push_back(-mesh->mBitangents[i].y);
            vertices.push_back(-mesh->mBitangents[i].z);
        }
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // store the proper (previously proceessed) material for this mesh
    submeshMaterialIndices.push_back(baseMeshMaterialIndex + mesh->mMaterialIndex);

    // create the vertex format
    VertexBufferLayout vertexBufferLayout = {};
    vertexBufferLayout.attributes.push_back(VertexBufferAttribute{ 0, 3, 0 });
    vertexBufferLayout.attributes.push_back(VertexBufferAttribute{ 1, 3, 3 * sizeof(float) });
    vertexBufferLayout.stride = 6 * sizeof(float);
    if (hasTexCoords)
    {
        vertexBufferLayout.attributes.push_back(VertexBufferAttribute{ 2, 2, vertexBufferLayout.stride });
        vertexBufferLayout.stride += 2 * sizeof(float);
    }
    if (hasTangentSpace)
    {
        vertexBufferLayout.attributes.push_back(VertexBufferAttribute{ 3, 3, vertexBufferLayout.stride });
        vertexBufferLayout.stride += 3 * sizeof(float);

        vertexBufferLayout.attributes.push_back(VertexBufferAttribute{ 4, 3, vertexBufferLayout.stride });
        vertexBufferLayout.stride += 3 * sizeof(float);
    }

    // add the submesh into the mesh
    Submesh submesh = {};
    submesh.vertexBufferLayout = vertexBufferLayout;
    submesh.vertices.swap(vertices);
    submesh.indices.swap(indices);
    myMesh->submeshes.push_back(submesh);
}

void ModuleResources::ProcessAssimpMaterial(aiMaterial* material, Material& myMaterial, std::string directory)
{
    aiString name;
    aiColor3D diffuseColor;
    aiColor3D emissiveColor;
    aiColor3D specularColor;
    ai_real shininess;
    material->Get(AI_MATKEY_NAME, name);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
    material->Get(AI_MATKEY_SHININESS, shininess);

    myMaterial.name = name.C_Str();
    myMaterial.albedo = vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
    myMaterial.emissive = vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b);
    myMaterial.smoothness = shininess / 256.0f;

    aiString aiFilename;
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        material->GetTexture(aiTextureType_DIFFUSE, 0, &aiFilename);
        //std::string filename = MakeString(aiFilename.C_Str());
        std::string filepath = directory + "/" + aiFilename.C_Str();
        myMaterial.albedoTextureIdx = LoadTexture2D(filepath.c_str());
    }
    if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
    {
        material->GetTexture(aiTextureType_EMISSIVE, 0, &aiFilename);
        std::string filepath = directory + "/" + aiFilename.C_Str();
        myMaterial.emissiveTextureIdx = LoadTexture2D(filepath.c_str());
    }
    if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
    {
        material->GetTexture(aiTextureType_SPECULAR, 0, &aiFilename);
        std::string filepath = directory + "/" + aiFilename.C_Str();
        myMaterial.specularTextureIdx = LoadTexture2D(filepath.c_str());
    }
    if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
    {
        material->GetTexture(aiTextureType_NORMALS, 0, &aiFilename);
        std::string filepath = directory + "/" + aiFilename.C_Str();
        myMaterial.normalsTextureIdx = LoadTexture2D(filepath.c_str());
    }
    if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
    {
        material->GetTexture(aiTextureType_HEIGHT, 0, &aiFilename);
        std::string filepath = directory + "/" + aiFilename.C_Str();
        myMaterial.bumpTextureIdx = LoadTexture2D(filepath.c_str());
    }

    //myMaterial.createNormalFromBump();
}

void ModuleResources::ProcessAssimpNode(const aiScene* scene, aiNode* node, Mesh* myMesh, u32 baseMeshMaterialIndex, std::vector<u32>& submeshMaterialIndices)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessAssimpMesh(scene, mesh, myMesh, baseMeshMaterialIndex, submeshMaterialIndices);
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessAssimpNode(scene, node->mChildren[i], myMesh, baseMeshMaterialIndex, submeshMaterialIndices);
    }
}

u32 ModuleResources::LoadModel(const char* filename)
{
    const aiScene* scene = aiImportFile(filename,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_PreTransformVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_OptimizeMeshes |
        aiProcess_SortByPType);

    if (!scene)
    {
        LOG_ERROR("Error loading mesh {0}: {1}", filename, aiGetErrorString());
        return UINT32_MAX;
    }

    M_Resources->meshes.push_back(Mesh{});
    Mesh& mesh = M_Resources->meshes.back();
    u32 meshIdx = (u32)M_Resources->meshes.size() - 1u;

    M_Resources->models.push_back(Model{});
    Model& model = M_Resources->models.back();
    model.meshIdx = meshIdx;
    u32 modelIdx = (u32)M_Resources->models.size() - 1u;

    std::string directory = GetDirectoryPart(filename);

    // Create a list of materials
    u32 baseMeshMaterialIndex = (u32)M_Resources->materials.size();
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        M_Resources->materials.push_back(Material{});
        Material& material = M_Resources->materials.back();
        ProcessAssimpMaterial(scene->mMaterials[i], material, directory);
    }

    ProcessAssimpNode(scene, scene->mRootNode, &mesh, baseMeshMaterialIndex, model.materialIdx);

    aiReleaseImport(scene);

    u32 vertexBufferSize = 0;
    u32 indexBufferSize = 0;

    for (u32 i = 0; i < mesh.submeshes.size(); ++i)
    {
        vertexBufferSize += mesh.submeshes[i].vertices.size() * sizeof(float);
        indexBufferSize += mesh.submeshes[i].indices.size() * sizeof(u32);
    }

    glGenBuffers(1, &mesh.vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.indexBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STATIC_DRAW);

    u32 indicesOffset = 0;
    u32 verticesOffset = 0;

    for (u32 i = 0; i < mesh.submeshes.size(); ++i)
    {
        const void* verticesData = mesh.submeshes[i].vertices.data();
        const u32   verticesSize = mesh.submeshes[i].vertices.size() * sizeof(float);
        glBufferSubData(GL_ARRAY_BUFFER, verticesOffset, verticesSize, verticesData);
        mesh.submeshes[i].vertexOffset = verticesOffset;
        verticesOffset += verticesSize;

        const void* indicesData = mesh.submeshes[i].indices.data();
        const u32   indicesSize = mesh.submeshes[i].indices.size() * sizeof(u32);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesSize, indicesData);
        mesh.submeshes[i].indexOffset = indicesOffset;
        indicesOffset += indicesSize;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return modelIdx;
    return 0;
}

#pragma endregion

#pragma region Public 
u32 ModuleResources::LoadProgram(const char* filepath, const char* programName)
{
    std::string programSource = ModuleResources::ReadTextFile(filepath);

    Program program = {};
    program.handle = CreateProgramFromSource(programSource, programName);
    program.filepath = filepath;
    program.programName = programName;
    program.lastWriteTimestamp = ModuleResources::GetFileLastWriteTimestamp(filepath);

    // getting info from the shader
    GLint attributeCount = 0;
    GLsizei attributeNameLength = 0;
    GLsizei attributeSize = 0;
    GLenum attributeType = 0;
    char attributeName[64] = "NONE";
    u8 stride = 0;

    glGetProgramiv(program.handle, GL_ACTIVE_ATTRIBUTES, &attributeCount);

    for (int i = 0; i < attributeCount; ++i)
    {
        glGetActiveAttrib(program.handle, i, ARRAY_COUNT(attributeName), &attributeNameLength, &attributeSize, &attributeType, attributeName);
        GLuint attributeLocation = glGetAttribLocation(program.handle, attributeName);

        if (i == 0)
            attributeSize = 3;
        else
            attributeSize = 2;

        VertexBufferAttribute attribute = { attributeLocation, attributeSize,stride };

        program.vertexInputLayout.attributes.push_back(attribute);
        stride += attributeSize;
    }

    program.vertexInputLayout.stride = stride;
    M_Resources->programs.push_back(program);
    return M_Resources->programs.size() - 1;
}


u32 ModuleResources::LoadTexture2D(const char* filepath)
{
    for (u32 texIdx = 0; texIdx < M_Resources->textures.size(); ++texIdx)
        if (M_Resources->textures[texIdx].filepath == filepath)
            return texIdx;

    Image image = ReadImage(filepath);

    if (image.pixels)
    {
        Texture tex = {};
        tex.handle = CreateTexture2DFromImage(image);
        tex.filepath = filepath;

        u32 texIdx = M_Resources->textures.size();
        M_Resources->textures.push_back(tex);

        FreeImage(image);
        return texIdx;
    }
    else
    {
        return UINT32_MAX;
    }
}
#pragma endregion



