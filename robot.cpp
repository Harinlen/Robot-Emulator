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
#include "robot.h"

#include <QDebug>

Robot::Robot() :
    RobotBase(),
    m_hasGuardianLine(false),
    m_guardianLine(QPointF(0,0), QPointF(0,0)),
    m_toP1Distance(0.0),
    m_movingSpeed(1.0)
{
    setPos(QPointF(0, 0));
}

Robot::Robot(QPointF pos) :
    Robot()
{
    setPos(pos);
}

Robot::Robot(qreal x, qreal y) :
    Robot()
{
    setPos(x, y);
}

void Robot::addToDetectList(Robot *robot)
{
    //Check the robot whether has already be in the list.
    if(!m_detectedRobotList.contains(robot))
    {
        m_detectedRobotList.append(robot);
    }
}

void Robot::removeFromDetectList(Robot *robot)
{
    //Remove all the robot from the list.
    m_detectedRobotList.removeOne(robot);
}

void Robot::moveOneStep()
{
    //Get the next step and set the position.
    setPos(nextStep());
    //If this robot has a guardian line, change the toP1Distance.
    if(m_hasGuardianLine)
    {
        m_toP1Distance+=m_movingSpeed;
    }
}

QPointF Robot::nextStep() const
{
    //If the robot has the guardian line, calculate the next step on the line.
    if(m_hasGuardianLine)
    {
        //Get the next position.
        qreal robotPosition= (m_toP1Distance + m_movingSpeed) /
                m_guardianLine.length();
        return m_guardianLine.pointAt(robotPosition);
    }
    //If not have a guardian line, then move to the direction.
    //Generate the direction line.
    QLineF directionLine(m_pos, m_pos+QPointF(m_detectRadius, 0));
    directionLine.setAngle(m_angle);
    directionLine.setLength(1.0);
    //The p2 is the next step position.
    return directionLine.p2();
}

void Robot::updateDirection()
{
    //----Magic! Don't touch!---
    //If the detected list is empty, then keep the direction.
    if(m_detectedRobotList.isEmpty())
    {
        //If the robot has a guardian line,
        if(m_hasGuardianLine)
        {
            //Check if the robot reach one side of the line.
            if(m_toP1Distance<=0.9)
            {
                //Move to the guardian line angle.
                m_movingSpeed=1.0;
                m_angle=m_guardianLine.angle();
                return;
            }
            if(m_toP1Distance>=m_guardianLine.length())
            {
                //Move to the opposite angle of the guardian line.
                m_movingSpeed=-1.0;
                m_angle=m_oppositeGuardianLine.angle();
                return;
            }
        }
        //Or else keep the direction.
        return;
    }
    //Now the detected robot list cannot be empty.
    //If the robot has a line to guard.
    if(m_hasGuardianLine)
    {
        //Check if the robot reach one side of the line.
        if(m_toP1Distance<=0.9 || m_toP1Distance>=m_guardianLine.length()-0.9)
        {
            //Move to the different direction.
            if(m_movingSpeed>0)
            {
                m_movingSpeed=-1.0;
                m_angle=m_oppositeGuardianLine.angle();
            }
            else
            {
                m_movingSpeed=1.0;
                m_angle=m_guardianLine.angle();
            }
            return;
        }
        //Or else, we should have move the robot to opposite direction of the
        //nearest robot.
        //The nearest robot have three kind of types:
        //  1. It doesn't have a guardian line.
        //  2. It has a guardian line, but it's not the same as mine.
        //  3. It has the same guardian line.
        //For the first type, ignore it.
        //For the second and third type,  there's one rule: the robot should
        //move to the direction which should leave that robot away.
        QList<RobotStatus> statusList;
        for(Robot *robot : m_detectedRobotList)
        {
            RobotStatus currentStatus;
            currentStatus.robot=robot;
            currentStatus.distance=QLineF(m_pos, robot->pos()).angle();
            statusList.append(currentStatus);
        }
        qSort(statusList);
        //Get the nearest robot, .
        RobotStatus nearestStatus=statusList.takeFirst();
        while(!nearestStatus.robot->hasGuardianLine() && !statusList.isEmpty())
        {
            nearestStatus=statusList.takeFirst();
        }
        //Check the nearest status.
        if(!nearestStatus.robot->hasGuardianLine())
        {
            //All the robot in the detect range don't has a guardian line.
            //They will move away from this point.
            return;
        }
        //So now, we get the nearest point which contains a guardian line.
        Robot *nearestRobot=nearestStatus.robot;
        //If these two robot has the same guardian line, and they are getting
        //closer(have the different speed), move to the other direction.
        //If these two robots have the different speed.
        //For this kinds of type,
        //p1  this
        //|   |
        //+---*><*----------------------
        //       |
        //       nearest
        //
        //Or for this kinds of type,
        //
        //                    this p2
        //                    |    |
        //-----------------*><*----+
        //                 |
        //                 nearest
        //
        //We have to change the direction.
        //
        //So now, there is an ugly thing we have to met.(What the fuck!)
        //The nearest point is not at the same line, but according to the
        //context, this line must be the neighbouring line. Like the following:
        //
        //  --------+
        //          |
        //          |
        //
        //We have change the direction when both of these robots are moving to
        //the same point, and that point is pretty interesting. It's the p1 for
        //the second line and the p2 for the first line. So:
        //
        //    this  p2(for this)
        //       |  |
        //  -----*>-+-p1(for nearest)
        //          |
        //          ^
        //          *
        //          |
        //
        //At this time, the moving speed of this and nearest will be different
        //(this is 1.0 and nearest is -1.0).
        //For another case, it will be like this:
        //
        //      p1(for this)  p1
        //                 |  |
        // p2(for nearest)-+-<*-------
        //                 |
        //                 ^
        //                 *
        //                 |
        //
        //At this time, the moving speed of this and nearest will be different
        //as well(this is -1.0 and nearest is 1.0).
        //We have to change the direction in these two cases.

        if(nearestRobot->movingSpeed()!=m_movingSpeed)
        {
            if(nearestRobot->guardianLine()==m_guardianLine)
            {
                if((m_movingSpeed>0 &&
                    m_toP1Distance<nearestRobot->toP1Distance()) ||
                        (m_movingSpeed<0 &&
                         m_toP1Distance>nearestRobot->toP1Distance()))
                {
                    //Move to the different direction.
                    moveToOppositeDirection();
                    //Ask the robot move to the differect direction as well.
                    nearestRobot->moveToOppositeDirection();
                }
            }
            else
            {
                if((m_movingSpeed>0 &&
                         m_toP1Distance>nearestRobot->toP1Distance()) ||
                   (m_movingSpeed<0 &&
                         m_toP1Distance<nearestRobot->toP1Distance()))
                {
                    //Move to the different direction.
                    moveToOppositeDirection();
                    //Ask the robot move to the differect direction as well.
                    nearestRobot->moveToOppositeDirection();
                }
            }
        }
        //Or else, keep moving.
        return;
    }
    //The prefer direction is to link all the detected robots, calculate the
    //average angle of the robot lists.
    qreal angleSum=0.0;
    for(Robot *robot : m_detectedRobotList)
    {
        angleSum+=QLineF(m_pos, robot->pos()).angle();
    }
    angleSum/=m_detectedRobotList.size();
    //Set the angle to the opposite angle.
    setAngle(angleSum+180.0);
}

bool Robot::hasGuardianLine() const
{
    return m_hasGuardianLine;
}

void Robot::setGuardianLine(const QLineF &line,
                            const QPointF &footPoint)
{
    //Clear the previous data.
    resetGuardianLine();
    //Check the length of the line.
    if(line.length()==0.0)
    {
        return;
    }
    //Set has guardian line flag.
    m_hasGuardianLine=true;
    //Save the guardian line.
    m_guardianLine=line;
    //Get the opposite guardian line.
    m_oppositeGuardianLine=QLineF(m_guardianLine.p2(), m_guardianLine.p1());
    QLineF directionAngle=QLineF(QPointF(0.0, 0.0), QPointF(10.0, 0));
    directionAngle.setAngle(m_angle);
    //If the current angle is nearly to the angle, then the moving speed will be
    //1.0(follow the direction of the line), or -1.0(reverse direction of the
    //line)
    if(m_oppositeGuardianLine.angleTo(directionAngle) <
            m_guardianLine.angleTo(directionAngle))
    {
        m_movingSpeed=1.0;
        m_angle=m_guardianLine.angle();
    }
    else
    {
        m_movingSpeed=-1.0;
        m_angle=m_oppositeGuardianLine.angle();
    }
    //Move the robot to the foot point.
    setPos(footPoint);
    //Save the initial distance to p1.
    m_toP1Distance=pointDistance(m_guardianLine.p1(), footPoint);
}

void Robot::resetGuardianLine()
{
    m_hasGuardianLine=false;
    m_guardianLine=QLineF();
    m_oppositeGuardianLine=QLineF();
    m_toP1Distance=0.0;
    m_movingSpeed=1.0;
}

void Robot::resetDetectList()
{
    m_detectedRobotList.clear();
}

void Robot::moveToOppositeDirection()
{
    if(!m_hasGuardianLine)
    {
        return;
    }
    //If the robot is moving along the line, change the direction back.
    if(m_movingSpeed>0)
    {
        m_movingSpeed=-1.0;
        m_angle=m_oppositeGuardianLine.angle();
    }
    else
    {
        m_movingSpeed=1.0;
        m_angle=m_guardianLine.angle();
    }
}
