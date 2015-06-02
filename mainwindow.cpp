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
#include <QAction>

#include "about.h"
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
    m_groundGenerator(new GenerateGround(this)),
    m_about(new QAction(this))
{
    //Set properties.
    setWindowIcon(QIcon("://res/icon.png"));
    setMinimumSize(500, 309);
    //Initial the scroll area.
    QScrollArea *groundArea=new QScrollArea(this);
    setCentralWidget(groundArea);
    groundArea->setAlignment(Qt::AlignCenter);
    groundArea->setWidget(m_ground);

    //Set the ground generator.
    m_ground->setGenerator(m_groundGenerator);

    //Give the ground to elements.
    m_panel->setGround(m_ground);
    m_robotManagement->setGround(m_ground);

    //Add action to menubar.
    m_menuBar->addCategoryAction(MenuBar::Help, m_about);
    //Give the menu bar to elements.
    m_ground->setMenuBar(m_menuBar);
    m_panel->setMenuBar(m_menuBar);
    m_robotManagement->setMenuBar(m_menuBar);

    //Add panel to bottom dock area.
    addDockWidget(Qt::RightDockWidgetArea, m_panel);
    m_panel->setAllowedAreas(Qt::LeftDockWidgetArea |
                             Qt::RightDockWidgetArea);

    //Configure the about action.
    connect(m_about,
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{About::showAbout(this);});
#ifdef Q_OS_MACX
    setWindowTitle("Robot Emulator");
#else
    setMenuBar(m_menuBar);
#endif

    retranslate();
}

void MainWindow::retranslate()
{
    m_about->setText(tr("About"));
}
