#include "treenavigationbar.h"
#include "treenavigationbarplugin.h"

#include <QtPlugin>

TreeNavigationBarPlugin::TreeNavigationBarPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void TreeNavigationBarPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool TreeNavigationBarPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *TreeNavigationBarPlugin::createWidget(QWidget *parent)
{
    return new TreeNavigationBar(parent);
}

QString TreeNavigationBarPlugin::name() const
{
    return QLatin1String("TreeNavigationBar");
}

QString TreeNavigationBarPlugin::group() const
{
    return QLatin1String("");
}

QIcon TreeNavigationBarPlugin::icon() const
{
    return QIcon();
}

QString TreeNavigationBarPlugin::toolTip() const
{
    return QLatin1String("");
}

QString TreeNavigationBarPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool TreeNavigationBarPlugin::isContainer() const
{
    return false;
}

QString TreeNavigationBarPlugin::domXml() const
{
    return QLatin1String("<widget class=\"TreeNavigationBar\" name=\"treeNavigationBar\">\n</widget>\n");
}

QString TreeNavigationBarPlugin::includeFile() const
{
    return QLatin1String("treenavigationbar.h");
}

