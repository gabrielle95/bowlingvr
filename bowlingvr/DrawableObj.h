#pragma once

#include <vector>
#include <iterator>

class DrawableObj
{
public:
	DrawableObj();
	~DrawableObj();
	bool Draw();
private:
	GLuint vao, vbo, ebo;
	std::vector<GLuint>elements;
	std::vector<float>vertices;
protected:
	void loadVertices(float data[], size_t size);
	void loadVertices(std::vector<float>data);

	void loadElements(GLuint data[], size_t size);
	void loadElements(std::vector<float>data);

	void Bind_vao();
	void Unbind_vao();
};