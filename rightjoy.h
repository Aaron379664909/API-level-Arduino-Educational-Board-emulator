#ifndef RIGHTJOY_H
#define RIGHTJOY_H
#include <QLabel>
class RightJoy : public QLabel{
    Q_OBJECT

    public:
        explicit RightJoy(QWidget *parent = 0);
public:
    void mouseMoveEvent(QMouseEvent*e);
    void mousePressEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // RIGHTJOY_H
