#pragma once

#include <iostream>
#include <vector>

#include "texture.hpp"
#include "vertex.hpp"

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	/**
	 * Clear the buffer for a new list.
	 */
	void clear();

	/**
	 * Merge the index buffers and fix the idxOffsets before
	 * drawing the batch.
	 */
	void complete();

	/**
	 * Send the blob of vertices to the GPU.
	 */
	void draw() const;

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
	std::vector<Vertex>        mVertices;
	std::vector<std::uint16_t> mIndices;

	DrawChannel  *mChannelList;
	DrawChannel **mChannelTail;
	DrawChannel  *mCurrent;
	DrawChannel  *mFreeChannels;

	Texture  mWhiteTexture;
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
