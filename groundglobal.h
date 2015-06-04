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

    /*!
     * \brief Get the color of the ground base.
     * \return The QColor of the ground base.
     */
    QColor baseColor() const;

    /*!
     * \brief Get the color of the ground.
     * \return The QColor of the ground.
     */
    QColor groundColor() const;

signals:
    /*!
     * \brief When this signal is emitted, it will ask the container to change
     * the base color.
     */
    void baseColorChanged(const QColor &color);

public slots:
    /*!
     * \brief Sets the color of the border.
     * \param borderColor The prefer color of the border.
     */
    void setBorderColor(const QColor &borderColor);

    /*!
     * \brief Sets the color of the barracks.
     * \param barracksColor The prefer color of the barracks.
     */
    void setBarracksColor(const QColor &barracksColor);

    /*!
     * \brief Sets the color of the ground base.
     * \param baseColor The prefer color of the ground.
     */
    void setBaseColor(const QColor &baseColor);

    /*!
     * \brief Sets the color of the ground.
     * \param groundColor The prefer color of the ground.
     */
    void setGroundColor(const QColor &groundColor);

private:
    static GroundGlobal *m_instance;
    explicit GroundGlobal(QObject *parent = 0);
    QColor m_borderColor, m_barracksColor, m_referenceLineColor, m_baseColor,
           m_groundColor;
};

#endif // GROUNDGLOBAL_H
