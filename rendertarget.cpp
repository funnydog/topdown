#include <iostream>
#include <cassert>

#include <GL/glew.h>

#include "color.hpp"
#include "glcheck.hpp"
#include "rendertarget.hpp"
#include "window.hpp"

RenderTarget::RenderTarget(const Window &window)
	: mIsBatching(true)
	, mChannelList(nullptr)
	, mChannelTail(&mChannelList)
	, mCurrent(nullptr)
	, mFreeChannels(nullptr)
	, mCanvas(&window)
	, mVBO(0)
	, mEBO(0)
{
	initialize();
}

RenderTarget::~RenderTarget()
{
	glCheck(glBindVertexArray(0));
	glCheck(glDeleteVertexArrays(1, &mTextureVAO));
	glCheck(glDeleteBuffers(1, &mEBO));
	glCheck(glDeleteBuffers(1, &mVBO));

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
RenderTarget::setCanvas(const Canvas &canvas)
{
	mCanvas = &canvas;
}

void
RenderTarget::initialize()
{
	mWhiteTexture.create(1, 1, &Color::White);
	mTextureShader.attachFile(ShaderType::Vertex, "assets/shaders/default.vert");
	mTextureShader.attachFile(ShaderType::Fragment, "assets/shaders/default.frag");
	mTextureShader.link();

	glm::vec2 size = mCanvas->getSize();
	mDefaultView.setCenter(size * 0.5f);
	mDefaultView.setSize(size);
	mView = mDefaultView;

	// bind a buffer to allow calling glVertexAttribPointer()
	glCheck(glGenBuffers(1, &mVBO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));

	glCheck(glGenBuffers(1, &mEBO));
	glCheck(glGenVertexArrays(1, &mTextureVAO));

	// setup the data layout for mTextureVAO
	glCheck(glBindVertexArray(mTextureVAO));
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

	// configure the shaders
	mTextureShader.use();
	ShaderUniform projection = mTextureShader.getUniform("Projection");
	projection.setMatrix4(mView.getTransform());
}

const View&
RenderTarget::getDefaultView() const
{
	return mDefaultView;
}

const View&
RenderTarget::getView() const
{
	return mView;
}

void
RenderTarget::setView(const View &view)
{
	mView = view;
}

void
RenderTarget::clear(Color color)
{
	glm::vec4 clearColor(color);
	glCheck(glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
}

void
RenderTarget::addLayer()
{
	// NOTE: by deleting the texture->channel association
	// we force to build another set of channels.
	mChannelMap.clear();
}

void
RenderTarget::beginBatch()
{
	mVertices.clear();
	mChannelMap.clear();
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
RenderTarget::draw()
{
	if (!mChannelList)
	{
		return;
	}

	if (mIsBatching)
	{
		mIsBatching = false;
		endBatch();
	}

	glCheck(glBindVertexArray(mTextureVAO));
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

	mTextureShader.use();

	const Texture *currentTexture = nullptr;
	for (auto channel = mChannelList; channel; channel = channel->next)
	{
		// skip empty channels
		if (!channel->texture || channel->idxBuffer.empty())
		{
			continue;
		}

		// dont bind against the same texture
		if (currentTexture != channel->texture)
		{
			currentTexture = channel->texture;
			currentTexture->bind(0);
		}

		// draw
		glCheck(glDrawElementsBaseVertex(
				GL_TRIANGLES,
				channel->idxBuffer.size(),
				GL_UNSIGNED_SHORT,
				reinterpret_cast<GLvoid*>(channel->idxOffset),
				channel->vtxOffset));
	}
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
	// switch to batching state if needed
	if (!mIsBatching)
	{
		mIsBatching = true;
		beginBatch();
	}

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
	if (auto it = mChannelMap.find(texture); it != mChannelMap.end())
	{
		// channel found
		mCurrent = it->second;
	}
	else
	{
		// or add a new one
		mCurrent = newChannel(texture, mVertices.size());
		mChannelMap[texture] = mCurrent;
	}
}

std::uint16_t
RenderTarget::getPrimIndex(unsigned idxCount, unsigned vtxCount)
{
	// ensure we have a current channel and the rendertarget is in
	// batching state.
	if (!mCurrent)
	{
		mIsBatching = true;
		beginBatch();
		mCurrent = newChannel(&mWhiteTexture, 0);
	}

	// ensure we have enough space for the indices
	unsigned index = mVertices.size() - mCurrent->vtxOffset;
	if (index + vtxCount > UINT16_MAX)
	{
		mCurrent = newChannel(mCurrent->texture, mVertices.size());
		mChannelMap[mCurrent->texture] = mCurrent;
		index = 0;
	}

	// reserve the space for the vertices and the indices
	mVertices.reserve(mVertices.size() + vtxCount);
	mCurrent->idxBuffer.reserve(mCurrent->idxBuffer.size() + idxCount);

	return index;
}

Vertex*
RenderTarget::getVertexArray(unsigned vtxCount)
{
	auto size = mVertices.size();
	mVertices.resize(size + vtxCount);
	return &mVertices[size];
}
