#pragma once

#include <iostream>
#include <vector>

#include "color.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "view.hpp"

class Canvas;
class Window;

class RenderTarget
{
public:
	RenderTarget(const Window &window);
	~RenderTarget();

	/**
	 * Clear the target with the given @color.
	 * @param[in] color
	 */
	void clear(Color = Color::Black);

	/**
	 * Clear the buffer for a new list.
	 */
	void beginBatch();

	/**
	 * Merge the index buffers and fix the idxOffsets before
	 * drawing the batch.
	 */
	void endBatch();

	/**
	 * Send the blob of vertices to the GPU.
	 */
	void drawBatch() const;

	/**
	 * Set the texture for the next primitive.
	 */
	void setTexture(const Texture *texture);

	/**
	 * Get the base index for the primitive and reserve space in
	 * the vertex and index buffers.
	 */
	std::uint16_t getPrimIndex(unsigned idxCount, unsigned vtxCount);

	void addIndex(std::uint16_t index);

	void addVertex(const Vertex &vertex);

	template <typename Iterator> void addIndices(unsigned base, Iterator start, Iterator end);

	template <typename Iterator> void addVertices(Iterator start, Iterator end);

	void setCanvas(const Canvas &canvas);

protected:
	void initialize();

private:
	struct DrawChannel
	{
		const Texture *texture;
		unsigned vtxOffset;
		unsigned idxOffset;
		std::vector<std::uint16_t> idxBuffer;
		DrawChannel *next;
	};

private:
	DrawChannel *newChannel(const Texture *texture, unsigned vtxOffset);

private:
	View mDefaultView;
	View mView;

	std::vector<Vertex>        mVertices;
	std::vector<std::uint16_t> mIndices;

	DrawChannel  *mChannelList;
	DrawChannel **mChannelTail;
	DrawChannel  *mCurrent;
	DrawChannel  *mFreeChannels;

	const Canvas *mCanvas;

	Texture  mWhiteTexture;
	Shader   mShader;
	unsigned mVBO;
	unsigned mEBO;
};

template <typename Iterator>
void
RenderTarget::addIndices(unsigned base, Iterator start, Iterator end)
{
	for (; start != end; ++start)
	{
		mCurrent->idxBuffer.push_back(base + *start);
	}
}

template <typename Iterator>
void
RenderTarget::addVertices(Iterator start, Iterator end)
{
	mVertices.insert(mVertices.end(), start, end);
}
