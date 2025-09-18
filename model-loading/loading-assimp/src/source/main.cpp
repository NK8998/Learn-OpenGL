
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main()
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile("does_not_exist.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene)
	{
		std::cout << "Assimp is alive! Error message: " << importer.GetErrorString() << std::endl;
	}
	else
	{
		std::cout << "Unexpectedly loaded a scene?? Assimp definitely works." << std::endl;
	}

	std::cout << "Hello world!" << std::endl;
	
}