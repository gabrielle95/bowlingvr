#include "Windows.h"
#include <GL/glew.h>
#include "AssimpModel.h"

AssimpModel::AssimpModel(Shader * shader, std::string filePath)
{
	Assimp::Importer importer;
	std::cout << "Assimp: Loading " << filePath << " ..." << std::endl;
	const aiScene *scene = importer.ReadFile(filePath, 
							aiProcess_CalcTangentSpace |
							aiProcess_Triangulate |
							aiProcess_JoinIdenticalVertices |
							aiProcess_SortByPType);

	if (!scene)
	{
		std::cerr << "Assimp: ERROR. Couldn't load model " << filePath << "!" << std::endl;
		std::cerr << importer.GetErrorString() << std::endl;
		return;
	}
	std::cout << "Assimp: SUCCESS loading " << filePath << "!" << std::endl;

	const int iVertexTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D);
	int iTotalVertices = 0;

	/*for(int i = 0; i < scene->mNumMeshes; i++)
	{

	}*/
}
