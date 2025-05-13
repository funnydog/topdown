#pragma once

#include <glm/glm.hpp>

#include <filesystem>

class Texture
{
public:
	Texture();

	bool loadFromFile(const std::filesystem::path &path);

	bool create(unsigned width, unsigned height,
	            const void *pixels=nullptr,
	            bool repeat=false, bool smooth=false);

	void update(const void *pixels);
	void update(const void *pixels, unsigned x, unsigned y, unsigned w, unsigned h);
	void update(const Texture &other, unsigned x = 0, unsigned y = 0);

	void destroy() noexcept;
	void bind() const noexcept;
	void bind(int textureUnit) const noexcept;

	glm::vec2 getSize() const;

	unsigned getWidth() const;
	unsigned getHeight() const;

	bool isRepeated() const;
	void setRepeated(bool repeated);

	bool isSmooth() const;
	void setSmooth(bool smooth);

private:
	unsigned mTexture;
};
