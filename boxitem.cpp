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

BoxItem::BoxItem(const QRectF &rect_, const QRect &rect_in,QGraphicsScene *scene,
                 bool lock,int id,int in, bool frame)
    : QObject(), QGraphicsRectItem(),m_in(in),m_in_rect(rect_in),
      m_resizing(false),m_lock(lock),m_id(id),
      m_frameOn(frame),m_framSize(4),m_frameRed(0),m_frameGreen(0),m_frameBlue(0),
      m_angle(0.0), m_shearHorizontal(0.0), m_shearVertical(0.0)
{

    setFlags(QGraphicsItem::ItemIsSelectable|
         #if QT_VERSION >= 0x040600
             QGraphicsItem::ItemSendsGeometryChanges|
         #endif
             QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsFocusable);

    QString str = "";
    if(m_in > 31)
    {
        str = tr("_HDMI%1").arg((m_in-32)%4 + 1); //HDMI1 - 4
    }
    singleName = tr("In%1%2").arg(m_in > 31? (m_in - 32)/2 + 1: m_in+1).arg(str);

    setPos(0,0);
    setRect(rect_);
    dm_rect = rect_;
    m_lastrect = dm_rect;

    // m_in_rect = QRect(0,0,1920,1080);

    scene->addItem(this);
    setFocus();
    setSelected(true);
    m_charge = false;
    AutoColor();
    initMenu();

    setAcceptDrops(true);
    setAcceptHoverEvents(true);


}

BoxItem::~BoxItem()
{

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
    if(m_lock || !isSelected())
        return;

    resetRect_in(rect);

}

void BoxItem::resetRect_in(QRectF rect)
{
    setPos(0,0);
    setRect(rect);

    m_charge = true;
    send_Dirty();
}

void BoxItem::resetRectIn_in(QRect rect)
{
    setInRect(rect);
    m_charge = true;
    send_Dirty();
}

void BoxItem::setIn(const int& in){
    if(m_lock ||!isSelected())
        return;
    m_in = in;
}


void BoxItem::setFrameOn(const bool& on){
    if(m_lock ||!isSelected())
        return;
    m_frameOn = on;
    MyAlleyway::getInstance()->Switch_setting_window(dynamic_cast<QObject*>(this));


}
void BoxItem::setFrameSize(const int& size) {
    if(m_lock ||!isSelected())
        return;
    m_framSize = size;
}

void BoxItem::setFrameColor(int r, int g, int b)
{
    if(m_lock ||!isSelected())
        return;
    m_frameRed = r;
    m_frameGreen = g;
    m_frameBlue = b;
}



void BoxItem::setFrame(bool on, int size, int r,int g,int b)
{
    m_frameOn = on;
    m_framSize = size;
    m_frameRed = r;
    m_frameGreen = g;
    m_frameBlue = b;
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
    if(rect.width() == 0)
    {
        rect.setWidth(0.1);
    }
    if(rect.height() == 0)
    {
        rect.setHeight(0.1);
    }

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
    if(rect1.width() == 0)
    {
        rect1.setWidth(0.1);
    }
    if(rect1.height() == 0)
    {
        rect1.setHeight(0.1);
    }
    qDebug()<<"rect1 = "<<rect1;

    //大小改变后边界判断以及大小限定
    if (!rect1.contains(tPos) ||rect1.width() < 0 || rect1.height() < 0|| (rect.width() < 50 || rect.height() < 50)) {
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


void BoxItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(this->isSelected()){
        p_move = event->scenePos() - dm_rect.topLeft(); //值不对

        int maxHeight = dm_rect.height();
        int maxWidth = dm_rect.width();

        int top = dm_rect.height()/15;
        int left = dm_rect.width()/15;
        int buttom = maxHeight - top;
        int right = maxWidth - left;

        if(p_move.x() < left && p_move.y() < top && p_move.x() > 0 && p_move.y() > 0)
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else if(p_move.x() > right && p_move.y() < top && p_move.x() < maxWidth && p_move.y() > 0)
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        else  if(p_move.x() < left &&  p_move.y() > buttom && p_move.x() > 0 && p_move.y() < maxHeight)
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        else if(p_move.x() > right && p_move.y() > buttom && p_move.x() < maxWidth && p_move.y() < maxHeight)
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else if(p_move.y() < top && p_move.y() > 0)
        {
            setCursor(Qt::SizeVerCursor);
        }
        else if(p_move.y() > buttom && p_move.y() < maxHeight)
        {
            setCursor(Qt::SizeVerCursor);
        }
        else if(p_move.x() <   left && p_move.x() > 0)
        {
            setCursor(Qt::SizeHorCursor);
        }
        else if(p_move.x() > right && p_move.x() < maxWidth)
        {
            setCursor(Qt::SizeHorCursor);
        }
        else if(dm_rect.contains(event->scenePos()))
        {
            setCursor(Qt::OpenHandCursor);
        }
        else
        {
            setCursor(Qt::ArrowCursor);
        }
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }

}

void BoxItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    m_resizing = false;
    if(this->isSelected()){
        p_move = event->scenePos() - dm_rect.topLeft(); //值不对

        int maxHeight = dm_rect.height();
        int maxWidth = dm_rect.width();

        int top = dm_rect.height()/15;
        int left = dm_rect.width()/15;
        int buttom = maxHeight - top;
        int right = maxWidth - left;

        if(p_move.x() < left && p_move.y() < top && p_move.x() > 0 && p_move.y() > 0)
        {
            moveType = 1;
            m_resizing = true;
        }
        else if(p_move.x() > right && p_move.y() < top && p_move.x() < maxWidth && p_move.y() > 0)
        {
            moveType = 2;
            m_resizing = true;
        }
        else  if(p_move.x() < left &&  p_move.y() > buttom && p_move.x() > 0 && p_move.y() < maxHeight)
        {
            moveType = 3;
            m_resizing = true;
        }
        else if(p_move.x() > right && p_move.y() > buttom && p_move.x() < maxWidth && p_move.y() < maxHeight)
        {
            moveType = 4;
            m_resizing = true;
        }
        else if(p_move.y() < top && p_move.y() > 0)
        {
            moveType = 5;
            m_resizing = true;
        }
        else if(p_move.y() > buttom && p_move.y() < maxHeight)
        {
            moveType = 6;
            m_resizing = true;
        }
        else if(p_move.x() <   left && p_move.x() > 0)
        {
            moveType = 7;
            m_resizing = true;
        }
        else if(p_move.x() > right && p_move.x() < maxWidth)
        {
            moveType = 8;
            m_resizing = true;
        }
        else
        {
            m_resizing = false;
            moveType = -1;
            QGraphicsRectItem::mousePressEvent(event);
        }
    }
    else
    {
        m_resizing = false;
        moveType = -1;
        QGraphicsRectItem::mousePressEvent(event);
    }


}


void BoxItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isSelected() || m_lock)//禁止操作
        return;
    if (m_resizing) {
        QRectF rectangle(rect());
        if(moveType == 1)
        {
            rectangle.setTopLeft(event->pos());
        }
        else if(moveType == 2)
        {
            rectangle.setTopRight(event->pos());
        }
        else  if(moveType == 3)
        {
            rectangle.setBottomLeft(event->pos());
        }
        else if(moveType == 4)
        {
            rectangle.setBottomRight(event->pos());
        }
        else if(moveType == 5)
        {
            rectangle.setTop(event->pos().y());
        }
        else  if(moveType == 6)
        {
            rectangle.setBottom(event->pos().y());
        }
        else if(moveType == 7)
        {
            rectangle.setLeft(event->pos().x());
        }
        else if(moveType == 8)
        {
            rectangle.setRight(event->pos().x());
        }

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
    if(m_charge){
        QPointF tPos = this->pos() + this->rect().topLeft();
        QRectF f_rect = this->rect();
        QRect rect(QPoint(tPos.x(),tPos.y()),QSize(f_rect.width(), f_rect.height()));
        int result = MyAlleyway::getInstance()->moveWinInPage(rect,m_in_rect,m_id);
        if(result == 0)
        {
            reNew();
        }
        else
        {
            m_lastrect = dm_rect;
            dm_rect = rect;
            MyAlleyway::getInstance()->Switch_setting_window(dynamic_cast<QObject*>(this));
        }

    }
    m_charge = false;
}

void BoxItem::initMenu()
{

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Fine-tuning Data"),
                          this,SLOT(setfine_tuning_data()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("微调数据"),
                          this,SLOT(setfine_tuning_data()));


    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Enlarge"),
                          this,SLOT(renlarge()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("放大"),
                          this,SLOT(renlarge()));

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("reduction"),
                          this,SLOT(reduction()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("还原"),
                          this,SLOT(reduction()));

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Top floor"),
                          this,SLOT(rsetTopLayer()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("顶层"),
                          this,SLOT(rsetTopLayer()));

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Bottom floor"),
                          this,SLOT(rsetButtonLayer()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("底层"),
                          this,SLOT(rsetButtonLayer()));


    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Frame"),
                          this,SLOT(a_setframe()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("边框"),
                          this,SLOT(a_setframe()));

    menuEngLish.addAction(QIcon("1.png"), QStringLiteral("Fine-tuning"),
                          this,SLOT(setfine_tuning()));
    menuChinese.addAction(QIcon("1.png"), QStringLiteral("微调"),
                          this,SLOT(setfine_tuning()));

}


void BoxItem::edit()
{
    this->setSelected(true);
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
    if(m_lock)
        return;
    resetRect_in(m_lastrect);//重写

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
#include <QTextCursor>
void BoxItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem* i, QWidget* w)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(w);

    QGraphicsRectItem::paint(painter,i,w);
    if (i->state & QStyle::State_Selected)
        op.state = QStyle::State_None;

    // 选中时绘制
    if (i->state & QStyle::State_Selected) {
        qreal itemPenWidth = pen().widthF();
        const qreal pad = itemPenWidth / 2;
        const qreal penWidth = 7;
        // 边框区域颜色
        // 绘制实线
        painter->setPen(QPen(QColor(Qt::yellow), penWidth, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));
        // 绘制虚线
        painter->setPen(QPen(QColor(Qt::red), penWidth, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));
    }
    else if(m_frameOn)
    {
        //将边框的大小+7便于用户查看，使用100的透明度方便用户查看是否选中

        painter->setPen(QPen(QColor(m_frameRed,m_frameGreen,m_frameBlue),m_framSize+7,Qt::SolidLine));
        painter->drawRect(this->rect());
    }

    if(m_lock){
        painter->setPen(QPen(Qt::black,8,Qt::SolidLine));
        painter->setBrush(Qt::black);
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

    {
        QPen pen(Qt::black);
        int pSize = MyAlleyway::getInstance()->penSize > 9 ?  MyAlleyway::getInstance()->penSize : 10;
        painter->setFont(QFont("宋体",pSize,QFont::Bold));
        painter->setPen(pen);

        QPointF pos = this->rect().topLeft() + QPointF(10,20);
        QString str = "";
        if(m_in > 31)
        {
            str = tr("_HDMI%1").arg((m_in-32)%4 + 1); //HDMI1 - 4
        }
        singleName = tr("In%1%2").arg(m_in > 31? (m_in - 32)/2 + 1: m_in+1).arg(str);

        painter->drawText(pos.x(),pos.y(),this->rect().width(), this->rect().height(),Qt::AlignLeft,tr("Win:[%1]\r\nX_Y_W_H:[%2,%3,%4,%5]\r\nSource:%6").arg(m_id+1)
                          .arg((int)(this->pos().x()+this->rect().x()))
                          .arg((int)(this->pos().y()+this->rect().y())).arg((int)this->rect().width()).arg((int)this->rect().height()).arg(singleName));


    }

    this->scene()->update();

}

void BoxItem::AutoColor()
{
    int red,green,blue;

    do{
        red = rand()%120;
        green = rand()%120;
        blue = rand()%120;
    }while(red>100 && green > 100 && blue > 100);

    QColor color = QColor(red+100,green+100,blue+100);
    QGraphicsRectItem::setBrush(color);
}


void BoxItem::a_setframe()
{
    if(!m_lock){
        m_frameOn = !m_frameOn;
        MyAlleyway::getInstance()->Switch_setting_window(dynamic_cast<QObject*>(this));
        emit updata_page();
    }
}
void BoxItem::setfine_tuning()
{
    if(!m_lock)
        emit fine_tuning(m_id);
}

void BoxItem::setfine_tuning_data()
{
    if(!m_lock){
        QRectF rect = scene()->sceneRect();
        QList<int> list;
        list<<m_id
           <<dm_rect.x()<<dm_rect.y()<<dm_rect.width()<<dm_rect.height()
          <<rect.width()<<rect.height()
         <<m_in_rect.x()<<m_in_rect.y()<<m_in_rect.width()<<m_in_rect.height();
        emit fine_tuning_data(list);
    }
}

#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>


void BoxItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(!m_lock){
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void BoxItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(!m_lock)
    {
        QByteArray value = event->mimeData()->data("application/tree");
        m_in = value[0];
        MyAlleyway::getInstance()->Switch_setting_winSingle(dynamic_cast<QObject*>(this));
        emit updata_page();
    }
    event->accept();
}


//读写重载
QDataStream &operator<<(QDataStream &out, const BoxItem &boxItem)
{
    //  bool lock = false;
    out << boxItem.getRect()<<boxItem.getInRect()<<boxItem.lock()<<boxItem.id()
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

    double z;
    in >>boxItem.m_in>>boxItem.m_frameOn>>boxItem.m_framSize
            >>boxItem.m_frameRed>>boxItem.m_frameGreen>>boxItem.m_frameBlue>>z;
    boxItem.setZValue(z);
    boxItem.AutoColor();

    QString str = "";
    if(boxItem.m_in > 31)
    {
        str = QObject::tr("_HDMI%1").arg((boxItem.m_in-32)%4 + 1); //HDMI1 - 4
    }
    boxItem.singleName = QObject::tr("In%1%2").arg(boxItem.m_in > 31? (boxItem.m_in - 32)/2 + 1: boxItem.m_in+1).arg(str);

    return in;
}


