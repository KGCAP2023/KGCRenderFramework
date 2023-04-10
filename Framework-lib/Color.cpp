#include "pch.h"
#include "Color.h"

RGBColor::RGBColor()
	:color(0)
{}

RGBColor::RGBColor(unsigned int val)
	: color(val)
{}

RGBColor::RGBColor(BYTE r, BYTE g, BYTE b)
	: RGBColor(r, g, b, 255)
{
}

RGBColor::RGBColor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

RGBColor::RGBColor(const RGBColor& src)
	:RGBColor(src.color)
{}

RGBColor& RGBColor::operator=(const RGBColor& src)
{
	this->color = src.color;
	return *this;
}

bool RGBColor::operator==(const RGBColor& rhs) const
{
	return (this->color == rhs.color);
}

bool RGBColor::operator!=(const RGBColor& rhs) const
{
	return !(*this == rhs);
}

constexpr BYTE RGBColor::GetR() const
{
	return this->rgba[0];
}
void RGBColor::SetR(BYTE r)
{
	this->rgba[0] = r;
}

constexpr BYTE RGBColor::GetG() const
{
	return this->rgba[1];
}
void RGBColor::SetG(BYTE g)
{
	this->rgba[1] = g;
}

constexpr BYTE RGBColor::GetB() const
{
	return this->rgba[2];
}
void RGBColor::SetB(BYTE b)
{
	this->rgba[2] = b;
}

constexpr BYTE RGBColor::GetA() const
{
	return this->rgba[3];
}
void RGBColor::SetA(BYTE a)
{
	this->rgba[3] = a;
}