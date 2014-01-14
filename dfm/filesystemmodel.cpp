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

#include <QImageReader>
#include <QDirIterator>
#include <QMutexLocker>

#include "filesystemmodel.h"
#include "iojob.h"
#include "thumbsloader.h"
#include "mainwindow.h"
#include "viewcontainer.h"

using namespace DFM;

QIcon
FileIconProvider::icon(const QFileInfo &info) const
{
#if 0
    if ( Store::config.icons.customIcons.contains(info.filePath()) )
        return QIcon(Store::config.icons.customIcons.value(info.filePath()));
#endif
    if ( info.absoluteFilePath() == QDir::rootPath() )
        if ( QIcon::hasThemeIcon("folder-system") )
            return QIcon::fromTheme("folder-system");
        else if ( QIcon::hasThemeIcon("inode-directory") )
            return QIcon::fromTheme("inode-directory");

    if ( ThumbsLoader::hasIcon(info.absoluteFilePath()) )
        if ( QIcon::hasThemeIcon(ThumbsLoader::icon(info.absoluteFilePath())) )
            return QIcon::fromTheme(ThumbsLoader::icon(info.absoluteFilePath()));

    QIcon icn = QFileIconProvider::icon(info);
    if ( QIcon::hasThemeIcon(icn.name()) )
        return QIcon::fromTheme(icn.name());

    return icn;
}

//-----------------------------------------------------------------------------

static bool lessThen(FileSystemModel::Node *n1, FileSystemModel::Node *n2)
{
    if ( n1 == n2 )
        return true;
    const bool desc = bool(n1->sortOrder() == Qt::DescendingOrder);
    const int column = n1->sortColumn();

    //dirs always first right?
    if ( n1->isDir() && !n2->isDir() )
        return true;
    if ( !n1->isDir() && n2->isDir() )
        return false;

    //hidden... last?
    if ( n1->hidden() && !n2->hidden() )
        return false;
    if ( !n1->hidden() && n2->hidden() )
        return true;

    bool lt = true;
    switch (column)
    {
    case 0: lt = n1->name().toLower()<n2->name().toLower(); break; //name
    case 1: lt = n1->size()<n2->size(); break; //size
    case 2: //type
        if (n1->suffix().toLower()==n2->suffix().toLower())
            lt = n1->name().toLower()<n2->name().toLower();
        else
            lt = n1->suffix().toLower()<n2->suffix().toLower();
        break;
    case 3: lt = n1->lastModified()<n2->lastModified(); break; //lastModified
    case 4: lt = n1->permissions()<n2->permissions(); break; //permissions
    default: break;
    }
    return desc?!lt:lt;
}

static QDir::Filters filters = QDir::AllEntries|QDir::System|QDir::NoDotAndDotDot|QDir::Hidden;
static FileSystemModel::Nodes s_deletedNodes;

FileSystemModel::Node::Node(FileSystemModel *model, const QString &path, Node *parent, const Type &t)
    : QFileInfo(path)
    , m_parent(parent)
    , m_isPopulated(false)
    , m_model(model)
    , m_filePath(path)
    , m_isLocked(false)
    , m_isChildLocked(false)
    , m_filter(QString(""))
    , m_type(t)
{
//    if ( t == FSNode )
//    {
        if ( !m_parent )
            m_name = "--";
        else if ( fileName().isEmpty() )
            m_name = filePath();
        else
            m_name = fileName();
//    }
//    else
//        m_name = filePath();
}

FileSystemModel::Node::~Node()
{
    for (int i = 0; i<HiddenFiles+1; ++i)
        if (!m_children[i].isEmpty())
            qDeleteAll(m_children[i]);
}

void
FileSystemModel::Node::insertChild(Node *node)
{
//    if (type() == SearchResult)
//    {
//        model()->beginInsertRows(QModelIndex(), m_children[Visible].count(), m_children[Visible].count());
//        m_mutex.lock();
//        m_children[Visible] << node;
//        m_mutex.unlock();
//        model()->endInsertRows();
//    }
//    else
//    {
        if ( node->hidden() && !showHidden() )
            m_children[Hidden] << node;
        else
        {
            int z = m_children[Visible].size(), i = -1;
            while ( ++i < z )
                if ( !lessThen(m_children[Visible].at(i), node) )
                    break;

            Q_ASSERT(i<=m_children[Visible].size());
            model()->beginInsertRows(model()->index(filePath()), i, i);
            m_mutex.lock();
            m_children[Visible].insert(i, node);
            m_mutex.unlock();
            model()->endInsertRows();
        }
//    }
}

int
FileSystemModel::Node::row()
{
    if (!parent())
        -1;

    return parent()->rowFor(this);
}

int
FileSystemModel::Node::childCount() const
{
    QMutexLocker locker(&m_mutex);
    return m_children[Visible].size();
}

FileSystemModel::Node
*FileSystemModel::Node::child(const int c)
{
    QMutexLocker locker(&m_mutex);
    if (c > -1 && c < m_children[Visible].size())
        return m_children[Visible].at(c);
    return 0;
}

FileSystemModel::Node
*FileSystemModel::Node::child(const QString &path)
{
    QMutexLocker locker(&m_mutex);
    int c = m_children[Visible].count();
    while ( --c > -1 )
    {
        Node *node = m_children[Visible].at(c);
        if ( node && node->filePath() == path )
            return node;
    }
    return 0;
}

QString FileSystemModel::Node::name() { return m_name; }

int
FileSystemModel::Node::rowFor(Node *child) const
{
    QMutexLocker locker(&m_mutex);
    return m_children[Visible].indexOf(child);
}

QString
FileSystemModel::Node::category()
{
    if (isHidden())
        return QString(isDir()?"HIDDEN (DIRECTORY)":"HIDDEN (FILE)");
    switch (sortColumn())
    {
    case 0: return isDir()?tr("DIRECTORY"):name().at(0).toUpper(); break;
    case 1: return tr(isDir()?"DIRECTORY":size()<1048576?"SMALL":size()<1073741824?"MEDIUM":"LARGE"); break;
    case 2: return data(2).toString().toUpper(); break;
    case 3:
    {
        int y,m,d;
        lastModified().date().getDate(&y, &m, &d);
        return QString("%1 %2").arg(QString::number(y), QString::number(m));
        break;
    }
    default: return QString("-"); break;
    }
    return QString();
}

bool FileSystemModel::Node::hidden() { return isHidden()&&!isLocked(); }

FileSystemModel *FileSystemModel::Node::model() { return m_model; }

void
FileSystemModel::Node::setFilter(const QString &filter)
{
    if (filter == m_filter || model()->isPopulating())
        return;

    m_filter = filter;
    emit model()->layoutAboutToBeChanged();
    for ( int i = 0; i < Filtered; ++i )
    {
        int c = m_children[i].count();
        while ( --c > -1 )
            if ( !m_children[i].at(c)->name().contains(filter, Qt::CaseInsensitive) )
                m_children[Filtered] << m_children[i].takeAt(c);
    }

    int f = m_children[Filtered].count();
    while ( --f > -1 )
        if ( m_children[Filtered].at(f)->name().contains(filter, Qt::CaseInsensitive) )
        {
            if ( m_children[Filtered].at(f)->hidden() && !showHidden() )
                m_children[Hidden] << m_children[Filtered].takeAt(f);
            else if ( m_children[Filtered].at(f)->hidden() && showHidden() )
                m_children[Visible] << m_children[Filtered].takeAt(f);
            else
                m_children[Visible] << m_children[Filtered].takeAt(f);
        }
    if ( m_children[Visible].count() > 1 )
        qStableSort(m_children[Visible].begin(), m_children[Visible].end(), lessThen);
    emit model()->layoutChanged();
}

bool
FileSystemModel::Node::rename(const QString &newName)
{
    const QString &oldName = absoluteFilePath();
    if ( QFile::rename(oldName, dir().absoluteFilePath(newName)) )
    {
        m_filePath = dir().absoluteFilePath(newName);
        m_name = newName;
        setFile(m_filePath);
        refresh();
        return true;
    }
    return false;
}

QVariant
FileSystemModel::Node::data(const int column)
{
    switch (column)
    {
    case 0: return name(); break;
    case 1: return isDir()?QString("--"):Ops::prettySize(size()); break;
    case 2:
    {
        if ( isDir() )
            return QString("directory");
        else if ( isSymLink() )
            return QString("symlink");
        else if ( suffix().isEmpty() && isExecutable() )
            return QString("exec");
        else if ( suffix().isEmpty() )
            return QString("file");
        else
            return suffix();
        break;
    }
    case 3: return lastModified(); break;
    case 4:
    {
        const QFile::Permissions p = permissions();
        QString perm;
        perm.append(p.testFlag(QFile::ReadUser)?"R, ":"-, ");
        perm.append(p.testFlag(QFile::WriteUser)?"W, ":"-, ");
        perm.append(p.testFlag(QFile::ExeUser)?"X":"-");
        return perm;
    }
    default: return QString("--");
    }
}

FileSystemModel::Node
*FileSystemModel::Node::nodeFromPath(const QString &path, bool checkOnly)
{
    if ( !isPopulated() )
    {
        if ( checkOnly )
            return 0;
        else
            rePopulate();
    }
    for ( int i = 0; i < Files; ++i )
    {
        if (!i)
            m_mutex.lock();
        int c = m_children[i].count();
        while ( --c > -1 )
        {
            Node *node = m_children[i].at(c);
            if ( node && node->filePath() == path )
            {
                if ( i )
                    m_children[Visible] << m_children[i].takeAt(c);
                m_mutex.unlock();
                return node;
            }
        }
        if (!i)
            m_mutex.unlock();
    }
    return 0;
}

FileSystemModel::Node
*FileSystemModel::Node::node(const QString &path, bool checkOnly)
{
    QFileInfo fi(path);
    if ( !fi.exists() || !fi.isAbsolute() )
        return 0;
    QStringList paths;
    QDir dir;
    if ( fi.isDir() )
        dir = QDir(path);
    else
    {
        dir = fi.dir();
        paths << dir.absolutePath();
    }
    paths << path;
    while (dir.cdUp())
        paths.prepend(dir.absolutePath());

    Node *n = model()->rootNode();

    while ( !paths.isEmpty() && n )
        n = n->nodeFromPath(paths.takeFirst(), checkOnly);

    if ( n && n->filePath() == path )
        return n;

    return 0;
}

bool
FileSystemModel::Node::hasChild(const QString &name, const bool nameIsPath, const bool onlyVisible)
{
    int vis = onlyVisible ? 0 : Files;
    for ( int i = 0; i < vis; ++i )
    {
        FileSystemModel::Nodes::const_iterator start = m_children[i].constBegin(), end = m_children[i].constEnd();
        while ( start != end )
        {
            bool check = (*start)->name() == name;
            if (nameIsPath)
                check = (*start)->filePath() == name;
            if (check)
                return true;
            ++start;
        }
    }
    return false;
}

void
FileSystemModel::Node::rePopulate()
{
    model()->startPopulating();
    if (!m_children[Files].isEmpty())
    {
        m_mutex.lock();
        s_deletedNodes+=m_children[Visible];
        m_children[Visible].clear();
        m_children[Visible]=m_children[Files];
        m_children[Files].clear();
        m_children[Hidden]=m_children[HiddenFiles];
        m_children[HiddenFiles].clear();
        m_mutex.unlock();
    }
    if ( isLocked() )
        setLocked(false);
    if ( filePath() == model()->rootPath() )
    {
        int sc = model()->sortColumn();
        Qt::SortOrder so = model()->sortOrder();
        Ops::getSorting(filePath(), sc, so);
        if ( sc != model()->sortColumn() || so != model()->sortOrder() )
            model()->setSort(sc, so);
    }
    if ( isPopulated() )
        for ( int i = 0; i < Files; ++i )
        {
            int c = m_children[i].count();
            while ( --c > -1 )
            {
                Node *node = m_children[i].at(c);
                node->refresh();
                if ( !node->exists() )
                {
                    if ( !i )
                    {
                        model()->beginRemoveRows(model()->index(filePath()), c, c);
                        m_mutex.lock();
                    }
                    s_deletedNodes << m_children[i].takeAt(c);
                    if ( !i )
                    {
                        m_mutex.unlock();
                        model()->endRemoveRows();
                    }
                }
            }
        }
    const QDir dir(filePath());
    if ( this == model()->rootNode() )
    {
        for ( int i = 0; i < QDir::drives().count(); ++i )
        {
            if ( hasChild(QDir::drives().at(i).filePath()) )
                continue;
            insertChild(new Node(model(), QDir::drives().at(i).filePath(), this));
        }
    }
    else if ( dir.isAbsolute() )
    {
        QDirIterator it(filePath(), filters);
        while ( it.hasNext() )
        {
            const QFileInfo file(it.next());
            if ( hasChild(file.fileName()) )
                continue;

            insertChild(new Node(model(), file.filePath(), this));
        }
    }
    model()->endPopulating();
    m_isPopulated = true;
    if ( filePath() == model()->rootPath() )
        emit model()->directoryLoaded(filePath());

    qDeleteAll(s_deletedNodes);
    s_deletedNodes.clear();

    for ( int i = 0; i < m_children[Visible].count(); ++i )
        if ( m_children[Visible].at(i)->isPopulated() )
            m_children[Visible].at(i)->rePopulate();
}

void
FileSystemModel::Node::sort()
{
    if ( model()->isPopulating() )
        return;
    if ( m_children[Visible].isEmpty() )
        return;

    if ( m_children[Visible].count() > 1 )
        qStableSort(m_children[Visible].begin(), m_children[Visible].end(), lessThen);

    int i = m_children[Visible].count();
    while ( --i > -1 )
    {
        Node *node = m_children[Visible].at(i);
        if ( node->isPopulated() && node->isDir() )
            node->sort();
    }
}

void
FileSystemModel::Node::setHiddenVisible(bool visible)
{
    emit model()->layoutAboutToBeChanged();
    if ( visible )
    {
        m_children[Visible]+=m_children[Hidden];
        qStableSort(m_children[Visible].begin(), m_children[Visible].end(), lessThen);
        m_children[Hidden].clear();
    }
    else
    {
        int i = m_children[Visible].count();
        while ( --i > -1 )
            if ( m_children[Visible].at(i)->hidden() )
                m_children[Hidden] << m_children[Visible].takeAt(i);
    }
    int i = m_children[Visible].count();
    while ( --i > -1 )
        if ( m_children[Visible].at(i)->isPopulated() )
            m_children[Visible].at(i)->setHiddenVisible(visible);
    emit model()->layoutChanged();
}

void
FileSystemModel::Node::search(const QString &fileName)
{
    m_filter=fileName;
    emit model()->layoutAboutToBeChanged();
    if (m_children[Files].isEmpty())
    {
        m_mutex.lock();
        m_children[HiddenFiles]=m_children[Hidden];
        m_children[Hidden].clear();
        m_children[Files]=m_children[Visible];
        m_children[Visible].clear();
        m_mutex.unlock();
    }
    else
    {
        m_mutex.lock();
        s_deletedNodes+=m_children[Visible]; //only previous searchresults here...
        m_children[Visible].clear();
        m_mutex.unlock();
    }
    emit model()->layoutChanged();

    emit model()->rootPathChanged(m_filePath);
    QDirIterator it(m_filePath, QDir::AllEntries|QDir::Hidden|QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()&&!model()->dataGatherer()->isCancelled())
    {
        const QFileInfo &file(it.next());
        if (file.fileName().contains(fileName, Qt::CaseInsensitive))
            insertChild(new FileSystemModel::Node(m_model, file.filePath(), this, FileSystemModel::Node::SearchResult));
    }
    emit model()->searchFinished();
    emit model()->directoryLoaded(m_filePath);
}

void
FileSystemModel::Node::endSearch()
{
    if (!m_children[Files].isEmpty())
    {
        emit model()->layoutAboutToBeChanged();
        s_deletedNodes+=m_children[Visible];
        m_children[Visible].clear();
        m_children[Visible]=m_children[Files];
        m_children[Hidden]=m_children[HiddenFiles];
        m_children[Files].clear();
        m_children[HiddenFiles].clear();
        emit model()->layoutChanged();
    }
}

//-----------------------------------------------------------------------------
# if 0
#ifdef Q_OS_LINUX
// for ioprio
#include <unistd.h>
#include <sys/syscall.h>
enum {
    IOPRIO_CLASS_NONE,
    IOPRIO_CLASS_RT,
    IOPRIO_CLASS_BE,
    IOPRIO_CLASS_IDLE
};

enum {
    IOPRIO_WHO_PROCESS = 1,
    IOPRIO_WHO_PGRP,
    IOPRIO_WHO_USER
};
#define IOPRIO_CLASS_SHIFT  13
#endif
#endif

void
DataGatherer::run()
{
#if 0
    bool ioPriorityWorked = false;
#if defined(Q_OS_LINUX) && defined(SYS_ioprio_set)

    // try setting the idle priority class

    ioPriorityWorked = ( syscall( SYS_ioprio_set, IOPRIO_WHO_PROCESS, 0, IOPRIO_CLASS_IDLE << IOPRIO_CLASS_SHIFT ) >= 0 );

    // try setting the lowest priority in the best-effort priority class (the default class)

    if( !ioPriorityWorked )
        ioPriorityWorked = ( syscall( SYS_ioprio_set, IOPRIO_WHO_PROCESS, 0, 7 | ( IOPRIO_CLASS_BE << IOPRIO_CLASS_SHIFT ) ) >= 0 );
#endif
    if( !ioPriorityWorked )
        setPriority( QThread::IdlePriority );
#endif

    switch ( m_task )
    {
    case Populate:
    {
        if ( !m_node )
            return;

        m_node->rePopulate();
        if ( m_node->filePath() != m_model->rootPath() )
            return;
        QFileSystemWatcher *watcher = m_model->dirWatcher();
        if ( !watcher->directories().isEmpty() )
            watcher->removePaths(watcher->directories());
        QStringList newPaths;
        FileSystemModel::Node *node = m_node;
        while ( node )
        {
            if ( !node->filePath().isEmpty() )
                newPaths << node->filePath();
            node->setLocked(true);
            node = node->parent();
        }
        if ( !newPaths.isEmpty() )
            watcher->addPaths(newPaths);
        break;
    }
    case Generate:
    {
        if ( QFileInfo(m_path).isDir() )
            m_result = m_model->rootNode()->node(m_path, false);
        if ( m_result )
            emit nodeGenerated(m_path, m_result);
        break;
    }
    case Search:
    {
        m_node->search(m_searchName);
        break;
    }
    default:
        break;
    }
}

void
DataGatherer::search(const QString &name, FileSystemModel::Node *node)
{
    if (isRunning())
        return;

    m_task = Search;
    m_node = node;
    m_searchName = name;
    start();
}

void
DataGatherer::populateNode(FileSystemModel::Node *node)
{
    if ( isRunning() || node == m_model->rootNode() )
        return;
    m_task = Populate;
    m_node = node;
    start();
}

void
DataGatherer::generateNode(const QString &path)
{
    if ( isRunning() )
        return;
    m_task = Generate;
    m_path = path;
    m_node = 0;
    m_result = 0;
    start();
}

//-----------------------------------------------------------------------------

FileSystemModel::FileSystemModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootNode(new Node(this))
    , m_view(0)
    , m_showHidden(false)
    , m_ip(new FileIconProvider(this))
    , m_sortOrder(Qt::AscendingOrder)
    , m_sortColumn(0)
    , m_it(new ImagesThread(this))
    , m_watcher(new QFileSystemWatcher(this))
    , m_container(0)
    , m_dataGatherer(new DataGatherer(this))
    , m_isPopulating(false)
{
    if ( ViewContainer *vc = qobject_cast<ViewContainer *>(parent) )
        m_container = vc;

    connect ( ThumbsLoader::instance(), SIGNAL(thumbFor(QString,QString)), this, SLOT(thumbFor(QString,QString)) );
    connect ( m_it, SIGNAL(imagesReady(QString)), this, SLOT(flowDataAvailable(QString)) );
    connect ( this, SIGNAL(rootPathChanged(QString)), m_it, SLOT(clearData()) );
    connect ( m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(dirChanged(QString)) );
    connect ( m_dataGatherer, SIGNAL(nodeGenerated(QString,FileSystemModel::Node*)), this, SLOT(nodeGenerated(QString,FileSystemModel::Node*)) );
    connect(this, SIGNAL(fileRenamed(QString,QString,QString)), ThumbsLoader::instance(), SLOT(fileRenamed(QString,QString,QString)));
    connect(m_dataGatherer, SIGNAL(finished()), this, SLOT(removeDeletedLater()));
}

FileSystemModel::~FileSystemModel()
{
    m_it->discontinue();
    if ( m_it->isRunning() )
        m_it->wait();
    delete m_rootNode;
}

void
FileSystemModel::setSort(const int sortColumn, const int sortOrder)
{
    m_sortColumn = sortColumn;
    m_sortOrder = (Qt::SortOrder)sortOrder;
    emit sortingChanged(m_sortColumn, (int)m_sortOrder);
}

void
FileSystemModel::nodeGenerated(const QString &path, FileSystemModel::Node *node)
{
    if ( path != rootPath() || node == m_rootNode )
        return;

    emit rootPathChanged(rootPath());
    dataGatherer()->populateNode(node);
}

void
FileSystemModel::setRootPath(const QString &path)
{
    if ( !QFileInfo(path).isDir() )
        return;
    Node *current = rootNode()->node(m_rootPath);
    if (current)
        current->endSearch();
    ThumbsLoader::clearQueue();
    m_rootPath = path;
    Node *node = rootNode()->node(path);
    if ( !node )
        dataGatherer()->generateNode(path);
    else
        nodeGenerated(path, node);
}

void
FileSystemModel::refresh()
{
    Node *node = rootNode()->node(rootPath());
    if ( node )
        dataGatherer()->populateNode(node);
}

void
FileSystemModel::dirChanged(const QString &path)
{
    const QModelIndex &idx = index(path);
    if ( !idx.isValid() || !idx.parent().isValid() )
        return;

    Node *node = rootNode()->node(path);
    if ( !node )
        return;
    node->refresh();
    if ( !node->exists() )
    {
        beginRemoveRows(idx.parent(), node->row(), node->row());
        delete node;
        endRemoveRows();
    }
    else
        dataGatherer()->populateNode(node);
}

Qt::ItemFlags
FileSystemModel::flags(const QModelIndex &index) const
{
    Node *node = fromIndex(index);
    Qt::ItemFlags flags /*= QAbstractItemModel::flags(index)*/=Qt::ItemIsEnabled;
    if ( node->isWritable() ) flags |= Qt::ItemIsEditable;
    if ( node->isDir() ) flags |= Qt::ItemIsDropEnabled;
    if ( node->isReadable() && !isPopulating() ) flags |= Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    return flags;
}

void
FileSystemModel::forceEmitDataChangedFor(const QString &file)
{
    const QModelIndex &idx = index(file);
    emit dataChanged(idx, idx);
    emit flowDataChanged(idx, idx);
}

void
FileSystemModel::flowDataAvailable(const QString &file)
{
    const QModelIndex &idx = index(file);
    emit flowDataChanged(idx, idx);
}

void
FileSystemModel::thumbFor(const QString &file, const QString &iconName)
{
    const QModelIndex &idx = index(file);
    emit dataChanged(idx, idx);
    if ( m_container->currentViewType() == ViewContainer::Flow )
        if ( !QFileInfo(file).isDir() )
            m_it->queueFile(file, ThumbsLoader::thumb(file), true);
        else if ( !iconName.isNull() )
            m_it->queueName(QIcon::fromTheme(iconName));
}

bool FileSystemModel::hasThumb(const QString &file) { return ThumbsLoader::instance()->hasThumb(file); }

QVariant
FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()||index.column()>4||index.row()>100000)
        return QVariant();

    Node *node = fromIndex(index);
    const QString &file = node->filePath();
    const int col = index.column();
    if ( node == m_rootNode )
        return QVariant();
    if ( role == Qt::DecorationRole && col > 0 )
        return QVariant();
    if ( role == FileNameRole )
        return node->name();
    if ( role == FilePathRole )
        return node->filePath();

    if ( role == Qt::TextAlignmentRole )
        if ( col == 1 )
            return int(Qt::AlignVCenter|Qt::AlignRight);
        else
            return int(Qt::AlignLeft|Qt::AlignVCenter);

    if ( role == Qt::FontRole && !col )
    {
        QFont f(qApp->font());
        if ( node->isSymLink() )
        {
            f.setItalic(true);
            return f;
        }
        if ( !node->isDir() && node->isExecutable() && (node->suffix().isEmpty()||node->suffix()=="sh"||node->suffix()=="exe") )
        {
            f.setUnderline(true);
            return f;
        }
    }

    if (role == Qt::ToolTipRole)
    {
        if (node->isSymLink())
            return QString("Points to %1").arg(node->symLinkTarget());
        QImageReader ir(file);
        if (ir.canRead())
        {
            const QSize &sz(ir.size());
            return QString("Width: %1, Height: %2").arg(QString::number(sz.width()), QString::number(sz.height()));
        }
    }

//    if (node->isDir() && !col && role == Qt::ForegroundRole)
//    {
//        const QPalette &p(qApp->palette());
//        const QColor &fg = Ops::colorMid(p.color(QPalette::Highlight), Ops::colorMid(p.color(QPalette::Text), p.color(QPalette::Base)));
//        return fg;
//    }

    const bool customIcon = Store::config.icons.customIcons.contains(file);

    if ( col == 0 && role == Qt::DecorationRole && customIcon )
        return QIcon(Store::config.icons.customIcons.value(file));

    if ( role == Qt::DecorationRole )
    {
        if ( ThumbsLoader::hasThumb(file) )
            return QIcon(QPixmap::fromImage(ThumbsLoader::thumb(file)));
        else
        {
            if ( (Store::config.views.showThumbs || node->isDir()) && !isPopulating() )
                ThumbsLoader::queueFile(file);
            return m_ip->icon(*node);
        }
    }

    if ( role >= FlowImg && role != Category )
    {
        const QIcon &icon = customIcon ? QIcon(Store::config.icons.customIcons.value(file)) : m_ip->icon(*node);

        if ( m_it->hasData(file) )
            return QPixmap::fromImage(m_it->flowData(file, role == FlowRefl));

        if ( !ThumbsLoader::hasThumb(file) && Store::config.views.showThumbs )
            ThumbsLoader::queueFile(file);

        if ( (ThumbsLoader::hasThumb(file) && !m_it->hasData(file) && Store::config.views.showThumbs) && !isPopulating() )
            m_it->queueFile(file, ThumbsLoader::thumb(file));

        if ( m_it->hasNameData(icon.name()) )
            return QPixmap::fromImage(m_it->flowNameData(icon.name(), role == FlowRefl));
        else if ( !icon.name().isEmpty() )
            m_it->queueName(icon);

        if ( role == FlowRefl )
            return QPixmap();

        return icon.pixmap(SIZE);
    }

    if (role == Category)
        return node->category();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    return node->data(col);
}

bool
FileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() || role != Qt::EditRole )
        return false;

    Node *node = fromIndex(index);
    const QString &newName = value.toString();
    const QString &oldName = node->fileName();
    const QString &path = node->path();
    if ( node->rename(newName) )
    {
        emit fileRenamed(path, oldName, newName);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant
FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation != Qt::Horizontal )
        return QVariant();
    if ( role == Qt::DisplayRole )
        switch ( section )
        {
        case 0: return tr("Name"); break;
        case 1: return tr("Size"); break;
        case 2: return tr("Type"); break;
        case 3: return tr("Last Modified"); break;
        case 4: return tr("Permissions"); break;
        default: break;
        }
    else if ( role == Qt::TextAlignmentRole )
        return bool(section == 1) ? int(Qt::AlignRight|Qt::AlignVCenter) : int(Qt::AlignLeft|Qt::AlignVCenter);
    return QVariant();
}

QMimeData
*FileSystemModel::mimeData(const QModelIndexList &indexes) const
{
    QList<QUrl> urls;
    for ( int i = 0; i < indexes.count(); ++i )
    {
        const QModelIndex &index = indexes.at(i);
        if (!index.isValid() || index.column())
            continue;
        Node *node = fromIndex(index);
        urls << QUrl::fromLocalFile(node->filePath());
    }
    QMimeData *data = new QMimeData();
    data->setUrls(urls);
    return data;
}

int
FileSystemModel::rowCount(const QModelIndex &parent) const
{
    return fromIndex(parent)->childCount();
}

FileSystemModel::Node
*FileSystemModel::fromIndex(const QModelIndex &index) const
{
    Node *node = static_cast<Node *>(index.internalPointer());
    if (index.isValid() && index.row() < 100000 && index.column() < 5 && node)
        return node;
    return m_rootNode;
}

QModelIndex
FileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
//    if (!hasIndex(row, column, parent))
//        return QModelIndex();
//    qDebug() << "requesting index" << row << column << parent;

    Node *parentNode = fromIndex(parent);
    Node *childNode = parentNode->child(row);

    if (childNode)
        return createIndex(row, column, childNode);
    else
        return createIndex(row, column, parentNode);
}

QModelIndex
FileSystemModel::index(const QString &path) const
{
    Node *node = m_rootNode->node(path);

    if (!node)
        if (Node *rootPathNode = m_rootNode->node(rootPath()))
            if (rootPathNode->isPopulated())
                node = rootPathNode->child(path);

    if (!node)
        return QModelIndex();

    return createIndex(node->row(), 0, node);
}

QModelIndex
FileSystemModel::parent(const QModelIndex &child) const
{
    Node *childNode = fromIndex(child);
    Node *parentNode = childNode->parent();

    if (!parentNode)
        return QModelIndex();
    if (parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

bool
FileSystemModel::hasChildren(const QModelIndex &parent) const
{
    return fromIndex(parent)->isDir();
}

bool
FileSystemModel::canFetchMore(const QModelIndex &parent) const
{
    Node *node = fromIndex(parent);
    return node->isDir()&&!node->isSearchResult();
}

void
FileSystemModel::fetchMore(const QModelIndex &parent)
{
    if ( parent.isValid() && parent.column() != 0 )
        return;
    Node *node = fromIndex(parent);
    if ( !node->isPopulated() )
        dataGatherer()->populateNode(node);
}

void
FileSystemModel::sort(int column, Qt::SortOrder order)
{
    if ( rootPath().isEmpty() || isPopulating() )
        return;
    const bool orderChanged = bool(m_sortColumn!=column||m_sortOrder!=order);
    m_sortColumn = column;
    m_sortOrder = order;
    qDebug() << "sort called" << rootPath() << column << order;
    emit layoutAboutToBeChanged();
    const QModelIndexList &oldList = persistentIndexList();
    QList<QPair<int, Node *> > old;
    for ( int i = 0; i < oldList.count(); ++i )
        old << QPair<int, Node *>(oldList.at(i).column(), fromIndex(oldList.at(i)));

    rootNode()->sort();

    QModelIndexList newList;
    for ( int i = 0; i < old.count(); ++i )
    {
        QPair<int, Node *> n = old.at(i);
        Node *node = n.second;
        QModelIndex idx = index(node->filePath());
        if ( n.first > 0 )
            idx = idx.sibling(idx.row(), n.first);
        newList << idx;
    }
    changePersistentIndexList(oldList, newList);
    if ( orderChanged )
        emit sortingChanged(column, (int)order);
    emit layoutChanged();

#ifdef Q_WS_X11
    if ( Store::config.views.dirSettings && orderChanged )
    {
        QDir dir(rootPath());
        QSettings settings(dir.absoluteFilePath(".directory"), QSettings::IniFormat);
        settings.beginGroup("DFM");
        QVariant varCol = settings.value("sortCol");
        if ( varCol.isValid() )
        {
            int col = varCol.value<int>();
            if ( col != column )
                settings.setValue("sortCol", column);
        }
        else
            settings.setValue("sortCol", column);
        QVariant varOrd = settings.value("sortOrd");
        if ( varCol.isValid() )
        {
            Qt::SortOrder ord = (Qt::SortOrder)varOrd.value<int>();
            if ( ord != order )
                settings.setValue("sortOrd", order);
        }
        else
            settings.setValue("sortOrd", order);
        settings.endGroup();
    }
#endif
}

void
FileSystemModel::setHiddenVisible(bool visible)
{
    if ( visible == m_showHidden )
        return;

    m_showHidden = visible;
    rootNode()->setHiddenVisible(showHidden());
    emit hiddenVisibilityChanged(m_showHidden);
}

bool
FileSystemModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!data->hasUrls())
        return false;

    Node *node = fromIndex(parent);
    if ( !node->isDir() )
        return false;

    IO::Job::copy(data->urls(), node->filePath(), true, true);
    return true;
}

QModelIndex
FileSystemModel::mkdir(const QModelIndex &parent, const QString &name)
{
    if ( parent.isValid() )
    {
        Node *node = fromIndex(parent);
        QDir dir(node->filePath());
        dir.mkdir(name);
        return index(dir.absoluteFilePath(name));
    }
    return QModelIndex();
}

QString
FileSystemModel::filePath(const QModelIndex &index) const
{
    return fromIndex(index)->filePath();
}

void
FileSystemModel::startPopulating()
{
    QMutexLocker locker(&m_mutex);
    m_isPopulating = true;
}

void
FileSystemModel::endPopulating()
{
    QMutexLocker locker(&m_mutex);
    m_isPopulating = false;
}

QModelIndexList
FileSystemModel::category(const QString &cat)
{
    QModelIndexList categ;
    const QModelIndex &parent = index(m_rootPath);
    if ( !parent.isValid() )
        return categ;
    const int count = rowCount(parent);
    for ( int i = 0; i<count; ++i )
    {
        const QModelIndex &child = index(i, 0, parent);
        if ( !child.isValid() )
            continue;
        if ( fromIndex(child)->category() == cat )
            categ << child;
    }
    return categ;
}

QModelIndexList
FileSystemModel::category(const QModelIndex &fromCat)
{
    return category(fromIndex(fromCat)->category());
}

QModelIndex
FileSystemModel::first(const QString &cat)
{
    category(cat).first();
}

QModelIndex
FileSystemModel::last(const QString &cat)
{
    category(cat).last();
}

QStringList
FileSystemModel::categories()
{
    const QModelIndex &parent = index(m_rootPath);
//    if ( !parent.isValid() )
//        return QStringList();
    const int count = rowCount(parent);
    QStringList cats;
    for ( int i = 0; i<count; ++i )
    {
        const QModelIndex &child = index(i, 0, parent);
        if ( !child.isValid() )
            continue;
        if ( !cats.contains(fromIndex(child)->category()) )
            cats << fromIndex(child)->category();
    }
    return cats;
}

void
FileSystemModel::search(const QString &fileName)
{
    emit searchStarted();
    Node *node = rootNode()->node(m_rootPath);
    node->setFilter(QString());
    node->endSearch();
    dataGatherer()->search(fileName, node);
}

void
FileSystemModel::endSearch()
{
    Node *node = rootNode()->node(m_rootPath);
    node->endSearch();
    node->setHiddenVisible(showHidden());
}

void
FileSystemModel::cancelSearch()
{
    dataGatherer()->cancel();
}

QString
FileSystemModel::currentSearchString()
{
    Node *node = rootNode()->node(m_rootPath);
    return node?node->filter():QString();
}

void
FileSystemModel::removeDeletedLater()
{
//    QTimer::singleShot(500, this, SLOT(removeDeleted()));
}

void
FileSystemModel::removeDeleted()
{
    qDeleteAll(s_deletedNodes);
    s_deletedNodes.clear();
}
