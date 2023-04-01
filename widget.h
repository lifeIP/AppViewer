#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QPalette>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QListView>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QMenuBar>
#include <QList>
#include <QDebug>
#include <QFileIconProvider>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <qt_windows.h>


#include <vector>
#include <string>


#include "editwindow.h"
#include "constants.h"
#include "dbase.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit                Widget(QWidget *parent = nullptr);
    ~Widget();


    virtual void            dragEnterEvent(QDragEnterEvent* event) override;
    virtual void            dropEvent(QDropEvent *event) override;


    bool                    loadIndex();

private slots:
    void                    closeEvent (QCloseEvent *event) override;
    bool                    eventFilter(QObject *obj, QEvent *event) override;
    void                    slotRemoveRecord();
    void                    slotOpenRecord();
    void                    slotEditRecord();

private:

    QMenuBar*               m_menu_bar;
    QListView*              m_imagesListView;
    QGridLayout*            m_gridLayout;
    QStandardItemModel*     m_imagesModel;
    std::vector<int>        m_index;
    EditWindow*             m_edit_window;
    QMenu*                  m_menu;
    DBase*                  m_db;
};

#endif // WIDGET_H
