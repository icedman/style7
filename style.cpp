#include "style.h"

bool Style::load(const QString& filename)
{
	return true;
}

QColor Style::readColor(QString clr, QColor defaultColor)
{
	QColor ret;
	QStringList sq = clr.split("=");
	if (sq.length() == 2)
	{
		QStringList rgb = sq.at(1).split("/");
		if (rgb.length() == 3)
		{
			if (sq.at(0) == "rgbi")
			{
				qreal r = rgb.at(0).toDouble();
				qreal g = rgb.at(1).toDouble();
				qreal b = rgb.at(2).toDouble();
				if (r <= 1.0f) r*=255;
				if (g <= 1.0f) g*=255;
				if (b <= 1.0f) b*=255;
				ret = QColor(r,g,b);
			} else {
				QString r = rgb.at(0) + rgb.at(0); r.truncate(2);
				QString g = rgb.at(1) + rgb.at(1); g.truncate(2);
				QString b = rgb.at(2) + rgb.at(2); b.truncate(2);
				ret = QColor(QString("#%1%2%3").arg(r).arg(g).arg(b));
			}
		}
	} else {
		ret = QColor(clr);
	}

	if (ret.isValid())
		return ret;
	return defaultColor;
}