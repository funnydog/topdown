#pragma once

enum class StateID
{
	None,
	Title,
	Menu,
	Settings,
	GamePlay,
	GameOver,
	Pause,
};

enum class TextureID
{
	TitleScreen,
	Explosion,
	// world textures
	Entities,
	Desert,
};

enum class FontID
{
	Title,
	Body,
};

template <typename Resource, typename Identifier>
class ResourceHolder;

class Texture;
typedef ResourceHolder<Texture, TextureID> TextureHolder;

class Font;
typedef ResourceHolder<Font, FontID> FontHolder;
