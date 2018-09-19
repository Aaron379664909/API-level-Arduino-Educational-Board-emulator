#ifndef LEFTBUTTON_H
#define LEFTBUTTON_H
#include <QLabel>
class LeftButton : public QLabel{
    Q_OBJECT

    public:
        explicit LeftButton(QWidget *parent = 0);
public:
    void mouseMoveEvent(QMouseEvent*e);
    void mousePressEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // LEFTBUTTON_H
