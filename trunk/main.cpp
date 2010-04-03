#include "bb.h"
#include "res.h"
#include "preview.h"
#include "s7app.h"

#include <QApplication>

#pragma comment(linker,"/SUBSYSTEM:CONSOLE")

class Widget : public QWidget
{
public:
	Widget()
	{
		//style.load("themes/outcomes.style");
		style.load("themes/nyz.style");
		Preview preview(&style, true);

		QImage desktop = preview.createDesktopPreview(QSize(512, 400));
		QImage bg("themes/bg.jpg");
		img = QImage(512,400, desktop.format());
		QPainter p(&img);
		p.setRenderHint(QPainter::Antialiasing, true);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);
		p.fillRect(img.rect(),QColor("#505050"));
		p.drawImage(img.rect(),bg,bg.rect());
		p.drawImage(img.rect(),desktop,desktop.rect());

		resize(500,350);
	}

	void paintEvent(QPaintEvent *event)
	{
		QPainter p(this);
		p.drawImage(QRect(2,2,img.width()*3,img.height()*3), img, img.rect());
		//p.drawImage(0,0,img);
	}

	void keyPressEvent(QKeyEvent *event)
	{
		close();
	}

	QImage img;
	BBStyle style;
};

int main(int argc, char **argv)
{
	QApplication::setGraphicsSystem("raster");
	QApplication app(argc, argv);

	Widget w;

#if 0
	ResHack res;
	if (!res.load(app.applicationDirPath() + "/aero/aero.msstyles"))
		qDebug("unable to load");

	QImage img = res.image("IMAGE","934");
	QPainter p(&img);
	p.setOpacity(0.5);
	p.fillRect(img.rect(), QColor(255,0,0));

	res.beginUpdate();
	res.updateImage("IMAGE","934",img);
	res.endUpdate();
#else


	S7App s7;
	s7.init();
	s7.load("themes/nyz.style");
	s7.run("preview.js");
	w.img = s7.image();
#endif;

	w.show();
	app.exec();

	return 0;
}
