#include "widget.h"
#include "ui_widget.h"
#include "constants.h"
#include "editwindow.h"
#include "winapi.h"

#include <QDebug>
#include <QFileIconProvider>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QProcess>

#include "dbase.h"
#include <ctime>
#include <fstream>
#include <string>
#include <cstdlib>
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



    m_db = new DBase(RootDir + "db_name.sqlite");



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
    m_db->get_indexes(m_index);
    loadIndex();

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
            if(m_imagesListView->indexAt(ev->pos()).row() == -1)
                return QObject::eventFilter(obj, event);

            slotOpenRecord();
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

/*
void Widget::loadLastVersion(const QString &filename){

    if(m_db->DBERROR < 0) this->close();

    m_db->load_last_version(m_exe_info);

    for(int i = 0; i < m_exe_info.size(); i++){
        QPixmap pixmap(m_exe_info.at(i).exe_icon.path());
        auto hm = new QStandardItem(QIcon(pixmap), m_exe_info.at(i).exe_name);
        hm->setEditable(0);

        m_imagesModel->appendRow(hm);
    }
}*/

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
    //m_imagesListView->setIconSize(QSize(128, 128));
    slotOpenRecord();
}

void Widget::slotRemoveRecord(){
    int row_id = m_imagesListView->selectionModel()->currentIndex().row();

    if(row_id < 0) return;
    if (QMessageBox::warning(this, "Удалить запись?",
                          "Вы уверены, что хотите удалить эту запись?",
                          QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    m_menu->hide();

    m_db->delete_app(m_index.at(row_id));
    m_index.erase(m_index.begin() + row_id);

    m_imagesModel->removeRows(row_id, 1);
}

void Widget::slotOpenRecord()
{
    if(!this->isEnabled()) return;
    int row_id = m_imagesListView->selectionModel()->currentIndex().row();
    if(row_id < 0) return;

    QString fileInfo(m_db->get_app_path(m_index.at(row_id)));

    // Открытие выбранного файла/программы/дирректории+++++++++++++++++++++++++++++++++++++++++++++++++
    qDebug() << fileInfo;

    //WinAPI link;
    //qDebug() << "link argument: " << link.getLinkArgument(fileInfo);

    HINSTANCE df = ShellExecuteW(NULL, L"open", (LPCTSTR)fileInfo.data(), NULL, NULL, SW_NORMAL);
    if((INT_PTR)df > 32) return;
    // Открытие выбранного файла/программы/дирректории-------------------------------------------------



    QMessageBox msgBox;
    int rslt = msgBox.warning(this, "Ошибка",
                   "Возможно, ярлык повреждён.\n"
                   "Попробуйте перейти в \"Расположение файла\" и\n"
                   "передайте в программу exe файл.",
                   "Понял",
                   "Непонял");
    qDebug() << rslt;
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
    QDir filePath;
    if(event->mimeData()->urls().isEmpty()){
        QMessageBox::critical(this, tr("Файловая ошибка"),
                                    tr("Этот файл не поддерживается "
                                    "данной платформой! \nПопробуйте другой"),
                                    QMessageBox::tr("Понял"));
        return;
    }


    filePath = event->mimeData()->urls()[0].toLocalFile();



    QFileInfo fileInfo = QFileInfo(filePath.path()).fileName();
    QString exe_name = fileInfo.completeBaseName();

    QDir exe_icon = FinalFilePath(exe_name) + QString(".png");

    //Получение иконки+++++++++++++++++++++++++++++++++++++++++++++++++++++
    QIcon icon;
    QFileIconProvider fileiconpr;
    icon = fileiconpr.icon(QFileInfo(filePath.path()));
    //Получение иконки-----------------------------------------------------

    m_db->add_new_app(exe_name.toUtf8(), filePath.path(), exe_icon.path());
    //m_db->get_indexes(m_index);


    //Сохранение иконки в файл+++++++++++++++++++++++++++++++++++++++++++++
    QPixmap pixmap = icon.pixmap(QSize(32,32));
    pixmap.save(exe_icon.path());
    //Сохранение иконки в файл---------------------------------------------

    // Добавление элемента в список++++++++++++++++++++++++++++++++++++++++
    auto hm = new QStandardItem(QIcon(pixmap), exe_name);
    hm->setEditable(0);
    m_imagesModel->appendRow(hm);
    // Добавление элемента в список----------------------------------------
}

bool Widget::loadIndex()
{
    m_index.clear();
    m_db->get_indexes(m_index);

    for(int i = 0; i < m_index.size(); i++){
        QPixmap pixmap(m_db->get_app_icon(m_index.at(i)));
        auto hm = new QStandardItem(QIcon(pixmap), m_db->get_app_name(m_index.at(i)));
        hm->setEditable(0);

        m_imagesModel->appendRow(hm);
    }
}


void Widget::closeEvent (QCloseEvent *event)
{
    event->accept();
}
