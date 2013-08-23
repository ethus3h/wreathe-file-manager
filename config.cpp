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


#include "config.h"
#include "mainwindow.h"
#include <QDir>
#include <QProcess>

using namespace DFM;

static Configuration *inst = 0;
Config Configuration::config;

Configuration::Configuration(QObject *parent) : QObject(parent)
{
#ifdef Q_WS_X11
    m_settings = new QSettings("dfm", "dfm");
#else
    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "dfm", "dfm");
#endif
    m_customActionsMenu.setTitle(tr("Custom Actions"));
}

Configuration
*Configuration::instance()
{
    if ( !inst )
        inst = new Configuration(qApp);
    return inst;
}

void
Configuration::readConfiguration()
{
    config.docks.lock = settings()->value( "docks.lock", 0 ).toInt();
    config.startPath = settings()->value("startPath", QDir::homePath()).toString();
    config.styleSheet = settings()->value("styleSheet", QString()).toString();

    config.behaviour.hideTabBarWhenOnlyOneTab = settings()->value("hideTabBarWhenOnlyOne", false).toBool();
    config.behaviour.systemIcons = settings()->value("useSystemIcons", false).toBool();
    config.behaviour.devUsage = settings()->value("drawDevUsage", false).toBool();
    config.behaviour.view = settings()->value("start.view", 0).toInt();   

    config.views.iconView.iconSize = settings()->value("iconView.iconSize", 3).toInt();
    config.views.flowSize = settings()->value("flowSize", QByteArray()).toByteArray();
    config.views.iconView.textWidth = settings()->value("textWidth", 16).toInt();
    config.views.detailsView.rowPadding = settings()->value("detailsView.rowPadding", 0).toInt();
    config.views.iconView.smoothScroll = settings()->value("smoothScroll", false).toBool();
    config.views.showThumbs = settings()->value("showThumbs", false).toBool();
    config.views.singleClick = settings()->value("views.singleClick", false).toBool();

    Configuration::settings()->beginGroup("CustomActions");
    foreach ( const QString &string, settings()->childKeys() )
    {
        QStringList actions = settings()->value(string).toStringList(); //0 == name, 1 == cmd, 2 == keysequence
        QAction *action = new QAction(actions[0], this);
        connect ( action, SIGNAL(triggered()), Operations::instance(), SLOT(customActionTriggered()) );
        action->setData(actions[1]);
        if ( actions.count() > 2 )
            action->setShortcut(QKeySequence(actions[2]));
        m_customActions << action;
        m_customActionsMenu.addAction(action);
    }
    Configuration::settings()->endGroup();

//    Configuration::settings()->beginGroup("Scripts");
//    foreach ( const QString &string, Configuration::settings()->childKeys())
//    {
//        QStringList actions = Configuration::settings()->value(string).toStringList(); //0 == name, 1 == script, 2 == keysequence
//        QAction *action = new QAction(actions[0], this);
//        action->setData(actions[1]);
//        if ( actions.count() > 2 )
//            action->setShortcut(QKeySequence(actions[2]));
//        connect (action, SIGNAL(triggered()), this, SLOT(scriptTriggered()));
//        VIEWS(addAction(action));
//    }
//    Configuration::settings()->endGroup();
}

void
Configuration::writeConfiguration()
{
    settings()->setValue("startPath", config.startPath);
    settings()->setValue("docks.lock", config.docks.lock);
    settings()->setValue("smoothScroll", config.views.iconView.smoothScroll);
    settings()->setValue("showThumbs", config.views.showThumbs);
    settings()->setValue("drawDevUsage", config.behaviour.devUsage);
    settings()->setValue("textWidth", config.views.iconView.textWidth);
    settings()->setValue("detailsView.rowPadding", config.views.detailsView.rowPadding);
    settings()->setValue("start.view", config.behaviour.view);
    settings()->setValue("iconView.iconSize", config.views.iconView.iconSize);
    settings()->setValue("flowSize", config.views.flowSize);
    settings()->setValue("hideTabBarWhenOnlyOne", config.behaviour.hideTabBarWhenOnlyOneTab);
    settings()->setValue("styleSheet", config.styleSheet);
    settings()->setValue("views.singleClick", config.views.singleClick);
}

static inline QString desktopInfo(const QString &desktop, bool name)
{
    QString appsPath("/usr/share/applications/");
    QFileInfo desktopFileInfo(appsPath + desktop);
    if(!desktopFileInfo.exists())
    {
        QString addToPath = desktop.split("-").at(0);
        QString newDesktop = desktop.split("-").at(1);
        QStringList tmp = QDir(appsPath).entryList(QStringList() << addToPath, QDir::Dirs);
        if(tmp.count())
            desktopFileInfo = QFileInfo(appsPath + tmp[0] + "/" + newDesktop);
        if(!desktopFileInfo.exists())
            return QString();
    }
    QFile desktopFile(desktopFileInfo.filePath());
    desktopFile.open(QFile::ReadOnly); //we should only get here if file exists...
    QTextStream stream(&desktopFile);
    QString n, e;
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        if(line.startsWith("name=", Qt::CaseInsensitive))
        {
            if(line.split("=").count() >= 2)
                n = line.split("=").at(1);
        }
        else if(line.startsWith("exec=", Qt::CaseInsensitive))
        {
            if(line.split("=").count() >= 2)
                e = line.split("=").at(1);
        }
        else continue;
    }
    desktopFile.close();
    return name ? n : e;
}

QList<QAction *>
Configuration::openWithActions(const QString &file)
{
    QFile mimeTypes("/usr/share/mime/globs");
    mimeTypes.open(QFile::ReadOnly);
    QStringList mimeTypesList = QString(mimeTypes.read(QFileInfo(mimeTypes).size())).split("\n", QString::SkipEmptyParts);
    mimeTypes.close();

    QMap<QString, QString> mimeMap;
    foreach(QString mimeType, mimeTypesList)
    {
        QStringList tmp = mimeType.split(":");
        if(tmp.count() >= 2)
        {
            const QString &mt = tmp.at(0);
            QString fn = tmp.at(1);
            fn.remove("*"); fn.remove(".");
            mimeMap[fn] = mt;
        }
    }

    QFile mimeInfo("/usr/share/applications/mimeinfo.cache");
    mimeInfo.open(QFile::ReadOnly);
    QStringList mimeList = QString( mimeInfo.read( QFileInfo( mimeInfo ).size() ) ).split( "\n", QString::SkipEmptyParts );
    mimeInfo.close();

    QMap<QString, QStringList> appsMap;

    foreach(QString mimeString, mimeList)
    {
        QStringList tmp = mimeString.split("=");
        if(tmp.count() >= 2)
        {
            QString mt = tmp[0]; QString apps = tmp[1];
            QStringList appsList = apps.split(";");
            appsList.removeOne("");
            appsMap[mt] = appsList;
        }
    }

    QStringList apps = QStringList() << appsMap[Operations::getMimeType(file)] << appsMap[mimeMap[QFileInfo(file).suffix()]];
    apps.removeDuplicates();

    QList<QAction *> actionList;
    foreach(QString app, apps)
    {
        QAction *action = new QAction(desktopInfo(app, true), instance() );
        action->setProperty("file", file);
        connect( action, SIGNAL( triggered() ), Operations::instance(), SLOT( openWith() ) );
        QVariant var;
        var.setValue(desktopInfo(app, false));
        action->setData(var);
        actionList << action;
    }
    return actionList;
}
