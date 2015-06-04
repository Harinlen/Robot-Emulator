/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef PANELDOCK_H
#define PANELDOCK_H

#include <QDockWidget>

class Panel;
class GroundBase;
class MenuBar;
/*!
 * \brief The PanelDock class provide a dock which containes a Panel class.
 */
class PanelDock : public QDockWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Contruct a Panel dock widget.
     * \param parent The main windows of the dock.
     */
    explicit PanelDock(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief The proxy set function of the panel widget.
     * \param ground The controlled ground class.
     */
    void setGround(GroundBase *ground);

    /*!
     * \brief The proxy set function of the panel widget.
     * \param menuBar The menu bar.
     */
    void setMenuBar(MenuBar *menuBar);

protected:
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void retranslate();

private:
    Panel *m_panel;
    QAction *m_dockVisible;
};

#endif // PANELDOCK_H
