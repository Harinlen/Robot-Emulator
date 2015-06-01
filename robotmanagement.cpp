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
#include <QStackedLayout>
#include <QMessageBox>

#include "ground.h"
#include "robot.h"
#include "menubar.h"
#include "robotaddwidget.h"
#include "groundrealtimepreviewer.h"

#include "robotmanagement.h"

#include <QDebug>

RobotManagement::RobotManagement(QWidget *parent) :
    QWidget(parent),
    m_ground(nullptr),
    m_groundPreview(new GroundRealtimePreviewer(this)),
    m_stackLayout(new QStackedLayout),
    m_robotAdd(new RobotAddWidget(this))
{
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    setLayout(mainLayout);
    mainLayout->addWidget(m_groundPreview);
    mainLayout->addLayout(m_stackLayout, 1);
    //If we are using Mac OS X, using the sheet window flag.
#ifdef Q_OS_MACX
    setWindowFlags(Qt::Sheet);
#else
    setWindowFlags(Qt::Dialog);
#endif

    //Initial the actions.
    for(int i=0; i<RobotManagementActionsCount; i++)
    {
        m_actions[i]=new QAction(this);
    }
    //Set shortcuts.
    m_actions[AddRobot]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_D));
    m_actions[ManageRobot]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));

    //Link the slots.
    connect(m_actions[AddRobot],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{addRobot();});
    connect(m_actions[ManageRobot],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{manageRobot();});

    //Generate the add robot widget.
    connect(m_robotAdd, &RobotAddWidget::requireClose,
            this, &RobotManagement::close);
    connect(m_robotAdd, &RobotAddWidget::requirePreviewRobot,
            m_groundPreview, &GroundRealtimePreviewer::previewRobot);
    connect(m_robotAdd, &RobotAddWidget::requireAddRobot,
            this, &RobotManagement::onActionAddRobot);
    m_stackLayout->addWidget(m_robotAdd);

    //Retranslate.
    retranslate();
}

GroundBase *RobotManagement::ground()
{
    return m_ground;
}

void RobotManagement::addRobot()
{
    //Show the add robot widget.
    m_stackLayout->setCurrentWidget(m_robotAdd);
    //Make the preview show the preview robot.
    m_groundPreview->setShowPreviewPoint(true);
    //Update the range.
    QRectF barracksBoundingRect=m_ground->barracks().boundingRect();
    m_robotAdd->updateXAndYRange(barracksBoundingRect.left(),
                                 barracksBoundingRect.top(),
                                 barracksBoundingRect.right(),
                                 barracksBoundingRect.bottom());
    //Show the dialog.
    show();
}

void RobotManagement::manageRobot()
{
    //Hide the preview robot.
    m_groundPreview->setShowPreviewPoint(false);
    //Show the dialog.
    show();
}

void RobotManagement::setGround(GroundBase *ground)
{
    //Save the ground.
    m_ground = ground;
    //Give the ground to the preview.
    m_groundPreview->setGround(m_ground);
}

void RobotManagement::setMenuBar(MenuBar *menuBar)
{
    menuBar->addCategoryAction(MenuBar::Robot, m_actions[AddRobot]);
    menuBar->addCategoryAction(MenuBar::Robot, m_actions[ManageRobot]);
}

void RobotManagement::retranslate()
{
    m_actions[AddRobot]->setText(tr("Add robot"));
    m_actions[ManageRobot]->setText(tr("Manage robots"));
}

void RobotManagement::onActionAddRobot(const QPointF &position,
                                       const qreal &angle)
{
    //Generate a robot.
    Robot *robot=new Robot(position);
    robot->setAngle(angle);
    //Try to add the robot, if sucessful, close the dialog.
    if(m_ground->addRobot(robot))
    {
        close();
        return;
    }
    //Or else, display an error information.
    QMessageBox::warning(this,
                         tr("Add Robot Failed"),
                         tr("The robot is not in the barracks, you cannot add this robot"));
}
