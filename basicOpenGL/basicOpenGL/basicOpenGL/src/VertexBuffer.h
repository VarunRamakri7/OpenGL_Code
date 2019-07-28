#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // Internal ID of renderer if using OpenGL

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};