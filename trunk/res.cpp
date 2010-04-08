#include "res.h"

#include <QFile>

ResHack::ResHack()
: hRes(0), hResUpdate(0)
{}

ResHack::~ResHack()
{
	unload();
	endUpdate(true);
}

bool ResHack::load(const QString& f)
{
	if (!QFile::exists(f))
	{
		qDebug("file not found %s", qPrintable(f));
		return false;
	}

	hRes = LoadLibraryExA(f.toAscii().data(), NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);
	if (!hRes)
		return false;

	filename = f;
	return true;
}

void ResHack::unload()
{
	if (hRes)
	{
		FreeLibrary(hRes);
		hRes = 0;
	}
}

bool ResHack::beginUpdate(QString outputFilename)
{
	if (!hRes)
		return false;

	if (outputFilename == "")
		outputFilename = filename;

	QString dst = outputFilename;
	if (outputFilename == filename)
		dst = "_aero.msstyles";
	CopyFileA(outputFilename.toAscii().data(), dst.toAscii().data(), FALSE);
	hResUpdate = BeginUpdateResourceA(dst.toAscii().data(), false);
	return (hResUpdate != 0);
}

void ResHack::endUpdate(bool discard)
{
	if (hResUpdate)
	{
		EndUpdateResource(hResUpdate, discard);
		hResUpdate = 0;
	}
}

QByteArray ResHack::resource(const QString& type, const QString& name)
{
	QByteArray data;
	HRSRC hRsrc = 0;
	if (name.toInt() > 0 && name.toInt() < 5000)
		hRsrc = FindResourceA(hRes, (char*)name.toInt(), type.toAscii().data());
	else
		hRsrc = FindResourceA(hRes, name.toAscii().data(), type.toAscii().data());
	if (hRsrc)
	{
		DWORD dwSize = SizeofResource(hRes, hRsrc);
		HANDLE hResource = LoadResource(hRes, hRsrc);
		data = QByteArray((const char*)hResource, dwSize);
		FreeResource(hRsrc);
	}
	return data;
}

QImage ResHack::image(const QString& type, const QString& name)
{
	return QImage::fromData(resource(type, name));
}

bool ResHack::updateResource(const QString& type, const QString& name, const QByteArray &res)
{
	if (name.toInt() > 0 && name.toInt() < 5000)
		return UpdateResourceA(hResUpdate, type.toAscii().data(), (char*)name.toInt(), 
		1033, (void*)res.data(), res.size());
	return UpdateResourceA(hResUpdate, type.toAscii().data(), name.toAscii().data(), 
		1033, (void*)res.data(), res.size());
}

bool ResHack::updateImage(const QString& type, const QString& name, const QImage &img)
{
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	img.save(&buffer, "PNG");
	return updateResource(type, name, ba);
}
