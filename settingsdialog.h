/**************************************************************************
*   Copyright (C) 2013 by Robert Metsaranta                               *
*   therealestrob@gmail.com                                               *
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
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QSettings>
#include <QFileInfo>
#include <QCheckBox>
#include <QDebug>
#include <QSlider>

namespace DFM
{

class StartupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StartupWidget(QWidget *parent = 0);
    inline QString startupPath() { return m_startPath->text(); }
    inline void setStartupPath(QString path) { m_startPath->setText(path); }
    inline int locked() { return m_lock; }
signals:

public slots:

private slots:
    inline void lockChanged()
    {
        QCheckBox *box = static_cast<QCheckBox *>(sender());
        int i = box->property("dock").toInt();
        if ( box->isChecked() )
            m_lock += i;
        else
            m_lock -= i;
    }

private:
    int m_lock;
    QLineEdit *m_startPath;
    QCheckBox *m_left, *m_right, *m_bottom;
};

class BehaviourWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BehaviourWidget(QWidget *parent = 0);
    QCheckBox *m_hideTabBar, *m_useCustomIcons, *m_drawDevUsage;
};

class ViewsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ViewsWidget(QWidget *parent = 0);
    QCheckBox *m_smoothScroll, *m_showThumbs;
    QSlider *m_iconWidth;
    QString m_iconWidthStr;
    QLabel *m_width;
private slots:
    inline void sliderChanged(const int &value) { m_width->setText( QString::number(value*2) + " px"); }
};

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    
signals:
    void settingsChanged();
    
public slots:
    void accept();

private:
    QPushButton *m_ok,*m_cancel;
    StartupWidget *m_startupWidget;
    BehaviourWidget *m_behWidget;
    ViewsWidget *m_viewWidget;
    QSettings *m_settings;
};

}

#endif // SETTINGSDIALOG_H