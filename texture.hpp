#pragma once

class Texture
{
	Texture();
	~Texture();

	Texture(const Texture &other) = delete;
	Texture& operator=(const Texture &other) = delete;
	Texture(Texture &&other) noexcept = delete;
	Texture& operator=(Texture &&ohter) noexcept = delete;

	bool create(unsigned width, unsigned height, bool repeat=false, bool smooth=false);
	void update(const void *pixels);
	void update(const void *pixels, unsigned x, unsigned y, unsigned w, unsigned h);

	unsigned getWidth() const;
	unsigned getHeight() const;

	bool isRepeated() const;
	void setRepeated(bool repeated);

	bool isSmooth() const;
	void setSmooth(bool smooth);

	static void bind(const Texture *texture, unsigned sampler) noexcept;

private:
	unsigned mTexture;
};
