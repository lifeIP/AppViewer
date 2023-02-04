#include "widget.h"
#include "ui_widget.h"
#include "QListWidgetItem"

#include <QMessageBox>

#include <fstream>
#include <vector>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);       // разрешаем события отпускания объектов данных
    setMinimumWidth(640);
    setMinimumHeight(480);

    /// Настраиваем интерфейс
    m_gridLayout = new QGridLayout(this);
    m_imagesListView = new QListView(this);

    // Создадим модель данных для списка изображений
    m_imagesModel = new QStandardItemModel(m_imagesListView);
    m_imagesListView->setModel(m_imagesModel);  // Установим модель во вьюшку для превью изображений
    m_gridLayout->addWidget(m_imagesListView);
    connect(m_imagesListView, &QListView::clicked, this, &Widget::onImagesListViewClicked);
    load_last_version("C:\\Users\\molok\\Desktop\\ICONS\\ICONS\\test_\\dist\\save_data.txt");
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

void Widget::save_last_version(const std::string& filename){//Is in development
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




//Is in development&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&








//Is in development&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&




Widget::~Widget()
{

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
    QString filePath = event->mimeData()->urls()[0].toLocalFile();
    // Создаём изображение
    QPixmap pixmap(filePath);
    // Помещаем его в область скроллинга через QLabel

    // Добавляем элемент в список
    m_imagesModel->appendRow(new QStandardItem(QIcon(pixmap), filePath));
}

void Widget::onImagesListViewClicked(const QModelIndex &index)
{
    // Когда кликаем по элементу в списке, то забираем путь к файлу
    QPixmap pixmap(m_imagesModel->data(index).toString());
}

void Widget::closeEvent (QCloseEvent *event)
{
    save_last_version("C:\\Users\\molok\\Desktop\\ICONS\\ICONS\\test_\\dist\\save_data.txt");
    event->accept();
}
