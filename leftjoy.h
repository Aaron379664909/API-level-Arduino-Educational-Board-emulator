#ifndef LEFTJOY_H
#define LEFTJOY_H
#include <QLabel>
class LeftJoy : public QLabel{
    Q_OBJECT

    public:
        explicit LeftJoy(QWidget *parent = 0);
public:
    void mouseMoveEvent(QMouseEvent*e);
    void mousePressEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent *e);
};
#endif // LEFTJOY_H
