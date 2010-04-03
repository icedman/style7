#pragma once

#include <QtGui>

class Frame
{
public:

	enum {
		FILL_NONE = 0,
		FILL_SOLID,
		FILL_GRADIENT,
		FILL_RADIAL_GRADIENT,
		FILL_CONICAL_GRADIENT,
	};

	enum {
		BEVEL_NONE = 0,
		BEVEL_RAISED,
		BEVEL_SUNKEN,
		BEVEL_FLAT,
	};

	enum {
		CORNER_NONE = 0,
		CORNER_ROUNDED,
		CORNER_SHARP
	};

	QColor background;
	QColor foreground;
	int fillType;

	QColor gradientColors[100];
	int gradientX;
	int gradientY;
	int gradientX2;
	int gradientY2;

	int marginLeft;
	int marginRight;
	int marginTop;
	int marginBottom;

	int borderLeft;
	int borderRight;
	int borderTop;
	int borderBottom;
	QColor borderColor;

	int bevelLeft;
	int bevelRight;
	int bevelTop;
	int bevelBottom;
	int bevelType;
	QColor bevelLighterColor;
	QColor bevelDarkerColor;

	int cornerRadius;
	int cornerLeftTop;
	int cornerRightTop;
	int cornerLeftBottom;
	int cornerRightBottom;
	int cornerType;

	Frame();
	QImage draw(QSize size);
	QPainterPath createPath(QRect rect);

	void setBorderWidth(int w);
	void setMarginWidth(int w);
	void setBevelWidth(int w);

private:

	void setGradientColors(QGradient& grad);
};