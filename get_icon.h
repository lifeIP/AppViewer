#ifndef GET_ICON_H
#define GET_ICON_H

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <QObject>
#include <QString>
#include <QDebug>

class get_icon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunnung NOTIFY runningChanged)
    Q_PROPERTY(QString PATH READ PATH    WRITE setPATH    NOTIFY PATHChanged)
    Q_PROPERTY(QString NAME READ NAME    WRITE setNAME    NOTIFY NAMEChanged)

    bool m_running;
    QString m_PATH;
    QString m_NAME;
    WSADATA m_wsaDataClient;
    WSADATA m_wsaDataServer;
    SOCKET m_ConnectSocketClient;
    SOCKET m_ConnectSocketServer;

public:
    explicit get_icon(QObject *parent = nullptr);
    ~get_icon();
    bool server_init();
    bool client_init();
    bool running() const;
    QString PATH() const;
    QString NAME() const;

signals:
    void finished();
    void runningChanged(bool running);
    void PATHChanged(QString PATH);
    void NAMEChanged(QString NAME);

public slots:
    void run();
    void setRunnung(bool running);
    void setPATH(QString PATH);
    void setNAME(QString NAME);
};

#endif // GET_ICON_H
