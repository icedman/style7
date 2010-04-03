#pragma once

#include "style.h"

class Preview
{
public:

	Preview(Style *style, bool aero);

	QImage createDesktopPreview(QSize size);
	QImage createFramePreview(QSize size, Frame *frame);

	Style *style;
	bool aero;
};
