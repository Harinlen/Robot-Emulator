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

#ifndef GROUNDPREVIEWER_H
#define GROUNDPREVIEWER_H

#include <QWidget>

class RobotBase;
class GroundGlobal;
/*!
 * \brief The GroundPreviewWidget class provides the basic information of a
 * ground. It will display the border and barracks, automatically zoom it to
 * specific size.
 */
class GroundPreviewer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a Ground preview widget.
     * \param parent The parent widget.
     */
    explicit GroundPreviewer(QWidget *parent = 0);

    /*!
     * \brief Destroy the group previewer.
     */
    ~GroundPreviewer();

    /*!
     * \brief The widget will display or hide the preview robot.
     * \return If display the preview robot, will return true, or else false.
     */
    bool showPreviewPoint() const;

    /*!
     * \brief The widget will display or hide the enemy.
     * \return If display the preview enemy, will return true, or else false.
     */
    bool showPreviewEnemy() const;

signals:

public slots:
    /*!
     * \brief Update the parameter of the previewer according to the new size.
     * \param groundSize The new size of the ground.
     */
    void onActionGroundSizeChanged(const QSize &groundSize);

    /*!
     * \brief Make the preview widget to display the preview enemy or not.
     * \param showPreviewEnemy The preview enabled value.
     */
    void setShowPreviewEnemy(bool showPreviewEnemy);

    /*!
     * \brief Set the preview border polygon.
     * \param groundBorder The border polygon.
     */
    void setPreviewBorder(const QPolygonF &groundBorder);

    /*!
     * \brief Set the preview barracks polygon.
     * \param groundBarracks The barracks polygon.
     */
    void setPreviewBarracks(const QPolygonF &groundBarracks);

    /*!
     * \brief Make the preview widget to display the preview robot or not.
     * \param showPreviewPoint The preview enabled value.
     */
    void setShowPreviewPoint(bool showPreviewPoint);

    /*!
     * \brief Will display a robot on the preview map.
     * \param position The preview robot position.
     * \param angle The angle of the robot.
     */
    void previewRobot(QPointF position, qreal angle);

    /*!
     * \brief Will display a enemy on the preview map.
     * \param position The preview enemy position.
     */
    void previewEnemy(QPointF position);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Get the point in preview mode mapped from the Ground class.
     * \param groundPoint The point on the Ground class.
     * \return The point mapped in the preview.
     */
    QPointF pointFromGround(const QPointF &groundPoint);

private:
    QPolygonF m_previewGround, m_previewBarracks;

    qreal m_xOffset, m_yOffset, m_groundParameter;
    bool m_showPreviewPoint, m_showPreviewEnemy;
    RobotBase *m_previewRobot, *m_previewEnemy;

    GroundGlobal *m_groundGlobal;
};

#endif // GROUNDPREVIEWER_H
