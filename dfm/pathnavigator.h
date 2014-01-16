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


#ifndef PATHNAVIGATOR_H
#define PATHNAVIGATOR_H

#include <QToolBar>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QComboBox>
#include <QStyleOptionComboBox>
#include <QEvent>
#include <QBoxLayout>

#include "filesystemmodel.h"
#include "operations.h"

namespace DFM
{
class PathNavigator;
class BreadCrumbs;
class PathBox;
class NavButton : public QWidget
{
    Q_OBJECT
public:
    NavButton(QWidget *parent = 0, const QString &path = QString(), const QString &text = QString());
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

signals:
    void navPath(const QString &path);
    void released();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dropEvent(QDropEvent *);
    void paintEvent(QPaintEvent *);

private:
    QString m_path, m_text;
    PathNavigator *m_nav;
    bool m_hasData, m_hasPress;
    int m_margin;
};

class Menu : public QMenu
{
    Q_OBJECT
public:
    inline explicit Menu(QWidget *parent = 0) : QMenu(parent) {}
protected:
    virtual void mousePressEvent(QMouseEvent *e);
signals:
    void newTabRequest( const QString &path );
};

class PathSeparator : public QWidget
{
    Q_OBJECT
public:
    inline explicit PathSeparator(QWidget *parent, const QString &path = QString(), FileSystemModel *fsModel = 0)
        : QWidget(parent)
        , m_path(path)
        , m_fsModel(fsModel)
        , m_nav(qobject_cast<PathNavigator *>(parent))
    {
        setAttribute(Qt::WA_Hover);
        setFixedSize(6, 7);
    }

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void leaveEvent(QEvent *e) {QWidget::leaveEvent(e); update(); }

private slots:
    void setPath();

private:
    QString m_path;
    PathNavigator *m_nav;
    FileSystemModel *m_fsModel;
};

class PathBox : public QComboBox
{
    Q_OBJECT
public:
    inline explicit PathBox(QWidget *parent = 0) : QComboBox(parent) {}
signals:
    void cancelEdit();
protected:
    void keyPressEvent(QKeyEvent *e) { if ( e->key() == Qt::Key_Escape ) emit cancelEdit(); else QComboBox::keyPressEvent(e); }
};

class PathNavigator : public QWidget
{
    Q_OBJECT
public:
    explicit PathNavigator(QWidget *parent = 0, FileSystemModel *model = 0);
    inline QString path() const { return m_path; }
    inline FileSystemModel *model() { return m_fsModel; }
    inline QList<NavButton *> navButtons() { return QList<NavButton *>(findChildren<NavButton *>()); }
    inline int count() { return navButtons().count(); }
    QSize sizeHint() const;
    
signals:
    void pathChanged(const QString &path);
    void edit();

protected:
    void mousePressEvent(QMouseEvent *e) { QWidget::mousePressEvent(e); m_hasPress = true; }
    void mouseReleaseEvent(QMouseEvent *e) { QWidget::mouseReleaseEvent(e); if ( m_hasPress ) emit edit(); m_hasPress = false; }
    void clear();

private slots:
     void genNavFromPath(const QString &path);

private:
    QString m_path;
    QStringList m_pathList;
    FileSystemModel *m_fsModel;
    QHBoxLayout *m_layout;
    QList<QWidget *> m_widgets;
    BreadCrumbs *m_bc;
    bool m_hasPress;
};

class BreadCrumbs : public QStackedWidget
{
    Q_OBJECT
public:
    explicit BreadCrumbs( QWidget *parent = 0, FileSystemModel *fsModel = 0 );
    inline void setEditable( const bool editable ) { setCurrentWidget( editable ? static_cast<QWidget *>(m_pathBox) : static_cast<QWidget *>(m_pathNav) ); currentWidget()->setFocus(); }
    inline QString currentPath() { return m_pathNav->path(); }
    inline bool isEditable() { return currentWidget() == static_cast<QWidget *>(m_pathBox); }
    inline PathNavigator *pathNav() { return m_pathNav; }
    inline PathBox *pathBox() { return m_pathBox; }
    QSize sizeHint() const;
public slots:
    void setRootPath( const QString &rootPath );
    inline void toggleEditable() { currentWidget() == m_pathNav ? setEditable(true) : setEditable(false); }
    void complete( const QString &path);
    void pathChanged( const QString &path );
signals:
    void newPath( const QString &path );
protected:
    bool eventFilter(QObject *o, QEvent *e)
    {
        if ( o == m_pathBox )
        {
            if ( e->type() == QEvent::Paint )
            {
                QPainter p(m_pathBox);
                p.setRenderHint(QPainter::Antialiasing);
                p.setPen(Qt::NoPen);
                p.setBrush(m_pathBox->palette().color(m_pathBox->foregroundRole()));
                QStyleOptionComboBox opt;
                opt.initFrom(m_pathBox);
                QRect r = style()->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxArrow);
                QPolygon triangle(3);
                int w = r.right(), x = r.x(), y = r.y(), h = r.bottom();
                triangle.putPoints(0, 3,   x,y,   w,y,    r.center().x(),h);
                QPainterPath path;
                path.addPolygon(triangle);
                path.closeSubpath();
                p.drawPath(path);
                p.end();
                return true;
            }
            // WHY THE FUCK CANT THE FOLLOWING COMPILE?!?!? 0_o
//            else if ( e->type() == QEvent::KeyRelease )
//                if ( static_cast<QKeyEvent *>(e)->key() == Qt::Key_Escape )
//                    return true;
        }
        return QStackedWidget::eventFilter(o, e);
    }
private:
    PathBox *m_pathBox;
    PathNavigator *m_pathNav;
    FileSystemModel *m_fsModel;
};

}


#endif // PATHNAVIGATOR_H