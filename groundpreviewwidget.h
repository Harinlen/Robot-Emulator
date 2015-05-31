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

#ifndef GROUNDPREVIEWWIDGET_H
#define GROUNDPREVIEWWIDGET_H

#include <QWidget>

class GroundBase;
class Robot;
/*!
 * \brief The GroundPreviewWidget class provides the basic information of a
 * ground. It will display the border and barracks, automatically zoom it to
 * specific size.
 */
class GroundPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a Ground preview widget.
     * \param parent The parent widget.
     */
    explicit GroundPreviewWidget(QWidget *parent = 0);
    ~GroundPreviewWidget();

    /*!
     * \brief The widget will display or hide the preview robot.
     * \return If display the preview robot, will return true, or else false.
     */
    bool showPreviewPoint() const;

signals:

public slots:
    /*!
     * \brief Set the rendering Ground class.
     * \param ground The render ground.
     */
    void setGround(GroundBase *ground);

    /*!
     * \brief Make the preview widget to display the preview robot or not.
     * \param showPreviewPoint The preview enabled value.
     */
    void setShowPreviewPoint(bool showPreviewPoint);

    /*!
     * \brief Will display a robot on the preview map.
     * \param position
     * \param angle
     */
    void previewRobot(QPointF position, qreal angle);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionBorderChanged();
    void onActionBarracksChanged();

private:
    inline QPointF pointFromGround(const QPointF &groundPoint);
    GroundBase *m_ground;
    QPolygonF m_previewGround, m_previewBarracks;

    qreal m_xOffset, m_yOffset, m_groundParameter;
    bool m_showPreviewPoint;
    Robot *m_previewRobot;
};

#endif // GROUNDPREVIEWWIDGET_H
