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


#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QPainter>
#include <QAbstractItemView>
#include <QDir>
#include <QFileIconProvider>
#include <QTimer>
#include <QMimeData>
#include <QSettings>
#include <QDateTime>
#include <QUrl>
#include <QLabel>
#include <QDebug>
#include <QMap>
#include <QSortFilterProxyModel>
#include "thumbsloader.h"
#include "viewcontainer.h"

namespace DFM
{
class FileSystemModel;

class History : public QObject
{
    Q_OBJECT
public:
    explicit History(QObject *parent = 0) : QObject(parent) {}
    bool canGoBack() { return m_historyList.count(); }
    QString lastVisited() { return m_historyList.last(); }
    QStringList all() { return m_historyList; }

public slots:
    void addPath( const QString &path ) { m_historyList << path; m_historyList.removeDuplicates(); }

private:
    QStringList m_historyList;
};

class FileIconProvider : public QObject, public QFileIconProvider
{
    Q_OBJECT
public:
    inline FileIconProvider(FileSystemModel *model = 0);
    QIcon icon(const QFileInfo &info) const;

public slots:
    void loadThemedFolders(const QString &path);

signals:
    void dataChanged(const QModelIndex &f, const QModelIndex &e);

private:
    FileSystemModel *m_fsModel;
};
class ThumbsLoader;
class ImagesThread;
class ViewContainer;
class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    enum Roles {
//        FileIconRole = Qt::DecorationRole,
//        FilePathRole = Qt::UserRole + 1,
//        FileNameRole = Qt::UserRole + 2,
//        FilePermissions = Qt::UserRole + 3,
        FlowImg = Qt::UserRole + 4,
        FlowRefl = Qt::UserRole +5,
        FlowShape = Qt::UserRole +6
    };
    explicit FileSystemModel(QObject *parent = 0);
    virtual ~FileSystemModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    static QStringList supportedThumbs( const bool filter = false );
    inline void refresh() { const QString &path = rootPath(); setRootPath(""); setRootPath(path); }
    bool hasThumb( const QString &file );
    bool hasFlowData( const QString &file );
    History *history() { return m_history; }
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    inline int sortingColumn() const { return m_sortCol; }
    inline Qt::SortOrder sortingOrder() const { return m_sortOrder; }
    void forceEmitDataChangedFor(const QString &file);
    inline FileIconProvider *ip() { return m_iconProvider; }

public slots:
    void setPath(const QString &path) { setRootPath(path); }

private slots:
    void emitRootIndex( const QString &path ) { emit rootPathAsIndex(index(path)); }
    void thumbFor( const QString &file );
    void flowDataAvailable( const QString &file );

signals:
    void rootPathAsIndex( const QModelIndex &index );
    void flowDataChanged( const QModelIndex &start, const QModelIndex &end );

private:
    friend class ImagesThread;
    QStringList m_nameThumbs;
    FileIconProvider *m_iconProvider;
    ThumbsLoader *m_thumbsLoader;
    ImagesThread *m_it;
    ViewContainer *m_container;
    History *m_history;
    int m_sortCol;
    Qt::SortOrder m_sortOrder;
    QString m_prevRp;
};

class FileProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FileProxyModel(QObject *parent = 0);
    FileSystemModel *fsModel() { return m_fsModel; }
    QString filePath(const QModelIndex &index) { return m_fsModel->filePath(mapToSource(index)); }
    QString fileName(const QModelIndex &index) { return m_fsModel->fileName(mapToSource(index)); }
    QIcon fileIcon(const QModelIndex &index) { return m_fsModel->fileIcon(mapToSource(index)); }
    FileIconProvider *iconProvider() { return static_cast<FileIconProvider *>(m_fsModel->iconProvider()); }
    QString rootPath() { return m_fsModel->rootPath(); }
    void setRootPath(const QString &rootPath) { m_fsModel->setRootPath(rootPath); }
    QFileInfo fileInfo(const QModelIndex &index) { return m_fsModel->fileInfo(mapToSource(index)); }
    bool isDir(const QModelIndex &index) { return m_fsModel->isDir(mapToSource(index)); }
    QDir rootDirectory() { return m_fsModel->rootDirectory(); }
    QModelIndex index(int row, int column, const QModelIndex &parent) const { return QSortFilterProxyModel::index(row, column, parent); }
    QModelIndex index(const QString &file, int column = 0) { return mapFromSource(m_fsModel->index(file, column)); }
    void mkdir(const QModelIndex &parent, const QString &name) { m_fsModel->mkdir(mapToSource(parent), name); }
    void setFilter(const QDir::Filters filters) { m_fsModel->setFilter(filters); }
    bool hasThumb(const QString &file) { return m_fsModel->hasThumb(file); }

private:
    FileSystemModel *m_fsModel;
};

}

Q_DECLARE_METATYPE(QPainterPath)

#endif // FILESYSTEMMODEL_H