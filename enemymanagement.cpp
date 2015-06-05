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
#include <QGroupBox>

#include "ground.h"
#include "enemy.h"
#include "menubar.h"
#include "enemyaddwidget.h"
#include "enemymanagewidget.h"
#include "groundrealtimepreviewer.h"

#include "enemymanagement.h"

#include <QDebug>

EnemyManagement::EnemyManagement(QWidget *parent) :
    QDialog(parent),
    m_previewGroup(new QGroupBox(this)),
    m_ground(nullptr),
    m_groundPreview(new GroundRealtimePreviewer(this)),
    m_stackLayout(new QStackedLayout),
    m_enemyAdd(new EnemyAddWidget(this)),
    m_enemyManage(new EnemyManageWidget(this))
{
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    setLayout(mainLayout);
    QBoxLayout *previewLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_previewGroup);
    m_groundPreview->setShowPreviewPoint(false);
    m_groundPreview->setDisplayRobots(false);
    previewLayout->addWidget(m_groundPreview);
    m_previewGroup->setLayout(previewLayout);
    mainLayout->addWidget(m_previewGroup);
    mainLayout->addLayout(m_stackLayout, 1);
    //If we are using Mac OS X, using the sheet window flag.
#ifdef Q_OS_MACX
    setWindowFlags(Qt::Sheet);
#else
    setWindowFlags(Qt::Dialog);
#endif

    //Initial the actions.
    for(int i=0; i<EnemyManagementActionsCount; i++)
    {
        m_actions[i]=new QAction(this);
    }
    //Set shortcuts.
    m_actions[AddEnemy]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
    m_actions[ManageEnemy]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_X));

    //Link the slots.
    connect(m_actions[AddEnemy],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{addEnemy();});
    connect(m_actions[ManageEnemy],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{manageEnemy();});

    //Generate the add robot widget.
    connect(m_enemyAdd, &EnemyAddWidget::requireClose,
            this, &EnemyManagement::close);
    connect(m_enemyAdd, &EnemyAddWidget::requirePreviewEnemy,
            m_groundPreview, &GroundRealtimePreviewer::previewEnemy);
    connect(m_enemyAdd, &EnemyAddWidget::requireAddEnemy,
            this, &EnemyManagement::onActionAddEnemy);
    m_stackLayout->addWidget(m_enemyAdd);

    //Configure the robot management widget.
    connect(m_enemyManage, &EnemyManageWidget::requireClose,
            this, &EnemyManagement::close);
    connect(m_enemyManage, &EnemyManageWidget::requireSelectEnemy,
            m_groundPreview, &GroundRealtimePreviewer::setSelectedEnemy);
    connect(m_enemyManage, &EnemyManageWidget::requireUpdateAllList,
            [=]
            {
                m_groundPreview->setEnemyPreviewList(m_enemyManage->enemyList(),
                                                      m_enemyManage->positionList());
            });
    connect(m_enemyManage, &EnemyManageWidget::requireUpdatePositionList,
            [=]
            {
                m_groundPreview->setPositionList(m_enemyManage->positionList());
            });
    m_stackLayout->addWidget(m_enemyManage);

    //Retranslate.
    retranslate();
}

GroundBase *EnemyManagement::ground()
{
    return m_ground;
}

void EnemyManagement::addEnemy()
{
    //Check the ground first. If it don't have a border, or barracks, we don't
    //need to popup this dialog.
    if(m_ground->border().isEmpty())
    {
        return;
    }
    //Show the add robot widget.
    m_enemyManage->disabledWidget();
    m_enemyAdd->enabledWidget();
    m_stackLayout->setCurrentWidget(m_enemyAdd);
    //Make the preview show the preview robot.
    m_groundPreview->setShowPreviewEnemy(true);
    //Hide the exist robot.
    m_groundPreview->setDisplayEnemys(false);
    //Show the dialog.
    show();
}

void EnemyManagement::manageEnemy()
{
    //Check the ground first. If it don't have a border, or barracks, we don't
    //need to popup this dialog.
    if(m_ground->border().isEmpty())
    {
        return;
    }
    //When you manage the robot, we must reset the ground.
    m_ground->reset();
    //Show and enabled the manage widget, disabled the add widget.
    m_enemyAdd->disabledWidget();
    m_enemyManage->enabledWidget();
    m_stackLayout->setCurrentWidget(m_enemyManage);
    //Hide the preview robot.
    m_groundPreview->setShowPreviewEnemy(false);
    //Show the exist robots.
    m_groundPreview->setDisplayEnemys(true);
    //Show the dialog.
    show();
}

void EnemyManagement::setGround(GroundBase *ground)
{
    //Save the ground.
    m_ground = ground;
    //Give the ground to the preview.
    m_groundPreview->setGround(m_ground);
    //Give the ground to the manage widget.
    m_enemyManage->setGround(m_ground);
}

void EnemyManagement::setMenuBar(MenuBar *menuBar)
{
    menuBar->addCategoryAction(MenuBar::Robot, m_actions[AddEnemy]);
    menuBar->addCategoryAction(MenuBar::Robot, m_actions[ManageEnemy]);
}

void EnemyManagement::retranslate()
{
    m_actions[AddEnemy]->setText(tr("Add enemy"));
    m_actions[ManageEnemy]->setText(tr("Manage enemys"));

    m_previewGroup->setTitle(tr("Previewer"));
}

void EnemyManagement::onActionAddEnemy(const QPointF &position)
{
    //Generate a robot.
    Enemy *enemy=new Enemy(position);
    //Try to add the robot, if sucessful, close the dialog.
    if(m_ground->addEnemy(enemy))
    {
        close();
        return;
    }
    //Or else, display an error information.
    QMessageBox::warning(this,
                         tr("Add Enemy Failed"),
                         tr("The enemy is not out of the border, or the position has already got an enemy.\n"
                            "Please change the position of the enemy."));
}
