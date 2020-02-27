#include "mypixmapitem.h"

mypixmapitem::mypixmapitem(QGraphicsPixmapItem *parent):QGraphicsPixmapItem(parent)
{

}
void mypixmapitem::mousePressEvent(QGraphicsSceneMouseEvent *event){

}
void mypixmapitem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    setPos(pos()+mapToParent(event->pos())-mapToParent(event->lastPos()));

}
void mypixmapitem::wheelEvent(QGraphicsSceneWheelEvent *event){
    int delta=event->delta();
    double factor=scale();
    if(delta>0){
        factor*=(1+0.1);
    }
    if(delta<0){
        factor*=(1-0.1);
    }
    setTransformOriginPoint(boundingRect().width()/2,
                            boundingRect().height()/2); //中点缩放
    setScale(factor);

}

void mypixmapitem::reset(){
    setPos(0,0);
    setScale(1.0);
}
