#ifndef EDITWINDOW_H
#define EDITWINDOW_H


#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QPoint>
#include <QFile>

class EditWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EditWindow(QWidget *parent = nullptr);

    void setWinSpawnPoint(const QPoint& point);
    QPoint getWinSpawnPoint() const;
    void setRowId(const int &row_id);
    int getRowId() const;
    void render();


signals:

public slots:


private:
    QPushButton*    m_button_0;
    QPushButton*    m_button_1;
    QLabel*         m_label_0;
    QLabel*         m_label_1;
    QGridLayout*    m_grid_layout_0;
    int             m_row_id;
};

#endif // EDITWINDOW_H
