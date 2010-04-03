#include "s7app.h"

#include <QtGui>

const char* frameTypes[Style::FRAMES_COUNT] = {
	"WINDOW",
	"WINDOW_TITLE",
	"WINDOW_TITLE_LABEL",
	"WINDOW_BUTTON",
	"WINDOW_BUTTON_HILITE",
	"WINDOW_BUTTON_PRESSED",
	"WINDOW_BUTTON_DISABLED",
	"UNFOCUS_WINDOW",
	"UNFOCUS_WINDOW_TITLE",
	"UNFOCUS_WINDOW_TITLE_LABEL",
	"UNFOCUS_WINDOW_BUTTON",
	"UNFOCUS_WINDOW_BUTTON_HILITE",
	"UNFOCUS_WINDOW_BUTTON_PRESSED",
	"UNFOCUS_WINDOW_BUTTON_DISABLED",

	"TASKBAR",
	"TASKBAR_BUTTON",
	"TASKBAR_BUTTON_HILITE",
	"TASKBAR_BUTTON_PRESSED",
	"TASKBAR_BUTTON_ACTIVE",
	"TASKBAR_CLOCK",
	"TASKBAR_DESKTOP_BUTTON",
	"TASKBAR_DESKTOP_BUTTON_HILITE",
	"TASKBAR_DESKTOP_BUTTON_PRESSED",

	"START_MENU",
	"START_MENU_ITEM",
	"START_MENU_ITEM_HILITE",
	"START_MENU_ITEM_PRESSED",
	"START_MENU_SHUTDOWN",
	"START_MENU_SHUTDOWN_HILITE",
	"START_MENU_SHUTDOWN_HILITE_PRESSED",
	"START_MENU_PROGRAMS",
	"START_MENU_PROGRAMS_ITEM",
	"START_MENU_PROGRAMS_ITEM_HILITE",
	"START_MENU_PROGRAMS_ITEM_PRESSED"
};

S7App::S7App() : analysis(false)
{}

S7App::~S7App()
{}

bool S7App::init()
{
	if (!res.load(QApplication::applicationDirPath() + "/aero/aero.msstyles")) {
		qDebug("unable to load template");
		return false;
	}

	engine.globalObject().setProperty("s7", engine.newQObject(this));
	for(int i = 0; i<Style::FRAMES_COUNT; i++)
		engine.globalObject().setProperty(frameTypes[i], i);

	return true;
}

bool S7App::load(const QString &filename)
{
	return style.load(filename);
}

void S7App::setTargetFile(const QString &f)
{
	filename = f;
}

void S7App::setTemplate(const QString &t, const QString &n)
{
	type = t;
	name = n;
	original = res.image(t, n);
}

bool S7App::begin(int w, int h)
{
	qDebug("begin %s", qPrintable(name));

	clip = QRect();

	if (w == 0 || h == 0)
	{
		w = original.width();
		h = original.height();
		if (analysis)
			w *= 2;
	}

	buffer = QImage(QSize(w,h), QImage::Format_ARGB32);

	QPainter p(&buffer);

	// clear
	p.save();
	p.setCompositionMode(QPainter::CompositionMode_Clear);
	p.fillRect(buffer.rect(), QColor("black"));
	p.restore();

	p.end();

	if (analysis)
		drawTemplate(0, 0);

	return true;
}

void S7App::end()
{
	qDebug("end");
	if (!filename.isEmpty())
		buffer.save(filename);


	if (!analysis)
	{
		res.updateImage(type, name, buffer);
		qDebug("resource updated");
	}

}

void S7App::drawTemplate(int x, int y)
{
	qDebug("drawTemplate");

	QPainter p(&buffer);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setRenderHint(QPainter::SmoothPixmapTransform, true);
	p.drawImage(x, y, original);

}

void S7App::drawFrame(int frame, int x, int y, int w, int h)
{
	qDebug("drawFrame");
	Frame f = style.frames[frame];
	QImage tmp = f.draw(QSize(w,h));

	QPainter p(&buffer);
	p.save();
	if (analysis)
		p.translate(QPoint(original.width(),0));
	if (clip.width() > 0 && clip.height() > 0)
		p.setClipRect(clip);
	p.translate(translate);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setRenderHint(QPainter::SmoothPixmapTransform, true);
	p.drawImage(x, y, tmp);
	p.restore();
}

void S7App::setClip(int x, int y, int w, int h)
{
	clip = QRect(x, y, w, h);
}

void S7App::setTranslate(int x, int y)
{
	translate = QPoint(x, y);
}

bool S7App::run(const QString &filename)
{
	res.beginUpdate();

	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly))
		return false;	
	engine.evaluate(f.readAll());

	res.endUpdate();

	return true;
}

void S7App::setAnalysis(bool a)
{
	analysis = a;
}

QImage& S7App::image()
{
	return buffer;
}