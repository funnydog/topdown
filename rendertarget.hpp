#pragma once

#include <span>
#include <unordered_map>
#include <vector>

#include "color.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "view.hpp"

class Canvas;
class Window;
class Font;

class RenderTarget
{
public:
	explicit RenderTarget(const Window &window);
	~RenderTarget();

	/**
	 * Get the View associated with the RenderTarget.
	 */
	const View& getView() const;

	/**
	 * Set the View associated with the RenderTarget.
	 * @param[in] view
	 */
	void setView(const View &view);

	/**
	 * Get the default View associated with the RenderTarget.
	 */
	const View& getDefaultView() const;

	/**
	 * Clear the target with the given @color.
	 * @param[in] color
	 */
	void clear(Color = Color::Black);

	/**
	 * Force a new draw command
	 */
	void addLayer();

	/**
	 * Send the blob of vertices to the GPU.
	 */
	void draw();

	/**
	 * Set the texture for the next primitive.
	 */
	void setTexture(const Texture *texture);

	/**
	 * Get the base index for the primitive and reserve space in
	 * the vertex and index buffers.
	 */
	std::uint16_t getPrimIndex(unsigned idxCount, unsigned vtxCount);

	template <typename Iterator> void addIndices(unsigned base, Iterator start, Iterator end);

	template <typename Iterator> void addVertices(Iterator start, Iterator end);

	Vertex* getVertexArray(unsigned vtxCount);

	void setCanvas(const Canvas &canvas);

	void draw(const std::string &text, glm::vec2 pos, Font &font, Color color);

protected:
	void initialize();

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
	struct DrawChannel
	{
		const Texture *texture;
		unsigned vtxOffset;
		unsigned idxOffset;
		std::vector<std::uint16_t> idxBuffer;
		DrawChannel *next;
	};

	DrawChannel *newChannel(const Texture *texture, unsigned vtxOffset);
	void beginBatch();
	void endBatch();

private:
	View mDefaultView;
	View mView;

	std::vector<std::uint16_t> mIndices;
	std::vector<PosUV>         mPosUV;
	std::vector<PosUVColor>    mPosUVColor;
	std::vector<Batch>         mBatches;

	unsigned mVertexOffset;
	unsigned mVertexCount;
	unsigned mIndexOffset;
	unsigned mIndexCount;

	std::vector<Vertex>        mVertices;
	std::unordered_map<const Texture *, DrawChannel*> mChannelMap;

	bool mIsBatching;
	DrawChannel  *mChannelList;
	DrawChannel **mChannelTail;
	DrawChannel  *mCurrent;
	DrawChannel  *mFreeChannels;

	const Canvas *mCanvas;

	Texture  mWhiteTexture;
	Shader   mTextureShader;
	Shader   mUniformColorShader;

	unsigned mTextureVAO;
	unsigned mPosUVVAO;
	unsigned mPosUVColorVAO;
	unsigned mVBO;
	unsigned mEBO;
};

template <typename Iterator>
void
RenderTarget::addIndices(unsigned base, Iterator start, Iterator end)
{
	for (; start != end; ++start)
	{
		mCurrent->idxBuffer.push_back(base + *start);
	}
}

template <typename Iterator>
void
RenderTarget::addVertices(Iterator start, Iterator end)
{
	mVertices.insert(mVertices.end(), start, end);
}
