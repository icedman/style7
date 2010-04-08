#include "s7app.h"

#include <QApplication>

#pragma comment(linker,"/SUBSYSTEM:CONSOLE")

class Widget : public QWidget
{
public:
	Widget()
	{
		resize(500,350);
	}

	void paintEvent(QPaintEvent *event)
	{
		QPainter p(this);
		p.drawImage(QRect(2,2,img.width()*3,img.height()*3), img, img.rect());
	}

	void keyPressEvent(QKeyEvent *event)
	{
		close();
	}

	QImage img;
};

int main(int argc, char **argv)
{
	QApplication::setGraphicsSystem("raster");
	QApplication app(argc, argv);

	Widget w;

	S7App s7;
	s7.init();
	s7.load("themes/nyz.style");
	s7.run("preview.js");
	w.img = s7.image();
	w.show();

	app.exec();

	return 0;
}
