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
#include <QBoxLayout>
#include <QScrollArea>

#include "ground.h"
#include "paneldock.h"
#include "menubar.h"
#include "robotmanagement.h"
#include "generateground.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ground(new Ground(this)),
    m_panel(new PanelDock(this)),
    m_menuBar(new MenuBar(this)),
    m_robotManagement(new RobotManagement(this)),
    m_groundGenerator(new GenerateGround(this))
{
    //A hack, using box layout to make the ground the center.
    QWidget *container=new QWidget(this);
    //Set the central widget.
    setCentralWidget(container);
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          container);
    container->setLayout(mainLayout);
    //Add ground and panel to layout.
    mainLayout->addWidget(m_ground, 1, Qt::AlignCenter);
    mainLayout->addWidget(m_panel);

    //Set the ground generator.
    m_ground->setGenerator(m_groundGenerator);

    //Give the ground to elements.
    m_panel->setGround(m_ground);
    m_robotManagement->setGround(m_ground);

    //Give the menu bar to elements.
    m_ground->setMenuBar(m_menuBar);
    m_panel->setMenuBar(m_menuBar);
    m_robotManagement->setMenuBar(m_menuBar);

    //Add panel to bottom dock area.
    addDockWidget(Qt::RightDockWidgetArea, m_panel);
    m_panel->setAllowedAreas(Qt::LeftDockWidgetArea |
                             Qt::RightDockWidgetArea);
#ifdef Q_OS_MACX
    setWindowTitle("Robot Emulator");
#else
    setMenuBar(m_menuBar);
#endif
}
