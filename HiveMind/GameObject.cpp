#include "GameObject.h"

////////////////MESH////////////////////////

void MeshC::Update()
{
	TransformC* t = owner->transform;

	glPushMatrix();
	glMultMatrixf(t->GetGlobalTransform());

	TextureC* tex = owner->texture;

	for (unsigned int i = 0; i < meshEnt.size(); ++i)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshEnt[i].vertexB);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (tex != nullptr && tex->GetEnable())
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, tex->texture);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, meshEnt[i].textureB);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshEnt[i].normalB);
		glNormalPointer(GL_FLOAT, 0, NULL);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEnt[i].indexB);

		const unsigned int MaterialIndex = meshEnt[i].materialIndx;

		glDrawElements(GL_TRIANGLES, meshEnt[i].numIndx, GL_UNSIGNED_INT, NULL);

	}

	glPopMatrix();
}

bool MeshC::LoadMesh(const char* fileName)
{

	bool ret = false;
	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		meshEnt.resize(scene->mNumMeshes);

		for (unsigned int i = 0; i < meshEnt.size(); i++) {
			const aiMesh* paiMeshs = scene->mMeshes[i];
			meshes.push_back(paiMeshs);
			Init(i, paiMeshs);
		}
	}
	else
	{
		LOG("Error loading '%s'", fileName);
	}
	return ret;
}

void MeshC::Buffers(const std::vector<vec3>& vertex, const std::vector<vec2>& textureC, const std::vector<vec3>& normals, const std::vector<unsigned int>& index)
{
	numIndx = index.size();

	glGenBuffers(1, &vertexB);
	glBindBuffer(GL_ARRAY_BUFFER, vertexB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size() * 3, &vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &textureB);
	glBindBuffer(GL_ARRAY_BUFFER, textureB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureC.size() * 2, &textureC[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalB);
	glBindBuffer(GL_ARRAY_BUFFER, normalB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size() * 3, &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index.size(), &index[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG("All buffers Ready!");
}

void MeshC::Init(unsigned int index, const aiMesh* paiMesh)
{
	meshEnt[index].materialIndx = paiMesh->mMaterialIndex;

	std::vector<vec3> vertices;
	std::vector<vec2> texCoords;
	std::vector<vec3> normals;
	std::vector<unsigned int> indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; ++i)
	{
		const aiVector3D* pos = &(paiMesh->mVertices[i]);
		const aiVector3D* normal = &(paiMesh->mNormals[i]);
		const aiVector3D* texCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		vertices.push_back(vec3(pos->x, pos->y, pos->z));
		texCoords.push_back(vec2(texCoord->x, texCoord->y));
		normals.push_back(vec3(normal->x, normal->y, normal->z));
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; ++i)
	{
		const aiFace& f = paiMesh->mFaces[i];
		indices.push_back(f.mIndices[0]);
		indices.push_back(f.mIndices[1]);
		indices.push_back(f.mIndices[2]);
	}

	meshEnt[index].Buffers(vertices, texCoords, normals, indices);
}

////////////////GAMEOBJECT///////////////
GameObject::GameObject(const char* _name, GameObject* _parent, const char* filePath, const char* materialPath)
{
	name = _name;
	parent = _parent;
	active = true;
	if (parent != nullptr)
	{
		parent->childs.push_back(this);
	}
	if(_parent != nullptr)
	{
		AddComponent(ComponentType::TRANSFORM);
		if (filePath != NULL)
			AddComponent(ComponentType::MESH, filePath);
		if (materialPath != NULL)    
			AddComponent(ComponentType::TEXTURE, materialPath);
	}
	
}

GameObject::~GameObject()
{
}
void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i)->GetEnable())
		{
			components.at(i)->Update();
		}
	}

}

void GameObject::AddComponent(ComponentType type, const char* fileName)
{
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
	if (type == ComponentType::TEXTURE)
	{
		texture = new TextureC(this, fileName);
		components.push_back(texture);
	}

}

void GameObject::RemoveComponent(ComponentType type)
{

}
Component* GameObject::GetAllComponents(ComponentType type)
{
	for (size_t i = 0; i < components.size(); i++)
		if (components.at(i)->type == type)
			return components.at(i);
		
	

	return nullptr;

}

Component* GameObject::GetComponent(ComponentType _type)
{
	for (size_t i = 0; i < components.size(); i++)
		if (components.at(i)->type == _type)
			return components.at(i);
		
	

	return nullptr;
}

void TextureC::LoadTexture(const char* Path)
{
	MeshC* renderMesh = owner->mesh;

	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(Path))
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		renderMesh->textureId = ilutGLBindTexImage();
		glBindTexture(GL_TEXTURE_2D, renderMesh->textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);


		ilDeleteImages(1, &imageId);
		glBindTexture(GL_TEXTURE_2D, 0);

		widthTex = ilGetInteger(IL_IMAGE_WIDTH);
		heightTex = ilGetInteger(IL_IMAGE_HEIGHT);
		texture = renderMesh->textureId;
	}

}

