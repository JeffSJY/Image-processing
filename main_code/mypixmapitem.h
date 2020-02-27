#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QObject>
#include<QGraphicsPixmapItem>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsSceneWheelEvent>

class mypixmapitem:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit mypixmapitem(QGraphicsPixmapItem *parent=0);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void wheelEvent(QGraphicsSceneWheelEvent *);
    void reset();
signals:
public slots:
};

#endif // MYPIXMAPITEM_H
