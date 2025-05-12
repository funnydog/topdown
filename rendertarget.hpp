#pragma once

#include <unordered_map>
#include <vector>

#include "color.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "view.hpp"

class Canvas;
class Window;

class RenderTarget
{
public:
	RenderTarget(const Window &window);
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

protected:
	void initialize();

private:
	struct DrawChannel
	{
		const Texture *texture;
		unsigned vtxOffset;
		unsigned idxOffset;
		std::vector<std::uint16_t> idxBuffer;
		DrawChannel *next;
	};

private:
	DrawChannel *newChannel(const Texture *texture, unsigned vtxOffset);
	void beginBatch();
	void endBatch();

private:
	View mDefaultView;
	View mView;

	std::vector<Vertex>        mVertices;
	std::vector<std::uint16_t> mIndices;
	std::unordered_map<const Texture *, DrawChannel*> mChannelMap;

	bool mIsBatching;
	DrawChannel  *mChannelList;
	DrawChannel **mChannelTail;
	DrawChannel  *mCurrent;
	DrawChannel  *mFreeChannels;

	const Canvas *mCanvas;

	Texture  mWhiteTexture;
	Shader   mTextureShader;
	unsigned mTextureVAO;
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
