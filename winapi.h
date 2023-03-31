#ifndef WINAPI_H
#define WINAPI_H

#include <qt_windows.h>
#include <QFileInfo>

class WinAPI
{
public:
    WinAPI();

    QString getLinkArgument(const QString& link);
};

#endif // WINAPI_H
