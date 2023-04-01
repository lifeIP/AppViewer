#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>
#include <QDir>

#define RootDir (QString(QDir::currentPath() + "/data/"))
#define FinalFilePath(fileName) (RootDir + QString(fileName))

#endif // CONSTANTS_H
