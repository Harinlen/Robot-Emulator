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

#ifndef ROBOT_H
#define ROBOT_H

#include <QLineF>
#include <QList>

#include "robotbase.h"

class QPainter;
/*!
 * \brief The Robot class contains all the information and description of a
 * robot, it contains the position, moving speed and direction of a robot. \n
 * You can set the start position of a robot at any position you want. \n
 * You have to recover the memory yourself. It don't have a parent. \n
 * This is not a QObject for memory reduce, so there's no signal and slots.
 */
class Robot : public RobotBase
{
public:
    /*!
     * \brief Construct the robot. The start position of this type of construct
     * will be QPointF(0, 0);
     */
    Robot();

    /*!
     * \brief Construct the robot with the give position.
     * \param pos The start position of the robot.
     */
    Robot(QPointF pos);

    /*!
     * \brief Construct the robot with the give position.
     * \param x The start x position of the robot.
     * \param y The start y position of the robot.
     */
    Robot(qreal x, qreal y);

    /*!
     * \brief Add a detected robot to the detection list. \n
     * If the robot has been in the detect list, it won't add it twice.
     * \param robot The robot which has been detected.
     */
    void addToDetectList(Robot *robot);

    /*!
     * \brief Remove one robot from the detect list.
     * If the robot is not in the list, nothing will happened.
     * \param robot The robot which should be removed.
     */
    void removeFromDetectList(Robot *robot);

    /*!
     * \brief Call this function will make the robot move 1 pixel length to the
     * direction.
     */
    void moveOneStep();

    /*!
     * \brief Get the next step of the robot is going to be.
     * \return The next step position.
     */
    QPointF nextStep() const;

    /*!
     * \brief Update the direction of the robot according to the detected list.
     */
    void updateDirection();

    /*!
     * \brief Get whether the robot has got a line to guard, each robot should
     * guard one line.
     * \return If the robot has a line to guard, return true, or else false.
     */
    bool hasGuardianLine() const;

    /*!
     * \brief Set a guardian line of to the robot. If the lenght of the line is
     * 0, will remove the guardian state of the robot.
     * \param line The guardian line of the robot.
     */
    void setGuardianLine(const QLineF &line, const QPointF &footPoint);

    /*!
     * \brief Clear the guardian line state data.
     */
    void resetGuardianLine();

    /*!
     * \brief Clear the detect list data.
     */
    void resetDetectList();

    /*!
     * \brief This function only available when there's a guardian line.\n
     * Move to the opposite direction of the line.
     */
    void moveToOppositeDirection();

    /*!
     * \brief The moving speed is only available when the robot got a guardian
     * line, return the moving speed.\n
     * This vale should be whether 1.0 or -1.0.
     * \return The moving speed on a guardian line.
     */
    qreal movingSpeed() const
    {
        return m_movingSpeed;
    }

    /*!
     * \brief This function is only available when the robot has a guardian
     * line. It's a parameter for changing the direction.
     * \return The robot distance to the p1 point of the guardian line.
     */
    qreal toP1Distance() const
    {
        return m_toP1Distance;
    }

    /*!
     * \brief Get the guardian line of the current robot.
     * \return The guardian line in QLineF format.
     */
    QLineF guardianLine() const
    {
        return m_guardianLine;
    }

    /*!
     * \brief Calculate the distance of two point.
     * \param p1 The first point.
     * \param p2 The second point.
     * \return The distance of two point.
     */
    static qreal pointDistance(const QPointF p1, const QPointF p2)
    {
        return QLineF(p1, p2).length();
    }

    /*!
     * \brief This will return the detected list is empty or not. It's used to
     * reset the robot.
     * \return If the list is empty, return true.
     */
    bool isDetectedListEmpty()
    {
        return m_detectedRobotList.isEmpty();
    }

private:
    struct RobotStatus
    {
        Robot *robot;
        qreal distance;
        RobotStatus():
            robot(nullptr),
            distance(0.0)
        {
            ;
        }
        friend bool operator < (const RobotStatus &status1,
                                const RobotStatus &status2)
        {
            return status1.distance < status2.distance;
        }
    };
    //Robot list information.
    QList<Robot *> m_detectedRobotList;

    //Gardian line information
    bool m_hasGuardianLine;
    QLineF m_guardianLine, m_oppositeGuardianLine;
    qreal m_toP1Distance, m_movingSpeed;
};

#endif // ROBOT_H
