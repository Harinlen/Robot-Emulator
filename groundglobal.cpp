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

#include "groundglobal.h"

GroundGlobal *GroundGlobal::m_instance=nullptr;

GroundGlobal *GroundGlobal::instance()
{
    return m_instance==nullptr?m_instance=new GroundGlobal:m_instance;
}

GroundGlobal::GroundGlobal(QObject *parent) :
    QObject(parent),
    m_borderColor(QColor(73, 229, 249, 100)),
    m_barracksColor(QColor(79,219,251)),
    m_referenceLineColor(QColor(27, 68, 76)),
    m_baseColor(QColor(1,7,20)),
    m_groundColor(QColor(0x00, 0x40, 0x51))
{
}

QColor GroundGlobal::groundColor() const
{
    return m_groundColor;
}

void GroundGlobal::setGroundColor(const QColor &groundColor)
{
    m_groundColor = groundColor;
}

QColor GroundGlobal::baseColor() const
{
    return m_baseColor;
}

void GroundGlobal::setBaseColor(const QColor &baseColor)
{
    //Save base color.
    m_baseColor = baseColor;
    //Ask to change base color.
    emit baseColorChanged(m_baseColor);
}

QColor GroundGlobal::borderColor()
{
    return m_borderColor;
}

QColor GroundGlobal::barracksColor()
{
    return m_barracksColor;
}

QColor GroundGlobal::referenceLineColor()
{
    return m_referenceLineColor;
}

void GroundGlobal::setBorderColor(const QColor &borderColor)
{
    m_borderColor=borderColor;
}

void GroundGlobal::setBarracksColor(const QColor &barracksColor)
{
    m_barracksColor=barracksColor;
}
