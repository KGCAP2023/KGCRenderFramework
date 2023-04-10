#pragma once
#include "pch.h"
typedef unsigned char BYTE;

class RGBColor
{
public:
	RGBColor();
	RGBColor(unsigned int val);
	RGBColor(BYTE r, BYTE g, BYTE b);
	RGBColor(BYTE r, BYTE g, BYTE b, BYTE a);
	RGBColor(const RGBColor& src);

	RGBColor& operator=(const RGBColor& src);
	bool operator==(const RGBColor& rhs) const;
	bool operator!=(const RGBColor& rhs) const;

	constexpr BYTE GetR() const;
	void SetR(BYTE r);

	constexpr BYTE GetG() const;
	void SetG(BYTE g);

	constexpr BYTE GetB() const;
	void SetB(BYTE b);

	constexpr BYTE GetA() const;
	void SetA(BYTE a);

private:
	union
	{
		BYTE rgba[4];
		unsigned int color;
	};
};