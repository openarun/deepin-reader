#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H

#include <QString>

namespace PF {
static QString getImagePath(const QString &imageName, const QString &priName, const QString &sTheme = "light")
{
    return QString(":/resources/%1/%2/%3.svg").arg(sTheme).arg(priName).arg(imageName);
}

static QString GetCurThemeName(const QString &sType)
{
    if (sType == "1") {
        return  "light";
    }
    if (sType == "2") {
        return  "dark";
    }
    return "light";
}

}

#endif // PUBLICFUNCTION_H