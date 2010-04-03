#pragma once

#include "style.h"
#include <QMap>

class QSettings;
class BBStyle : public Style
{
public:

	bool load(const QString& filename);

protected:

	virtual void loadFrame(QString key, int frame, QSettings *settings);

	QMap<QString,int> styleMap;
};