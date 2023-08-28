#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(std::vector<float> Data)
{
	glGenVertexArrays(1, &mRendererID);
	glBindVertexArray(mRendererID);

	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Data[0]) * Data.size(), Data.data(), GL_STATIC_DRAW);
}

VertexArrayObject::~VertexArrayObject()
{
	Delete();
}

void VertexArrayObject::AddAttribute(int size)
{
	mAttributes.push_back(size);

	int strideSize = 0;
	for (int attr : mAttributes)
		strideSize += attr;

	int offset = 0;
	for (unsigned int i = 0; i < mAttributes.size(); i++)
	{
		glVertexAttribPointer(i, mAttributes[i], GL_FLOAT, GL_FALSE, strideSize * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += mAttributes[i];
	}
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(mRendererID);
}

void VertexArrayObject::Delete()
{
	glDeleteVertexArrays(1, &mRendererID);
	//glDeleteBuffers(1, &mVertexBufferID);
}
