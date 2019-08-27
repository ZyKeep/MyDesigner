#include "myalleyway.h"

#include <QColor>
#include <QDebug>
#include <iterator>
#include <assert.h>
int MyPage::m_MAXCount = 0;
QMap<QList<int>, int> MyPage::m_ARRANGE;
#define TMPVALUE 100
#define WINMAXVALUE 64

#define OFFSETX 32
#define OFFSETY 8

MyPage::MyPage()
{
    m_offer_ch = 0;
}

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
    rect.m_out_rect = QRect(0,0,
                            m_page.width()/2,m_page.height()/2);

    rect.m_in_rect = QRect(0,0,1920,1080);
    rect.m_in_rect_t = QRectF(0,0,1,1);
    addToPage(pageId,rect);
}


void MyPage::CreatWinInPage( QRect& rect_,const int& pageId,const int& ch, QRect&all_rect, QRect&in_rect)
{
    MyRect rect;
    rect.m_ch = ch-m_offer_ch;
    rect.m_out_rect = QRect(rect_.x() - m_page.x(), rect_.y() - m_page.y(),
                            rect_.width(), rect_.height());
    //    rect.m_out_rect = rect_;

    double all_w = all_rect.width();
    double all_h = all_rect.height();

    double x =(rect_.x() -  all_rect.x())/all_w;
    double y = (rect_.y() -  all_rect.y())/all_h;
    double w = rect_.width() / all_w;
    double h = rect_.height() / all_h;

    rect.m_in_rect_t = QRectF(x,y,w,h);
    rect.m_in_rect = QRect(x*in_rect.width() + in_rect.x(),y*in_rect.height() + in_rect.y(),
                           w*in_rect.width(),h*in_rect.height());


    addToPage(pageId,rect);
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


bool MyPage::repalceToPage(const int &id, const QRect &rect_, const QRect& all_rect, const QRect& in_rect)
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

    //------------------------------------------生成比例
    double all_w = all_rect.width();
    double all_h = all_rect.height();

    double x =(rect_.x() -  all_rect.x())/all_w;
    double y = (rect_.y() -  all_rect.y())/all_h;
    double w = rect_.width() / all_w;
    double h = rect_.height() / all_h;

    rect.m_in_rect_t = QRectF(x,y,w,h);

    rect.m_in_rect = QRect(x*in_rect.width() + in_rect.x(),y*in_rect.height() + in_rect.y(),
                           w*in_rect.width(),h*in_rect.height());

    rect.m_out_rect.setX(rect_.x() - m_page.x());
    rect.m_out_rect.setY(rect_.y() - m_page.y());
    rect.m_out_rect.setWidth(rect_.width());
    rect.m_out_rect.setHeight(rect_.height());


    addToPage(id, rect);

    return upRepalce;
}

void MyPage::addToPage(const int &id, const MyRect &rect)
{
    m_win[id] = rect;
}

//100存储删除的信息
bool MyPage::delToPage(const int &id)
{
    bool del  = false;
    if(m_win.find(id) != m_win.end())
    {
        del = true;
        m_win[TMPVALUE] = m_win[id];
        m_shift_win[TMPVALUE] = m_shift_win[id];
    }

    if(del){
        m_win.remove(id);
    }
    return del;//删除了1，需要更新
}

void MyPage::delWinInAll(const int& id)
{
    m_shift_win.remove(id);
}

#include "algorithm"
#include "iostream"
using namespace std;


MyAlleyway::MyAlleyway(){

    isSend  = true;
    //创建组

    {
        QList<int> list;
        list<<1<<0;
        MyPage::m_ARRANGE[list] = 0;
    }

    {
        QList<int> list;
        list<<0<<1;
        MyPage::m_ARRANGE[list] = 1;
    }

    //-------------------------------------------------
    {
        QList<int> list;
        list<<3<<2<<1<<0;
        MyPage::m_ARRANGE[list] = 0;
    }

    {
        QList<int> list;
        list<<3<<2<<0<<1;
        MyPage::m_ARRANGE[list] = 1;
    }

    {
        QList<int> list;
        list<<3<<1<<2<<0;
        MyPage::m_ARRANGE[list] = 2;
    }

    {
        QList<int> list;
        list<<3<<1<<0<<2;
        MyPage::m_ARRANGE[list] = 3;
    }

    {
        QList<int> list;
        list<<3<<0<<2<<1;
        MyPage::m_ARRANGE[list] = 4;
    }


    {
        QList<int> list;
        list<<3<<0<<1<<2;
        MyPage::m_ARRANGE[list] = 5;
    }

    {
        QList<int> list;
        list<<2<<3<<1<<0;
        MyPage::m_ARRANGE[list] = 6;
    }

    {
        QList<int> list;
        list<<2<<3<<0<<1;
        MyPage::m_ARRANGE[list] = 7;
    }


    {
        QList<int> list;
        list<<2<<1<<3<<0;
        MyPage::m_ARRANGE[list] = 8;
    }

    {
        QList<int> list;
        list<<2<<1<<0<<3;
        MyPage::m_ARRANGE[list] = 9;
    }


    {
        QList<int> list;
        list<<2<<0<<1<<3;
        MyPage::m_ARRANGE[list] = 10;
    }

    {
        QList<int> list;
        list<<2<<0<<3<<1;
        MyPage::m_ARRANGE[list] = 11;
    }


    {
        QList<int> list;
        list<<1<<2<<3<<0;
        MyPage::m_ARRANGE[list] = 12;
    }

    {
        QList<int> list;
        list<<1<<2<<0<<3;
        MyPage::m_ARRANGE[list] = 13;
    }

    {
        QList<int> list;
        list<<1<<3<<2<<0;
        MyPage::m_ARRANGE[list] = 14;
    }

    {
        QList<int> list;
        list<<1<<3<<0<<2;
        MyPage::m_ARRANGE[list] = 15;
    }

    {
        QList<int> list;
        list<<1<<0<<3<<2;
        MyPage::m_ARRANGE[list] = 16;
    }

    {
        QList<int> list;
        list<<1<<0<<2<<3;
        MyPage::m_ARRANGE[list] = 17;
    }


    {
        QList<int> list;
        list<<0<<1<<2<<3;
        MyPage::m_ARRANGE[list] = 18;
    }
    {
        QList<int> list;
        list<<0<<1<<3<<2;
        MyPage::m_ARRANGE[list] = 19;
    }


    {
        QList<int> list;
        list<<0<<2<<1<<3;
        MyPage::m_ARRANGE[list] = 20;
    }
    {
        QList<int> list;
        list<<0<<2<<3<<1;
        MyPage::m_ARRANGE[list] = 21;
    }


    {
        QList<int> list;
        list<<0<<3<<2<<1;
        MyPage::m_ARRANGE[list] = 22;
    }
    {
        QList<int> list;
        list<<0<<3<<1<<2;
        MyPage::m_ARRANGE[list] = 23;
    }
    empty = 0;

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
    m_page.clear();
}

void MyAlleyway::outMes()
{
    qDebug()<<"------------------check data-------------------";
    qDebug()<<"--- data in add";
    qDebug()<<m_COUNTWIN;
    qDebug()<<m_WIN_layer;
    qDebug()<<"----data in page";
    qDebug()<<MyPage::m_MAXCount;
    for(int i = 0; i < m_MAXPAGE; ++i)
    {
        qDebug()<<m_page[i].m_win.keys();


    }
    qDebug()<<"--------------------end--------------------";

}

//初始化页面数据,请在外部删除数据
void MyAlleyway::init(int* w, int* h,int row, int col,int pageCount)
{
    m_page.clear(); //清除数据
    m_WIN_layer.clear();//删除数据
    //将一块分为几个页面

    m_MAXPAGE = row*col;

    m_row = row;
    m_col = col;

    dm_max_x = 0;
    dm_max_y = 0;

    //每个页面的最大容量
    MyPage::m_MAXCount = pageCount;

    m_INCardCount = (pageCount == 4? 1: 2);

    for(int i = 0; i < m_MAXPAGE/m_INCardCount; ++i)
    {
        m_out_Card.push_back(0);
    }
    //计算页面矩阵
    double j_add = 0;
    double w_add = 0;
    for(int j = 0, k = 0; j < row; ++j)
    {
        w_add = 0;
        for(int i = 0; i < col; ++i)
        {
            m_page.push_back(MyPage());

            if(pageCount == 2 && (k&1) == 1)
            {
                m_page[k].m_offer_ch = 2;
            }
            else
            {
                m_page[k].m_offer_ch = 0;
            }
            m_page[k++].m_page = QRect(QPoint(w_add,j_add), QSize(w[i],h[j]));
            w_add += w[i];
        }
        j_add += h[j];

    }

    dm_max_y = j_add;
    dm_max_x = w_add;



    in_max_x = 1920;
    in_max_y = 1080;

    m_COUNTWIN = 0;//当前窗口数目
}



/*******************************
 * @Introduce: 生成数值的范围，依据输出分辨率
 * @Parameter: x = 宽， y = 高
 * @Result:
 *******************************/
void MyAlleyway::setMaxInValue(int x, int y)
{

    in_max_x = x;
    in_max_y = y;
}


/*******************************
 * @Introduce:依据鼠标点所在的位置在其页面上创建项
 * @Parameter: order = 项的标识， point = 点坐标
 * @Result: order = -1 : 点不在有效区域内， order = -2： 当前选中位置满了
 *******************************/

QRect MyAlleyway::creatPage(int& order,const QPoint &point)
{
    for(int j = 0; j < m_MAXPAGE; ++j)
    {
        if(m_page[j].m_page.contains(point)){

            if(!m_page[j].isFull())
            {
                order = creatId();
                if(order < 0)
                {
                    return QRect(0,0,0,0);
                }
                m_page[j].CreatWinInPage(order);

                upCreatWIN_layer(order);
                return QRect(m_page[j].m_page.x(), m_page[j].m_page.y(),
                             m_page[j].m_page.width()/2,m_page[j].m_page.height()/2);//大小
            }
            order = -2;
            return QRect(0,0,0,0);
        }
    }
    order = -1;
    return QRect(0,0,0,0);
}


void MyAlleyway::creatPage(int& id,  QRect& rect,QRect&  rect_in,int* list)
{
    for(int j = 0; j < m_MAXPAGE; ++j)
    {
        if(m_page[j].m_page.intersects(rect))
        {
            QRect rect_ = m_page[j].m_page & rect;
            m_page[j].CreatWinInPage(rect_,id, list[j],rect,rect_in);
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
    return -2;
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
        m_page[j].delWinInAll(id);
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
        m_page[j].m_shift_win.clear();
    }
    m_WIN_layer.clear();
    m_COUNTWIN = 0;
}

//0表示没有， 1-N, new_ = m_COUNTWIN+1 表示删除
bool MyAlleyway::upWIN_layer(const int &id,int new_)
{
    Q_ASSERT(id < m_WIN_layer.size());

    int older = m_WIN_layer[id];
    m_WIN_layer[id] = new_;//添加

    if(older == new_)
        return false;

    if(older == 0)
    {
        return true;
    }


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

    return true;
}


void MyAlleyway::upPageData()
{

    int k_index = 0;
    for(int i = 0; i < m_MAXPAGE/m_INCardCount; ++i)
    {
        QMap<int,int> layer_ch;
        QMap<int,int> use_ch;
        QMap<int,MyRect>::iterator it;
        for(int ik = 0 ; ik < m_INCardCount; ++ik){ //偏移

            for(it = m_page[k_index].m_win.begin(); it != m_page[k_index].m_win.end(); ++it)
            {
                int layer =  m_WIN_layer[it.key()] + ik*100;
                layer_ch[layer] = it->m_ch + m_page[k_index].m_offer_ch;//添加偏移
            }
            ++k_index; //页面
        }


        if(layer_ch.size()){

            QList<int> list;
            for(int j = 0; j < 4; ++j){
                list<<-1;
                use_ch[j] = 0;
            }

            QMap<int,int>::iterator it_;


            //小的被放到前面了
            for(it_ = layer_ch.begin(); it_ != layer_ch.end(); ++it_)
            {
                int ch = it_.value();
                //      list[star++] = ch;
                use_ch[ch] = -1;
            }
            int star = 4 -1; //最大的在前面
            for(int j = 0; j < 4; ++j)
            {
                if(use_ch[j] != -1)
                {
                    list[star--] = j;
                }
            }


            for(it_ = layer_ch.begin(); it_ != layer_ch.end(); ++it_)
            {
                int ch = it_.value();
                list[star--] = ch;
            }

            m_out_Card[i] = MyPage::m_ARRANGE[list];
        }
        else
            m_out_Card[i] = 0;

    }

    qDebug()<<"out card layer"<<m_out_Card;
    getWinLayer();

}

void MyAlleyway::getWinLayer()
{
    map<int,int> map_win;
    int value;
    for(int i = 0; i < m_WIN_layer.size(); ++i)
    {
        value = m_WIN_layer[i];
        if(value > 0)
        {
            map_win[value] = i;
        }
    }

    QList<int> list;
    map<int,int>::reverse_iterator it;
    for(it = map_win.rbegin(); it != map_win.rend(); ++it)
    {
        list.append(it->second);
    }
    emit s_win_mes(list);
}

//删除
void MyAlleyway::clear()
{
    m_MAXPAGE = 0;
    m_COUNTWIN = 0;
    m_WIN_layer.clear();

}


int MyAlleyway::getLayerById(const int &id)
{
    if(id < m_WIN_layer.size())
    {
        return m_WIN_layer[id];
    }
    return -1;
}


/*******************************
 * @Introduce:移动item后对应数据规则计算函数
 * @Parameter:
 * @Result: -1 表示要移动层次关系， 0 还原， 1移动正常
 *******************************/
int MyAlleyway::moveWinInPage(const QRect& rect,const QRect&rect_in,const int& id)
{

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
                if(m_page[j].repalceToPage(id,rect_,rect,rect_in))
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
                if(find)
                {
                    find = false;
                    stat = m_page[k].m_page.topLeft();
                }
                if(is_w_add)
                {
                    continue;
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

    QMetaObject::invokeMethod(item, "resetRect_in", Qt::DirectConnection,
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
        emit upItemLayer();//更新显示
    upPageData();
    Switch_setting_window(item);
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

    upPageData();
    Switch_setting_window(item);

}


//=========切换器：设置窗口_OL（0A）==================
void MyAlleyway::Switch_setting_window(QObject *item)
{
    if(!isSend)
        return;
    QByteArray list;
    list = Item_Message(item);

    emit S_setting_window(list);
}

//信号
void MyAlleyway::Switch_setting_winSingle(QObject* item)
{
    QByteArray list;
    int id;
    //基础信息
    if (item->property("m_lock").isValid()){

        id = item->property("m_id").value<int>();
        list.append(id);

        list.append(item->property("m_in").value<int>());

    }

    emit S_setting_winSingle(list);
}

/*******************************
 * @Introduce: 获取项的内容
 * @Parameter: 项
 * @Result: 内容： id isopen lock in
 *******************************/
QByteArray MyAlleyway::Item_Message(QObject* item)
{
    QByteArray list;
    int id;
    //基础信息
    if (item->property("m_lock").isValid()){

        id = item->property("m_id").value<int>();
        list.append(id);

        list.append(getLayerById(id) == 0 ? 0 : 1);//isOpen

        list.append(item->property("m_in").value<int>());

        int frameOn = item->property("m_frameOn").value<bool>();
        list.append(frameOn);

        int count;
        list.append(LED_CH(id,count));

        {
            QRectF value = item->property("dm_rect").value<QRectF>();
            list.append(value.x());
            list.append(int(value.x())>>8);

            list.append(value.width());
            list.append(int(value.width())>>8);

            list.append(value.y());
            list.append(int(value.y())>>8);

            list.append(value.height());
            list.append(int(value.height())>>8);

        }

        {
            QRect value = item->property("m_in_rect").value<QRect>();

            list.append(value.x());
            list.append(int(value.x())>>8);

            list.append(value.width());
            list.append(int(value.width())>>8);

            list.append(value.y());
            list.append(int(value.y())>>8);

            list.append(value.height());
            list.append(int(value.height())>>8);
        }


        list.append(LED_Overlay());

        list.append(count);


        {
            QList<int> frame;
            int size = item->property("m_framSize").value<int>();
            size = (size/2) -1;
            frame<<size;
            frame<< item->property("m_frameRed").value<int>();
            frame<< item->property("m_frameGreen").value<int>();
            frame<<item->property("m_frameBlue").value<int>();

            list.append(LED_Item(id,frameOn,frame));
        }

    }
    return list;

}

QByteArray MyAlleyway::Item_Win_Message(QObject* item)
{
    QByteArray list;
    int id;
    //基础信息
    id = item->property("m_id").value<int>();
    list.append(id);

    list.append(getLayerById(id) == 0 ? 0 : 1);//isOpen


    list.append(item->property("m_in").value<int>());

    list.append(int(1));//

    int count;
    list.append(LED_CH(id,count));

    {
        QRectF value = item->property("dm_rect").value<QRectF>();
        list.append(value.x());
        list.append(int(value.x())>>8);

        list.append(value.width());
        list.append(int(value.width())>>8);

        list.append(value.y());
        list.append(int(value.y())>>8);

        list.append(value.height());
        list.append(int(value.height())>>8);

    }


    return list;
}

QByteArray MyAlleyway::Item_In_Message(QObject* item)
{
    QByteArray list;


    {
        QRect value = item->property("m_in_rect").value<QRect>();

        list.append(value.x());
        list.append(int(value.x())>>8);

        list.append(value.width());
        list.append(int(value.width())>>8);

        list.append(value.y());
        list.append(int(value.y())>>8);

        list.append(value.height());
        list.append(int(value.height())>>8);
    }

    return list;
}

QByteArray MyAlleyway::LED_CH(int &id,int& count)
{
    QByteArray list;
    list.resize(16);
    count = 0;
    int index;
    for(int i = 0; i < m_MAXPAGE; ++i)
    {
        index= i/2;

        int tmp = -1;
        if(m_page[i].m_win.find(TMPVALUE) != m_page[i].m_win.end())
        {
            // tmp = TMPVALUE;
            ++count; //被关闭窗口，只修改添加数目
        }
        else if(m_page[i].m_win.find(id) != m_page[i].m_win.end())
        {
            tmp = id;
        }

        if(tmp != -1)
        {
            if(i%2 == 0)
            {
                list[index] = ((m_page[i].m_win[tmp].m_ch+ m_page[i].m_offer_ch) << 4);
            }
            else
            {
                list[index] =  list[index] + (uchar)(m_page[i].m_win[tmp].m_ch + m_page[i].m_offer_ch);
            }
            ++count;
        }
        else
        {
            if(i%2 == 0)
            {
                list[index] = 0xf0;
            }
            else
            {
                list[index] = list[index] +  (uchar)0x0f;
            }
        }
    }

    for(int i  = m_MAXPAGE; i < 32; ++i)
    {
        index= i/2;
        if(i% 2 == 0)
        {
            list[index] = 0xf0;
        }
        else
        {
            list[index] = list[index] + 0x0f;
        }
    }

    return list;
}

QByteArray MyAlleyway::LED_Overlay()
{
    QByteArray list;
    int icount = m_WIN_layer.size();
    for(int i = 0; i < icount; ++i)
    {
        list.append(m_WIN_layer[i]);
    }
    for(int i  = icount; i < WINMAXVALUE; ++i)
    {
        list.append(empty);
    }
    qDebug()<<list;
    return list;
}


QByteArray MyAlleyway::LED_Item(int &id, int &frameOn, QList<int> &frame)
{

    QByteArray list;
    for(int i = 0; i < m_MAXPAGE; ++i)
    {
        int index;
        if(m_page[i].m_win.find(TMPVALUE) != m_page[i].m_win.end())
        {
            index = TMPVALUE;
        }
        else if(m_page[i].m_win.find(id) != m_page[i].m_win.end())
        {
            index = id;
        }
        else
        {
            continue; //没有继续找
        }

        {
            int outCard = i / m_INCardCount;
            list.append(outCard); //输出卡

            int value_ = ((index == TMPVALUE ? 0 : 1)<<7) + ( m_out_Card[outCard]<<2) +
                    (m_page[i].m_win[index].m_ch + m_page[i].m_offer_ch); //哪一个

            list.append(value_);

            MyRect myrect = m_page[i].m_win[index];
            MyShiftRect myshiftrect = m_page[i].m_shift_win[index];


            int x = myrect.m_in_rect.x() + myshiftrect.m_in_rect.x() + OFFSETX;
            list.append(x);
            list.append(x>>8);

            int width = myrect.m_in_rect.width() + myshiftrect.m_in_rect.width();
            list.append(width);
            list.append(width>>8);


            int y = myrect.m_in_rect.y() + myshiftrect.m_in_rect.y() + OFFSETY;
            list.append(y);
            list.append(y>>8);


            int height = myrect.m_in_rect.height() + myshiftrect.m_in_rect.height();
            list.append(height);
            list.append(height>>8);

            //================================================
            x = myrect.m_out_rect.x() + myshiftrect.m_out_rect.x() + OFFSETX;
            list.append(x);
            list.append(x>>8);


            width = myrect.m_out_rect.width() + myshiftrect.m_out_rect.width();
            list.append(width);
            list.append(width>>8);


            y = myrect.m_out_rect.y() + myshiftrect.m_out_rect.y() + OFFSETY;
            list.append(y);
            list.append(y>>8);


            height = myrect.m_out_rect.height() + myshiftrect.m_out_rect.height();
            list.append(height);
            list.append(height>>8);

            //边框计算
            if(frameOn == 0)
            {
                list.append(empty);
            }
            else
            {
                int value = 0x00;
                if(myrect.m_in_rect_t.x() == 0)
                {
                    value |= 1; //左边角
                }

                if(myrect.m_in_rect_t.y() == 0)
                {
                    value |= 4; //上边角
                }

                if(myrect.m_in_rect_t.x() + myrect.m_in_rect_t.width() == 1)
                {
                    value |= 2; //右边角
                }

                if(myrect.m_in_rect_t.y() + myrect.m_in_rect_t.height() == 1)
                {
                    value |= 8; //下边角
                }


                list.append(value);


            }
            for(int i = 0; i < frame.size(); ++i)
            {
                list.append(frame[i]);
            }
        }

        if(index == 100)
        {
            m_page[i].delToPage(100); //用了就删除
        }
    }

    return list;
}

void MyAlleyway::repalceCHValue(QByteArray& array)
{
    for(int i = 0; i < m_MAXPAGE/m_INCardCount; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            array[368*j+23*i] = ( m_out_Card[i]<<2);
        }
    }
}

QList<int> MyAlleyway::Win_Rect(int& id)
{
    QList<int> list;
    for(int i = 0; i < m_MAXPAGE; ++i)
    {

        if(m_page[i].m_win.find(id) != m_page[i].m_win.end())
        {
            list<<(i); //页面
            MyRect myrect = m_page[i].m_win[id];
            list<<myrect.m_ch + m_page[i].m_offer_ch;

            MyShiftRect myshiftrect = m_page[i].m_shift_win[id];

            list<<myrect.m_in_rect.x()+ myshiftrect.m_in_rect.x();
            list<<myrect.m_in_rect.y()+ myshiftrect.m_in_rect.y();
            list<<myrect.m_in_rect.width() + + myshiftrect.m_in_rect.width();
            list<<myrect.m_in_rect.height()+ myshiftrect.m_in_rect.height();

            list<<myrect.m_out_rect.x() + myshiftrect.m_out_rect.x();
            list<<myrect.m_out_rect.y() + myshiftrect.m_out_rect.y();
            list<<myrect.m_out_rect.width() + myshiftrect.m_out_rect.width();
            list<<myrect.m_out_rect.height() + myshiftrect.m_out_rect.height();
        }
    }
    return list;
}

void MyAlleyway::LED_CH_ALL(QObject* item, QByteArray& by)
{
    //基础信息
    if (item->property("m_lock").isValid()){
        int in = item->property("m_in").value<int>();
        int id =   id = item->property("m_id").value<int>();

        for(int i = 0; i < m_MAXPAGE; ++i)
        {
            int index;
            if(m_page[i].m_win.find(id) != m_page[i].m_win.end())
            {
                index = id;
            }
            else
            {
                continue; //没有继续找
            }

            {
                QByteArray list;
                int outCard = i / m_INCardCount;
                int ch =    (m_page[i].m_win[index].m_ch + m_page[i].m_offer_ch);
                qDebug()<<"id = "<<id<< m_out_Card[outCard];

                int value_ = ((index == TMPVALUE ? 0 : 1)<<7) + ( m_out_Card[outCard]<<2); //哪一个

                list.append(value_);

                list.append(in);
                MyRect myrect = m_page[i].m_win[index];
                MyShiftRect myshiftrect = m_page[i].m_shift_win[index];


                int x = myrect.m_in_rect.x()+ myshiftrect.m_in_rect.x() + OFFSETX;
                list.append(x);
                list.append(x>>8);

                int width = myrect.m_in_rect.width() + myshiftrect.m_in_rect.width();
                list.append(width);
                list.append(width>>8);


                int y = myrect.m_in_rect.y() + myshiftrect.m_in_rect.y() + OFFSETY;
                list.append(y);
                list.append(y>>8);


                int height = myrect.m_in_rect.height() + myshiftrect.m_in_rect.height();
                list.append(height);
                list.append(height>>8);


                //================================================
                x = myrect.m_out_rect.x() + myshiftrect.m_out_rect.x() + OFFSETX;
                list.append(x);
                list.append(x>>8);


                width = myrect.m_out_rect.width() + myshiftrect.m_out_rect.width();
                list.append(width);
                list.append(width>>8);


                y = myrect.m_out_rect.y() + myshiftrect.m_out_rect.y() + OFFSETY;
                list.append(y);
                list.append(y>>8);


                height = myrect.m_out_rect.height() + myshiftrect.m_out_rect.height();
                list.append(height);
                list.append(height>>8);

                int frameOn = item->property("m_frameOn").value<bool>();
                //边框计算
                if(frameOn == 0)
                {
                    list.append(empty);
                }
                else
                {

                    int value = 0x00;
                    if(myrect.m_in_rect_t.x() == 0)
                    {
                        value |= 1; //左边角
                    }

                    if(myrect.m_in_rect_t.y() == 0)
                    {
                        value |= 4; //上边角
                    }

                    if(myrect.m_in_rect_t.x() + myrect.m_in_rect_t.width() == 1)
                    {
                        value |= 2; //右边角
                    }

                    if(myrect.m_in_rect_t.y() + myrect.m_in_rect_t.height() == 1)
                    {
                        value |= 8; //下边角
                    }

                    list.append(value);


                }
                uchar c = 0x00;
                for(int ik = 29; ik <= 32; ++ik)
                {
                    list.append(c);
                }

                by.replace(ch*368+outCard*23, 23, list);
            }
        }


    }

}

void MyAlleyway::setTrim(const int&id,const int&led, const QList<int>value)
{
    if(led >=  m_MAXPAGE) //safety inspection
        return;
    if(m_page[led].m_win.find(id) != m_page[led].m_win.end())
    {

        MyRect myrect = m_page[led].m_win[id];
        MyShiftRect myshiftrect = m_page[led].m_shift_win[id];

        myshiftrect.m_in_rect.setX(value[0] - myrect.m_in_rect.x());
        myshiftrect.m_in_rect.setY(value[1] - myrect.m_in_rect.y());
        myshiftrect.m_in_rect.setWidth(value[2] - myrect.m_in_rect.width());
        myshiftrect.m_in_rect.setHeight(value[3] - myrect.m_in_rect.height());

        qDebug()<<"in rect"<<myrect.m_in_rect<<myshiftrect.m_in_rect;

        myshiftrect.m_out_rect.setX(value[4] - myrect.m_out_rect.x());
        myshiftrect.m_out_rect.setY(value[5] - myrect.m_out_rect.y());
        myshiftrect.m_out_rect.setWidth(value[6] - myrect.m_out_rect.width());
        myshiftrect.m_out_rect.setHeight(value[7] - myrect.m_out_rect.height());

        qDebug()<<"out rect"<<myrect.m_out_rect<<myshiftrect.m_out_rect;
        m_page[led].m_shift_win[id] = myshiftrect;//write in data
    }
}


//=======================================================

#include <QDataStream>
QDataStream &operator<<(QDataStream &out, const MyRect &r)
{
    out<<r.m_ch<<r.m_out_rect<<r.m_in_rect<<r.m_in_rect_t;
    return out;
}

QDataStream &operator>>(QDataStream &in, MyRect &r)
{
    in>>r.m_ch>>r.m_out_rect>>r.m_in_rect>>r.m_in_rect_t;
    return in;
}


QDataStream &operator<<(QDataStream &out, const MyPage &page)
{
    out<<page.m_win<<page.m_page<<page.m_offer_ch;
    return out;
}

QDataStream &operator>>(QDataStream &in, MyPage &page)
{
    in>>page.m_win>>page.m_page>>page.m_offer_ch;
    return in;
}



QDataStream &operator<<(QDataStream &out, const MyAlleyway &way)
{
    out<<way.m_COUNTWIN<<way.m_WIN_layer<<way.m_MAXPAGE<<way.m_row
      <<way.m_col<<way.dm_max_x<<way.dm_max_y<<way.in_max_x<<way.in_max_y
     <<way.m_INCardCount;

    out<<way.m_page;
    out<<way.m_out_Card;

    out<<MyPage::m_MAXCount;
    return out;
}

QDataStream &operator>>(QDataStream &in, MyAlleyway &way)
{

    in>>way.m_COUNTWIN>>way.m_WIN_layer>>way.m_MAXPAGE>>way.m_row
            >>way.m_col>>way.dm_max_x>>way.dm_max_y>>way.in_max_x>>way.in_max_y
            >>way.m_INCardCount;

    in>>way.m_page;
    in>>way.m_out_Card;
    in>>MyPage::m_MAXCount;
    way.upPageData(); //更新层次关系
    return in;
}








