#include "widget.h"
#include "ui_widget.h"
#include "constants.h"
#include "editwindow.h"

#include <QDebug>
#include <QFileIconProvider>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

#include <ctime>
#include <fstream>
#include <string>
#include <iostream>
#include <qt_windows.h>
#include <shobjidl.h>
#include <map>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);       // разрешаем события отпускания объектов данных
    setMinimumWidth(640);
    setMinimumHeight(480);

    // Инициа лизация виджетов
    m_gridLayout = new QGridLayout(this);
    m_imagesListView = new QListView(this);
    m_push_button = new QPushButton(QString("Открыть"), this);
    m_menu_bar = new QMenuBar(this);
    m_edit_window = new EditWindow(this);

    // Настройка внешнего вида меню
    m_menu_bar->addMenu("Title");


    // Создание модели данных для списка
    m_imagesModel = new QStandardItemModel(m_imagesListView);
    m_imagesListView->setModel(m_imagesModel);  // Установим модель во вьюшку для превью изображений


    // Помещаем в сетку виджеты
    m_gridLayout->addWidget(m_menu_bar);
    m_gridLayout->addWidget(m_imagesListView);
    m_gridLayout->addWidget(m_push_button);

    /* Создаем объект контекстного меню */
    m_menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction* openDevice = new QAction("Открыть", this);
    QAction* editDevice = new QAction("Редактировать", this);
    QAction* deleteDevice = new QAction("Удалить", this);

    /* Устанавливаем действия в меню */
    m_menu->addAction(openDevice);
    m_menu->addAction(editDevice);
    m_menu->addAction(deleteDevice);

    // Связываем событие и его последствия

    connect(openDevice, SIGNAL(triggered()), this, SLOT(slotOpenRecord()));
    connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord()));

    // Инициализируем разные события
    qApp->installEventFilter(this); // Запускаает фильтр событий, нужный для eventFilter

    connect(m_push_button, SIGNAL(released()), this, SLOT(slotButtonTriggered()));

    //connect( drag, SIGNAL(destroyed()), this, SLOT(dragDestroyed()));
    loadLastVersion(FinalFilePath("icons_info.iof"));
}


Widget::~Widget()
{
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{

    if (obj == m_imagesListView->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);
        if (ev->buttons() & Qt::LeftButton)
        {
            qDebug()<< "double left clicked" << ev->pos();
            if(m_imagesListView->indexAt(ev->pos()).row() == -1)
                return QObject::eventFilter(obj, event);

        }
    }
    else if (obj == m_imagesListView->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);
        if ((ev->buttons() & Qt::RightButton)&& this->isEnabled())
        {
            if(m_imagesListView->indexAt(ev->pos()).row() == -1)
                return QObject::eventFilter(obj, event);

            /* Вызываем контекстное меню */
            m_menu->popup(m_imagesListView->viewport()->mapToGlobal(ev->pos()));
            m_menu->exec();
        }
    }
    return QObject::eventFilter(obj, event);
}


void Widget::loadLastVersion(const QString &filename){

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    while (!file.atEnd()) {
        Widget::exeInfo exe_info;

        exe_info.exe_name = file.readLine().trimmed();
        if(exe_info.exe_name.isEmpty()) break;

        exe_info.exe_icon.setPath(file.readLine().trimmed());
        if(exe_info.exe_icon.path().isEmpty()) break;

        exe_info.exe_path.setPath(file.readLine().trimmed());
        if(exe_info.exe_path.path().isEmpty()) break;

        QPixmap pixmap(exe_info.exe_icon.path());
        auto hm = new QStandardItem(QIcon(pixmap), exe_info.exe_name);
        hm->setEditable(0);

        m_imagesModel->appendRow(hm);
        m_exe_info.push_back(exe_info);
    }
    file.close();
}


void Widget::saveLastVersion(const QString &filename){
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    for(int i = 0; i < m_exe_info.size(); ++i){
        file.write(m_exe_info.at(i).exe_name.toUtf8());
        file.write("\n");
        file.write(m_exe_info.at(i).exe_icon.path().toUtf8());
        file.write("\n");
        file.write(m_exe_info.at(i).exe_path.path().toUtf8());
        file.write("\n");
    }
    file.close();
}

void Widget::slotEditRecord(){
    int row_id = m_imagesListView->selectionModel()->currentIndex().row();
    if(row_id >= 0){
        if (QMessageBox::warning(this, "Модифицировать запись?",
                              "Вы уверены, что хотите модифицировать эту запись?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_edit_window->move(m_menu->pos().x(), m_menu->pos().y());
            m_edit_window->setRowId(row_id);
            m_edit_window->render();
            m_edit_window->show();
        }
        else {
            return;
        }
    }
}

void Widget::slotButtonTriggered()
{
    m_imagesListView->setIconSize(QSize(128, 128));
}

void Widget::slotRemoveRecord(){
    int row_id = m_imagesListView->selectionModel()->currentIndex().row();

    if(row_id < 0) return;
    if (QMessageBox::warning(this, "Удалить запись?",
                          "Вы уверены, что хотите удалить эту запись?",
                          QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    m_menu->hide();

    m_exe_info.erase(m_exe_info.begin() + row_id);
    m_imagesModel->removeRows(row_id, 1);
}

QString Widget::forWindowsGETPATH(LPCTSTR pszShortcut) //https://www.rsdn.org/article/winshell/shortcuts.xml
{
   IPersistFile* ppf;
   IShellLink* pshl;
   WIN32_FIND_DATA wfd;

   // инициализируем COM-библиотеку
   ::CoInitialize(nullptr);

   // создаем COM-объект и получаем указатель на IPersistFile
   ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
      IID_IPersistFile, reinterpret_cast<void**>(&ppf));

   // загружаем ярлык
#if defined(_UNICODE)
   ppf->Load(pszShortcut, STGM_READ);
#else
   LPWSTR pwszTemp = new WCHAR[_MAX_PATH];
   mbstowcs(pwszTemp, pszShortcut, _MAX_PATH);
   ppf->Load(pwszTemp, STGM_READ);
   delete[] pwszTemp;
#endif// получаем указатель на IShellLink
   ppf->QueryInterface(IID_IShellLinkA, reinterpret_cast<void**>(&pshl));

   // ищем объект, на который ссылается ярлык
   pshl->Resolve(nullptr, SLR_ANY_MATCH | SLR_NO_UI);

   // получаем имя объекта и выводим его на консоль
   LPTSTR pszTarget = new TCHAR[_MAX_PATH];
   pshl->GetPath(pszTarget, _MAX_PATH, &wfd, 0);
   QString result = (LPSTR)pszTarget;
   delete[] pszTarget;

   // убираем за собой
   pshl->Release();
   ppf->Release();

   // завершаем работу с COM-библиотекой
   ::CoFreeUnusedLibraries();
   ::CoUninitialize();
   return result;
}

void Widget::slotOpenRecord()
{
    int row_id = m_imagesListView->selectionModel()->currentIndex().row();
    if(row_id < 0) return;

    QDir path = m_exe_info.at(row_id).exe_path;

    // Открытие выбранного файла/программы/дирректории+++++++++++++++++++++++++++++++++++++++++++++++++
    ///РЕАЛИЗАЦИЯ ДЛЯ WINDOWS+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    QString compath = "open ";
    compath += forWindowsGETPATH((LPCTSTR)path.path().utf16());

    ///РЕАЛИЗАЦИЯ ДЛЯ WINDOWS---------------------------------------------------------------------------------
    // Открытие выбранного файла/программы/дирректории-------------------------------------------------


    /*
    QMessageBox msgBox;
    int rslt = msgBox.warning(this, "Ошибка",
                   "Возможно, ярлык повреждён.\n"
                   "Попробуйте перейти в \"Расположение файла\" и\n"
                   "передайте в программу exe файл.",
                   "Понял",
                   "Непонял");
    qDebug() << rslt;*/
}


void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    // Обязательно необходимо допустить событие переноса данных в область окна приложения
    event->accept();
}


void Widget::dropEvent(QDropEvent *event)
{
    // Когда отпускаем файл в область приложения,
    // то забираем путь к файлу из MIME данных
    QString filePath;
    if(event->mimeData()->urls().isEmpty()){
        QMessageBox::critical(this, tr("Файловая ошибка"),
                                    tr("Этот файл не поддерживается "
                                    "данной платформой! \nПопробуйте другой"),
                                    QMessageBox::tr("Понял"));
        return;
    }


    filePath = event->mimeData()->urls()[0].toLocalFile();

    Widget::exeInfo exe_info;
    exe_info.exe_path = filePath;

    QFileInfo fileInfo = QFileInfo(filePath).fileName();
    exe_info.exe_name = fileInfo.completeBaseName();

    exe_info.exe_icon = FinalFilePath(exe_info.exe_name) + QString(".png");

    //Получение иконки+++++++++++++++++++++++++++++++++++++++++++++++++++++
    QIcon icon;
    QFileIconProvider fileiconpr;
    icon = fileiconpr.icon(QFileInfo(filePath));
    //Получение иконки-----------------------------------------------------

    m_exe_info.push_back(exe_info);

    //Сохранение иконки в файл+++++++++++++++++++++++++++++++++++++++++++++
    QPixmap pixmap = icon.pixmap(QSize(32,32));
    pixmap.save(exe_info.exe_icon.path());
    //Сохранение иконки в файл---------------------------------------------

    // Добавление элемента в список++++++++++++++++++++++++++++++++++++++++
    auto hm = new QStandardItem(QIcon(pixmap), exe_info.exe_name);
    hm->setEditable(0);
    m_imagesModel->appendRow(hm);
    // Добавление элемента в список----------------------------------------

    saveLastVersion(FinalFilePath("icons_info.iof"));
}


void Widget::closeEvent (QCloseEvent *event)
{
    saveLastVersion(FinalFilePath("icons_info.iof"));
    event->accept();
}
