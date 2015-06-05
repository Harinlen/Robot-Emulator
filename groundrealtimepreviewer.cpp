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
#include <QPainter>
#include "groundbase.h"
#include "robot.h"
#include "enemy.h"

#include "groundrealtimepreviewer.h"

#include <QDebug>

GroundRealtimePreviewer::GroundRealtimePreviewer(QWidget *parent) :
    GroundPreviewer(parent),
    m_proxyRobot(new Robot()),
    m_proxyEnemy(new Enemy()),
    m_ground(nullptr),
    m_displayRobots(false),
    m_displayEnemies(false),
    m_selectedRobot(nullptr),
    m_selectedEnemy(nullptr)
{
}

GroundRealtimePreviewer::~GroundRealtimePreviewer()
{
    delete m_proxyRobot;
}

void GroundRealtimePreviewer::setGround(GroundBase *ground)
{
    //Save the ground.
    m_ground=ground;
    //Link the ground.
    connect(m_ground, &GroundBase::groundSizeChanged,
            [=](const QSize &groundSize)
            {
                //Update the ground size.
                onActionGroundSizeChanged(groundSize);
                //Update the border and barracks.
                onActionBorderChanged();
                onActionBarracksChanged();
            });
    connect(m_ground, &GroundBase::borderChanged,
            this, &GroundRealtimePreviewer::onActionBorderChanged);
    connect(m_ground, &GroundBase::barracksChanged,
            this, &GroundRealtimePreviewer::onActionBarracksChanged);
}

void GroundRealtimePreviewer::onActionBorderChanged()
{
    //Check if the ground is available.
    if(m_ground==nullptr)
    {
        return;
    }
    //Set the ground border.
    setPreviewBorder(m_ground->border());
}

void GroundRealtimePreviewer::onActionBarracksChanged()
{
    //Check if the ground is available.
    if(m_ground==nullptr)
    {
        return;
    }
    //Set the barracks polygon.
    setPreviewBarracks(m_ground->barracks());
}

bool GroundRealtimePreviewer::displayEnemies() const
{
    return m_displayEnemies;
}

void GroundRealtimePreviewer::setDisplayEnemies(bool displayEnemies)
{
    //Save the display enemies data.
    m_displayEnemies = displayEnemies;
    //Update.
    update();
}

bool GroundRealtimePreviewer::displayRobots() const
{
    return m_displayRobots;
}

void GroundRealtimePreviewer::setDisplayRobots(bool displayRobots)
{
    //Save the display robot settings.
    m_displayRobots=displayRobots;
    //Update.
    update();
}

void GroundRealtimePreviewer::setSelectedEnemy(Enemy *selectedEnemy)
{
    //Save the selected enemy.
    m_selectedEnemy=selectedEnemy;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::setSelectedRobot(Robot *selectedRobot)
{
    //Save the selected robot.
    m_selectedRobot=selectedRobot;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::setEnemyPositionList(const QList<QPointF> &positions)
{
    //The size of the position list should be the same as enemys.
    if(positions.size()!=m_enemies.size())
    {
        return;
    }
    //Save the position list.
    m_enemyPositions=positions;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::setEnemyPreviewList(const QList<Enemy *> &enemys,
                                                  const QList<QPointF> &positions)
{
    //Save all the informations.
    m_enemies=enemys;
    m_enemyPositions=positions;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::setPositionList(const QList<QPointF> &positions)
{
    //The size of position list should be the same as robots.
    if(positions.size()!=m_robots.size())
    {
        return;
    }
    //Save the positions.
    m_positions=positions;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::setAngleList(const QList<qreal> &angles)
{
    //The size of angle should be the same as robots.
    if(angles.size()!=m_robots.size())
    {
        return;
    }
    //Save the angle list.
    m_angles=angles;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::setRobotsPreviewList(const QList<Robot *> &robots,
                                                   const QList<QPointF> &positions,
                                                   const QList<qreal> &angles)
{
    //Save all the informations.
    m_robots=robots;
    m_positions=positions;
    m_angles=angles;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::paintEvent(QPaintEvent *event)
{
    //Draw the map.
    GroundPreviewer::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //If the user wants to draw the exist robot, then paint the robot.
    if(m_displayRobots)
    {
        QPen robotPen(RobotBase::robotBorder());
        robotPen.setWidth(2);
        painter.setPen(robotPen);
        painter.setBrush(RobotBase::robotColor());
        painter.setOpacity(0.2);
        int selectedIndex=-1;
        for(int i=0; i<m_positions.size(); i++)
        {
            //Check the selected robot
            if(m_robots.at(i)==m_selectedRobot)
            {
                selectedIndex=i;
                continue;
            }
            //Set the preview pos to the preview robot.
            m_proxyRobot->setPos(pointFromGround(m_positions.at(i)));
            //Draw the robot.
            m_proxyRobot->paintRobot(&painter);
        }
        //Paint the selected robot.
        if(selectedIndex!=-1)
        {
            painter.setOpacity(1.0);
            //Draw the parameter.
            m_proxyRobot->setPos(pointFromGround(m_positions.at(selectedIndex)));
            m_proxyRobot->setAngle(m_angles.at(selectedIndex));
            painter.setPen(Qt::NoPen);
            m_proxyRobot->paintRobotDetectArea(&painter);
            painter.setPen(RobotBase::directionLineColor());
            painter.setBrush(Qt::NoBrush);
            m_proxyRobot->paintRobotParameter(&painter);
            painter.setPen(robotPen);
            painter.setBrush(RobotBase::robotColor());
            m_proxyRobot->paintRobot(&painter);
        }
    }
    //If the user wants to draw the exist enemy, then paint the enemy.
    if(m_displayEnemies)
    {
        QPen enemyPen(RobotBase::robotBorder());
        enemyPen.setWidth(2);
        painter.setPen(enemyPen);
        painter.setBrush(RobotBase::robotColor());
        painter.setOpacity(0.2);
        int selectedIndex=-1;
        for(int i=0; i<m_enemyPositions.size(); i++)
        {
            //Check the selected robot
            if(m_enemies.at(i)==m_selectedEnemy)
            {
                selectedIndex=i;
                continue;
            }
            //Set the preview pos to the preview robot.
            m_proxyEnemy->setPos(pointFromGround(m_enemyPositions.at(i)));
            //Draw the robot.
            m_proxyEnemy->paintRobot(&painter);
        }
        //Paint the selected robot.
        if(selectedIndex!=-1)
        {
            painter.setOpacity(1.0);
            //Draw the parameter.
            m_proxyEnemy->setPos(pointFromGround(m_enemyPositions.at(selectedIndex)));
            painter.setPen(Qt::NoPen);
            m_proxyEnemy->paintEnemyDetectArea(&painter);
            painter.setPen(enemyPen);
            painter.setBrush(RobotBase::robotColor());
            m_proxyEnemy->paintRobot(&painter);
        }
    }
}

