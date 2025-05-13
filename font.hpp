#pragma once

#include <filesystem>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "color.hpp"
#include "texture.hpp"

class RenderTarget;

struct Glyph
{
	glm::vec2 uvPos;
	glm::vec2 uvSize;
	glm::vec2 size;
	glm::vec2 bearing;
	float advance;
};

class Font
{
public:
	Font();
	~Font();

	Font(const Font &) = delete;
	Font& operator=(const Font &) = delete;
	Font(Font &&) noexcept = delete;
	Font& operator=(Font &&) noexcept = delete;

	bool loadFromFile(const std::filesystem::path &path, unsigned size);
	void destroy();

	glm::vec2 getSize(const std::string &text);

	const Glyph &getGlyph(char32_t codepoint);
	const Texture &getTexture() const;
	float getLineHeight() const;

	void draw(RenderTarget &target, const glm::mat4 &transform,
		  const std::string &text, Color color);
private:
	void resizeTexture(unsigned newWidth, unsigned newHeight);

private:
	std::unordered_map<char32_t, Glyph> mGlyphs;
	std::vector<std::uint8_t> mPixelBuffer;
	Texture mTexture;
	FT_Library mFT;
	FT_Face mFace;
	float mLineHeight;
	int mPositionX;
	int mPositionY;
	int mMaxHeight;
};
