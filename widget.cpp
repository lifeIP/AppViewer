#include "widget.h"
#include "ui_widget.h"
#include "QListWidgetItem"
#include "pyapi.h"


#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <ctime>
#include <fstream>
#include <string>
#include <iostream>

#define CURRENT_DIRECTORY(fileName) (QString("C:\\Users\\molok\\Desktop\\ICONS\\ICONS\\test_\\") + QString(fileName))

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);       // разрешаем события отпускания объектов данных
    setMinimumWidth(640);
    setMinimumHeight(480);

    // Инициализация виджетов
    m_gridLayout = new QGridLayout(this); 
    m_imagesListView = new QListView(this);
    m_push_button = new QPushButton(this);
    m_menu_bar = new QMenuBar(this);


    // Настройка внешнего вида меню
    m_menu_bar->addMenu("Title");


    // Создание модели данных для списка
    m_imagesModel = new QStandardItemModel(m_imagesListView);
    m_imagesListView->setModel(m_imagesModel);  // Установим модель во вьюшку для превью изображений


    // Помещаем в сетку виджеты
    m_gridLayout->addWidget(m_menu_bar);
    m_gridLayout->addWidget(m_imagesListView);
    m_gridLayout->addWidget(m_push_button);

    // Инициализируем разные события
    qApp->installEventFilter(this); // Запускаает фильтр событий, нужный для eventFilter

    connect(&m_thread_0, &QThread::started,   &m_GetIcon,  &PyAPI::run);
    connect(&m_GetIcon,  &PyAPI::finished, &m_thread_0, &QThread::terminate);
    m_GetIcon.moveToThread(&m_thread_0);


    /// запуск потока (тест)
    m_thread_0.start();

    /// запуск потока (тест)
    //connect(m_imagesListView, &QListView::clicked, this, &Widget::);// Связывает событие и его последствия

    load_last_version(CURRENT_DIRECTORY("icons_info.iof").toStdString());

}


Widget::~Widget()
{
}


void Widget::slotEditRecord(){

}


bool Widget::eventFilter(QObject *obj, QEvent *event)
{

    if (obj == m_imagesListView->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);
        if (ev->buttons() & Qt::LeftButton)
        {
            qDebug()<< "double left clicked" << ev->pos();
            qDebug()<<  m_imagesListView->indexAt(ev->pos()).data();
        }
    }
    else if (obj == m_imagesListView->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);
        if (ev->buttons() & Qt::RightButton)
        {
            /* Создаем объект контекстного меню */
            QMenu * menu = new QMenu(this);
            /* Создаём действия для контекстного меню */
            QAction * editDevice = new QAction("Редактировать", this);
            QAction * deleteDevice = new QAction("Удалить", this);

            /* Устанавливаем действия в меню */
            menu->addAction(editDevice);
            menu->addAction(deleteDevice);

            // Связываем событие и его последствия
            //connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));
            connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord()));

            /* Вызываем контекстное меню */
            menu->popup(m_imagesListView->viewport()->mapToGlobal(ev->pos()));
            menu->exec();
        }
    }

    return QObject::eventFilter(obj, event);
}


void Widget::load_last_version(const std::string& filename){

    std::ifstream file;
    file.open (filename);
    if(file.is_open())
    {
        while (!file.eof()) {
            Widget::exe_info exe_info;


            getline(file, exe_info.exe_name);
            getline(file, exe_info.exe_icon);
            getline(file, exe_info.exe_path);

            if(exe_info.exe_name == "") break;

            QPixmap pixmap(exe_info.exe_icon.c_str());
            auto hm = new QStandardItem(QIcon(pixmap), exe_info.exe_name.c_str());
            hm->setEditable(0);

            m_imagesModel->appendRow(hm);
            m_exe_info.push_back(exe_info);
        }
        file.close();
    }
    else {
        // тут происходит что-то для логирования
    }
}


void Widget::save_last_version(const std::string& filename){
    std::ofstream file;
    file.open (filename);
    if(file.is_open()){
        for(int i = 0; i < m_exe_info.size(); ++i){
            file << m_exe_info.at(i).exe_name << "\n" <<
                    m_exe_info.at(i).exe_icon << "\n" <<
                    m_exe_info.at(i).exe_path << "\n";
        }
        file.close();
    }
    else {
        // тут происходит что-то для логирования
    }
}


void Widget::slotRemoveRecord(){
    int row_id = m_imagesListView->selectionModel()->currentIndex().row();
    if(row_id >= 0){
        qDebug() << "row_id" << m_imagesListView->selectionModel()->currentIndex().row();
        if (QMessageBox::warning(this, "Удаление записи",
                              "Вы уверены, что хотите удалить эту запись?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_exe_info.erase(m_exe_info.begin() + row_id);
            qDebug() << "row_id_in" << m_imagesListView->selectionModel()->currentIndex().row();
            m_imagesModel->removeRows(row_id, 1);
        }
        else {
            return;
        }
    }
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
    //QString filePath = event->mimeData()->urls()[0].toLocalFile();
    QString filePath = event->mimeData()->urls()[0].toLocalFile();

    Widget::exe_info exe_info;
    exe_info.exe_path = filePath.toStdString();

    QString fileName = QFileInfo(filePath).fileName();
    QString tmp = fileName.mid(0, fileName.size() - 5);
    exe_info.exe_name = tmp.toStdString();

    tmp = CURRENT_DIRECTORY(tmp) + QString(".png");
    exe_info.exe_icon = tmp.toStdString();

    m_GetIcon.setPATH(exe_info.exe_path.c_str());
    m_GetIcon.setNAME(exe_info.exe_icon.c_str());
    m_GetIcon.setRunnung(1);

    m_exe_info.push_back(exe_info);

    QPixmap pixmap(exe_info.exe_icon.c_str());

    // Добавляем элемент в список
    m_imagesModel->appendRow(new QStandardItem(QIcon(pixmap), exe_info.exe_name.c_str()));
}


void Widget::closeEvent (QCloseEvent *event)
{
    m_GetIcon.setRunnung(0);
    m_thread_0.terminate();
    save_last_version(CURRENT_DIRECTORY("icons_info.iof").toStdString());
    event->accept();
}
