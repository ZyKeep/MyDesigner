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
    QRect m_rect;//x,y,w,h
    QRectF m_in_rect;//存储占用比例， x，w * width  y。h * height

};
QDataStream &operator<<(QDataStream &out, const MyRect &r);
QDataStream &operator>>(QDataStream &in, MyRect &r);



class MyPage
{
public:
    bool isFull();
    bool isNotAddTo(const int&id);
    int CreatWinToPage(const int& pageId);//返回id
    void CreatWinInPage(const int& pageId);

    int findCH();
    bool repalceToPage(const int&id, const QRect& rect, const QRect& all_rect);
    void addToPage(const int&id, const MyRect& rect);
    bool delToPage(const int&id);

public:
    QRect m_page;//页面的大小位置

    QMap<int,MyRect> m_win;//第一个参数是id，第二个参数是将窗口切成矩阵在本类中
    int m_Ch_Overlay;//图层占用的位置排列


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

    void setLanguage(bool isEngLish);
    //初始化页面
    void init(int* w, int* h,int row, int col,int pageCount);


    void upCreatWIN_layer(const int& id);
    void upDelWin_layer(const int& id);
    void clearAll_win();

    int creatId();


    int  moveWinInPage(const QRect& rect,const int& id);

    void upPageData();

    QRect creatPage(int& order);//创建页面
    QRect creatPage(int& order,const QPoint &point);//创建页面
    void creatPage(int &order, QRectF& rect);

    void clear();
    void clearInit();
    int getLayerById(const int& id){return m_WIN_layer[id];}

    void UpLayerByUser() {emit upItemLayer();}


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
public slots:
    void enlarge();
    void setTopLayer();//修改层次
    void setButtonLayer();
public:
    bool isEnglish;


public:
    QVector<MyPage> m_page;

    int m_MAXPAGE;//最大页数

    int m_row;
    int m_col;

    int dm_max_x;
    int dm_max_y;

    int m_COUNTWIN;//窗口数目
    QVector<int> m_WIN_layer;//窗口的层次,下标表示win id， 内容是layer, 0代表没有被使用

};


QDataStream &operator<<(QDataStream &out, const MyAlleyway &way);
QDataStream &operator>>(QDataStream &in, MyAlleyway &way);

#endif // MYALLEYWAY_H
