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

#include "groundrealtimepreviewer.h"

GroundRealtimePreviewer::GroundRealtimePreviewer(QWidget *parent) :
    GroundPreviewer(parent),
    m_proxyRobot(new Robot()),
    m_ground(nullptr),
    m_displayRobots(false),
    m_selectedRobot(nullptr)
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

void GroundRealtimePreviewer::setSelectedRobot(Robot *selectedRobot)
{
    //Save the selected robot.
    m_selectedRobot=selectedRobot;
    //Update the previewer.
    update();
}

void GroundRealtimePreviewer::setPositionList(const QList<QPointF> &positions)
{
    //The size of position should be the same as robots.
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
    //If the user wants to draw the exist robot, then paint the robot.
    if(m_displayRobots)
    {
        //Initial painter.
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::TextAntialiasing |
                               QPainter::SmoothPixmapTransform, true);
        for(int i=0; i<m_positions.size(); i++)
        {
            //Set the preview pos to the preview robot.
            m_proxyRobot->setPos(pointFromGround(m_positions.at(i)));
            //Draw the robot.
            m_proxyRobot->paintRobot(&painter);
            //Check the selected robot
            if(m_robots.at(i)==m_selectedRobot)
            {
                //Draw the parameter.
                m_proxyRobot->setAngle(m_angles.at(i));
                m_proxyRobot->paintRobotParameter(&painter);
            }
        }
    }
}

