#include <codecvt>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <string>

#include "rendertarget.hpp"
#include "font.hpp"

namespace
{
const int TEXTURE_WIDTH = 1024;
const int TEXTURE_HEIGHT = 1024;
const int PADDING = 2;

static inline unsigned
roundUp2(unsigned v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return v + 1;
}
}

Font::Font()
	: mFT(nullptr)
	, mFace(nullptr)
	, mLineHeight(0)
	, mPositionX(0)
	, mPositionY(0)
	, mMaxHeight(0)
{
	mTexture.setSmooth(true);
}

Font::~Font()
{
	FT_Done_Face(mFace);
	FT_Done_FreeType(mFT);
}

bool
Font::loadFromFile(const std::filesystem::path &path, unsigned size)
{
	FT_Done_FreeType(mFT);
	if (FT_Init_FreeType(&mFT))
	{
		std::cerr << "Font::loadFromFile() - Cannot initialize the freetype2 library"
			  << std::endl;
		return false;
	}

	FT_Done_Face(mFace);
	if (FT_New_Face(mFT, path.c_str(), 0, &mFace))
	{
		std::cerr << "Font::loadFromFile() - Failed to load the font "
			  << path << std::endl;
		return false;
	}
	FT_Set_Pixel_Sizes(mFace, 0, size);

	mLineHeight = static_cast<int>((mFace->size->metrics.ascender -
					mFace->size->metrics.descender) >> 6);
	mGlyphs.clear();
	mPositionX = mPositionY = mMaxHeight = 0;

	return true;
}

void
Font::draw(RenderTarget &target, const glm::mat4 &transform,
	   const std::string &text, Color color) const
{
	if (text.empty())
	{
		return;
	}

	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cv;
	float x = 0.f;
	target.setTexture(&mTexture);
	for (auto codepoint: cv.from_bytes(text))
	{
		getGlyph(codepoint);
	}

	for (auto codepoint: cv.from_bytes(text))
	{
		const auto &glyph = getGlyph(codepoint);
		x += glyph.bearing.x;
		float y = mLineHeight - glyph.bearing.y;
		auto p0 = glm::vec2(transform * glm::vec4(x, y, 0, 1));
		auto p1 = glm::vec2(transform * glm::vec4(x, y + glyph.size.y, 0, 1));
		auto p2 = glm::vec2(transform * glm::vec4(x + glyph.size.x, y, 0, 1));
		auto p3 = glm::vec2(transform * glm::vec4(x + glyph.size.x, y + glyph.size.y, 0, 1));
		Vertex vertices[] = {
			{ p0, glyph.topLeft, color },
			{ p1, {glyph.topLeft.x, glyph.bottomRight.y}, color },
			{ p2, {glyph.bottomRight.x, glyph.topLeft.y}, color },
			{ p3, glyph.bottomRight, color },
		};
		const std::uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };

		unsigned base = target.getPrimIndex(6, 4);
		target.addIndices(base, indices + 0, indices + 6);
		target.addVertices(vertices + 0, vertices + 4);

		x += glyph.advance - glyph.bearing.x;
	}
}

glm::vec2
Font::getSize(const std::string &text) const
{
	int width = 0;
	int height = 0;
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cv;
	for (auto codepoint: cv.from_bytes(text))
	{
		const auto &glyph = getGlyph(codepoint);
		if (height < glyph.size.y + glyph.bearing.y)
		{
			height = glyph.size.y + glyph.bearing.y;
		}
		width += glyph.advance;
	}
	return { width, height };
}

const Texture&
Font::getTexture() const
{
	return mTexture;
}

void
Font::resizeTexture(unsigned newWidth, unsigned newHeight) const
{
	auto oldWidth = mTexture.getWidth();
	auto oldHeight = mTexture.getHeight();

	Texture newTexture;
	newTexture.setSmooth(mTexture.isSmooth());
	newTexture.create(newWidth, newHeight);
	newTexture.update(mTexture);
	std::swap(mTexture, newTexture);

	glm::vec2 scale{
		static_cast<float>(oldWidth) / newWidth,
		static_cast<float>(oldHeight) / newHeight
	};
	for (auto &[codepoint, glyph]: mGlyphs)
	{
		glyph.topLeft *= scale;
		glyph.bottomRight *= scale;
	}
}

const Font::Glyph&
Font::getGlyph(char32_t codepoint) const
{
	if (const auto it = mGlyphs.find(codepoint); it != mGlyphs.end())
	{
		return it->second;
	}

	if (FT_Load_Char(mFace, codepoint, FT_LOAD_RENDER))
	{
		throw std::runtime_error(
			"Font::getGlyph() - cannot load the glyph for codepoint "
			+ std::to_string(codepoint));
	}

	int bmWidth = mFace->glyph->bitmap.width + 2 * PADDING;
	int bmHeight = mFace->glyph->bitmap.rows + 2 * PADDING;
	if (mMaxHeight < bmHeight)
	{
		mMaxHeight = bmHeight;
	}

	auto texWidth = mTexture.getWidth();
	auto texHeight = mTexture.getHeight();
	bool resize = false;
	if (unsigned right = mPositionX + bmWidth; right > texWidth)
	{
		unsigned newTexWidth = texWidth
			? texWidth * 2
			: roundUp2(right);
		if (newTexWidth <= TEXTURE_WIDTH)
		{
			texWidth = newTexWidth;
			resize = true;
		}
		else
		{
			mPositionY += mMaxHeight;
			mPositionX = 0;
			mMaxHeight = 0;
		}
	}
	if (unsigned bottom = mPositionY + bmHeight; bottom > texHeight)
	{
		unsigned newTexHeight = texHeight
			? texHeight * 2
			: roundUp2(bottom);
		if (newTexHeight <= TEXTURE_HEIGHT)
		{
			texHeight = newTexHeight;
			resize = true;
		}
		else
		{
			throw std::runtime_error("Font::getGlyph() - "
						 "no space left in the texture");
		}
	}
	if (resize)
	{
		resizeTexture(texWidth, texHeight);
	}

	// clear the pixel buffer
	mPixelBuffer.resize(bmWidth * bmHeight * 4);
	std::uint8_t *pix = mPixelBuffer.data();
	for (std::size_t i = 0; i < mPixelBuffer.size() / 4; ++i)
	{
		*pix++ = 255;
		*pix++ = 255;
		*pix++ = 255;
		*pix++ = 0;
	}

	// render the pixel
	pix = mFace->glyph->bitmap.buffer;
	for (int y = PADDING; y < bmHeight - PADDING; ++y)
	{
		for (int x = PADDING; x < bmWidth - PADDING; ++x)
		{
			const std::size_t index = x + y * bmWidth;
			mPixelBuffer[index * 4 + 3] = pix[x - PADDING];
		}
		pix += mFace->glyph->bitmap.pitch;
	}

	// upload the data
	mTexture.update(mPixelBuffer.data(), mPositionX, mPositionY, bmWidth, bmHeight);

	bmWidth -= 2 * PADDING;
	bmHeight -= 2 * PADDING;

	Glyph glyph;
	glyph.topLeft.x = static_cast<float>(mPositionX + PADDING) / texWidth;
	glyph.topLeft.y = static_cast<float>(mPositionY + PADDING) / texHeight;
	glyph.bottomRight.x = glyph.topLeft.x + static_cast<float>(bmWidth) / texWidth;
	glyph.bottomRight.y = glyph.topLeft.y + static_cast<float>(bmHeight) / texHeight;

	glyph.size = glm::ivec2(bmWidth, bmHeight);
	glyph.bearing = glm::ivec2(mFace->glyph->bitmap_left,
				   mFace->glyph->bitmap_top);
	glyph.advance = mFace->glyph->advance.x >> 6;

	const auto [it, success] = mGlyphs.insert(std::make_pair(codepoint, std::move(glyph)));
	if (!success)
	{
		throw std::runtime_error("Font::getGlyph() - "
					 "can't add the glyph to the map");
	}

	mPositionX += bmWidth + 2 * PADDING;

	return it->second;
}
