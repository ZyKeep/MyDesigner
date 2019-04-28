#include "slidenavigationplugin.h"
#include "treenavigationbarplugin.h"
#include "myui.h"

MyUI::MyUI(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new SlidenavigationPlugin(this));
    m_widgets.append(new TreeNavigationBarPlugin(this));

}

QList<QDesignerCustomWidgetInterface*> MyUI::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(myuiplugin, MyUI)
#endif // QT_VERSION < 0x050000
