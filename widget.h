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
#include <QMenuBar>
#include <QList>
#include <QThread>

#include <vector>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    // Метод события перетаскивания
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    // Метод события отпускания объекта с данными
    virtual void dropEvent(QDropEvent *event) override;

    /// Loads data when the program starts.
    void loadLastVersion(const QString& filename);

    /// Saves changes when data is modified.
    void saveLastVersion(const QString& filename);

    struct exeInfo{
        QString exe_name;
        QString exe_path;
        QString exe_icon;
    };

private slots:
    void closeEvent (QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void slotRemoveRecord();
    void slotEditRecord();

private:
    void update_list();

    QMenuBar*               m_menu_bar;
    QPushButton*            m_push_button;
    QListView*              m_imagesListView;   // Список с изображениями
    QGridLayout*            m_gridLayout;       // Сетка для интерфейса
    QStandardItemModel*     m_imagesModel;      // Модель данных с изображениями
    std::vector<exeInfo>    m_exe_info;
};

#endif // WIDGET_H
