#include "qnavigationwidget.h"
#include "qnavigationwidgetplugin.h"

#include <QtPlugin>

QNavigationWidgetPlugin::QNavigationWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QNavigationWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QNavigationWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QNavigationWidgetPlugin::createWidget(QWidget *parent)
{
    return new QNavigationWidget(parent);
}

QString QNavigationWidgetPlugin::name() const
{
    return QLatin1String("QNavigationWidget");
}

QString QNavigationWidgetPlugin::group() const
{
    return QLatin1String("");
}

QIcon QNavigationWidgetPlugin::icon() const
{
    return QIcon(QLatin1String(":/qnavigationwidget.ico"));
}

QString QNavigationWidgetPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QNavigationWidgetPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QNavigationWidgetPlugin::isContainer() const
{
    return false;
}

QString QNavigationWidgetPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QNavigationWidget\" name=\"qnavigationwidget\">\n</widget>\n");
}

QString QNavigationWidgetPlugin::includeFile() const
{
    return QLatin1String("qnavigationwidget.h");
}

