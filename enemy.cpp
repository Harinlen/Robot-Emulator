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

#include "enemy.h"

Enemy::Enemy() :
    m_destory(false)
{
}

Enemy::Enemy(QPointF pos) :
    Enemy()
{
    setPos(pos);
}

Enemy::Enemy(qreal x, qreal y) :
    Enemy()
{
    setPos(x, y);
}

QPointF Enemy::m_target=QPointF(0,0);

QPointF Enemy::target()
{
    return m_target;
}

void Enemy::setTarget(const QPointF &target)
{
    m_target = target;
}

void Enemy::moveOneStep()
{
    QLineF routine(m_pos, m_target);
    if(routine.length()<0.9)
    {
        //The enemy reach the target point.
        return;
    }
    setPos(routine.pointAt(1/routine.length()));
}
bool Enemy::destory() const
{
    return m_destory;
}

void Enemy::setDestory(bool destory)
{
    m_destory = destory;
}


