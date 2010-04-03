#pragma once

#include <QtCore>
#include <QtScript>

#include "res.h"
#include "bb.h"

class S7App : public QObject
{
	Q_OBJECT

public:

	S7App();
	~S7App();

public Q_SLOTS:

	bool init();
	bool load(const QString &filename);
	void setTargetFile(const QString &filename);
	void setTemplate(const QString &type, const QString &name);
	void setAnalysis(bool a);

	bool begin(int w = 0, int h = 0);
	void end();
	void drawTemplate(int x, int y);
	void drawFrame(int frame, int x, int y, int w, int h);
	void setClip(int x, int y, int w, int h);
	void setTranslate(int x, int y);

	bool run(const QString &filename);

public:

	QImage& image();

private:

	QScriptEngine engine;
	QString filename;
	QString type;
	QString name;

	QRect clip;
	QPoint translate;

	BBStyle style;
	ResHack res;
	QImage buffer;
	QImage original;
	bool analysis;
};