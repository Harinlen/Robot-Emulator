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

#ifndef GROUNDGLOBAL_H
#define GROUNDGLOBAL_H

#include <QObject>
#include <QColor>

/*!
 * \brief The GroundGlobal class is a singleton class for ground. It contains
 * several global settings of the ground, like the color configure.
 */
class GroundGlobal : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the instance of the ground global.
     * \return The singleton instance.
     */
    static GroundGlobal *instance();

    /*!
     * \brief Get the color of the border.
     * \return The QColor of the border.
     */
    QColor borderColor();

    /*!
     * \brief Get the color of the barracks border.
     * \return The QColor of the barracks border.
     */
    QColor barracksColor();

    /*!
     * \brief Get the color of the reference line.
     * \return The QColor of the reference line.
     */
    QColor referenceLineColor();

signals:

public slots:
    /*!
     * \brief Sets the color of the border.
     * \param borderColor The prefer color of the border.
     */
    void setBorderColor(const QColor &borderColor);

private:
    static GroundGlobal *m_instance;
    explicit GroundGlobal(QObject *parent = 0);
    QColor m_borderColor, m_barracksColor, m_referenceLineColor;
};

#endif // GROUNDGLOBAL_H
