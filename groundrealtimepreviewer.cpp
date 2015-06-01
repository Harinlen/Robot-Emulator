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
#include "groundbase.h"

#include "groundrealtimepreviewer.h"

GroundRealtimePreviewer::GroundRealtimePreviewer(QWidget *parent) :
    GroundPreviewer(parent),
    m_ground(nullptr)
{
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
