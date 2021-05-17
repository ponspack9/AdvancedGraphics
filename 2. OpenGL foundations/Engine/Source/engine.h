//
// engine.h: This file contains the types and definitions relative to the engine.
//

#pragma once
#pragma warning(disable : 4267) // conversion from X to Y, possible loss of data

#pragma region Defines

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))

#define ASSERT(condition, message) assert((condition) && message)

#define KB(count) (1024*(count))
#define MB(count) (1024*KB(count))
#define GB(count) (1024*MB(count))

#define PI  3.14159265359f
#define TAU 6.28318530718f

#pragma endregion

#pragma region Typedef

typedef glm::vec2               vec2;
typedef glm::vec3               vec3;
typedef glm::vec4               vec4;
typedef glm::ivec2              ivec2;
typedef glm::ivec3              ivec3;
typedef glm::ivec4              ivec4;
typedef char                    i8;
typedef short                   i16;
typedef int                     i32;
typedef long long int           i64;
typedef unsigned char           u8;
typedef unsigned short          u16;
typedef unsigned int            u32;
typedef unsigned long long int  u64;
typedef float                   f32;
typedef double                  f64;

#pragma endregion

#pragma region Enums
enum Mode
{
    Mode_TexturedQuad,
    Mode_Count
};

enum MouseButton {
    LEFT,
    RIGHT,
    MOUSE_BUTTON_COUNT
};

enum Key {
    K_SPACE,
    K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
    K_A, K_B, K_C, K_D, K_E, K_F, K_G, K_H, K_I, K_J, K_K, K_L, K_M,
    K_N, K_O, K_P, K_Q, K_R, K_S, K_T, K_U, K_V, K_W, K_X, K_Y, K_Z,
    K_ENTER, K_ESCAPE,
    KEY_COUNT
};

enum ButtonState {
    BUTTON_IDLE,
    BUTTON_PRESS,
    BUTTON_PRESSED,
    BUTTON_RELEASE
};

#pragma endregion

#pragma region Structs

    struct Image
    {
        void* pixels;
        ivec2 size;
        i32   nchannels;
        i32   stride;
    };

    struct Texture
    {
        GLuint      handle;
        std::string filepath;
    };

    struct Input {
        glm::vec2   mousePos;
        glm::vec2   mouseDelta;
        ButtonState mouseButtons[MOUSE_BUTTON_COUNT];
        ButtonState keys[KEY_COUNT];
    };
    
    // VBO
    struct VertexBufferAttribute
    {
        u8 location;
        u8 componentCount;
        u8 offset;
    };

    struct VertexBufferLayout
    {
        std::vector<VertexBufferAttribute> attributes;
        u8 stride;
    };

    // Shader
    struct VertexShaderAttribute
    {
        u8 location;
        u8 componentCount;
    };

    struct VertexShaderLayout
    {
        std::vector<VertexShaderAttribute> attributes;
    };

    struct Program
    {
        GLuint             handle;
        std::string        filepath;
        std::string        programName;
        u64                lastWriteTimestamp;
        VertexBufferLayout vertexInputLayout;

        // activate the shader
    // ------------------------------------------------------------------------
        void use() const
        {
            glUseProgram(handle);
        }
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) const
        {
            glUniform1i(glGetUniformLocation(handle, name.c_str()), (int)value);
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string& name, int value) const
        {
            glUniform1i(glGetUniformLocation(handle, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string& name, float value) const
        {
            glUniform1f(glGetUniformLocation(handle, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value) const
        {
            glUniform2fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string& name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(handle, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string& name, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string& name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(handle, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string& name, const glm::vec4& value) const
        {
            glUniform4fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, float x, float y, float z, float w) const
        {
            glUniform4f(glGetUniformLocation(handle, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string& name, const glm::mat2& mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string& name, const glm::mat3& mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string& name, const glm::mat4& mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
    };

    // VBO + Shader
    struct VAO
    {
        GLuint handle;
        GLuint programHandle;
    };

    // Models
    struct Submesh
    {
        VertexBufferLayout vertexBufferLayout;
        std::vector<float> vertices;
        std::vector<u32> indices;
        u32 vertexOffset;
        u32 indexOffset;

        std::vector<VAO> vaos; // to use different shaders to each submesh
    };

    struct Mesh
    {
        std::vector<Submesh> submeshes;
        GLuint vertexBufferHandle;
        GLuint indexBufferHandle;
    };

    struct Material
    {
        std::string name;
        vec3 albedo;
        vec3 emissive;
        f32 smoothness;
        Texture* albedoTexture;
        Texture* emissiveTexture;
        Texture* specularTexture;
        Texture* normalsTexture;
        Texture* bumpTexture;

        std::vector<Texture*> GetTextures()
        {
            std::vector<Texture*> tex;
            tex.push_back(albedoTexture);
            tex.push_back(emissiveTexture);
            tex.push_back(specularTexture);
            tex.push_back(normalsTexture);
            tex.push_back(bumpTexture);

            return tex;
        }
    };



#pragma endregion

#pragma region Globals

#pragma endregion