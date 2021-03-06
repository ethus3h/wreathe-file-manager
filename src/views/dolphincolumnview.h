/***************************************************************************
 *   Copyright (C) 2007-2009 by Peter Penz <peter.penz19@gmail.com>        *
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

#ifndef DOLPHINCOLUMNVIEW_H
#define DOLPHINCOLUMNVIEW_H

#include "dolphinview.h"
#include "dolphintreeview.h"

#include <QFont>
#include <QSize>
#include <QStyleOption>

#include <KUrl>

class DolphinColumnViewContainer;
class DolphinModel;
class DolphinSortFilterProxyModel;
class DolphinDirLister;
class KFileItem;
class QLabel;
class SelectionManager;
class ViewExtensionsFactory;

/**
 * Represents one column inside the DolphinColumnViewContainer.
 */
class DolphinColumnView : public DolphinTreeView
{
    Q_OBJECT

public:
    DolphinColumnView(QWidget* parent,
                      DolphinColumnViewContainer* container,
                      const KUrl& url);
    virtual ~DolphinColumnView();

    /**
     * An active column is defined as column, which shows the same URL
     * as indicated by the URL navigator. The active column is usually
     * drawn in a lighter color. All operations are applied to this column.
     */
    void setActive(bool active);
    bool isActive() const;

    /**
     * Sets the directory URL of the child column that is shown next to
     * this column. This property is used for a visual indication
     * of the shown directory, it does not trigger a loading of the model.
     * When no url is selected and the user presses right, then child
     * url will be used as column.
     */
    void setChildUrl(const KUrl& url);
    KUrl childUrl() const;

    /** Sets the directory URL that is shown inside the column widget. */
    void setUrl(const KUrl& url);

    /** Returns the directory URL that is shown inside the column widget. */
    KUrl url() const;

    /**
     * Updates the background color dependent from the activation state
     * \a isViewActive of the column view.
     */
    void updateBackground();

    /**
     * Returns the item on the position \a pos. The KFileItem instance
     * is null if no item is below the position.
     */
    KFileItem itemAt(const QPoint& pos) const;

    virtual void setSelectionModel(QItemSelectionModel* model);

protected:
    virtual QStyleOptionViewItem viewOptions() const;
    virtual bool event(QEvent* event);
    virtual void startDrag(Qt::DropActions supportedActions);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dragMoveEvent(QDragMoveEvent* event);
    virtual void dropEvent(QDropEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void contextMenuEvent(QContextMenuEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous);
    virtual QRect visualRect(const QModelIndex& index) const;
    virtual bool acceptsDrop(const QModelIndex& index) const;
    virtual bool eventFilter(QObject* watched, QEvent* event);

private slots:
    void setZoomLevel(int level);

    void slotEntered(const QModelIndex& index);
    void requestActivation();
    void updateFont();

    void slotShowPreviewChanged();

    void slotDirListerCompleted();

private:
    /** Used by DolphinColumnView::setActive(). */
    void activate();

    /** Used by DolphinColumnView::setActive(). */
    void deactivate();

    void updateDecorationSize(bool showPreview);

private:
    bool m_active;
    DolphinColumnViewContainer* m_container;
    SelectionManager* m_selectionManager;
    ViewExtensionsFactory* m_extensionsFactory;
    KUrl m_url;      // URL of the directory that is shown
    KUrl m_childUrl; // URL of the next column that is shown

    QFont m_font;
    QSize m_decorationSize;

    DolphinDirLister* m_dirLister;
    DolphinModel* m_dolphinModel;
    DolphinSortFilterProxyModel* m_proxyModel;

    QLabel* m_resizeWidget;
    int m_resizeXOrigin;

    friend class DolphinColumnViewContainer;
};

#endif
