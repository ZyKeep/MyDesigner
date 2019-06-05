#include "slidenavigationplugin.h"
#include "navlistviewplugin.h"
#include "colorprogressbarplugin.h"
#include "mybuttonbarplugin.h"
#include "animationbuttonplugin.h"
#include "qnavigationwidgetplugin.h"
#include "lightpointplugin.h"
#include "mygraphicsviewplugin.h"
#include "qswitchbuttonplugin.h"
#include "myui.h"

MyUI::MyUI(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new SlidenavigationPlugin(this));
    m_widgets.append(new NavListViewPlugin(this));
    m_widgets.append(new ColorProgressBarPlugin(this));
    m_widgets.append(new MyButtonBarPlugin(this));
    m_widgets.append(new AnimationButtonPlugin(this));
    m_widgets.append(new QNavigationWidgetPlugin(this));
    m_widgets.append(new LightPointPlugin(this));
    m_widgets.append(new MyGraphicsViewPlugin(this));
    m_widgets.append(new QSwitchButtonPlugin(this));

}

QList<QDesignerCustomWidgetInterface*> MyUI::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(myuiplugin, MyUI)
#endif // QT_VERSION < 0x050000
