#include "bb.h"

#include <QSettings>
#include <QFile>
#include <QTemporaryFile>
#include <QMap>

typedef struct _tStyleMap {
	char name[32];
	int value;
} StyleMap;

const StyleMap styleMaps[] =
{
	{"window.frame", Style::WINDOW},
	{"window.title.focus", Style::WINDOW_TITLE},
	{"window.label.focus", Style::WINDOW_TITLE_LABEL},
	{"window.button.focus", Style::WINDOW_BUTTON},
	{"window.button.pressed", Style::WINDOW_BUTTON_HILITE},
	{"window.button.pressed", Style::WINDOW_BUTTON_PRESSED},
	{"window.button.unfocus", Style::WINDOW_BUTTON_DISABLED},
	{"window.frame", Style::UNFOCUS_WINDOW},
	{"window.title.unfocus", Style::UNFOCUS_WINDOW_TITLE},
	{"window.label.unfocus", Style::UNFOCUS_WINDOW_TITLE_LABEL},
	{"window.button.unfocus", Style::UNFOCUS_WINDOW_BUTTON},
	{"window.button.pressed", Style::UNFOCUS_WINDOW_BUTTON_HILITE},
	{"window.button.pressed", Style::UNFOCUS_WINDOW_BUTTON_PRESSED},
	{"window.button.unfocus", Style::UNFOCUS_WINDOW_BUTTON_DISABLED},

	{"toolbar", Style::TASKBAR},
	{"toolbar.button", Style::TASKBAR_BUTTON},
	{"toolbar.button.pressed", Style::TASKBAR_BUTTON_HILITE},
	{"toolbar.button.pressed", Style::TASKBAR_BUTTON_PRESSED},
	{"toolbar.windowLabel", Style::TASKBAR_BUTTON_ACTIVE},
	{"toolbar.label", Style::TASKBAR_CLOCK},
	{"toolbar.button", Style::TASKBAR_DESKTOP_BUTTON},
	{"toolbar.button.pressed", Style::TASKBAR_DESKTOP_BUTTON_HILITE},
	{"toolbar.button.pressed", Style::TASKBAR_DESKTOP_BUTTON_PRESSED},

	{"menu.frame", Style::START_MENU},
	// {"", Style::START_MENU_ITEM},
	{"menu.hilite", Style::START_MENU_ITEM_HILITE},
	{"menu.active", Style::START_MENU_ITEM_PRESSED},
	// {"", Style::START_MENU_PROGRAMS},
	// {"", Style::START_MENU_PROGRAMS_ITEM},
	// {"menu.hilite", Style::START_MENU_PROGRAMS_ITEM_HILITE},
	// {"menu.active", Style::START_MENU_PROGRAMS_ITEM_PRESSED},
	// {"", Style::START_MENU_SHUTDOWN},
	// {"menu.hilite", Style::START_MENU_SHUTDOWN_HILITE},
	// {"menu.active", Style::START_MENU_SHUTDOWN_HILITE_PRESSED},

	{"",-1}
};
bool BBStyle::load(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QString content = file.readAll();
	content.replace(":", "=");

	memset(frames, 0, sizeof(Frame) * FRAMES_COUNT);

	// grey colors
	int rs = 0;
	int re = 255;
	for(int i = 0; i<105; i+=5)
	{
		content.replace(QString("grey%1").arg(i), 
			QVariant(QColor(rs+re*i/100, rs+re*i/100, rs+re*i/100)).toString());
	}

	QTemporaryFile temp;
	temp.open();
	temp.write(content.toAscii());
	temp.close();

	QSettings settings(temp.fileName(), QSettings::IniFormat);

	for(int i=0;;i++)
	{
		if (styleMaps[i].value==-1)
			break;
		loadFrame(styleMaps[i].name, styleMaps[i].value, &settings);
	}

	// fixes

	// window borders
	frames[WINDOW_TITLE].borderLeft = frames[WINDOW].borderLeft;
	frames[WINDOW_TITLE].borderRight = frames[WINDOW].borderRight;
	frames[WINDOW_TITLE].borderTop = frames[WINDOW].borderTop;
	frames[WINDOW_TITLE].borderBottom = frames[WINDOW].borderBottom;
	frames[UNFOCUS_WINDOW_TITLE].borderLeft = frames[UNFOCUS_WINDOW].borderLeft;
	frames[UNFOCUS_WINDOW_TITLE].borderRight = frames[UNFOCUS_WINDOW].borderRight;
	frames[UNFOCUS_WINDOW_TITLE].borderTop = frames[UNFOCUS_WINDOW].borderTop;
	frames[UNFOCUS_WINDOW_TITLE].borderBottom = frames[UNFOCUS_WINDOW].borderBottom;

	return true;
}

#define BB_GETCOLOR(var, key) var = readColor(settings->value(key).toString(), var);
#define BB_GETWIDTH(var, key) var = settings->value(key, var).toInt();

void BBStyle::loadFrame(QString key, int f, QSettings *settings)
{
	Frame *frame = &frames[f];

	// style
	QString style = settings->value(key).toString();
	if (style.indexOf("solid", 0, Qt::CaseInsensitive)!=-1)
		frame->fillType = Frame::FILL_SOLID;
	if (style.indexOf("gradient", 0, Qt::CaseInsensitive)!=-1)
		frame->fillType = Frame::FILL_GRADIENT;
	if (style.indexOf("raised", 0, Qt::CaseInsensitive)!=-1)
		frame->bevelType = Frame::BEVEL_RAISED;
	if (style.indexOf("sunken", 0, Qt::CaseInsensitive)!=-1)
		frame->bevelType = Frame::BEVEL_SUNKEN;
	if (style.indexOf("outline", 0, Qt::CaseInsensitive)!=-1)
		frame->bevelType = Frame::BEVEL_FLAT;

	// defaults
	int borderWidth = 0;
	if (style.indexOf("border", 0, Qt::CaseInsensitive)!=-1)
	{
		BB_GETWIDTH(borderWidth, "borderWidth");
		if (!frame->borderColor.isValid())
			BB_GETCOLOR(frame->borderColor, "borderColor");
	}
	int bevelWidth = 0;
	if (frame->bevelType!=Frame::BEVEL_NONE)
		BB_GETWIDTH(bevelWidth, "bevelWidth");

	// border
	if (f == WINDOW || f == TASKBAR)
	{
		BB_GETWIDTH(borderWidth, "frameWidth");
		BB_GETWIDTH(borderWidth, "borderWidth");
		BB_GETCOLOR(frame->borderColor, key + ".focusColor");
	}
	if (f == UNFOCUS_WINDOW)
	{
		BB_GETWIDTH(borderWidth, "frameWidth");
		BB_GETWIDTH(borderWidth, "borderWidth");
		BB_GETCOLOR(frame->borderColor, key + ".unfocusColor");
	}
	BB_GETCOLOR(frame->borderColor, key + ".borderColor");
	BB_GETWIDTH(borderWidth, key + ".borderWidth");
	frame->setBorderWidth(borderWidth);

	// bevel
	BB_GETWIDTH(bevelWidth, key + ".bevelWidth");
	frame->setBevelWidth(bevelWidth);

	// margin
	// corners

	// background / foreground
	BB_GETCOLOR(frame->background, key + ".background");
	BB_GETCOLOR(frame->background, key + ".backgroundColor");
	BB_GETCOLOR(frame->background, key + ".color");
	BB_GETCOLOR(frame->foreground, key + ".foreground");
	BB_GETCOLOR(frame->foreground, key + ".foregroundColor");
	BB_GETCOLOR(frame->foreground, key + ".textColor");
	BB_GETCOLOR(frame->foreground, key + ".picColor");

	// gradients
	frame->gradientColors[0] = frame->background;
	frame->gradientColors[99] = frame->background;
	BB_GETCOLOR(frame->gradientColors[0], key + ".color");
	BB_GETCOLOR(frame->gradientColors[0], key + ".color1");
	BB_GETCOLOR(frame->gradientColors[99], key + ".colorTo");
	BB_GETCOLOR(frame->gradientColors[99], key + ".color2");

	if (style.indexOf("vertical", 0, Qt::CaseInsensitive)!=-1)
	{
		frame->gradientX = 0;
		frame->gradientY = 0;
		frame->gradientX2 = 0;
		frame->gradientY2 = 100;
	}
	if (style.indexOf("horizontal", 0, Qt::CaseInsensitive)!=-1)
	{
		frame->gradientX = 0;
		frame->gradientY = 0;
		frame->gradientX2 = 100;
		frame->gradientY2 = 0;
	}
	if (style.indexOf("diagonal", 0, Qt::CaseInsensitive)!=-1)
	{
		frame->gradientX = 0;
		frame->gradientY = 0;
		frame->gradientX2 = 100;
		frame->gradientY2 = 100;
	}
	if (style.indexOf("crossdiagonal", 0, Qt::CaseInsensitive)!=-1)
	{
		frame->gradientX = 100;
		frame->gradientY = 0;
		frame->gradientX2 = 0;
		frame->gradientY2 = 100;
	}
	if (style.indexOf("elliptic", 0, Qt::CaseInsensitive)!=-1 ||
		style.indexOf("rectangle", 0, Qt::CaseInsensitive)!=-1)
	{
		frame->fillType = Frame::FILL_RADIAL_GRADIENT;
		frame->gradientX = 50;
		frame->gradientY = 50;
		frame->gradientX2 = 50;
		frame->gradientY2 = 50;
	}
	if (style.indexOf("pipecross", 0, Qt::CaseInsensitive)!=-1 ||
		style.indexOf("pyramid", 0, Qt::CaseInsensitive)!=-1)
	{
		frame->fillType = Frame::FILL_CONICAL_GRADIENT;
		frame->gradientX = 50;
		frame->gradientY = 50;
		frame->gradientX2 = 100;
		frame->gradientY2 = 50;
		QColor color1 = frame->gradientColors[0];
		QColor color2 = frame->gradientColors[99];
		frame->gradientColors[0] = color1;
		frame->gradientColors[12] = color2;
		frame->gradientColors[25] = color1;
		frame->gradientColors[37] = color2;
		frame->gradientColors[50] = color1;
		frame->gradientColors[62] = color2;
		frame->gradientColors[75] = color1;
		frame->gradientColors[87] = color2;
		frame->gradientColors[99] = color1;
	}
}
