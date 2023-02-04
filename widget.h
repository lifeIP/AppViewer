#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPalette>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QScrollArea>
#include <QLabel>
#include <QListView>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QPushButton>

#include <vector>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    // Метод события перетаскивания
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    // Метод события отпускания объекта с данными
    virtual void dropEvent(QDropEvent *event) override;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// Loads data when the program starts.
    void load_last_version(const std::string& filename);

    /// Saves changes when data is modified.
    void save_last_version(const std::string& filename);

    struct exe_info{
        std::string exe_name;
        std::string exe_path;
        std::string exe_icon;
    };
    //-------------------------------------------------------------------------------------------


private slots:
    // Слот для обработки кликов по элементам списка
    void onImagesListViewClicked(const QModelIndex& index);

    void closeEvent (QCloseEvent *event);

private:
    QPushButton*            m_push_button;
    QListView*              m_imagesListView;   // Список с изображениями
    QGridLayout*            m_gridLayout;       // Сетка для интерфейса
    QStandardItemModel*     m_imagesModel;      // Модель данных с изображениями
    std::vector<exe_info>   m_exe_info;
};

#endif // WIDGET_H
