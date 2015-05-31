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

#include <QPointF>
#include <QLineF>
#include <QList>
#include <QColor>

class QPainter;
/*!
 * \brief The Robot class contains all the information and description of a
 * robot, it contains the position, moving speed and direction of a robot. \n
 * You can set the start position of a robot at any position you want. \n
 * You have to recover the memory yourself. It don't have a parent. \n
 * This is not a QObject for memory reduce, so there's no signal and slots.
 */
class Robot
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
     * \brief The position of the robot.
     * \return The QPointF format robot position.
     */
    QPointF pos() const;

    /*!
     * \brief Set the position of a robot.
     * \param pos The position of the robot.
     */
    void setPos(const QPointF &pos);

    /*!
     * \brief This is an overloaded function.\n
     * Set the position of a robot.
     * \param x The x position of the robot.
     * \param y The y position of the robot.
     */
    inline void setPos(qreal x, qreal y)
    {
        setPos(QPointF(x, y));
    }

    /*!
     * \brief Paint the robot with the specific painter.
     * \param painter
     */
    void paintRobot(QPainter *painter);

    /*!
     * \brief Get the size of all the robots.
     * \return The size of all robots.
     */
    static int robotSize();

    /*!
     * \brief Change the size of all the robots.
     * \param robotSize The new size of all robots.
     */
    static void setRobotSize(int robotSize);

    /*!
     * \brief Get the detect radius of all the robots.
     * \return The detect radius of all robots.
     */
    static int detectRadius();

    /*!
     * \brief Change all the detect radius of all the robots.
     * \param detectRadius The new detect radius of all the robots.
     */
    static void setDetectRadius(int detectRadius);

    /*!
     * \brief Sets the color of the robot.
     * \param robotColor The prefer color of all robots.
     */
    static void setRobotColor(const QColor &robotColor);

    /*!
     * \brief Get the color of all robots.
     * \return The QColor of the robots.
     */
    static QColor robotColor();

    /*!
     * \brief Get the color of the detection radius border of the robots.
     * \return The QColor of the detection radius border of the robots.
     */
    static QColor detectRadiusColor();

    /*!
     * \brief Change the color of the robot detection radius border.
     * \param robotColor The prefer color of all the detection radius border of
     * robots.
     */
    static void setDetectRadiusColor(const QColor &detectRadiusColor);

    /*!
     * \brief Get the moving angle of the robot. \n
     * The default angle of the robot is 0 (3'o clock position).
     * \return The angle of the robot.
     */
    qreal angle() const;

    /*!
     * \brief Change the moving angle of the robot, the value should be ranged
     * from 0 degrees to 360 degrees. The program will automatically change the
     * value.
     * \param angle The prefer angle of the robot.
     */
    void setAngle(const qreal &angle);

    /*!
     * \brief Get the color the all the direction line color of the robots.
     * \return The color of the direction line.
     */
    static QColor directionLineColor();

    /*!
     * \brief Change the color of the direction line.
     * \param directionLineColor The direction line color.
     */
    static void setDirectionLineColor(const QColor &directionLineColor);

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

    static int m_robotSize, m_detectRadius;
    QPointF m_pos;
    QList<Robot *> m_detectedRobotList;

    static QColor m_robotColor, m_detectRadiusColor, m_directionLineColor;
    qreal m_angle;

    //Gardian line information
    bool m_hasGuardianLine;
    QLineF m_guardianLine, m_oppositeGuardianLine;
    qreal m_toP1Distance, m_movingSpeed;
};

#endif // ROBOT_H
