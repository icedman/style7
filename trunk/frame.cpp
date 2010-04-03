#include "frame.h"

Frame::Frame()
: background(QColor("#c0c0c0")),
foreground(QColor("#000000")),
fillType(FILL_NONE),
//gradientColors[100],
gradientX(50),
gradientY(0),
gradientX2(50),
gradientY2(100),
marginLeft(0),
marginRight(0),
marginTop(0),
marginBottom(0),
borderLeft(0),
borderRight(0),
borderTop(0),
borderBottom(0),
borderColor(QColor("#000000")),
bevelLeft(0),
bevelRight(0),
bevelTop(0),
bevelBottom(0),
bevelType(BEVEL_NONE),
bevelLighterColor(QColor(255,255,255,50)),
bevelDarkerColor(QColor(50,50,50,150)),
cornerRadius(0),
cornerLeftTop(0),
cornerRightTop(0),
cornerLeftBottom(0),
cornerRightBottom(0),
cornerType(CORNER_NONE)
{}

QImage Frame::draw(QSize size)
{
	QImage img(size, QImage::Format_ARGB32);
	QPainter p(&img);

	p.setRenderHint(QPainter::Antialiasing, true);
	p.setRenderHint(QPainter::SmoothPixmapTransform, true);

	// clear
	p.save();
	p.setCompositionMode(QPainter::CompositionMode_Clear);
	p.fillRect(img.rect(), QColor("black"));
	p.restore();

	// frame path
	QRect frameRect = img.rect();
	// apply margin
	frameRect.adjust(marginLeft, marginTop, -marginRight, -marginBottom);
	QPainterPath framePath = createPath(frameRect);

	// border path
	QRect borderRect = frameRect;
	borderRect.adjust(borderLeft, borderTop, -borderRight, -borderBottom);
	QPainterPath borderPath = framePath.subtracted(createPath(borderRect));

	// bevel path
	QRect bevelRect = borderRect;
	bevelRect.adjust(bevelLeft, bevelTop, -bevelRight, -bevelBottom);
	QPainterPath bevelPath = createPath(borderRect).subtracted(createPath(bevelRect));
	QPainterPath tempPath;
	tempPath.moveTo(frameRect.left()+1, frameRect.bottom());
	tempPath.lineTo(frameRect.right(), frameRect.top()+1);
	tempPath.lineTo(frameRect.bottomRight());
	QPainterPath bevelLighterPath = bevelPath.subtracted(tempPath);
	QPainterPath bevelDarkerPath = bevelPath.intersected(tempPath);

	// draw background
	p.save();
	p.setClipPath(framePath);
	switch(fillType)
	{
	case FILL_SOLID:
		p.fillRect(frameRect, background);
		break;
	case FILL_GRADIENT:
		{
			QLinearGradient grad;
			setGradientColors(grad);
			grad.setStart(size.width()*gradientX/100,size.height()*gradientY/100);
			grad.setFinalStop(size.width()*gradientX2/100,size.height()*gradientY2/100);
			p.fillRect(frameRect, grad);
		}
		break;
	case FILL_RADIAL_GRADIENT:
		{
			QRadialGradient grad;
			setGradientColors(grad);
			grad.setCenter(size.width()*gradientX/100,size.height()*gradientY/100);
			grad.setFocalPoint(size.width()*gradientX2/100,size.height()*gradientY2/100);
			grad.setRadius((frameRect.width() + frameRect.height()) / 2);
			p.fillRect(frameRect, grad);
		}
		break;
	case FILL_CONICAL_GRADIENT:
		{
			QConicalGradient grad;
			double angle = atan2((double)gradientY - gradientY2, gradientX2 - gradientX) * 180 / 3.14;
			setGradientColors(grad);
			grad.setAngle(angle);
			grad.setCenter(size.width()*gradientX/100,size.height()*gradientY/100);
			p.fillRect(frameRect, grad);
		}
		break;
	}
	p.restore();

	// draw borders
	p.save();
	p.setClipPath(borderPath);
	p.fillRect(frameRect, borderColor);
	p.restore();

	// draw bevel
	QColor lighter = bevelLighterColor;
	QColor darker = bevelDarkerColor;
	switch(bevelType)
	{
	case BEVEL_SUNKEN:
		{
			QColor t = lighter;
			lighter = darker;
			darker = t;
		}
		break;
	case BEVEL_FLAT:
		{
			darker = lighter;
		}
		break;
	case BEVEL_NONE:
		{
			lighter = QColor(0,0,0,0);
			darker = QColor(0,0,0,0);
		}
		break;
	}

	p.save();
	p.setClipPath(bevelLighterPath);
	p.fillRect(borderRect, lighter);
	p.restore();
	p.save();
	p.setClipPath(bevelDarkerPath);
	p.fillRect(borderRect, darker);
	p.restore();
	return img;
}

QPainterPath Frame::createPath(QRect rect)
{
	int x = rect.x();
	int y = rect.y();
	int w = rect.width();
	int h = rect.height();

	int lt = 0;
	int rt = 0;
	int lb = 0;
	int rb = 0;
	int radius = 0;

	QPainterPath p;

	if (cornerType != CORNER_NONE)
	{
		lt = cornerLeftTop;
		rt = cornerRightTop;
		lb = cornerLeftBottom;
		rb = cornerRightBottom;
	}

	if (cornerType == CORNER_ROUNDED)
	{
		int b = w >> 1;
		p.addRoundedRect(rect, cornerRadius, cornerRadius);
		if (lt == 0)
		{
			QPainterPath pcorner;
			pcorner.moveTo(x,y);
			pcorner.lineTo(x+b,y);
			pcorner.lineTo(x,y+b);
			p = p.united(pcorner);
		}
		if (rt == 0)
		{
			QPainterPath pcorner;
			pcorner.moveTo(x+w-b,y);
			pcorner.lineTo(x+w,y);
			pcorner.lineTo(x+w,y+b);
			p = p.united(pcorner);
		}
		if (lb == 0)
		{
			QPainterPath pcorner;
			pcorner.moveTo(x,y+h);
			pcorner.lineTo(x+b,y+h);
			pcorner.lineTo(x,y+h-b);
			p = p.united(pcorner);
		}
		if (rb == 0)
		{
			QPainterPath pcorner;
			pcorner.moveTo(x+w-b,y+h);
			pcorner.lineTo(x+w,y+h);
			pcorner.lineTo(x+w,y+h-b);
			p = p.united(pcorner);
		}
	} else {
		p.moveTo(x + lt, y);
		p.lineTo(x + w - rt, y);
		p.lineTo(x + w, y + rt);
		p.lineTo(x + w, y + h - rb);
		p.lineTo(x + w - rb, y + h);
		p.lineTo(x + lb, y + h);
		p.lineTo(x, y + h - lb);
		p.lineTo(x, y + lt);
	}

	return p.simplified();
}

void Frame::setGradientColors(QGradient& grad)
{
	for(int i = 0; i<100; i++)
	{
		if (gradientColors[i].isValid())
			grad.setColorAt((qreal)i/100, gradientColors[i]);
	}
}

void Frame::setBorderWidth(int w)
{
	borderTop = borderBottom = borderLeft = borderRight = w;
}

void Frame::setMarginWidth(int w)
{
	marginTop = marginBottom = marginLeft = marginRight = w;
}

void Frame::setBevelWidth(int w)
{
	bevelTop = bevelBottom = bevelLeft = bevelRight = w;
}