#include "get_icon.h"
#include <QDebug>

#define DEFAULT_PORT_CLIENT "2701"
#define DEFAULT_PORT_SERVER "2702"

#define DEFAULT_BUFLEN 512

bool get_icon::server_init(){
    if (WSAStartup(MAKEWORD(2,2), &m_wsaDataServer) != 0) {
        qDebug() << "This is exception! In get_icon! (Server) Good luck...";
    }
    qDebug() << "Был вызван конструктор";
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    if (getaddrinfo(NULL, DEFAULT_PORT_SERVER, &hints, &result) != 0) {
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

bool get_icon::client_init(){
    if (WSAStartup(MAKEWORD(2,2), &m_wsaDataClient) != 0) {
        qDebug() << "This is exception! In get_icon! (Client) Good luck...";
    }
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    if (getaddrinfo("127.0.0.1", DEFAULT_PORT_CLIENT, &hints, &result) != 0) {
       WSACleanup();
       qDebug() << "This is exception! In get_icon! (Client//getaddrinfo) Good luck...";
    }
    m_ConnectSocketClient = INVALID_SOCKET;

    ptr=result;

    // Create a SOCKET for connecting to server
    m_ConnectSocketClient = socket(ptr->ai_family, ptr->ai_socktype,
        ptr->ai_protocol);

    if (m_ConnectSocketClient == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        qDebug() << "This is exception! In get_icon! (Client//getaddrinfo//socket) Good luck...";
    }

    // Connect to server.
    int iResult = ::connect(m_ConnectSocketClient, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_ConnectSocketClient);
        m_ConnectSocketClient = INVALID_SOCKET;
        qDebug() << "This is exception! In get_icon! (Client//getaddrinfo//socket//connect) Good luck...";
    }

    freeaddrinfo(result);

    if (m_ConnectSocketClient == INVALID_SOCKET) {
        WSACleanup();
        qDebug() << "This is exception! In get_icon! (Client//getaddrinfo//socket//connect//freeaddrinfo) Good luck...";
    }
}

get_icon::get_icon(QObject *parent) :
    QObject(parent),
    m_PATH(""),
    m_NAME("")
{

    // Сервер++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    server_init();
    // Сервер--------------------------------------------------------------------

    // Клиент++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //    client_init();
    // Клиент--------------------------------------------------------------------

}

get_icon::~get_icon()
{
    closesocket(m_ConnectSocketClient);
    WSACleanup();
    closesocket(m_ConnectSocketServer);
    WSACleanup();
}

bool get_icon::running() const
{
    return m_running;
}

QString get_icon::PATH() const
{
    return m_PATH;
}

QString get_icon::NAME() const
{
    return m_NAME;
}

void get_icon::run()
{
    qDebug() << "Работа на сервере";
    if ( listen(m_ConnectSocketServer, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(m_ConnectSocketServer);
        WSACleanup();
        qDebug() << "This is exception! In get_icon! (Server//run) Good luck...";
    }
    while(true){
        SOCKET ClientSocket;
        // Accept a client socket
        ClientSocket = accept(m_ConnectSocketServer, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            closesocket(m_ConnectSocketServer);
            WSACleanup();
            qDebug() << "This is exception! In get_icon! (Server//accept) Good luck...";
            continue;
        }
        qDebug() << "\n\n\nРабота на сервере\n\n\n";
        qDebug() << "Работа на сервере\n\n\n";
        qDebug() << "Работа на сервере\n\n\n";
        char send_data[] = "*msgs**endw**msge*";
        send(ClientSocket, send_data, 18, 0);
        /// Тут происходит работа с клиентом
    }
    emit finished();
}







void get_icon::setRunnung(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(m_running);
}

void get_icon::setPATH(QString PATH)
{
    if (m_PATH == PATH)
        return;

    m_PATH = PATH;
    emit PATHChanged(m_PATH);
}

void get_icon::setNAME(QString NAME)
{
    if (m_NAME == NAME)
        return;

    m_NAME = NAME;
    emit NAMEChanged(m_NAME);
}
