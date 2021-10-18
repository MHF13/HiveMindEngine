#include "Globals.h"
#include "Application.h"
#include "LoadModel.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"



LoadModel::LoadModel(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

LoadModel::~LoadModel()
{}

// -----------------------------------------------------------------
bool LoadModel::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	//meter el Load Scene en una funcion y le envias scene, tiene un for que le todas las meshes que tenga el archivo


	const aiScene* scene = aiImportFile("Assets/Models/fish.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
		LOG("Load %s", "Assets/Models/fish.fbx");

		/////////////////////////////////////////////////
		model Mod1;
		aiMesh AAAA = **scene->mMeshes;
		aiMesh AAA = **scene->mMeshes;
		Mod1.num_vertex = AAAA.mNumVertices;
		Mod1.vertex = new float[Mod1.num_vertex * 3];
		memcpy(Mod1.vertex, AAAA.mVertices, sizeof(float) * Mod1.num_vertex * 3);
		LOG("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\nNew mesh with %d vertices", Mod1.num_vertex);

		if (AAAA.HasFaces())
		{
			Mod1.num_index = AAAA.mNumFaces * 3;
			Mod1.index = new uint[Mod1.num_index]; // assume each face is a triangle
			for (uint i = 0; i < AAAA.mNumFaces; ++i)
			{
				if (AAAA.mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
					memcpy(&Mod1.index[i * 3], AAA.mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
		////////////////////////////////////////////////////////

	}
	else
		LOG("Error loading scene %s", "Assets/Models/fish.fbx");
	
	//Dentro del for hara esto con cada ciclo (meter esto en una funcion, void aiMesh)
	
	
	return ret;
}

// -----------------------------------------------------------------
bool LoadModel::CleanUp()
{
	LOG("Cleaning camera");
	// detach log stream
	aiDetachAllLogStreams();
	
	return true;
}

