#pragma once

#include <glad/glad.h>
#include <vector>
#include <iostream>

class VertexArrayObject
{
private:
	std::vector<int> mAttributes;

	unsigned int mRendererID;
	unsigned int mVertexBufferID;

public:
	~VertexArrayObject();
	VertexArrayObject(float* Data, unsigned int size);

	void AddAttribute(int size);

	void Bind();
	void Delete();
};