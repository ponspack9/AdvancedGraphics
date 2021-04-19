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

    // VBO
    glGenBuffers(1, &App->embeddedVertices);
    glBindBuffer(VBO, App->embeddedVertices);
    glBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(VBO, 0);

    // EBO
    glGenBuffers(1, &App->embeddedElements);
    glBindBuffer(EBO, App->embeddedElements);
    glBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(EBO, 0);

    // VAO
    // Element 0, has 3 components, that are floats, no need to normalize, 
    glGenVertexArrays(1, &App->vao);
    glBindVertexArray(App->vao);
    glBindBuffer(VBO, App->embeddedVertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexV3V2), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexV3V2), (void*)12);
    glEnableVertexAttribArray(1);

    glBindBuffer(EBO, App->embeddedElements);
    glBindVertexArray(0);


    // Programs
    App->texturedGeometryProgramIdx = M_Resources->LoadProgram("shaders.glsl", "TEXTURED_GEOMETRY");

    Program& texturedGeometryProgram = App->programs[App->texturedGeometryProgramIdx];

    App->programUniformTexture = glGetUniformLocation(texturedGeometryProgram.handle, "uTexture");

    // Textures
    App->diceTexIdx = M_Resources->LoadTexture2D("dice.png");
    App->whiteTexIdx = M_Resources->LoadTexture2D("color_white.png");
    App->blackTexIdx = M_Resources->LoadTexture2D("color_black.png");
    App->normalTexIdx = M_Resources->LoadTexture2D("color_normal.png");
    App->magentaTexIdx = M_Resources->LoadTexture2D("color_magenta.png");


    App->mode = Mode::Mode_TexturedQuad;

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
    for (int i = 0; i < App->programs.size(); ++i)
    {
        Program& program = App->programs[i];
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
        LOG_ERROR("Could not open file %s", filename);
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
    App->programs.push_back(program);

    return App->programs.size() - 1;
}


u32 ModuleResources::LoadTexture2D(const char* filepath)
{
    for (u32 texIdx = 0; texIdx < App->textures.size(); ++texIdx)
        if (App->textures[texIdx].filepath == filepath)
            return texIdx;

    Image image = ReadImage(filepath);

    if (image.pixels)
    {
        Texture tex = {};
        tex.handle = CreateTexture2DFromImage(image);
        tex.filepath = filepath;

        u32 texIdx = App->textures.size();
        App->textures.push_back(tex);

        FreeImage(image);
        return texIdx;
    }
    else
    {
        return UINT32_MAX;
    }
}

#pragma endregion



