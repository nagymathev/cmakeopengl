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
	VertexArrayObject(std::vector<float> Data);

	void AddAttribute(int size);

	void Bind();
	void Delete();
};