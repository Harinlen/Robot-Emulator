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

#ifndef ENEMY_H
#define ENEMY_H

#include "robotbase.h"

/*!
 * \brief The Enemy class record all the basic information of a enemy.
 */
class Enemy : public RobotBase
{
public:
    /*!
     * \brief Construct a Enemy class. The start position of this type of
     * construct will be QPointF(0, 0);
     */
    Enemy();

    /*!
     * \brief Construct the enemy with the give position.
     * \param pos The start position of the robot.
     */
    Enemy(QPointF pos);

    /*!
     * \brief Construct the enemy with the give position.
     * \param x The start x position of the enemy.
     * \param y The start y position of the enemy.
     */
    Enemy(qreal x, qreal y);

    /*!
     * \brief Get the target point of the enemy.
     * \return The target point.
     */
    static QPointF target();

    /*!
     * \brief Set the target point of the enemy.
     * \param target Enemy's target point.
     */
    static void setTarget(const QPointF &target);

    /*!
     * \brief Ask the enemy move one step to the target point.
     */
    void moveOneStep();

    /*!
     * \brief Whether this enemy is destoried by robot.
     * \return The destory status.
     */
    bool destory() const;

    /*!
     * \brief Set the destory status.
     * \param destory Destory status.
     */
    void setDestory(bool destory);

private:
    static QPointF m_target;
    bool m_destory;
};

#endif // ENEMY_H
