#pragma once

#include <span>
#include <unordered_map>
#include <vector>

#include "color.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Window;
class Font;
class RectangleShape;
struct Frame;

class RenderTarget
{
public:
	bool create(const Window &window);
	void destroy();
	void setViewport(unsigned width, unsigned height);

	/**
	 * Clear the target with the given @color.
	 * @param[in] color
	 */
	void clear(Color = Color::Black);

	void draw(const std::string &text, glm::vec2 pos, Font &font, Color color);
	void draw(const RectangleShape &rect);
	void draw(const Texture &texture, glm::vec2 pos);

	void beginFrames(const Texture &texture);
	void addFrame(const Frame &frame, glm::vec2 pos);
	void endFrames();

private:
	struct PosUV
	{
		glm::vec2 pos;
		glm::vec2 uv;
	};

	struct PosUVColor
	{
		glm::vec2 pos;
		glm::vec2 uv;
		uint32_t color;
	};

	struct Batch
	{
		unsigned vertexOffset;
		unsigned indexOffset;
		unsigned indexCount;
	};

	void reserve(unsigned vertexCount, std::span<const std::uint16_t> indices);
	void startBatch();
	void saveBatch();
	void drawBuffers() const;

private:
	std::vector<std::uint16_t> mIndices;
	std::vector<PosUV>         mPosUV;
	std::vector<PosUVColor>    mPosUVColor;
	std::vector<Batch>         mBatches;

	unsigned mVertexOffset = 0;
	unsigned mVertexCount = 0;
	unsigned mIndexOffset = 0;
	unsigned mIndexCount = 0;

	Texture  mWhiteTexture;
	Shader   mTextureShader;
	Shader   mUniformColorShader;

	unsigned mPosUVVAO = 0;
	unsigned mPosUVColorVAO = 0;
	unsigned mVBO = 0;
	unsigned mEBO = 0;
};
