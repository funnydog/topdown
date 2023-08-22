#pragma once

class Texture
{
	Texture();
	~Texture();

	Texture(const Texture &other) = delete;
	Texture& operator=(const Texture &other) = delete;

	Texture(Texture &&other) noexcept = default;
	Texture& operator=(Texture &&ohter) noexcept = default;

	bool create(unsigned width, unsigned height,
		    const void *pixels=nullptr, bool repeat=false, bool smooth=false);
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
