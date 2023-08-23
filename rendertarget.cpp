#include <iostream>
#include <cassert>

#include <GL/glew.h>

#include "color.hpp"
#include "glcheck.hpp"
#include "rendertarget.hpp"

RenderTarget::RenderTarget()
	: mChannelList(nullptr)
	, mChannelTail(&mChannelList)
	, mCurrent(nullptr)
	, mFreeChannels(nullptr)
{
	mWhiteTexture.create(1, 1, &Color::White);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
}

RenderTarget::~RenderTarget()
{
	glDeleteBuffers(1, &mEBO);
	glDeleteBuffers(1, &mVBO);

	beginBatch();
	DrawChannel *channel = mFreeChannels;
	while (channel)
	{
		DrawChannel *next = channel->next;
		delete channel;
		channel = next;
	}
}

void
RenderTarget::beginBatch()
{
	mVertices.clear();
	*mChannelTail = mFreeChannels;
	mFreeChannels = mChannelList;
	mChannelList = mCurrent = nullptr;
	mChannelTail = &mChannelList;
}

void
RenderTarget::endBatch()
{
	mIndices.clear();
	for (auto channel = mChannelList; channel; channel = channel->next)
	{
		channel->idxOffset = mIndices.size() * sizeof(std::uint16_t);
		mIndices.insert(mIndices.end(),
				channel->idxBuffer.begin(),
				channel->idxBuffer.end());
	}
}

void
RenderTarget::drawBatch() const
{
	if (!mChannelList)
	{
		return;
	}

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER,
			     mVertices.size() * sizeof(mVertices[0]),
			     mVertices.data(),
			     GL_STREAM_DRAW));

	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			     mIndices.size() * sizeof(mIndices[0]),
			     mIndices.data(),
			     GL_STREAM_DRAW));

	glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(
			0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<GLvoid*>(offsetof(Vertex, pos))));
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribPointer(
			1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<GLvoid*>(offsetof(Vertex, uv))));
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(
			2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
			reinterpret_cast<GLvoid*>(offsetof(Vertex, color))));

	for (auto channel = mChannelList; channel; channel = channel->next)
	{
		if (!channel->texture || channel->idxBuffer.empty())
		{
			continue;
		}
		Texture::bind(channel->texture, 0);
		glCheck(glDrawElementsBaseVertex(
				GL_TRIANGLES,
				channel->idxBuffer.size(),
				GL_UNSIGNED_SHORT,
				reinterpret_cast<GLvoid*>(channel->idxOffset),
				channel->vtxOffset));
	}
	glCheck(glDisableVertexAttribArray(2));
	glCheck(glDisableVertexAttribArray(1));
	glCheck(glDisableVertexAttribArray(0));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

RenderTarget::DrawChannel *
RenderTarget::newChannel(const Texture *texture, unsigned vtxOffset)
{
	DrawChannel *channel;
	if (mFreeChannels)
	{
		channel = mFreeChannels;
		channel->idxBuffer.clear();
		mFreeChannels = channel->next;
	}
	else
	{
		channel = new DrawChannel();
	}

	// channel initialization
	channel->texture = texture;
	channel->vtxOffset = vtxOffset;
	channel->idxOffset = 0;
	channel->next = nullptr;

	// update the channel list
	*mChannelTail = channel;
	mChannelTail = &channel->next;

	return channel;
}

void
RenderTarget::setTexture(const Texture *texture)
{
	// the null texture means a white texture;
	if (!texture)
	{
		texture = &mWhiteTexture;
	}

	// return early if the texture is the same
	if (mCurrent && mCurrent->texture == texture)
	{
		return;
	}

	// look for a channel with the same texture
	DrawChannel *channel;
	for (channel = mChannelList; channel; channel = channel->next)
	{
		if (channel->texture == texture)
		{
			break;
		}
	}

	// or create a new one if it doesn't exist
	if (!channel)
	{
		channel = newChannel(texture, mVertices.size());
	}

	mCurrent = channel;
}

std::uint16_t
RenderTarget::getPrimIndex(unsigned idxCount, unsigned vtxCount)
{
	// ensure we have a current channel
	if (!mCurrent)
	{
		mCurrent = newChannel(&mWhiteTexture, 0);
	}

	// ensure we have enough space for the indices
	unsigned index = mVertices.size() - mCurrent->vtxOffset;
	if (index + vtxCount > UINT16_MAX)
	{
		mCurrent = newChannel(mCurrent->texture, mVertices.size());
		index = 0;
	}

	// reserve the space for the vertices and the indices
	mVertices.reserve(mVertices.size() + vtxCount);
	mCurrent->idxBuffer.reserve(mCurrent->idxBuffer.size() + idxCount);

	return index;
}

void
RenderTarget::addIndex(std::uint16_t index)
{
	mCurrent->idxBuffer.push_back(index);
}

void
RenderTarget::addVertex(const Vertex &vertex)
{
	mVertices.push_back(vertex);
}
