#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "color.hpp"
#include "font.hpp"
#include "rectangleshape.hpp"
#include "glcheck.hpp"
#include "rendertarget.hpp"
#include "window.hpp"

namespace
{
static const std::uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };
static const glm::vec2 units[] = {
	{ 0.f, 0.f },
	{ 0.f, 1.f },
	{ 1.f, 0.f },
	{ 1.f, 1.f },
};
}

RenderTarget::RenderTarget()
	: mVertexOffset(0)
	, mVertexCount(0)
	, mIndexOffset(0)
	, mIndexCount(0)
	, mIsBatching(true)
	, mChannelList(nullptr)
	, mChannelTail(&mChannelList)
	, mCurrent(nullptr)
	, mFreeChannels(nullptr)
	, mTextureVAO(0)
	, mPosUVVAO(0)
	, mPosUVColorVAO(0)
	, mVBO(0)
	, mEBO(0)
{
	mWhiteTexture.create(1, 1, &Color::White);

        // shader creation and configuration
	mTextureShader.create();
	mTextureShader.attachFile(ShaderType::Vertex, "assets/shaders/default.vert");
	mTextureShader.attachFile(ShaderType::Fragment, "assets/shaders/default.frag");
	mTextureShader.link();

	mUniformColorShader.create();
	mUniformColorShader.attachFile(ShaderType::Vertex, "assets/shaders/simple.vs");
	mUniformColorShader.attachFile(ShaderType::Fragment, "assets/shaders/uniformcolor.fs");
	mUniformColorShader.link();

	// bind a buffer to allow calling glVertexAttribPointer()
	glCheck(glGenBuffers(1, &mVBO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glGenBuffers(1, &mEBO));

	// setup the data layout for mTextureVAO
	glCheck(glGenVertexArrays(1, &mTextureVAO));
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

	// layout for PosUV
	glCheck(glGenVertexArrays(1, &mPosUVVAO));
	glCheck(glBindVertexArray(mPosUVVAO));
	glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(
		        0, 2, GL_FLOAT, GL_FALSE, sizeof(PosUV),
		        reinterpret_cast<GLvoid*>(offsetof(PosUV, pos))));
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribPointer(
		        1, 2, GL_FLOAT, GL_FALSE, sizeof(PosUV),
		        reinterpret_cast<GLvoid*>(offsetof(PosUV, uv))));

	// layout for PosUVColor
	glCheck(glGenVertexArrays(1, &mPosUVColorVAO));
	glCheck(glBindVertexArray(mPosUVColorVAO));
	glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(
		        0, 2, GL_FLOAT, GL_FALSE, sizeof(PosUVColor),
		        reinterpret_cast<GLvoid*>(offsetof(PosUVColor, pos))));
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribPointer(
		        1, 2, GL_FLOAT, GL_FALSE, sizeof(PosUVColor),
		        reinterpret_cast<GLvoid*>(offsetof(PosUVColor, uv))));
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(
		        2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PosUVColor),
		        reinterpret_cast<GLvoid*>(offsetof(PosUVColor, color))));
}

RenderTarget::~RenderTarget()
{
	glCheck(glBindVertexArray(0));
	glCheck(glDeleteVertexArrays(1, &mPosUVColorVAO));
	glCheck(glDeleteVertexArrays(1, &mPosUVVAO));
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
RenderTarget::setViewport(unsigned width, unsigned height)
{
	glm::mat4 proj = glm::ortho(
		0.0f, static_cast<GLfloat>(width),
		static_cast<GLfloat>(height), 0.0f,
		-1.0f, 1.0f);
	mTextureShader.use();
	mTextureShader.getUniform("Projection").setMatrix4(proj);
	mUniformColorShader.use();
	mUniformColorShader.getUniform("projection").setMatrix4(proj);
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

void
RenderTarget::reserve(unsigned vertexCount, std::span<const std::uint16_t> indices)
{
	auto base = mVertexCount - mVertexOffset;
	if (base + vertexCount > UINT16_MAX)
	{
		saveBatch();
		base = 0;
	}
	mVertexCount += vertexCount;
	for (auto i : indices)
	{
		mIndices.push_back(base + i);
	}
	mIndexCount += indices.size();
}

void
RenderTarget::startBatch()
{
	mIndices.clear();
	mBatches.clear();
	mVertexOffset = mVertexCount = 0;
	mIndexOffset = mIndexCount = 0;
}

void
RenderTarget::saveBatch()
{
	mBatches.emplace_back(
		mVertexOffset,
		mIndexOffset * sizeof(mIndices[0]),
		mIndexCount - mIndexOffset);
	mVertexOffset = mVertexCount;
	mIndexOffset = mIndexCount;
}

void
RenderTarget::drawBuffers() const
{
	// upload the indices
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	                     mIndices.size() * sizeof(mIndices[0]),
	                     mIndices.data(),
	                     GL_STREAM_DRAW));

	// draw all the batches
	for (const auto &batch : mBatches)
	{
		glCheck(glDrawElementsBaseVertex(
			        GL_TRIANGLES,
			        batch.indexCount,
			        GL_UNSIGNED_SHORT,
			        reinterpret_cast<GLvoid*>(batch.indexOffset),
			        batch.vertexOffset));
	}
}

void
RenderTarget::draw(const std::string &text, glm::vec2 pos, Font &font, Color color)
{
	if (text.empty())
	{
		return;
	}

	// NOTE: this ensures the glyphs are rendered in the texture before drawing
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cv;
	for (auto codepoint : cv.from_bytes(text))
	{
		font.getGlyph(codepoint);
	}

	mUniformColorShader.use();
	mUniformColorShader.getUniform("uniformColor").setVector4f(color);
	font.getTexture().bind(0);

	mPosUV.clear();
	startBatch();
	pos.y += font.getLineHeight();
	for (auto codepoint : cv.from_bytes(text))
	{
		const auto &g = font.getGlyph(codepoint);
		pos.x += g.bearing.x;
		pos.y -= g.bearing.y;
		reserve(4, indices);
		for (auto unit : units)
		{
			PosUV v;
			v.pos = g.size * unit + pos;
			v.uv = g.uvSize * unit + g.uvPos;
			mPosUV.push_back(v);
		}
		pos.x += g.advance - g.bearing.x;
		pos.y += g.bearing.y;
	}
	saveBatch();

	glCheck(glBindVertexArray(mPosUVVAO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER,
	                     mPosUV.size() * sizeof(mPosUV[0]),
	                     mPosUV.data(),
	                     GL_STREAM_DRAW));

	drawBuffers();
}

void
RenderTarget::draw(const RectangleShape &rect)
{
	mUniformColorShader.use();
	mUniformColorShader.getUniform("uniformColor").setVector4f(rect.getColor());
	mWhiteTexture.bind(0);
	mPosUV.clear();
	startBatch();
	reserve(4, indices);

	auto transform = rect.getTransform();
	auto size = rect.getSize();
	for (auto unit : units)
	{
		PosUV v;
		glm::vec4 pos = glm::vec4(unit * size, 0.f, 1.f);
		v.pos = transform * pos;
		v.uv = unit;
		mPosUV.push_back(v);
	}
	saveBatch();

	glCheck(glBindVertexArray(mPosUVVAO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER,
	                     mPosUV.size() * sizeof(mPosUV[0]),
	                     mPosUV.data(),
	                     GL_STREAM_DRAW));

	drawBuffers();
}
