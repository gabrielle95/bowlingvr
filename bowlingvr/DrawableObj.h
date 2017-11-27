#pragma once

#include <vector>
#include <iterator>
#include <SDL2/SDL.h>
#include <bullet/btBulletDynamicsCommon.h>
class DrawableObj
{
public:
	DrawableObj();
	~DrawableObj();
	bool Draw();
private:
	GLuint vao, vbo, ebo;
	GLuint textureID;
	SDL_Surface* texture;
	std::vector<GLuint>elements;
	std::vector<float>vertices;
protected:
	void loadVertices(float data[], size_t size);
	void loadVertices(std::vector<float>data);

	void loadTexture(std::string fileName);

	void loadElements(GLuint data[], size_t size);
	void loadElements(std::vector<unsigned int>data);

	void Bind_vao();
	void Unbind_vao();
};