#include "slidenavigation.h"
#include "slidenavigationplugin.h"

#include <QtPlugin>

SlidenavigationPlugin::SlidenavigationPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void SlidenavigationPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool SlidenavigationPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *SlidenavigationPlugin::createWidget(QWidget *parent)
{
    return new SlideNavigation(parent);
}

QString SlidenavigationPlugin::name() const
{
    return QLatin1String("SlideNavigation");
}

QString SlidenavigationPlugin::group() const
{
    return QLatin1String("");
}

QIcon SlidenavigationPlugin::icon() const
{
    return QIcon(QLatin1String(":/slidenavigation.ico"));
}

QString SlidenavigationPlugin::toolTip() const
{
    return QLatin1String("");
}

QString SlidenavigationPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool SlidenavigationPlugin::isContainer() const
{
    return false;
}

QString SlidenavigationPlugin::domXml() const
{
    return QLatin1String("<widget class=\"SlideNavigation\" name=\"slidenavigation\">\n</widget>\n");
}

QString SlidenavigationPlugin::includeFile() const
{
    return QLatin1String("slidenavigation.h");
}

