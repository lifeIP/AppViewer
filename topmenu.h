#ifndef TOPMENU_H
#define TOPMENU_H

#include <QObject>
#include <QWidget>
#include <QMenuBar>


class TopMenu : public QWidget
{
    Q_OBJECT
public:
    explicit                TopMenu(QWidget *parent = nullptr);

signals:

public slots:

private:
    QMenuBar*               m_menu_bar;
};

#endif // TOPMENU_H
