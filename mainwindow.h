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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "filesystemmodel.h"
#include "placesview.h"
#include "iconview.h"
#include "detailsview.h"
#include "columnsview.h"
#include "flowview.h"
#include "searchbox.h"
#include "viewcontainer.h"
#include "tabbar.h"
#include "config.h"
#include "infowidget.h"
#include "dockwidget.h"
#include "recentfoldersview.h"

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QProcess>

namespace DFM
{

class TabBar;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow( QStringList arguments = QStringList() );
    inline ViewContainer *activeContainer() { return m_activeContainer; }
    inline PlacesView *placesView() { return m_placesView; }
    inline QToolBar *toolBar() { return m_toolBar; }
    static ViewContainer *currentContainer();
    static MainWindow *currentWindow();
    static MainWindow *window( QWidget *w ) { QWidget *p = w; while ( p->parentWidget() ) p = p->parentWidget(); return static_cast<MainWindow *>(p); }
    static PlacesView *places();
    static QList<MainWindow *> openWindows();
    inline ViewContainer *containerForTab( int tab ) { return static_cast<ViewContainer *>(m_stackedWidget->widget(tab)); }
    inline InfoWidget *infoWidget() { return m_infoWidget; }
    inline QList<QAction *> acts() const { return m_actions; }
    inline QMenu *mainMenu() { return m_mainMenu; }
    inline QSlider *iconSizeSlider() { return m_iconSizeSlider; }
    void updateConfig();
    void createMenus();

public slots:
    void addTab(const QString &path = QDir::homePath());
    void receiveMessage(const QStringList &message);
    inline void setRootPath( const QString &path ) { m_model->setPath(path); }

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *ke);
    void windowActivationChange(bool wasActive);
    void showEvent(QShowEvent *e);
    void createActions();
    void createToolBars();
    void writeSettings();
    void createSlider();

private slots:
    void updateIcons();
    void goHome();
    void goBack();
    void goUp();
    void goForward();
    void setViewIcons();
    void setViewDetails();
    void setViewCols();
    void filterCurrentDir(const QString &filter);
    void checkViewAct();
    void customCommand();
    void toggleMenuVisible();
    void toggleStatusVisible();
    void about();
    void deleteCurrentSelection();
    void toggleHidden();
    void cutSelection();
    void copySelection();
    void setClipBoard();
    void pasteSelection();
    void rename();
    void flowView();
    void refreshView();
    void genPlace();
    void mainSelectionChanged(QItemSelection selected,QItemSelection notselected);
    void togglePath();
    void rootPathChanged(const QString &path);
    void updateStatusBar(const QString &path);
    void createDirectory();
    void setViewIconSize(int);
    void setSliderPos(int size);
    void tabChanged(int currentIndex);
    void setActions();
    void openTab();
    void newTab();
    void viewItemHovered( const QModelIndex &index );
    void viewClearHover();
    void showSettings();
    void fileProperties();
    void hidePath();
    void tabClosed(int);
    void tabMoved(int,int);
    void stackChanged(int);
    void newWindow() { (new MainWindow(QStringList() << m_activeContainer->model()->rootPath()))->show(); }
    void readSettings();
    void activatePath(const QString &folder) { m_activeContainer->model()->setPath(folder); }
    void addBookmarks();
    void setSorting();
    void sortingChanged(const int column, const Qt::SortOrder order);

signals:
    void viewChanged( QAbstractItemView *view );
    void settingsChanged();

private:
    QStatusBar *m_statusBar;
    QWidget *m_toolBarSpacer;
    SearchBox *m_filterBox;
    Docks::DockWidget *m_dockLeft, *m_dockRight, *m_dockBottom;
    QString statusMessage, slctnMessage, m_appPath;
    QItemSelection *currentSelection;
    QSlider *m_iconSizeSlider;
    QLayout *m_statusLayout;
    InfoWidget *m_infoWidget;
    RecentFoldersView *m_recentFoldersView;
    FileSystemModel *m_model;
    ViewContainer *m_activeContainer;
    PlacesView *m_placesView;
    QStackedWidget *m_stackedWidget;
    TabBar *m_tabBar;
    QMainWindow *m_tabWin;
    QList<QAction *> m_actions;
    QMenu *m_mainMenu;
    QToolButton *m_sortButton;

    QMenu *m_fileMenu, *m_editMenu, *m_goMenu, *m_viewMenu, *m_helpMenu;
    QToolBar *m_toolBar;

    /* needs to get converted to enum / array */
    QAction *m_goBackAct, *m_goUpAct, *m_goForwardAct, *m_homeAct, *m_exitAct, *m_aboutAct, *m_aboutQtAct
    , *m_placeAct, *m_renPlaceAct, *m_rmPlaceAct, *m_iconViewAct, *m_listViewAct, *m_colViewAct, *m_delCurrentSelectionAct
    , *m_showHiddenAct, *m_mkDirAct, *m_copyAct, *m_cutAct, *m_pasteAct, *m_renameAct, *m_placeContAct, *m_flowAct, *m_refreshAct
    , *m_placeIconAct, *m_cstCmdAct, *m_menuAct, *m_statAct, *m_pathVisibleAct, *m_addTabAct, *m_openInTabAct, *m_configureAct, *m_propertiesAct
    , *m_pathEditAct, *m_newWindowAct, *m_sortNameAct, *m_sortSizeAct, *m_sortDateAct, *m_sortTypeAct, *m_sortDescAct;

    QActionGroup *m_sortActs;

    bool m_cut;
};

}
#endif
