#ifndef GET_ICON_H
#define GET_ICON_H

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <QObject>
#include <QString>
#include <QDebug>

class PyAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunnung NOTIFY runningChanged)
    Q_PROPERTY(QString PATH READ PATH    WRITE setPATH    NOTIFY PATHChanged)
    Q_PROPERTY(QString NAME READ NAME    WRITE setNAME    NOTIFY NAMEChanged)

    bool m_running;
    QString m_PATH;
    QString m_NAME;
    WSADATA m_wsaDataServer;
    SOCKET m_ConnectSocketServer;

public:
    explicit PyAPI(QObject *parent = nullptr);
    ~PyAPI();
    /// Returns the flag value indicating that the code is under execution.
    bool running() const;
    /// Returns the value specified in advance
    QString PATH() const;
    /// Returns the value specified in advance
    QString NAME() const;

signals:
    void finished();
    void runningChanged(bool running);
    void PATHChanged(QString PATH);
    void NAMEChanged(QString NAME);

public slots:
    /// The function that starts the work.
    void run();
    /// Sets the flag that is responsible for executing the code in the run() function.
    void setRunnung(bool running);
    /// A function that sets the value of a property named PATH.
    void setPATH(QString PATH);
    /// A function that sets the value of a property named NAME.
    void setNAME(QString NAME);

private:
    bool server_init();
};

#endif // GET_ICON_H
