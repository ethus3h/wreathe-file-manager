/***************************************************************************
 *   Copyright (C) 2006 by Peter Penz <peter.penz19@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#include "detailsviewsettingspage.h"

#include "iconsizegroupbox.h"
#include "dolphinfontrequester.h"
#include "dolphin_detailsmodesettings.h"

#include <KDialog>
#include <KLocale>

#include <settings/dolphinsettings.h>

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>

#include <views/zoomlevelinfo.h>

DetailsViewSettingsPage::DetailsViewSettingsPage(QWidget* parent) :
    ViewSettingsPageBase(parent),
    m_iconSizeGroupBox(0),
    m_fontRequester(0),
    m_expandableFolders(0)
{
    const int spacing = KDialog::spacingHint();
    const int margin = KDialog::marginHint();
    const QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    setSpacing(spacing);
    setMargin(margin);

    // Create "Icon" properties
    m_iconSizeGroupBox = new IconSizeGroupBox(this);
    m_iconSizeGroupBox->setSizePolicy(sizePolicy);

    const int min = ZoomLevelInfo::minimumLevel();
    const int max = ZoomLevelInfo::maximumLevel();
    m_iconSizeGroupBox->setDefaultSizeRange(min, max);
    m_iconSizeGroupBox->setPreviewSizeRange(min, max);

    // create "Text" properties
    QWidget* textGroup = new QGroupBox(i18nc("@title:group", "Text"), this);
    textGroup->setSizePolicy(sizePolicy);

    QLabel* fontLabel = new QLabel(i18nc("@label:listbox", "Font:"), textGroup);
    m_fontRequester = new DolphinFontRequester(textGroup);

    QHBoxLayout* textLayout = new QHBoxLayout(textGroup);
    textLayout->addWidget(fontLabel, 0, Qt::AlignRight);
    textLayout->addWidget(m_fontRequester);

    // create "Expandable Folders" checkbox
    m_expandableFolders = new QCheckBox(i18nc("@option:check", "Expandable folders"), this);

    // Add a dummy widget with no restriction regarding
    // a vertical resizing. This assures that the dialog layout
    // is not stretched vertically.
    new QWidget(this);

    loadSettings();

    connect(m_iconSizeGroupBox, SIGNAL(defaultSizeChanged(int)), this, SIGNAL(changed()));
    connect(m_iconSizeGroupBox, SIGNAL(previewSizeChanged(int)), this, SIGNAL(changed()));
    connect(m_fontRequester, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(m_expandableFolders, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
}

DetailsViewSettingsPage::~DetailsViewSettingsPage()
{
}

void DetailsViewSettingsPage::applySettings()
{
    DetailsModeSettings* settings = DolphinSettings::instance().detailsModeSettings();

    const int iconSize = ZoomLevelInfo::iconSizeForZoomLevel(m_iconSizeGroupBox->defaultSizeValue());
    const int previewSize = ZoomLevelInfo::iconSizeForZoomLevel(m_iconSizeGroupBox->previewSizeValue());
    settings->setIconSize(iconSize);
    settings->setPreviewSize(previewSize);

    const QFont font = m_fontRequester->font();
    settings->setUseSystemFont(m_fontRequester->mode() == DolphinFontRequester::SystemFont);
    settings->setFontFamily(font.family());
    settings->setFontSize(font.pointSizeF());
    settings->setItalicFont(font.italic());
    settings->setFontWeight(font.weight());

    settings->setExpandableFolders(m_expandableFolders->isChecked());

    settings->writeConfig();
}

void DetailsViewSettingsPage::restoreDefaults()
{
    DetailsModeSettings* settings = DolphinSettings::instance().detailsModeSettings();
    settings->useDefaults(true);
    loadSettings();
    settings->useDefaults(false);
}

void DetailsViewSettingsPage::loadSettings()
{
    DetailsModeSettings* settings = DolphinSettings::instance().detailsModeSettings();

    const QSize iconSize(settings->iconSize(), settings->iconSize());
    const int iconSizeValue = ZoomLevelInfo::zoomLevelForIconSize(iconSize);
    m_iconSizeGroupBox->setDefaultSizeValue(iconSizeValue);

    const QSize previewSize(settings->previewSize(), settings->previewSize());
    const int previewSizeValue = ZoomLevelInfo::zoomLevelForIconSize(previewSize);
    m_iconSizeGroupBox->setPreviewSizeValue(previewSizeValue);

    if (settings->useSystemFont()) {
        m_fontRequester->setMode(DolphinFontRequester::SystemFont);
    } else {
        QFont font(settings->fontFamily(),
                   qRound(settings->fontSize()));
        font.setItalic(settings->italicFont());
        font.setWeight(settings->fontWeight());
        font.setPointSizeF(settings->fontSize());
        m_fontRequester->setMode(DolphinFontRequester::CustomFont);
        m_fontRequester->setCustomFont(font);
    }

    m_expandableFolders->setChecked(settings->expandableFolders());
}

#include "detailsviewsettingspage.moc"
