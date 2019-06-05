#ifndef BOXITEM_HPP
#define BOXITEM_HPP
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


#include <QBrush>
#include <QGraphicsRectItem>
#include <QPen>
#include <QMenu>

class QAction;
class QActionGroup;
class QDataStream;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
//为了防止翻转，在外部设置最小大小

class BoxItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)

    Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(double angle READ angle WRITE setAngle)

    Q_PROPERTY(double shearHorizontal READ shearHorizontal
               WRITE setShearHorizontal)
    Q_PROPERTY(double shearVertical READ shearVertical
               WRITE setShearVertical)


    Q_PROPERTY(QRectF dm_rect READ getRect WRITE resetRect)
    Q_PROPERTY(bool m_lock READ lock WRITE setLock)
    Q_PROPERTY(int m_id READ id WRITE setId)
    Q_PROPERTY(int m_in READ in WRITE setIn)

    Q_PROPERTY(bool m_frameOn READ frameOn WRITE setFrameOn)
    Q_PROPERTY(int m_framSize READ frameSize WRITE setFrameSize)
    Q_PROPERTY(int m_frameRed READ frameRed)
    Q_PROPERTY(int m_frameGreen READ frameGreen)
    Q_PROPERTY(int m_frameBlue READ frameBlue)

public:


    explicit BoxItem(const QRectF &rect, QGraphicsScene *scene,
                     bool lock,int id, bool send = false);
    int type() const { return QGraphicsItem::UserType+1; }

    double angle() const { return m_angle; }
    double shearHorizontal() const { return m_shearHorizontal; }
    double shearVertical() const { return m_shearVertical; }


    QRectF getRect() const{return this->dm_rect;}
    bool lock() const { return m_lock; }
    int id() const { return m_id; }
    int in() const { return m_in; }

    bool frameOn() const {return m_frameOn;}
    int frameSize() const { return m_framSize; }
    int frameRed() const { return m_frameRed; }
    int frameGreen() const { return m_frameGreen; }
    int frameBlue() const { return m_frameBlue; }


    Q_INVOKABLE void setZIn(const int& z){this->setZValue(z);}
    void AutoColor();

signals:
    void dirty();
    void setTopLayer();//修改层次
    void setButtonLayer();
    void enlarge();//放大
public slots:
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAngle(double angle);
    void setShearHorizontal(double shearHorizontal)
    { setShear(shearHorizontal, m_shearVertical); }
    void setShearVertical(double shearVertical)
    { setShear(m_shearHorizontal, shearVertical); }
    void setShear(double shearHorizontal, double shearVertical);

    //修改rect的值
    void resetRect(QRectF rect);
    void setSize(const QSize& size);
    void setLock(const bool& lock){m_lock = lock;}
    void setId(int id){m_id = id;}
    void setIn(const int& in){m_in = in;}

    void setFrameOn(const bool& on){m_frameOn = on;}
    void setFrameSize(const int& size) {m_framSize = size;}
    void setFrameColor(int r, int g, int b);




    void edit();

    void reNew();

    void reduction();//还原


    void a_setLock(){setLock(!m_lock);}
    void a_setframe(){m_frameOn = !m_frameOn;}

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);


    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) { edit(); }
    void contextMenuEvent(QGraphicsSceneContextMenuEvent*) { edit(); }
private:
    void chargeRect(const QRectF& rect);
public:
    bool m_send;
private:
    QAction *createMenuAction(QMenu *menu, const QIcon &icon,
                              const QString &text, bool checked,bool isAble = false,
                              QActionGroup *group=0, const QVariant &data=QVariant());
    void updateTransform();
    bool isPointContains(QPointF&newPos);
    bool isRectContains(const QRectF&rect);
    void adjustRect(QRectF& rect);
    void send_Dirty();
    void initMenu();
    bool m_resizing;
    bool m_charge;
    double m_angle;
    double m_shearHorizontal;
    double m_shearVertical;

private:
    QRectF dm_rect; //x,y,w,h;,只是用于作为快速输出
    QRectF m_lastrect;//记录上次的位置信息
    bool m_lock;//锁住状态
    int m_id;//win编号
    int m_in;
    bool m_frameOn;//边框开关
    int m_framSize;//线框宽度
    int m_frameRed;
    int m_frameGreen;
    int m_frameBlue;


private:
    QMenu menuEngLish;
    QMenu menuChinese;


};


QDataStream &operator<<(QDataStream &out, const BoxItem &boxItem);
QDataStream &operator>>(QDataStream &in, BoxItem &boxItem);


#endif // BOXITEM_HPP
