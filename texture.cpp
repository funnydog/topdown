#include <cassert>
#include <iostream>

#include <GL/glew.h>

#include "glcheck.hpp"
#include "texture.hpp"
#include "stb_image.h"

Texture::Texture()
	: mTexture(0)
{
}

Texture::~Texture()
{
	if (mTexture)
	{
		glCheck(glDeleteTextures(1, &mTexture));
	}
}

bool
Texture::create(unsigned width, unsigned height, const void *pixels, bool repeat, bool smooth)
{
	if (width == 0 || height == 0)
	{
		std::cerr << "Failed to create the texture, invalid size ("
			  << width << ", " << height << ")" << std::endl;
		return false;
	}

	// TODO: check for NPOT support and max size support
	if (!mTexture)
	{
		glCheck(glGenTextures(1, &mTexture));
		if (!mTexture)
		{
			return false;
		}
	}
	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			static_cast<GLsizei>(width),
			static_cast<GLsizei>(height),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixels));

	GLint parameter = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter));

	parameter = smooth ? GL_LINEAR : GL_NEAREST;
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter));

	return true;
}

void
Texture::update(const void *pixels)
{
	update(pixels, 0, 0, getWidth(), getHeight());
}

void
Texture::update(const void *pixels, unsigned x, unsigned y, unsigned w, unsigned h)
{
	assert(x + w <= getWidth() && "Destination x coordinate is outside of the texture");
	assert(y + h <= getHeight() && "Destination y coordinate is outside of the texture");

	if (pixels == nullptr || mTexture == 0)
	{
		return;
	}

	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			static_cast<GLint>(x),
			static_cast<GLint>(y),
			static_cast<GLsizei>(w),
			static_cast<GLsizei>(h),
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixels));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	glCheck(glFlush());
}

bool
Texture::loadFromFile(const std::filesystem::path &path)
{
	int width, height, channels;
	auto *pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);
	if (!pixels)
	{
		std::cerr << "Texture::loadFromFile - Cannot load " << path.string()
			  << std::endl;
		return false;
	}
	bool result = create(width, height, pixels);
	stbi_image_free(pixels);
	return result;
}

unsigned
Texture::getWidth() const
{
	GLint width;
	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	return width;
}

unsigned
Texture::getHeight() const
{
	GLint height;
	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	return height;
}

bool
Texture::isRepeated() const
{
	GLint glWrapping;
	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &glWrapping));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	return glWrapping == GL_REPEAT;
}

void
Texture::setRepeated(bool repeated)
{
	GLint glWrapping = repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapping));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapping));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}

bool
Texture::isSmooth() const
{
	GLint glFiltering;
	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &glFiltering));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	return glFiltering == GL_LINEAR;
}

void
Texture::setSmooth(bool smooth)
{
	GLint glFiltering = smooth ? GL_LINEAR : GL_NEAREST;
	glCheck(glBindTexture(GL_TEXTURE_2D, mTexture));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFiltering));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFiltering));
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}

void
Texture::bind(const Texture *texture, unsigned sampler) noexcept
{
	assert(sampler < 16 && "Sampler value out of bounds");
	if (texture)
	{
		glActiveTexture(GL_TEXTURE0 + sampler);
		glBindTexture(GL_TEXTURE_2D, texture->mTexture);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + sampler);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
