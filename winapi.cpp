#include "winapi.h"
#include <qt_windows.h>
#include <shobjidl.h>
#include <QFileInfo>
#include <QDebug>

WinAPI::WinAPI()
{

}


///Пока ненужная фигня
QString WinAPI::getLinkArgument(const QString &link)
{
    CoInitialize(nullptr);

    IPersistFile* ppf;
    IShellLink* pshl;
    WIN32_FIND_DATA wfd;

    // создаем COM-объект и получаем указатель на IPersistFile
    CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
          IID_IPersistFile, reinterpret_cast<void**>(&ppf));

    // загружаем ярлык
    ppf->Load((LPCTSTR)link.toStdString().c_str(), STGM_READ);

    // получаем указатель на IShellLink
    ppf->QueryInterface(IID_IShellLinkA, reinterpret_cast<void**>(&pshl));

    // ищем объект, на который ссылается ярлык
    pshl->Resolve(NULL, SLR_ANY_MATCH | SLR_NO_UI);


    LPTSTR pszDest = new TCHAR[255];
    pshl->GetArguments(pszDest, 255);

    qDebug() << "Аргумент+";
    qDebug() << *pszDest;
    qDebug() << "Аргумент-";
    delete[] pszDest;

    // убираем за собой
    pshl->Release();
    ppf->Release();

    // завершаем работу с COM-библиотекой
    CoFreeUnusedLibraries();
    CoUninitialize();

    return "result";
}
