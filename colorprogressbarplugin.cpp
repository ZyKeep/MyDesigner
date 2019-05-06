#include "colorprogressbar.h"
#include "colorprogressbarplugin.h"

#include <QtPlugin>

ColorProgressBarPlugin::ColorProgressBarPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void ColorProgressBarPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ColorProgressBarPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ColorProgressBarPlugin::createWidget(QWidget *parent)
{
    return new ColorProgressBar(parent);
}

QString ColorProgressBarPlugin::name() const
{
    return QLatin1String("ColorProgressBar");
}

QString ColorProgressBarPlugin::group() const
{
    return QLatin1String("");
}

QIcon ColorProgressBarPlugin::icon() const
{
    return QIcon(QLatin1String(":/colorprogressbar.ico"));
}

QString ColorProgressBarPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ColorProgressBarPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ColorProgressBarPlugin::isContainer() const
{
    return false;
}

QString ColorProgressBarPlugin::domXml() const
{
    return QLatin1String("<widget class=\"ColorProgressBar\" name=\"colorprogressbar\">\n</widget>\n");
}

QString ColorProgressBarPlugin::includeFile() const
{
    return QLatin1String("colorprogressbar.h");
}

