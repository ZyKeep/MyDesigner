#include "myalleyway.h"

#include <QColor>
#include <QDebug>
#include <iterator>
#include <assert.h>
int MyPage::m_MAXCount = 0;
QMap<QList<int>, int> MyPage::m_ARRANGE;

bool MyPage::isFull()
{
    return m_win.size() >= m_MAXCount;
}

//false 可以
bool MyPage::isNotAddTo(const int& id)
{
    if(m_win.find(id) != m_win.end())
        return false;
    return isFull();
}

void MyPage::CreatWinInPage(const int& pageId)
{
    int ch = findCH();
    assert(ch != -1);

    MyRect rect;
    rect.m_ch = ch;
    rect.m_rect = QRect(m_page.x(), m_page.y(),
                        m_page.width()/2,m_page.height()/2);

    addToPage(pageId,rect);
}

int MyPage::CreatWinToPage(const int& pageId)
{
    int ch = findCH();
    assert(ch != -1);
    int id = pageId*MyPage::m_MAXCount + ch;
    MyRect rect;
    rect.m_ch = ch;
    rect.m_rect = QRect(m_page.x(), m_page.y(),
                        m_page.width()/2,m_page.height()/2);

    rect.m_in_rect = QRectF(0,0,1,1); //开始的时候是完整的，没有被分屏
    addToPage(id,rect);
    return id;
}

int MyPage::findCH()
{
    QMap<int,MyRect>::iterator it;
    int full = (1<<MyPage::m_MAXCount) - 1;//2进制，每个位代表一个
    for(it = m_win.begin(); it != m_win.end();++it)
    {
        full -= (1<<(it->m_ch));//移除
    }

    for(int i = 0; i < MyPage::m_MAXCount; ++i)
    {
        if((full & (1<<i)) != 0)
        {
            return i;
        }
    }

    return -1;//出现意外
}


bool MyPage::repalceToPage(const int &id, const QRect &rect_, const QRect& all_rect)
{
    int ch;
    bool upRepalce = false;

    if(m_win.find(id) == m_win.end())
    {
        ch = findCH();
        upRepalce = true;
    }
    else
    {
        ch = m_win[id].m_ch;
    }
    MyRect rect;
    rect.m_ch = ch;
    rect.m_rect = rect_;

    //------------------------------------------生成比例
    double all_w = all_rect.width();
    double all_h = all_rect.height();

    double x =(rect_.x() -  all_rect.x())/all_w;
    double y = (rect_.y() -  all_rect.y())/all_h;
    double w = rect_.width() / all_w;
    double h = rect_.height() / all_h;

    rect.m_in_rect = QRectF(x,y,w,h);
  //  qDebug()<<rect.m_in_rect;

    addToPage(id, rect);

    return upRepalce;
}

void MyPage::addToPage(const int &id, const MyRect &rect)
{
    m_win[id] = rect;
}

bool MyPage::delToPage(const int &id)
{
    return (m_win.remove(id) != 0);//删除了1，需要更新
}

#include "algorithm"
#include "iostream"
using namespace std;
void permutation(char* str,int length,int & index)
{
    // sort(str,str+length);
    do
    {
        QList<int> list;
        char s = str[length];
        for(int i=length -1;i >= 0;i--)
            list<<(s- str[i] -1);
     //   qDebug()<<"list = "<<list<<"----"<<index;//kankan

        MyPage::m_ARRANGE[list] = index++;
    }while(next_permutation(str,str+length));

}

MyAlleyway::MyAlleyway(){

    //创建组
    if(MyPage::m_ARRANGE.size() == 0){
        char str[] = "01234";

        int index = 0;
        permutation(str,2,index);
        index = 0;
        permutation(str,4,index);
    }

}


MyAlleyway::MyAlleyway(const MyAlleyway&){

}


MyAlleyway& MyAlleyway::operator=(const MyAlleyway&){
    return *instance;
}


//在此处初始化
MyAlleyway* MyAlleyway::instance = new MyAlleyway();

void MyAlleyway::setLanguage(bool isEngLish)
{
    isEnglish = isEngLish;
}


MyAlleyway::~MyAlleyway()
{
     clearInit();
}

//初始化页面数据
void MyAlleyway::init(int* w, int* h,int row, int col,int pageCount)
{
    //将一块分为几个页面

    m_MAXPAGE = row*col;

    m_row = row;
    m_col = col;
    //计算页面矩阵
    int j_add = 0;
    for(int j = 0, k = 0; j < row; ++j)
    {
        int w_add = 0;
        for(int i = 0; i < col; ++i)
        {
            m_page.push_back(MyPage());

            m_page[k].m_Ch_Overlay = 0;
            m_page[k++].m_page = QRect(QPoint(w_add,j_add), QSize(w[i],h[j]));
            w_add += w[i];
        }
        j_add += h[j];
    }

    dm_max_x = 0;
    for(int i = 0; i< col; ++i)
    {
        dm_max_x += w[i];
    }

    dm_max_y = 0;
    for(int i = 0; i< row; ++i)
    {
        dm_max_y += h[i];
    }

    //每个页面的最大容量
    MyPage::m_MAXCount = pageCount;

    m_COUNTWIN = 0;//当前窗口数目
}

/*******************************
 * @Introduce: 创建页面内容
 * @Parameter:  order： 序号  0-N
 * @Result:
 *******************************/
QRect MyAlleyway::creatPage(int& order)
{
    for(int j = 0; j < m_MAXPAGE; ++j)
    {

        if(!m_page[j].isFull())
        {
            order = m_page[j].CreatWinToPage(j);
            upCreatWIN_layer(order);
            return m_page[j].m_win[order].m_rect;//大小
        }
    }
    order = -1;
    return QRect(0,0,0,0);
}

QRect MyAlleyway::creatPage(int& order,const QPoint &point)
{
    for(int j = 0; j < m_MAXPAGE; ++j)
    {
        if(m_page[j].m_page.contains(point)){
            if(!m_page[j].isFull())
            {
                order = creatId();
                m_page[j].CreatWinInPage(order);

                upCreatWIN_layer(order);
       //         qDebug()<< m_page[j].m_win[order].m_rect;
                return m_page[j].m_win[order].m_rect;//大小
            }
        }
    }
    order = -1;
    return QRect(0,0,0,0);
}

void MyAlleyway::creatPage(int &order, QRectF& rect)
{
    QRect rect_(rect.x(), rect.y(),rect.width(), rect.height());

    for(int j = 0; j < m_MAXPAGE; ++j)
    {
        if(m_page[j].m_page.contains(rect_)){
            if(!m_page[j].isFull())
            {
                m_page[j].CreatWinInPage(order);

                upCreatWIN_layer(order);
            }
        }
    }
}

int MyAlleyway::creatId()
{
    if(m_COUNTWIN == m_WIN_layer.size())
    {
        return m_COUNTWIN;
    }
    else
    {
        for(int i = 0; i < m_WIN_layer.size(); ++i)
            if(m_WIN_layer[i] == 0)
                return i;
    }
    //到此可能出现错误
    return -1;
}

void MyAlleyway::upCreatWIN_layer(const int &id)
{
    ++m_COUNTWIN;
    if(id < m_WIN_layer.size())//对象已经被创建过
    {
        if(upWIN_layer(id,m_COUNTWIN))
            upPageData();
    }
    else
    {
        m_WIN_layer.append(m_COUNTWIN);
        upPageData();
    }

}

void MyAlleyway::upDelWin_layer(const int &id)
{

    for(int j = 0; j < m_MAXPAGE; ++j)
    {
        m_page[j].delToPage(id); //在页面内删除
    }

    upWIN_layer(id,0);
    --m_COUNTWIN; //删除

    upPageData();
}

void MyAlleyway::clearAll_win()
{
    for(int j = 0; j < m_MAXPAGE; ++j)
    {
        m_page[j].m_win.clear();//删除
    }
    m_WIN_layer.clear();
    m_COUNTWIN = 0;
}

//0表示没有， 1-N, new_ = m_COUNTWIN+1 表示删除
bool MyAlleyway::upWIN_layer(const int &id,int new_)
{
    if(id < m_WIN_layer.size())
    {
        int older = m_WIN_layer[id];
        m_WIN_layer[id] = new_;//添加

      //  qDebug()<<id<<' '<<older<<" "<<new_;
        if(older == new_)
            return false;

        if(new_ == 0)
        {
            new_ = m_COUNTWIN;
        }
        int operatorI = older < new_ ? -1 : 1;
        if(new_ < older)
        { //交换
            swap(older,new_);
        }

        for(int i = 0; i < m_WIN_layer.size(); ++i)
        {
            if(m_WIN_layer[i] == 0 || i == id)
            {
                continue;
            }
            if(m_WIN_layer[i] >= older && m_WIN_layer[i]<= new_)
            {
                m_WIN_layer[i] += operatorI;
            }
        }
    }

    qDebug()<<m_WIN_layer;
    return true;
}

void MyAlleyway::upPageData()
{

    for(int i = 0; i < m_MAXPAGE; ++i)
    {
        QMap<int,int> layer_ch;
        QMap<int,MyRect>::iterator it;
     //   qDebug()<<m_page[i].m_win.keys();
        for(it = m_page[i].m_win.begin(); it != m_page[i].m_win.end(); ++it)
        {
            int layer =  m_WIN_layer[it.key()];
            layer_ch[layer] = it->m_ch;
        }


        if(layer_ch.size()){
         //   qDebug()<<"layer_ch = "<<layer_ch;
            QList<int> list;
            for(int j = 0; j < MyPage::m_MAXCount; ++j)
                list<<-1;

            QMap<int,int>::iterator it_;
            int star =  MyPage::m_MAXCount - layer_ch.size();
            for(it_ = layer_ch.begin(); it_ != layer_ch.end(); ++it_)
            {
                list[it_.value()] = star++;
            }
            star = 0;
            for(int j = 0; j < MyPage::m_MAXCount; ++j)
            {
                if(list[j] == -1)
                {
                    list[j] = star++;
                }
            }
            m_page[i].m_Ch_Overlay = MyPage::m_ARRANGE[list];
            qDebug()<<"list = "<<list<<" "<<m_page[i].m_Ch_Overlay;
        }
        m_page[i].m_Ch_Overlay = 0;

    }
}

//删除
void MyAlleyway::clear()
{
    m_MAXPAGE = 0;
    m_COUNTWIN = 0;
    m_WIN_layer.clear();

}

void MyAlleyway::clearInit()
{
    clearAll_win();
    m_page.clear();
    qDebug()<<"clear init";
}


/*******************************
 * @Introduce:移动item后对应数据规则计算函数
 * @Parameter:
 * @Result: -1 表示要移动层次关系， 0 还原， 1移动正常
 *******************************/
int MyAlleyway::moveWinInPage(const QRect& rect,const int& id)
{
    qDebug()<<"move = "<<id;

    //检查能不能添加到页面内
    bool checkIsEmpty = true;
    for(int j = 0; j < m_MAXPAGE; ++j)
    {
        if(m_page[j].m_page.intersects(rect))
        {
            if(m_page[j].isNotAddTo(id))
            {
                checkIsEmpty = false;
                break;
            }
        }
    }
    if(checkIsEmpty)//能添加
    {
        bool upPage = false;
        for(int j = 0; j < m_MAXPAGE; ++j)
        {

            if(m_page[j].m_page.intersects(rect))
            {
                QRect rect_ = m_page[j].m_page & rect;
                if(m_page[j].repalceToPage(id,rect_,rect))
                {
                    upPage = true;
                }
            }
            else
            {
                if(m_page[j].delToPage(id))
                {
                    upPage = true;
                }
            }
        }

        if(upPage)
        {
            upPageData();
        }
    }


    return checkIsEmpty;
}

void MyAlleyway::enlarge()
{
    QObject *item =  dynamic_cast<QObject*>(sender());
    QRectF rect_;
    int id;
    if (item->property("dm_rect").isValid()){
        rect_ = item->property("dm_rect").value<QRectF>();
        id = item->property("id").value<int>();
    }
    else
    {
        return;
    }



    QRect rect(rect_.x(), rect_.y(), rect_.width(), rect_.height());
    //已经是满屏
    if(dm_max_x == rect.width() && dm_max_y == rect.height())
    {
        return;
    }

    //计算放大的范围
    int j_add = 0;
    int w_add = 0;
    bool is_w_add = false;
    QPointF stat;
    bool find = true;
    for(int j = 0, k = 0; j < m_row; ++j)
    {
        int h = 0;
        for(int i = 0; i < m_col; ++i,++k)
        {
            if(m_page[k].m_page.intersects(rect))
            {

                h = m_page[k].m_page.height();
                if(is_w_add)
                {
                    continue;
                }
                if(find)
                {
                    find = false;
                    stat = m_page[k].m_page.topLeft();
                }
                w_add += m_page[k].m_page.width();
            }
        }

        if(h != 0)
        {
            is_w_add  =true;
        }

        j_add += h;
    }



    //自己已经暂满了，全屏
    if(w_add == rect.width() && j_add == rect.height())
    {
        w_add = dm_max_x;
        j_add = dm_max_y;
        stat = QPointF(0,0);
    }

    QRectF newRect(QPointF(stat),QSizeF(w_add, j_add));
    qDebug()<<"newRect = "<<newRect;
    QMetaObject::invokeMethod(item, "resetRect", Qt::DirectConnection,
                              Q_ARG(QRectF, newRect));


}

void MyAlleyway::setTopLayer()//修改层次
{
    QObject *item =  dynamic_cast<QObject*>(sender());
    int id;
    if (item->property("m_id").isValid()){
        id = item->property("m_id").value<int>();
    }
    else
    {
        return;
    }
    int layer = m_COUNTWIN;
    if(upWIN_layer(id,layer))
        emit upItemLayer();
}

void MyAlleyway::setButtonLayer()
{
    QObject *item =  dynamic_cast<QObject*>(sender());
    int id;
    if (item->property("m_id").isValid()){
        id = item->property("m_id").value<int>();
    }
    else
    {
        return;
    }
    int layer = 1;
    if(upWIN_layer(id,layer))
        emit upItemLayer();
}

#include <QDataStream>
QDataStream &operator<<(QDataStream &out, const MyRect &r)
{
    out<<r.m_ch<<r.m_rect<<r.m_in_rect;
    return out;
}

QDataStream &operator>>(QDataStream &in, MyRect &r)
{
    in>>r.m_ch>>r.m_rect>>r.m_in_rect;
    return in;
}


QDataStream &operator<<(QDataStream &out, const MyPage &page)
{
    out<<page.m_Ch_Overlay<<page.m_win;
    return out;
}

QDataStream &operator>>(QDataStream &in, MyPage &page)
{
    in>>page.m_Ch_Overlay>>page.m_win;
    return in;
}

QDataStream &operator<<(QDataStream &out, const MyAlleyway &way)
{
    out<<way.m_COUNTWIN<<way.m_WIN_layer;
    qDebug()<<"out ALLEay = " <<way.m_WIN_layer;
    for(int i = 0; i < way.m_MAXPAGE; ++i)
    {
        out<<way.m_page[i];
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, MyAlleyway &way)
{
    in>>way.m_COUNTWIN>>way.m_WIN_layer;
    qDebug()<<"cin ALLEay = " <<way.m_WIN_layer;
    for(int i = 0; i < way.m_MAXPAGE; ++i)
    {
        in>>way.m_page[i];
    }

    return in;
}





