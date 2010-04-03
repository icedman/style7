#include "preview.h"

Preview::Preview(Style *style, bool aero)
: style(style), aero(aero)
{}

QImage Preview::createDesktopPreview(QSize size)
{
	QImage img(size, QImage::Format_ARGB32);

	Frame tempFrame;
	Frame tempFrame2;
	Frame tempFrame3;
	QImage tempImage;

	int w = size.width();
	int h = size.height();
	qreal opacity = aero ? 0.8 : 1.0;

	QPainter p(&img);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setRenderHint(QPainter::SmoothPixmapTransform, true);

	// clear
	p.save();
	p.setCompositionMode(QPainter::CompositionMode_Clear);
	p.fillRect(img.rect(), QColor("black"));
	p.restore();

	// taskbar
	int tbh = 0.10 * 400;
	int tbw = 1.2 * tbh;
	int tdw = 0.4 * tbw;
	p.save();
	p.setOpacity(opacity);
	tempFrame = style->frames[Style::TASKBAR];
	tempImage = tempFrame.draw(QSize(w, tbh));
	p.drawImage(0,h - tbh, tempImage);
	p.restore();

	// taskbar button
	tempFrame2 = style->frames[Style::TASKBAR_BUTTON];
	tempFrame3 = style->frames[Style::TASKBAR_BUTTON_PRESSED];
	tempFrame2.borderTop = tempFrame.borderTop;
	tempFrame2.borderBottom = tempFrame.borderBottom;
	tempFrame3.borderTop = tempFrame.borderTop;
	tempFrame3.borderBottom = tempFrame.borderBottom;
	p.save();
	p.setOpacity(opacity);
	tempImage = tempFrame2.draw(QSize(tbw, tbh));
	p.drawImage(tbw,h - tbh, tempImage);
	tempImage = tempFrame3.draw(QSize(tbw, tbh));
	p.drawImage(tbw + tbw + 2,h - tbh, tempImage);
	p.restore();

	// taskbar desktop button
	tempFrame2.borderLeft = tempFrame.borderRight;
	tempFrame2.borderLeft = tempFrame.borderLeft;
	p.save();
	p.setOpacity(opacity);
	tempImage = tempFrame2.draw(QSize(tdw, tbh));
	p.drawImage(w-tdw,h-tbh, tempImage);
	p.restore();

	// taskbar clock
	tempFrame = style->frames[Style::TASKBAR_CLOCK];
	tempImage = tempFrame.draw(QSize(1.2 * tbw,tbh-8));
	p.drawImage(w-tempImage.width()-tdw-2,h-tbh+4,tempImage);

	// window
	int ww = 0.5 * 512;
	int wh = 0.5 * 400;
	int wt = 36;
	int wb = 10;
	int tr = 0.2 * ww;
	p.save();
	Frame contentFrame;
	contentFrame.background = QColor("#ffffff");
	contentFrame.fillType = Frame::FILL_SOLID;
	contentFrame.setMarginWidth(2);
	int wOffset = Style::UNFOCUS_WINDOW - Style::WINDOW;
	for(int i = 0; i<2; i++)
	{
		p.save();
		p.setOpacity(opacity);
		tempFrame = style->frames[-(wOffset * i) + Style::UNFOCUS_WINDOW];
		tempFrame2 = style->frames[-(wOffset * i) + Style::UNFOCUS_WINDOW_TITLE];
		tempFrame3 = style->frames[-(wOffset * i) + Style::UNFOCUS_WINDOW_TITLE_LABEL];
		tempFrame2.borderLeft = tempFrame.borderLeft;
		tempFrame2.borderRight = tempFrame.borderRight;
		tempFrame2.borderTop = tempFrame.borderTop;
		tempFrame2.borderBottom = tempFrame.borderBottom;
		tempImage = tempFrame2.draw(QSize(ww,wh));
		// window frame
		p.drawImage(w - ww - 10, 20, tempImage);
		// buttons
		Frame buttonFrame = style->frames[-(wOffset * i) + Style::UNFOCUS_WINDOW_BUTTON];
		tempImage = buttonFrame.draw(QSize(24,24));
		p.drawImage(w - ww - 10 + ww - 32, 20 + 8, tempImage);
		// label
		tempImage = tempFrame3.draw(QSize(ww - 16 - 32,wt - 8));
		p.drawImage(w - ww - 10 + 8, 20 + 8, tempImage);
		p.restore();
		// contents
		tempImage = contentFrame.draw(QSize(ww-(wb*2),wh-wt-(wb*2)));
		p.drawImage(w - ww - 10 + wb, 20 + wb + wt, tempImage);
		p.translate(-tr,tr);
	}
	p.restore();

	// menu
	int mw = 0.5 * 400;
	int mh = 0.5 * 400;
	p.save();
	p.setOpacity(opacity);
	tempFrame = style->frames[Style::START_MENU];
	tempImage = tempFrame.draw(QSize(mw, mh));
	p.drawImage(0,h-tbh-mh,tempImage);
	contentFrame.setMarginWidth(0);
	tempImage = contentFrame.draw(QSize(0.5 * mw,mh-16));
	p.drawImage(0+8,h-tbh-mh+8,tempImage);
	p.restore();

	return img;
}

QImage Preview::createFramePreview(QSize size, Frame *frame)
{
	QImage img(size, QImage::Format_ARGB32);
	return img;
}