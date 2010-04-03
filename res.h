#pragma once

#include <windows.h>
#include <QtGui>

class ResHack
{
public:

	ResHack();
	~ResHack();

	bool load(const QString& filename);
	void unload();

	bool beginUpdate(QString outputFilename = "");
	void endUpdate(bool discard = false);

	QByteArray resource(const QString& type, const QString& name);
	QImage image(const QString& type, const QString& name);
	bool updateResource(const QString& type, const QString& name, const QByteArray &res);
	bool updateImage(const QString& type, const QString& name, const QImage &img);

private:
	HMODULE hRes;
	HANDLE hResUpdate;
	QString filename;
	QString outputFilename;
};