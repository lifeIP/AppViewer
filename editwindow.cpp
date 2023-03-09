#include "editwindow.h"
#include "constants.h"

#include <QPixmap>
#include <QDebug>
#include <QDir>

EditWindow::EditWindow(QWidget *mainwin, QWidget *parent) : QWidget(parent), mainwin(mainwin)
{
    m_grid_layout_0 = new QGridLayout(this);

    m_label_1 = new QLabel("Text", this);

    m_button_0 = new QPushButton("Изменить икону", this);
    m_button_1 = new QPushButton("Изменить название", this);

    m_label_0 = new QLabel(this);
    setRowId(-1);

    m_grid_layout_0->addWidget(m_label_0, 0, 0);
    m_grid_layout_0->addWidget(m_button_0, 0, 1);
    m_grid_layout_0->addWidget(m_label_1, 1, 0);
    m_grid_layout_0->addWidget(m_button_1, 1, 1);

}

void EditWindow::setWinSpawnPoint(const QPoint &point)
{
    this->move(point);
}

QPoint EditWindow::getWinSpawnPoint() const
{
    return QPoint(0, 0);
}

void EditWindow::setRowId(const int &row_id)
{
    m_row_id = row_id;
}

int EditWindow::getRowId() const
{
    return m_row_id;
}

void EditWindow::render()
{
    mainwin->setEnabled(0);

    if(m_row_id < 0){
        return;
    }
    QFile file(FinalFilePath("icons_info.iof"));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    int limitation = 3 * m_row_id;
    int counter = 0;
    QString data;
    while (!file.atEnd()) {
        data = file.readLine();
        if(counter == limitation+1){
            data = data.mid(0, data.size() - 1);
            qDebug() << data;
            QPixmap pixmap(data);
            m_label_0->setPixmap(pixmap);
            return;
        }
        counter++;
    }
}

void EditWindow::closeEvent(QCloseEvent *event)
{
    mainwin->setEnabled(1);
}
