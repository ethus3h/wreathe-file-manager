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


#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QLineEdit>
#include <QPixmap>
#include <QDebug>
#include <QToolButton>
#include <QTimer>

namespace DFM
{

enum SearchMode { Filter = 0, Search = 1 };


class SearchIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit SearchIndicator(QWidget *parent = 0);

public slots:
    inline void start() { setVisible(true); m_timer->start(); }
    inline void stop() { setVisible(false); m_timer->stop(); }

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void animate();

private:
    QPixmap m_pixmap;
    bool m_in;
    int m_step;
    QTimer *m_timer;
};

class SearchBox;
class SearchTypeSelector : public QToolButton
{
    Q_OBJECT
public:
    explicit SearchTypeSelector(QWidget *parent = 0);

signals:
    void modeChanged(const SearchMode mode);

private slots:
    inline void filter() { emit modeChanged(Filter); }
    inline void search() { emit modeChanged(Search); }
    void cancel();
    void closeSearch();
};

class ClearSearch : public QToolButton
{
    Q_OBJECT
public:
    explicit ClearSearch(QWidget *parent = 0);
};

class SearchBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchBox(QWidget *parent = 0);
    inline SearchMode mode() { return m_mode; }

public slots:
    void setMode(const SearchMode mode);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void setClearButtonEnabled(QString filter) { m_clearSearch->setVisible(!filter.isEmpty()); }
    void search();

private:
    SearchTypeSelector *m_selector;
    ClearSearch *m_clearSearch;
    int m_margin;
    SearchMode m_mode;
};

}

#endif // SEARCHBOX_H