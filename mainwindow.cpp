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
#include <QAction>
#include <QScrollArea>
#include <QScrollBar>

#include "about.h"
#include "gridwidget.h"
#include "ground.h"
#include "groundglobal.h"
#include "paneldock.h"
#include "menubar.h"
#include "robotmanagement.h"
#include "enemymanagement.h"
#include "generateground.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ground(new Ground(this)),
    m_panel(new PanelDock(this)),
    m_menuBar(new MenuBar(this)),
    m_robotManagement(new RobotManagement(this)),
    m_enemyManagement(new EnemyManagement(this)),
    m_groundGenerator(new GenerateGround(this)),
    m_about(new QAction(this))
{
    //Set properties.
    setWindowIcon(QIcon("://res/icon.png"));
    setMinimumSize(500, 309);
    //Initial the grid widget.
    GridWidget *gridWidget=new GridWidget(this);
    connect(m_ground->showCoordinate(),
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            gridWidget, &GridWidget::setCoordinate);
    setCentralWidget(gridWidget);
    //Initial the scroll area.
    QScrollArea *groundArea=new QScrollArea(this);
    gridWidget->setWidget(groundArea);
    groundArea->setAutoFillBackground(true);
    groundArea->setAlignment(Qt::AlignCenter);
    groundArea->setWidget(m_ground);
    groundArea->setFrameStyle(QFrame::NoFrame);
    groundArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {"
                                                   "   border: 0px solid grey;"
                                                   "   background: rgba(0, 0, 0, 0);"
                                                   "   width: 8px;"
                                                   "}"
                                                   "QScrollBar::handle:vertical {"
                                                   "   background: rgba(100, 100, 100);"
                                                   "   min-height: 10px;"
                                                   "   border-radius: 4px;"
                                                   "}"
                                                   "QScrollBar::add-line:vertical {"
                                                   "   border: 0px solid grey;"
                                                   "   background: rgba(0, 0, 0, 100);"
                                                   "   height: 0px;"
                                                   "   subcontrol-position: down;"
                                                   "   subcontrol-origin: margin;"
                                                   "}"
                                                   "QScrollBar::sub-line:vertical {"
                                                   "   border: 0px solid grey;"
                                                   "   background: rgba(0, 0, 0, 100);"
                                                   "   height: 0px;"
                                                   "   subcontrol-position: up;"
                                                   "   subcontrol-origin: margin;"
                                                   "}");
    groundArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {"
                                                     "   border: 0px solid grey;"
                                                     "   background: rgba(0, 0, 0, 0);"
                                                     "   height: 8px;"
                                                     "}"
                                                     "QScrollBar::handle:horizontal {"
                                                     "   background: rgba(100, 100, 100);"
                                                     "   min-height: 10px;"
                                                     "   border-radius: 4px;"
                                                     "}"
                                                     "QScrollBar::add-line:horizontal {"
                                                     "   border: 0px solid grey;"
                                                     "   background: rgba(0, 0, 0, 100);"
                                                     "   width: 0px;"
                                                     "   subcontrol-position: down;"
                                                     "   subcontrol-origin: margin;"
                                                     "}"
                                                     "QScrollBar::sub-line:horizontal {"
                                                     "   border: 0px solid grey;"
                                                     "   background: rgba(0, 0, 0, 100);"
                                                     "   width: 0px;"
                                                     "   subcontrol-position: up;"
                                                     "   subcontrol-origin: margin;"
                                                     "}");

    //Update the palette of ground area.
    QPalette pal=groundArea->palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    groundArea->setPalette(pal);
    pal=gridWidget->palette();
    pal.setColor(QPalette::Window, GroundGlobal::instance()->baseColor());
    gridWidget->setPalette(pal);
    //Link the color changed signal.
    connect(GroundGlobal::instance(), &GroundGlobal::baseColorChanged,
            [=](const QColor &color)
            {
                QPalette pal=gridWidget->palette();
                pal.setColor(QPalette::Window, color);
                gridWidget->setPalette(pal);
            });

    //Set the ground generator.
    m_ground->setGenerator(m_groundGenerator);

    //Give the ground to elements.
    m_panel->setGround(m_ground);
    m_robotManagement->setGround(m_ground);
    m_enemyManagement->setGround(m_ground);

    //Add action to menubar.
    m_menuBar->addCategoryAction(MenuBar::Help, m_about);
    //Give the menu bar to elements.
    m_ground->setMenuBar(m_menuBar);
    m_panel->setMenuBar(m_menuBar);
    m_robotManagement->setMenuBar(m_menuBar);
    m_enemyManagement->setMenuBar(m_menuBar);

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
