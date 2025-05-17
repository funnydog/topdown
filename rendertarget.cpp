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
#include "world.hpp"

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

bool
RenderTarget::create(const Window &window)
{
	mWhiteTexture.create(1, 1, &Color::White);

	// shader creation and configuration
	mTextureShader.create();
	mTextureShader.attachFile(ShaderType::Vertex, "assets/shaders/simple.vs");
	mTextureShader.attachFile(ShaderType::Fragment, "assets/shaders/texture.fs");
	mTextureShader.link();

	mUniformColorShader.create();
	mUniformColorShader.attachFile(ShaderType::Vertex, "assets/shaders/simple.vs");
	mUniformColorShader.attachFile(ShaderType::Fragment, "assets/shaders/uniformcolor.fs");
	mUniformColorShader.link();

	// bind a buffer to allow calling glVertexAttribPointer()
	glCheck(glGenBuffers(1, &mVBO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glGenBuffers(1, &mEBO));

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

	auto size = window.getSize();
	setViewport(size.x, size.y);
	return true;
}

void
RenderTarget::destroy()
{
	glCheck(glBindVertexArray(0));
	glCheck(glDeleteVertexArrays(1, &mPosUVColorVAO));
	glCheck(glDeleteVertexArrays(1, &mPosUVVAO));
	glCheck(glDeleteBuffers(1, &mEBO));
	glCheck(glDeleteBuffers(1, &mVBO));
}

void
RenderTarget::setViewport(unsigned width, unsigned height)
{
	glm::mat4 proj = glm::ortho(
		0.0f, static_cast<GLfloat>(width),
		static_cast<GLfloat>(height), 0.0f,
		-1.0f, 1.0f);
	mTextureShader.use();
	mTextureShader.getUniform("projection").setMatrix4(proj);
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

void
RenderTarget::draw(const Texture &texture, glm::vec2 pos)
{
	auto size = texture.getSize();

	mPosUV.clear();
	startBatch();
	reserve(4, indices);
	for (auto unit : units)
	{
		PosUV v;
		v.pos = unit * size + pos;
		v.uv = unit;
		mPosUV.push_back(v);
	}
	saveBatch();

	mTextureShader.use();
	texture.bind(0);
	glCheck(glBindVertexArray(mPosUVVAO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER,
	                     mPosUV.size() * sizeof(mPosUV[0]),
	                     mPosUV.data(),
	                     GL_STREAM_DRAW));

	drawBuffers();
}

void
RenderTarget::beginFrames(const Texture &texture)
{
	mPosUV.clear();
	startBatch();
	mTextureShader.use();
	texture.bind(0);
}

void
RenderTarget::addFrame(const Frame &drw, glm::vec2 pos)
{
	reserve(4, indices);
	for (auto unit : units)
	{
		mPosUV.emplace_back(
			unit * drw.size + pos,
			unit * drw.uvSize + drw.uvPos
			);
	}
}

void
RenderTarget::endFrames()
{
	saveBatch();

	glCheck(glBindVertexArray(mPosUVVAO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER,
	                     mPosUV.size() * sizeof(mPosUV[0]),
	                     mPosUV.data(),
	                     GL_STREAM_DRAW));
	drawBuffers();
}
