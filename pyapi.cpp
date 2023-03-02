#include "pyapi.h"
#include <QDebug>
#include <QDir>

#include <cstdlib>

#define DEFAULT_PORT_SERVER "2702"


bool PyAPI::server_init(){
    if (WSAStartup(MAKEWORD(2,2), &m_wsaDataServer) != 0) {
        qDebug() << "This is exception! In get_icon! (Server) Good luck...";
    }
    qDebug() << "Был вызван конструктор";
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    if (getaddrinfo(nullptr, DEFAULT_PORT_SERVER, &hints, &result) != 0) {
        WSACleanup();
        qDebug() << "This is exception! In get_icon! (Server//getaddrinfo) Good luck...";
    }

    m_ConnectSocketServer = INVALID_SOCKET;

    // Create a SOCKET for the server to listen for client connections

    m_ConnectSocketServer = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (m_ConnectSocketServer  == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        qDebug() << "This is exception! In get_icon! (Server//getaddrinfo//socket) Good luck...";
    }

    // Setup the TCP listening socket

    if (bind(m_ConnectSocketServer, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(m_ConnectSocketServer);
        WSACleanup();
        qDebug() << "This is exception! In get_icon! (Server//getaddrinfo//socket//bind) Good luck...";
    }

}


PyAPI::PyAPI(QObject *parent) :
    QObject(parent),
    m_PATH(""),
    m_NAME("")
{
    // Сервер++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    server_init();
    // Сервер--------------------------------------------------------------------
}


PyAPI::~PyAPI()
{
    closesocket(m_ConnectSocketServer);
    WSACleanup();
}


bool PyAPI::running() const
{
    return m_running;
}


QString PyAPI::PATH() const
{
    return m_PATH;
}


QString PyAPI::NAME() const
{
    return m_NAME;
}


void PyAPI::run()
{
    if ( listen(m_ConnectSocketServer, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(m_ConnectSocketServer);
        WSACleanup();
    }

    while(1) {
        while(m_running){
            if(!(m_PATH.size() > 0 && m_NAME.size() > 0)){
                m_running = false;
                continue;
            }
            qDebug() << m_PATH;
            qDebug() << m_NAME;
            SOCKET ClientSocket;
            system("start C:\\Users\\molok\\Desktop\\ICONS\\VisualPart\\IconsViewer\\LOAD_ICONS_FROM_INFO_FILE\\LOAD_ICONS_FROM_INFO_FILE.exe");
            // Accept a client socket
            ClientSocket = accept(m_ConnectSocketServer, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET) {
                closesocket(m_ConnectSocketServer);
                WSACleanup();
                m_running = false;
                continue;
            }

            /// Тут происходит работа с клиентом
            const std::string path =  QDir::fromNativeSeparators(m_PATH).toStdString();
            const std::string name =  QDir::fromNativeSeparators(m_NAME).toStdString();
            std::string send_data = "*msgs**0000**path*" + path + "*name*" + name + "*msge*";
            send(ClientSocket, send_data.c_str(), send_data.size(), 0);
            /// Тут происходит работа с клиентом
            m_running = false;
        }
    }
    emit finished();
}


void PyAPI::setRunnung(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(m_running);
}


void PyAPI::setPATH(QString PATH)
{
    if (m_PATH == PATH)
        return;

    m_PATH = PATH;
    emit PATHChanged(m_PATH);
}


void PyAPI::setNAME(QString NAME)
{
    if (m_NAME == NAME)
        return;

    m_NAME = NAME;
    emit NAMEChanged(m_NAME);
}
