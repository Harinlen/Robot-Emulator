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

#ifndef POLYGONEDITOR_H
#define POLYGONEDITOR_H

#include <QWidget>

class QToolButton;
class QTreeView;
class QStandardItemModel;
/*!
 * \brief The PolygonEditor class can edit or generate a QPolygon.
 */
class PolygonEditor : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct Ground Editor.
     * \param parent The parent widget.
     */
    explicit PolygonEditor(QWidget *parent = 0);

    /*!
     * \brief Get the current ground.
     * \return The ground polygon.
     */
    QPolygonF polygon();

signals:
    /*!
     * \brief When user add, modify or remove a point, then this signal will be
     * emitted.
     * \param ground The new ground polygon.
     */
    void polygonChange(QPolygonF polygon);

public slots:
    /*!
     * \brief Set the polygon to be edit.
     * \param polygon The polygon.
     */
    void setPolygon(const QPolygonF &polygon);

private slots:
    void retranslate();

private:
    inline void addPoint(const QPointF &point);
    enum GroundEditActions
    {
        AddPoint,
        RemovePoint,
        GroundEditActionsCount
    };
    QToolButton *m_actions[GroundEditActionsCount];

    QTreeView *m_groundPoints;
    QStandardItemModel *m_groundData;
};

#endif // POLYGONEDITOR_H
