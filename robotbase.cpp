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

#include "robotbase.h"

RobotBase::RobotBase() :
    m_pos(QPointF(0, 0)),
    m_angle(0.0)
{
}

QPointF RobotBase::pos() const
{
    return m_pos;
}

void RobotBase::setPos(const QPointF &pos)
{
    m_pos = pos;
}

void RobotBase::paintRobot(QPainter *painter)
{
    //Draw the robot.
    painter->drawEllipse(m_pos, m_robotSize, m_robotSize);
}

void RobotBase::paintRobotParameter(QPainter *painter)
{
    //Draw the direction of the robot.
    QLineF directionLine(m_pos, m_pos+QPointF(m_detectRadius, 0));
    directionLine.setAngle(m_angle);
    painter->drawLine(directionLine);
}

void RobotBase::paintRobotDetectArea(QPainter *painter)
{
    //Draw the detect radius.
    m_detectRadiusGradient.setFocalPoint(m_pos);
    m_detectRadiusGradient.setCenter(m_pos);
    painter->setBrush(m_detectRadiusGradient);
    painter->drawEllipse(m_pos, m_detectRadius, m_detectRadius);
}

int RobotBase::m_robotSize=3;

int RobotBase::robotSize()
{
    return m_robotSize;
}

void RobotBase::setRobotSize(int robotSize)
{
    m_robotSize = robotSize;
}

int RobotBase::m_detectRadius=20;

int RobotBase::detectRadius()
{
    return m_detectRadius;
}

void RobotBase::setDetectRadius(int detectRadius)
{
    m_detectRadius = detectRadius;
}

QColor RobotBase::m_robotColor=QColor(59,120,133);

void RobotBase::setRobotColor(const QColor &robotColor)
{
    m_robotColor=robotColor;
}

QColor RobotBase::robotColor()
{
    return m_robotColor;
}

QColor RobotBase::m_detectRadiusColor=QColor(44,132,99);

QColor RobotBase::detectRadiusColor()
{
    return m_detectRadiusColor;
}

void RobotBase::setDetectRadiusColor(const QColor &detectRadiusColor)
{
    m_detectRadiusColor = detectRadiusColor;
    //Change the gradient color.
    m_detectRadiusGradient.setColorAt(1, m_detectRadiusColor);
}

qreal RobotBase::angle() const
{
    return m_angle;
}

void RobotBase::setAngle(const qreal &angle)
{
    m_angle = angle;
    //Change the angle.
    while(m_angle>360.0)
    {
        m_angle-=360.0;
    }
    while(m_angle<0.0)
    {
        m_angle+=360.0;
    }
}

QColor RobotBase::m_directionLineColor=QColor(98, 228, 238);

QColor RobotBase::directionLineColor()
{
    return m_directionLineColor;
}

void RobotBase::setDirectionLineColor(const QColor &detectLineColor)
{
    m_directionLineColor = detectLineColor;
}

QRadialGradient RobotBase::m_detectRadiusGradient=QRadialGradient();

void RobotBase::initialRobotPatameters()
{
    m_detectRadiusGradient.setRadius(m_detectRadius);
    m_detectRadiusGradient.setColorAt(0, QColor(0,0,0,0));
    m_detectRadiusGradient.setColorAt(0.6, QColor(0,0,0,0));
    m_detectRadiusGradient.setColorAt(1, m_detectRadiusColor);
}

QColor RobotBase::m_robotBorder=QColor(255,255,255);

QColor RobotBase::robotBorder()
{
    return m_robotBorder;
}

void RobotBase::setRobotBorder(const QColor &robotBorder)
{
    m_robotBorder = robotBorder;
}
