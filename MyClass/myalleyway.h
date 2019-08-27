#ifndef MYALLEYWAY_H
#define MYALLEYWAY_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QRect>

class QPoint;
class MyRect
{
public:
    int m_ch;//图层   0-3
    QRect m_out_rect;//x,y,w,h
    QRect m_in_rect;//输入值
    QRectF m_in_rect_t; //占比

};

class MyShiftRect
{
public:
    QRect m_out_rect;
    QRect m_in_rect;
};

QDataStream &operator<<(QDataStream &out, const MyRect &r);
QDataStream &operator>>(QDataStream &in, MyRect &r);



class MyPage
{
public:
    MyPage();
    bool isFull();
    bool isNotAddTo(const int&id);

    void CreatWinInPage(const int& pageId);
    void CreatWinInPage( QRect& rect, const int& pageId,const int& ch, QRect&rect_all, QRect&rect_in);

    int findCH();
    bool repalceToPage(const int&id, const QRect& rect, const QRect& all_rect, const QRect& in_rect);
    void addToPage(const int&id, const MyRect& rect);
    bool delToPage(const int&id);

    void delWinInAll(const int& id);

public:
    QRect m_page;//页面的大小位置

    QMap<int,MyRect> m_win;//第一个参数是id，第二个参数是将窗口切成矩阵在本类中
    QMap<int,MyShiftRect> m_shift_win; //偏移量

    int m_offer_ch;

    static int m_MAXCount;//最大个数
    static QMap<QList<int>, int> m_ARRANGE;//排列, 自动生成
};

QDataStream &operator<<(QDataStream &out, const MyPage &page);
QDataStream &operator>>(QDataStream &in, MyPage &page);


class MyAlleyway : public QObject
{
    Q_OBJECT
public:
    static MyAlleyway* getInstance()
    {
        return instance;
    }

    void outMes();
    void setLanguage(bool isEngLish);
    //初始化页面
    void init(int* w, int* h,int row, int col,int pageCount = 2);

    void setMaxInValue(int x, int y);

    void upCreatWIN_layer(const int& id);
    void upDelWin_layer(const int& id);
    void clearAll_win();

    int creatId();


    int  moveWinInPage(const QRect& rect,const QRect&rect_in,const int& id);

    void upPageData();

    QRect creatPage(int& order,const QPoint &point);//创建页面
    void creatPage(int& id,  QRect& rect, QRect& rect_in,int*list);

    void clear();

    int getLayerById(const int& id);

    void UpLayerByUser() {emit upItemLayer();}

    //================================================
    void Switch_setting_window(QObject* item);
    void Switch_setting_winSingle(QObject* item);


    QByteArray Item_Message(QObject* item);

    QByteArray Item_Win_Message(QObject* item);
     QByteArray Item_In_Message(QObject* item);

    QByteArray LED_CH(int& id,int& count);
    QByteArray LED_Overlay();
    QByteArray LED_Item(int & id, int& frameOn, QList<int>& frame);

    void repalceCHValue(QByteArray& array);

    void LED_CH_ALL(QObject* item, QByteArray& by);

    QList<int> Win_Rect(int& id);
    void setTrim(const int&id,const int&led, const QList<int>value);

    void reWin_layer();

    void getWinLayer();

private:
    bool upWIN_layer(const int& id,  int new_);//更新窗口层次数组
private:
    MyAlleyway();
    ~MyAlleyway();
    //把复制构造函数和=操作符也设为私有,防止被复制
    MyAlleyway(const MyAlleyway&);
    MyAlleyway& operator=(const MyAlleyway&);

    static MyAlleyway* instance;

signals:
    void upItemLayer();
    void S_setting_window(const QByteArray);
    void S_setting_winSingle(const QByteArray);

     void s_win_mes(const QList<int>);

public slots:
    void enlarge();
    void setTopLayer();//修改层次
    void setButtonLayer();
public:
    bool isEnglish;


public:
    bool isSend;


    int m_MAXPAGE;//最大页数

    int m_row;
    int m_col;

    double dm_max_x;
    double dm_max_y;

    int in_max_x;//输入的x
    int in_max_y;//输入的y
    int m_INCardCount;//输入卡计算



    int m_COUNTWIN;//窗口数目

    unsigned char empty;


    QVector<MyPage> m_page; //页面
    QVector<int> m_WIN_layer;//窗口的层次,下标表示win id， 内容是layer, 0代表没有被使用
    QVector<int> m_out_Card;

    double penSize; //缩放因子的大小
};


QDataStream &operator<<(QDataStream &out, const MyAlleyway &way);
QDataStream &operator>>(QDataStream &in, MyAlleyway &way);

#endif // MYALLEYWAY_H
