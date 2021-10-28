#include "GameObject.h"

////////////////MESH////////////////////////
MeshC::MeshC(GameObject* _owner, const char* fileName) : Component(_owner, ComponentType::MESH)
{
	VB = 0;
	IB = 0;
	numIndices = 0;

	LoadMesh(fileName);

}

bool MeshC::LoadMesh(const char* fileName)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	bool ret = false;
	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		InitFromScene(scene, fileName);
		ret = true;
	}
	else
	{
		LOG("Error loading '%s'", fileName);
	}

	return ret;
}
void MeshC::InitFromScene(const aiScene* pScene, const char* fileName)
{
	m_Entries.resize(pScene->mNumMeshes);

	// Initialize the Meshses in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMeshs = pScene->mMeshes[i];
		activeMeshes.push_back(paiMeshs);
		InitMesh(i, paiMeshs);
	}
}

void MeshC::Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
	const std::vector<unsigned int>& Indices)
{
	numIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &TB);
	glBindBuffer(GL_ARRAY_BUFFER, TB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &Indices[0], GL_STATIC_DRAW);

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

}
void MeshC::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].materialIndex = paiMesh->mMaterialIndex;

	std::vector<float3> Vertices;
	std::vector<float2> texCord;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertices.push_back(float3(pPos->x, pPos->y, pPos->z));
		texCord.push_back(float2(pTexCoord->x, pTexCoord->y));
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[Index].Init(Vertices, texCord, Indices);
}
void MeshC::Render()
{
	TransformC* t = new TransformC(nullptr);
	t = dynamic_cast<TransformC*>(owner->GetComponent(ComponentType::TRANSFORM));
	glPushMatrix();
	glMultMatrixf(t->transform.M);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].TB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].VB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].materialIndex;

		glDrawElements(GL_TRIANGLES, m_Entries[i].numIndices, GL_UNSIGNED_INT, 0);
	}

	glPopMatrix();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void MeshC::Clear()
{
	aiDetachAllLogStreams();
}
////////////////GAMEOBJECT///////////////
GameObject::GameObject(const char* _name, GameObject* _parent, const char* filePath, int _id)
{
	name = _name;
	parent = _parent;
	enabled = true;
	id = _id;
	if (parent != nullptr)
	{
		parent->childs.push_back(this);
	}
	if(_parent != nullptr)
	{
		AddComponent(ComponentType::TRANSFORM);
		AddComponent(ComponentType::MESH, filePath);
	}
	
}

GameObject::~GameObject()
{
}
void GameObject::Update()
{
	transform->Update();
	mesh->Update();
}
void GameObject::Enable()
{
	enabled = true;
}

void GameObject::Disable()
{
	enabled = false;
}

void GameObject::CleanUp()
{
}

void GameObject::AddComponent(ComponentType type, const char* fileName)
{
	//Component* ret = nullptr;
	if (type == ComponentType::TRANSFORM)
	{

		transform = new TransformC(this);
		components.push_back(transform);
	}
	if (type == ComponentType::MESH)
	{
		mesh = new MeshC(this, fileName);
		components.push_back(mesh);

	}

}

void GameObject::RemoveComponent(ComponentType type)
{

}
Component* GameObject::GetAllComponents(ComponentType type)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components.at(i)->type == type)
		{
			return components.at(i);
		}
	}

	return nullptr;

}

Component* GameObject::GetComponent(ComponentType _type)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components.at(i)->type == _type)
		{
			return components.at(i);
		}
	}

	return nullptr;
}
