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
#include <QPoint>
#include <QDir>
#include <vector>

#include <qt_windows.h>
#include "editwindow.h"
#include "constants.h"
#include "dbase.h"

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


    bool loadIndex();

private slots:
    void closeEvent (QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void slotRemoveRecord();
    void slotOpenRecord();
    void slotEditRecord();

    void slotButtonTriggered();
private:

    QMenuBar*               m_menu_bar;
    QPushButton*            m_push_button;
    QListView*              m_imagesListView;   // Список с изображениями
    QGridLayout*            m_gridLayout;       // Сетка для интерфейса
    QStandardItemModel*     m_imagesModel;      // Модель данных с изображениями
    std::vector<int>        m_index;
    EditWindow*             m_edit_window;
    QMenu*                  m_menu;
    DBase*                  m_db;
};

#endif // WIDGET_H
