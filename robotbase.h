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

#ifndef ROBOTBASE_H
#define ROBOTBASE_H

#include <QPointF>
#include <QColor>
#include <QRadialGradient>

/*!
 * \brief The RobotBase class is a base class of robot, it contains all the
 * basic functions of a robot.
 */
class RobotBase
{
public:
    /*!
     * \brief Construct a RobotBase class.
     */
    RobotBase();

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
     * \param painter The painting painter.
     */
    void paintRobot(QPainter *painter);

    /*!
     * \brief Paint the parameter of the robot with the specific painter.\n
     * The parameters of a robots include the range and angle.
     * \param painter The painting painter.
     */
    void paintRobotParameter(QPainter *painter);

    /*!
     * \brief Paint the detect area of the current robot with the specific
     * painter.
     * \param painter The painting painter.
     */
    void paintRobotDetectArea(QPainter *painter);

    /*!
     * \brief Paint the detect area of the current enemy robot with the specific
     * painter.
     * \param painter The painting painter.
     */
    void paintEnemyDetectArea(QPainter *painter);

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
     * \brief Initial the grapihcs parameters of all the Robot.
     */
    static void initialRobotPatameters();

    /*!
     * \brief Get the color the all the robot border color.
     * \return The QColor of all the robot border.
     */
    static QColor robotBorder();

    /*!
     * \brief Change the color of the robot border.
     * \param robotBorder The QColor of the robot border.
     */
    static void setRobotBorder(const QColor &robotBorder);

    /*!
     * \brief Get the color the all the enemy border color.
     * \return The QColor of all the enemy border.
     */
    static QColor enemyRadiusColor();

    /*!
     * \brief Set the color of the enemy radius.
     * \param enemyRadiusColor The QColor of the enemy.
     */
    static void setEnemyRadiusColor(const QColor &enemyRadiusColor);

protected:
    QPointF m_pos;
    qreal m_angle;

    static int m_robotSize, m_detectRadius;
    static QColor m_robotColor;
    static QColor m_detectRadiusColor;
    static QColor m_directionLineColor;
    static QColor m_robotBorder;
    static QColor m_enemyRadiusColor;
    static QRadialGradient m_detectRadiusGradient;
    static QRadialGradient m_enemyRadiusGradient;
};

#endif // ROBOTBASE_H
