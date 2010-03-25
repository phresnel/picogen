//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#include "spectralcolorpicker.hh"
#include "spectralcolorpickerplugin.hh"

#include <QtCore/QtPlugin>

SpectralColorPickerPlugin::SpectralColorPickerPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void SpectralColorPickerPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool SpectralColorPickerPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *SpectralColorPickerPlugin::createWidget(QWidget *parent)
{
    return new SpectralColorPicker(parent);
}

QString SpectralColorPickerPlugin::name() const
{
    return QLatin1String("SpectralColorPicker");
}

QString SpectralColorPickerPlugin::group() const
{
    return QLatin1String("");
}

QIcon SpectralColorPickerPlugin::icon() const
{
    return QIcon();
}

QString SpectralColorPickerPlugin::toolTip() const
{
    return QLatin1String("");
}

QString SpectralColorPickerPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool SpectralColorPickerPlugin::isContainer() const
{
    return false;
}

QString SpectralColorPickerPlugin::domXml() const
{
    return QLatin1String("<widget class=\"SpectralColorPicker\" name=\"spectralColorPicker\">\n</widget>\n");
}

QString SpectralColorPickerPlugin::includeFile() const
{
    return QLatin1String("spectralcolorpicker.hh");
}

Q_EXPORT_PLUGIN2(spectralcolorpickerplugin, SpectralColorPickerPlugin)
