#pragma once

#include <string>

#include "color.hpp"
#include "font.hpp"
#include "transformable.hpp"

class Text: public Transformable
{
public:
	explicit Text(Font &font, const std::string &string = "", Color color = Color::White);

	const Font &getFont() const;
	void setFont(Font &font);

	const std::string& getString() const;
	void setString(const std::string &string);

	Color getColor() const;
	void setColor(Color color);

	void centerOrigin();

	glm::vec2 getSize() const;

	void draw(RenderTarget &target, const glm::mat4 &transform) const;

private:
	Font        *mFont;
	std::string  mString;
	Color        mColor;
};
