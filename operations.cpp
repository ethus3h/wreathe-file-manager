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


#include "operations.h"
#include "mainwindow.h"
#include <QFileInfo>
#include <QVector>
#include <QDebug>
#include <QProcess>
#include <QApplication>
#include <QAction>
#include <QItemSelectionModel>

//#include <blkid/blkid.h> //dev block id info... maybe need later

using namespace DFM;

static Ops *s_instance = 0;

Ops
*Ops::instance()
{
    if ( !s_instance )
        s_instance = new Ops(qApp);
    return s_instance;
}

QString
Ops::getMimeType(const QString &file)
{
#ifdef Q_WS_X11
    const magic_t &mgcMime = magic_open( MAGIC_MIME_TYPE );
    magic_load( mgcMime, NULL );
    return QString( magic_file( mgcMime, file.toStdString().c_str() ) );
#else
    return QString();
#endif
}

QString
Ops::getFileType(const QString &file)
{
#ifdef Q_WS_X11
    magic_t mgcMime = magic_open( MAGIC_CONTINUE ); //print anything we can get
    magic_load( mgcMime, NULL );
    return QString( magic_file( mgcMime, file.toStdString().c_str() ) );
#else
    return QString();
#endif
}

QColor
Ops::colorMid(const QColor c1, const QColor c2, int i1, int i2)
{
    int r,g,b,a;
    int i3 = i1+i2;
    r = qMin(255,(i1*c1.red() + i2*c2.red())/i3);
    g = qMin(255,(i1*c1.green() + i2*c2.green())/i3);
    b = qMin(255,(i1*c1.blue() + i2*c2.blue())/i3);
    a = qMin(255,(i1*c1.alpha() + i2*c2.alpha())/i3);
    return QColor(r,g,b,a);
}

void
Ops::openFile(const QString &file)
{
    if(!QFileInfo(file).exists())
        return;

    QProcess process;
    if(QFileInfo(file).isExecutable() && (QFileInfo(file).suffix().isEmpty() ||
                                                         QFileInfo(file).suffix() == "sh" ||
                                                         QFileInfo(file).suffix() == "exe")) // windows executable
    {
        process.startDetached(file);
    }
    else
    {
        QStringList list;
        list << file;

#ifdef Q_WS_X11 //linux
        process.startDetached("xdg-open",list);
#else //windows
        process.startDetached("cmd /c start " + list.at(0)); //todo: fix, this works but shows a cmd window real quick
#endif
    }
}

void
Ops::openWith()
{
    QAction *action = static_cast<QAction *>( sender() );
    QString program( action->data().toString().split( " " ).at( 0 ) );
    QProcess::startDetached( program, QStringList() << action->property("file").toString() );
}

void
Ops::customActionTriggered()
{
    QStringList action(static_cast<QAction *>(sender())->data().toString().split(" "));
    const QString &app = action.takeFirst();

    FileSystemModel *fsModel = MainWindow::currentContainer()->model();
    QItemSelectionModel *isModel = MainWindow::currentContainer()->selectionModel();

    if ( isModel->hasSelection() )
    {
        if ( isModel->selectedRows().count() )
            foreach ( const QModelIndex &index, isModel->selectedRows() )
                QProcess::startDetached(app, QStringList() << action << fsModel->filePath( index ));
        else if ( isModel->selectedIndexes().count() )
            foreach ( const QModelIndex &index, isModel->selectedIndexes() )
                QProcess::startDetached(app, QStringList() << action << fsModel->filePath( index ));
    }
    else
    {
        QProcess::startDetached(app, QStringList() << action << fsModel->rootPath());
    }
}

void
Ops::setRootPath()
{
    QAction *action = static_cast<QAction *>(sender());
    MainWindow::currentContainer()->model()->setRootPath(action->data().toString());
}

QString
Ops::prettySize(quint64 bytes)
{
  if (bytes & (0x3fff<<50))
     return QString::number( (bytes>>50) + ((bytes>>40) & (0x3ff)) / 1024.0, 'f', 2 ) + " PiB";
  else if (bytes & (0x3ff<<40))
     return QString::number( (bytes>>40) + ((bytes>>30) & (0x3ff)) / 1024.0, 'f', 2 ) + " TiB";
  else if (bytes & (0x3ff<<30))
     return QString::number( (bytes>>30) + ((bytes>>20) & (0x3ff)) / 1024.0, 'f', 2 ) + " GiB";
  else if (bytes & (0x3ff<<20))
     return QString::number( (bytes>>20) + ((bytes>>10) & (0x3ff)) / 1024.0, 'f', 2 ) + " MiB";
  else if (bytes & (0x3ff<<10))
     return QString::number( (bytes>>10) + ((bytes) & (0x3ff)) / 1024.0, 'f', 2 ) + " KiB";
  else
     return QString::number( bytes, 'f', 0 ) + "Bytes";
}

