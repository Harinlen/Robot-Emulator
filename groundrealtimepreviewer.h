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

#ifndef GROUNDREALTIMEPREVIEWER_H
#define GROUNDREALTIMEPREVIEWER_H

#include "groundpreviewer.h"

class GroundBase;
/*!
 * \brief The GroundRealtimePreview class is a advanced ground preview. It can
 * preview a ground data in real-time. This means you have to set a GroundBase
 * to the previewer.
 */
class GroundRealtimePreviewer : public GroundPreviewer
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a GroundRealtimePreviewer.
     * \param parent The parent widget.
     */
    explicit GroundRealtimePreviewer(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the rendering Ground class.
     * \param ground The render ground.
     */
    void setGround(GroundBase *ground);

private slots:
    void onActionBorderChanged();
    void onActionBarracksChanged();

private:
    GroundBase *m_ground;
};

#endif // GROUNDREALTIMEPREVIEWER_H
