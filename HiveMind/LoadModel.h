#pragma once
#include <vector>
#include <string>
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#pragma comment (lib, "ExternalLib/Assimp/libx86/assimp-vc142-mt.lib")

#include "glew.h"

#include <Importer.hpp>      // C++ importer interface  // Output data structure
#include "MathGeoLib.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "il.h"

#include "Globals.h"

struct Vertex
{
    float3 m_pos;
    float2 m_tex;
    float3 m_normal;

    Vertex() {}

    Vertex(const float3& pos, const float2& tex, const float3& normal)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = normal;
    }

    Vertex(const float3& pos, const float2& tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = float3(0.0f, 0.0f, 0.0f);
    }
};
/*
uint id_index = 0; // index in VRAM
uint num_index = 0;
uint* index = nullptr;
uint id_vertex = 0; // unique vertex in VRAM
uint num_vertex = 0;
float* vertex = nullptr;
*/
class Mesh
{
public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const char* Filename);
    bool LoadTexture(const char* Filename);
    void Render();

   

private:
    bool InitFromScene(const aiScene* pScene, const char* Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitTexture(const aiScene* pScene, const char* Filename);
    void Clear();
    void Init(const std::vector<float3>& Vertices, const std::vector<float2>& texCoords, const std::vector<unsigned int>& Indices);


#define INVALID_MATERIAL 0xFFFFFFFF

private:
    const char* filePath;

    GLuint textureID;
    uint CHECKERS_HEIGHT = 64;
    uint CHECKERS_WIDTH = 64;
    GLubyte checkerImage[64][64][4];

    GLuint VB;
    GLuint TB;
    GLuint IB;
    unsigned int numIndices;
    unsigned int materialIndex;

    ////////textures
    ILuint imageID;



    std::vector<Mesh> m_Entries;
    //std::vector<GLuint> textureID;

};
