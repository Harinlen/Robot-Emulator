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

#ifndef GENERATEGROUNDBASE_H
#define GENERATEGROUNDBASE_H

#include <QDialog>

/*!
 * \brief The GenerateGround class is a widget which can help user to generate
 * the border and barracks.
 */
class GenerateGroundBase : public QDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a GenerateGroundBase dialog.
     * \param parent Set the parent widget.
     */
    GenerateGroundBase(QWidget *parent = 0):QDialog(parent){}

    /*!
     * \brief The current border information.
     * \return The border polygon of the editor result.
     */
    virtual QPolygonF border() const=0;

    /*!
     * \brief The current barracks information.
     * \return The barracks polygon of the editor result.
     */
    virtual QPolygonF barracks() const=0;

signals:

public slots:
    /*!
     * \brief Set the border polygon, make the editor to display this polygon.
     * \param border The border polygon.
     */
    virtual void setBorder(const QPolygonF &border)=0;

    /*!
     * \brief Set the barracks polygon, make the editor to display this polygon.
     * \param border The barracks polygon.
     */
    virtual void setBarracks(const QPolygonF &barracks)=0;
};

#endif // GENERATEGROUNDBASE_H
