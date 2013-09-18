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


#include "columnsview.h"
#include "viewcontainer.h"
#include "mainwindow.h"
#include "filesystemmodel.h"
#include "operations.h"
#include <QPainter>

using namespace DFM;

class ColumnsDelegate : public QStyledItemDelegate
{
public:
    explicit ColumnsDelegate(QWidget *parent = 0)
        : QStyledItemDelegate(parent)
        , m_view(static_cast<ColumnsView *>(parent)){}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if ( index.data().toString() == m_view->activeFileName() )
        {
            QColor h = m_view->palette().color(QPalette::Highlight);
            h.setAlpha(64);
            painter->fillRect(option.rect, h);
        }
        QStyledItemDelegate::paint(painter, option, index);
    }
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        int w = option.fontMetrics.width(index.data().toString());
        int h = 16;
        return QSize(w, h);
    }

private:
    ColumnsView *m_view;
};

ColumnsView::ColumnsView(QWidget *parent, FileSystemModel *fsModel, const QModelIndex &rootIndex)
    : QListView(parent)
    , m_parent(static_cast<ColumnsWidget *>(parent))
    , m_pressPos(QPoint())
    , m_activeFile(QString())
    , m_fsModel(0)
{
    setItemDelegate(new ColumnsDelegate(this));
    setViewMode(QListView::ListMode);
    setResizeMode(QListView::Adjust);
    setIconSize(QSize(16, 16));
    setUniformItemSizes(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::MoveAction);
    setAcceptDrops(true);
    setDragEnabled(true);
    setEditTriggers(QAbstractItemView::SelectedClicked|QAbstractItemView::EditKeyPressed);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionRectVisible(true);
    setMouseTracking(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setTextElideMode(Qt::ElideNone);
    if ( fsModel )
    {
        setModel(fsModel);
        if ( rootIndex.isValid() )
            setRootIndex(rootIndex);
        m_rootPath = fsModel->filePath(rootIndex);
        connect(fsModel, SIGNAL(rowsInserted(const QModelIndex & , int , int)), this, SLOT(updateWidth()));
        connect(fsModel, SIGNAL(rowsRemoved(const QModelIndex & , int , int)), this, SLOT(updateWidth()));
    }
    updateWidth();
}

void
ColumnsView::setFilter(QString filter)
{
    for (int i = 0; i < model()->rowCount(rootIndex()); i++)
    {
        const QModelIndex &index = model()->index(i,0,rootIndex());
        if (index.data().toString().contains(filter, Qt::CaseInsensitive))
            setRowHidden(i, false);
        else
            setRowHidden(i, true);
    }
}

void
ColumnsView::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier && state() != QAbstractItemView::EditingState )
    {
        if ( selectionModel()->selectedIndexes().count() )
            foreach ( const QModelIndex &index, selectionModel()->selectedIndexes() )
                if ( index.column() == 0 )
                    emit activated(index);
        event->accept();
        return;
    }
    QListView::keyPressEvent(event);
}

void
ColumnsView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu popupMenu;
    if ( Store::customActions().count() )
        popupMenu.addMenu(Store::customActionsMenu());
    popupMenu.addActions( ViewContainer::rightClickActions() );
    const QString &file = static_cast<FileSystemModel *>( model() )->filePath( indexAt( event->pos() ) );
    QMenu openWith( tr( "Open With" ), this );
    openWith.addActions( Store::openWithActions( file ) );
    foreach( QAction *action, actions() )
    {
        popupMenu.addAction( action );
        if ( action->objectName() == "actionDelete" )
            popupMenu.insertSeparator( action );
        if ( action->objectName() == "actionCustomCmd" )
            popupMenu.insertMenu( action, &openWith );
    }
    popupMenu.exec( event->globalPos() );
}

void
ColumnsView::mouseReleaseEvent(QMouseEvent *e)
{
    const QModelIndex &index = indexAt(e->pos());
    if ( !Store::config.views.singleClick
         && e->pos() == m_pressPos
         && e->modifiers() == Qt::NoModifier
         && index.isValid()
         && m_fsModel->fileInfo(index).isDir() )
    {
        e->accept();
        if ( e->button() == Qt::LeftButton )
            emit activated(index);
        else if ( e->button() == Qt::MiddleButton )
        {
            emit newTabRequest(index);
            return;
        }
    }
    QListView::mouseReleaseEvent(e);
}

void
ColumnsView::paintEvent(QPaintEvent *e)
{
    QListView::paintEvent(e);
    if ( m_parent->currentView() && m_parent->currentView() == this )
    {
        QPainter p(viewport());
        p.setPen(QPen(palette().color(QPalette::Highlight), 3.0f));
        p.drawRect(viewport()->rect().adjusted(0,0,-1,-1));
        p.end();
    }
}

void
ColumnsView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);
    m_fsModel = static_cast<FileSystemModel *>(model);
}

void
ColumnsView::updateWidth()
{
    if ( !QFileInfo(m_rootPath).exists() )
    {
        emit pathDeleted(this);
        return;
    }
    QStringList list = QDir(m_fsModel->filePath(rootIndex())).entryList(QDir::AllEntries|QDir::AllDirs|QDir::NoDotAndDotDot|QDir::System);
    int w = 0;
    while ( !list.isEmpty() )
    {
        const int W = fontMetrics().boundingRect(list.takeFirst()).width();
        if ( W > w )
            w = W;
    }
    w+=22; //icon
    w+=style()->pixelMetric(QStyle::PM_ScrollBarExtent, 0, this);
    setFixedWidth(qMax(64, w));
    setFixedHeight(m_parent->height()-(style()->pixelMetric(QStyle::PM_ScrollBarExtent)*m_parent->verticalScrollBar()->isVisible()));
}
