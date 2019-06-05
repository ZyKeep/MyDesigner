/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/


#include "boxitem.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QMenu>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>


#include "MyClass/myalleyway.h"

#include <QDebug>
bool isDirtyChange(QGraphicsItem::GraphicsItemChange change)
{
    return (change == QGraphicsItem::ItemPositionChange ||
            change == QGraphicsItem::ItemPositionHasChanged ||
            change == QGraphicsItem::ItemTransformChange ||
            change == QGraphicsItem::ItemTransformHasChanged);
}

BoxItem::BoxItem(const QRectF &rect_, QGraphicsScene *scene,
                 bool lock,int id, bool send)
    : QObject(), QGraphicsRectItem(),
      m_resizing(false),m_lock(lock),m_id(id),m_send(send),
      m_frameOn(false),m_framSize(1),m_frameRed(0),m_frameGreen(0),m_frameBlue(0),
      m_angle(0.0), m_shearHorizontal(0.0), m_shearVertical(0.0),m_in(0)
{

    setFlags(QGraphicsItem::ItemIsSelectable|
         #if QT_VERSION >= 0x040600
             QGraphicsItem::ItemSendsGeometryChanges|
         #endif
             QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsFocusable);

    setPos(0,0);
    setRect(rect_);
    dm_rect = rect_;
    m_lastrect = dm_rect;



    scene->addItem(this);
    setSelected(!send);//设定
    setFocus();

    m_charge = false;
    AutoColor();
    initMenu();
}


void BoxItem::setPen(const QPen &pen_)
{
    if (isSelected() && pen_ != pen()) {
        QGraphicsRectItem::setPen(pen_);
    }
}


void BoxItem::setBrush(const QBrush &brush_)
{
    if (isSelected() && brush_ != brush()) {
        QGraphicsRectItem::setBrush(brush_);
    }
}


void BoxItem::setAngle(double angle)
{
    if (isSelected() && !qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        updateTransform();
    }
}

//=-=================+调整大小
void BoxItem::resetRect(QRectF rect)
{
    qDebug()<<"in resert"<<rect;
    if(m_lock)
        return;
    adjustRect(rect);
    setRect(rect);
    setPos(0,0);
    m_charge = true;
    send_Dirty();


}

void BoxItem::setSize(const QSize& size)
{
    resetRect(QRectF(QPointF(dm_rect.topLeft()),
                     QSizeF(size.width(), size.height())));
}

void BoxItem::setShear(double shearHorizontal, double shearVertical)
{
    if (isSelected() && (!qFuzzyCompare(m_shearHorizontal, shearHorizontal) ||
                         !qFuzzyCompare(m_shearVertical, shearVertical))) {
        m_shearHorizontal = shearHorizontal;
        m_shearVertical = shearVertical;
        updateTransform();
    }
}


void BoxItem::updateTransform()
{
    QTransform transform;
    transform.shear(m_shearHorizontal, m_shearVertical);
    transform.rotate(m_angle);
    setTransform(transform);
}

bool BoxItem::isPointContains(QPointF&newPos)
{
    QPointF mPos = this->rect().topLeft();
    QPointF tPos = newPos + mPos;

    QRectF rect = scene()->sceneRect();
    rect.setSize(rect.size() - this->rect().size());
    if (!rect.contains(tPos)) {
        newPos.setX(qMin(rect.right()-mPos.x(), qMax(newPos.x(), rect.left()-mPos.x())));
        newPos.setY(qMin(rect.bottom()-mPos.y(), qMax(newPos.y(), rect.top()-mPos.y())));
        return false;
    }

    return true;
}

bool BoxItem::isRectContains(const QRectF&rect)
{
    QPointF tPos = this->pos() + rect.topLeft();

    QRectF rect1 = scene()->sceneRect();
    rect1.setSize(rect1.size() - rect.size());
    //大小改变后边界判断以及大小限定
    if (!rect1.contains(tPos) || (rect.width() < 50 && rect.height() < 50)) {
        return false;
    }


    return true;
}

void BoxItem::adjustRect(QRectF& rect)
{
    QRectF rect1 = scene()->sceneRect();
    rect1.setSize(rect1.size() - rect.size());//得到最小大小

    if(rect1.width() < 0 || rect1.height() < 0)
    {
        rect.setSize(scene()->sceneRect().size());
    }

    if(rect.x() < rect1.left())
        rect.setX(rect1.left());
    else if(rect.x() > rect.right())
    {
        rect.setX(rect.right());
    }

    if(rect.y() < rect1.top())
        rect.setY(rect1.top());
    else if(rect.y() > rect.bottom())
    {
        rect.setY(rect.bottom());
    }
}




QVariant BoxItem::itemChange(GraphicsItemChange change,
                             const QVariant &value)
{
    if (isDirtyChange(change)){


        // value is the new position.
        QPointF newPos = value.toPointF();
        if (!isPointContains(newPos)) {
            // Keep the item inside the scene rect.
            return newPos;
        }
        m_charge = true;
    }
    else if(change == QGraphicsItem::ItemSelectedChange)
    {
        send_Dirty();
    }

    return QGraphicsRectItem::itemChange(change, value);
}


void BoxItem::keyPressEvent(QKeyEvent *event)
{
    if(m_lock)
        return;
    m_charge = false;
    if (event->modifiers() & Qt::ShiftModifier ||
            event->modifiers() & Qt::ControlModifier) {
        bool move = event->modifiers() & Qt::ControlModifier;
        bool changed = true;
        double dx1 = 0.0;
        double dy1 = 0.0;
        double dx2 = 0.0;
        double dy2 = 0.0;
        switch (event->key()) {
        case Qt::Key_Left:
            if (move)
                dx1 = -1.0;
            dx2 = -1.0;
            break;
        case Qt::Key_Right:
            if (move)
                dx1 = 1.0;
            dx2 = 1.0;
            break;
        case Qt::Key_Up:
            if (move)
                dy1 = -1.0;
            dy2 = -1.0;
            break;
        case Qt::Key_Down:
            if (move)
                dy1 = 1.0;
            dy2 = 1.0;
            break;
        default:
            changed = false;
        }
        if (changed) {
            QRectF rect_1 = rect().adjusted(dx1, dy1, dx2, dy2);
            if(isRectContains(rect_1) ){
                setRect(rect_1);
                m_charge = true;
                event->accept();
            }
            return;
        }
    }
    QGraphicsRectItem::keyPressEvent(event);
}

void BoxItem::keyReleaseEvent(QKeyEvent *)
{
    send_Dirty();
}


void BoxItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{


    if (event->modifiers() & Qt::ShiftModifier) {
        m_resizing = true;
        setCursor(Qt::SizeAllCursor);
    }
    else
        QGraphicsRectItem::mousePressEvent(event);
}


void BoxItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isSelected() || m_lock)//禁止操作
        return;
    if (m_resizing) {
        QRectF rectangle(rect());
        if (event->pos().x() < rectangle.x())
            rectangle.setBottomLeft(event->pos());
        else
            rectangle.setBottomRight(event->pos());
        if(isRectContains(rectangle)){
            m_charge = true;
            setRect(rectangle);
            scene()->update();
        }
    }
    else{
        QGraphicsRectItem::mouseMoveEvent(event);
    }
}


void BoxItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
    }
    else{
        QGraphicsRectItem::mouseReleaseEvent(event);

    }
    send_Dirty();
}


void BoxItem::send_Dirty()
{
    if(m_send)
    {
        return;
    }
    if(m_charge){
        QPointF tPos = this->pos() + this->rect().topLeft();
        QRectF f_rect = this->rect();
        QRect rect(QPoint(tPos.x(),tPos.y()),QSize(f_rect.width(), f_rect.height()));
        int result = MyAlleyway::getInstance()->moveWinInPage(rect,m_id);
        if(result == 0)
        {
            reNew();
        }
        else
        {
            m_lastrect = dm_rect;
            dm_rect = rect;
            emit  dirty();//传递要不要更新信息
        }


    }
    m_charge = false;
}

void BoxItem::initMenu()
{
    QAction *showLock = createMenuAction(&menuEngLish, QIcon(),
                                         "lock", m_lock, true);
    QAction *showLock_1 = createMenuAction(&menuChinese, QIcon(),
                                           QStringLiteral("锁"), m_lock, true);
    connect(showLock, SIGNAL(triggered()),
            this, SLOT(a_setLock()));
    connect(showLock_1, SIGNAL(triggered()),
            this, SLOT(a_setLock()));

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("enlarge"),
                          this,SIGNAL(enlarge()));
    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("reduction"),
                          this,SLOT(reduction()));


    menuChinese.addAction(QIcon("1.png"), QStringLiteral("放大"),
                          this,SIGNAL(enlarge()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("还原"),
                          this,SLOT(reduction()));

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Top floor"),
                          this,SIGNAL(setTopLayer()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("顶层"),
                          this,SIGNAL(setTopLayer()));

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Bottom floor"),
                          this,SIGNAL(setButtonLayer()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("底层"),
                          this,SIGNAL(setButtonLayer()));


    QAction *a_setframe = createMenuAction(&menuEngLish, QIcon(),
                                           "Frame", m_frameOn, true);
    QAction *showframe_1 = createMenuAction(&menuChinese, QIcon(),
                                            QStringLiteral("边框"), m_frameOn, true);


    connect(a_setframe, SIGNAL(triggered()),
            this, SLOT(a_setframe()));
    connect(showframe_1, SIGNAL(triggered()),
            this, SLOT(a_setframe()));
}


void BoxItem::edit()
{

    if(MyAlleyway::getInstance()->isEnglish)
        menuEngLish.exec(QCursor::pos());
    else
        menuChinese.exec(QCursor::pos());

}


void BoxItem::reNew() //拒绝修改
{
    setPos(0,0);
    setRect(dm_rect);
}

void BoxItem::reduction()//还原
{
    resetRect(m_lastrect);//重写
}

QAction *BoxItem::createMenuAction(QMenu *menu, const QIcon &icon,
                                   const QString &text, bool checked,bool isAble, QActionGroup *group,
                                   const QVariant &data)
{
    QAction *action = menu->addAction(icon, text);
    action->setCheckable(isAble);
    action->setChecked(checked);
    if (group)
        group->addAction(action);
    action->setData(data);
    return action;
}

void BoxItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem* i, QWidget* w)
{
    QGraphicsRectItem::paint(painter,i,w);
    if(m_lock){
        painter->setPen(QPen(Qt::black,8,Qt::SolidLine));
        painter->setBrush(Qt::darkGray);
        int m_count = dm_rect.size().width() < dm_rect.size().height() ?
                    dm_rect.size().width() : dm_rect.size().height();
        m_count /= 5;
        QPointF f = this->rect().center();
        f.setX(f.x() - (m_count/2));
        //  f -= QRectF(m_count/2, m_count/2);
        QRectF rectangle(f, QSizeF(m_count*0.9, m_count*0.9));//设定扇形所在的矩形，扇形画在矩形内部
        painter->drawEllipse(rectangle);

        QRectF rect(QPoint(f.x()+m_count, f.y() + m_count/2),QSizeF(m_count*1.5, m_count*0.1) );
        painter->drawRect(rect);
        QRectF rect1(QPoint(f.x()+m_count*1.6, f.y() + m_count/2),QSizeF(m_count*0.05, m_count*0.5) );
        painter->drawRect(rect1);
        QRectF rect2(QPoint(f.x()+m_count*2.1, f.y() + m_count/2),QSizeF(m_count*0.05, m_count*0.5) );
        painter->drawRect(rect2);
    }

    if(m_frameOn)
    {
        //将边框的大小+7便于用户查看，使用100的透明度方便用户查看是否选中
        painter->setPen(QPen(QColor(m_frameRed,m_frameGreen,m_frameBlue,100),m_framSize+7,Qt::SolidLine));
        painter->drawRect(this->rect());
    }
}

void BoxItem::AutoColor()
{
    QColor color = QColor(rand()%200,rand()%240,rand()%240);
    QGraphicsRectItem::setBrush(color.lighter(100));
}


void BoxItem::setFrameColor(int r, int g, int b)
{
    m_frameRed = r;
    m_frameGreen = g;
    m_frameBlue = b;
}


//读写重载
QDataStream &operator<<(QDataStream &out, const BoxItem &boxItem)
{
    out << boxItem.getRect()<<boxItem.lock()<<boxItem.id()
        <<boxItem.in()<<boxItem.frameOn()<<boxItem.frameSize()
       <<boxItem.frameRed()<<boxItem.frameGreen()<<boxItem.frameBlue()
      <<boxItem.zValue();

    return out;
}


QDataStream &operator>>(QDataStream &in, BoxItem &boxItem)
{
    //在初始化的时候被加入
//    QRectF rect;
//    bool lock;
//    int id;
    int in_;

    bool on;
    int size;
    int r;
    int g;
    int b;
    double z;
    in >>in_>>on>>size>>r>>g>>b>>z;
    boxItem.setIn(in_);
    boxItem.setFrameOn(on);
    boxItem.setFrameSize(size);
    boxItem.setFrameColor(r,g,b);
    boxItem.AutoColor();
    boxItem.m_send = false;
    return in;
}


