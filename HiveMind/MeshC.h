/*#pragma once
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

#include "GameObject.h"
class Component;
class GameObject;
*/
/*struct Vertex
{
    float3 m_pos;
    float2 m_tex;
    float3 m_normal;

    Vertex() {}

    Vertex(const float3& pos, const float3& normal)
    {
        m_pos = pos;
        m_normal = normal;
    }

    Vertex(const float3& pos, const float2& tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = float3(0.0f, 0.0f, 0.0f);
    }
};*/

//class MeshC : public Component
//{
//public:
//    MeshC(GameObject* _owner, const char* fileName) : Component(_owner, ComponentType::MESH) 
//    {
//        VB = 0;
//        IB = 0;
//        numIndices = 0;
//
//        LoadMesh(fileName);
//    }
//
//    ~MeshC();
//
//    bool LoadMesh(const char* fileName);
//    void Render();
//
//   
//
//private:
//    bool InitFromScene(const aiScene* pScene, const char* fileName);
//    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
//    void Clear();
//    void Init(const std::vector<float3>& Vertices, const std::vector<unsigned int>& Indices);
//
//
//#define INVALID_MATERIAL 0xFFFFFFFF
//
//private:
//    const char* filePath;
//
//    GLuint textureID;
//    uint CHECKERS_HEIGHT = 64;
//    uint CHECKERS_WIDTH = 64;
//    GLubyte checkerImage[64][64][4];
//
//    GLuint VB;
//    GLuint IB;
//    unsigned int numIndices;
//    unsigned int materialIndex;
//
//
//
//
//    std::vector<MeshC> m_Entries;
//   
//};
