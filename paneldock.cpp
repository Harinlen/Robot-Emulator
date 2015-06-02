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
#include <QAction>

#include "panel.h"
#include "menubar.h"

#include "paneldock.h"

PanelDock::PanelDock(QWidget *parent) :
    QDockWidget(parent),
    m_panel(new Panel(this)),
    m_dockVisible(new QAction(this))
{
    //Set the widget.
    setWidget(m_panel);
    //Set the action.
    m_dockVisible->setCheckable(true);
    connect(m_dockVisible,
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{isVisible()?hide():show();});

    retranslate();
}

void PanelDock::setGround(GroundBase *ground)
{
    m_panel->setGround(ground);
}

void PanelDock::setMenuBar(MenuBar *menuBar)
{
    m_panel->setMenuBar(menuBar);
    //Add dock visible action to menu bar.
    menuBar->addCategoryAction(MenuBar::View, m_dockVisible);
}

void PanelDock::hideEvent(QHideEvent *event)
{
    //Hide the dock.
    QDockWidget::hideEvent(event);
    //Update the action.
    m_dockVisible->setChecked(false);
}

void PanelDock::showEvent(QShowEvent *event)
{
    //Hide the dock.
    QDockWidget::showEvent(event);
    //Update the action.
    m_dockVisible->setChecked(true);
}

void PanelDock::retranslate()
{
    setWindowTitle(tr("Timeline"));
    m_dockVisible->setText(tr("Control Dock"));
}
