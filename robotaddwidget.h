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

#ifndef ROBOTADDWIDGET_H
#define ROBOTADDWIDGET_H

#include <QWidget>

class QPushButton;
class QSpinBox;
class QSlider;
class QLabel;
class GroundBase;
/*!
 * \brief The RobotAddWidget class is a widget for user to generate a robot in a
 * user friend way. The user can set the initial position and the initial angle
 * of the robot. The user can preview the robot state on the map.
 */
class RobotAddWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the RobotAddWidget class.
     * \param parent The parent widget class.
     */
    explicit RobotAddWidget(QWidget *parent = 0);

    /*!
     * \brief Update the X position And Y position range.
     * \param minX The minimal X coordinate value.
     * \param minY The minimal Y coordinate value.
     * \param maxX The maximum X coordinate value.
     * \param maxY The maximum Y coordinate value.
     */
    void updateXAndYRange(const int &minX, const int &minY,
                          const int &maxX, const int &maxY);

    /*!
     * \brief Call this function will bind the default button and enabled this
     * widget.
     */
    void enabledWidget();

    /*!
     * \brief Call this function will unbind the default button and disable this
     * widget.
     */
    void disabledWidget();

signals:
    /*!
     * \brief When this signal is emitted, the robot management dialog will be
     * closed.
     */
    void requireClose();

    /*!
     * \brief When this signal is emitted, will ask the preview widget to draw a
     * preview robot on the widget.
     * \param position The preview robot position.
     * \param angle The preview robot angle.
     */
    void requirePreviewRobot(QPointF position,
                             qreal angle);

    /*!
     * \brief This signal will ask the parent to try to add robot.
     * \param position The prefer robot position.
     * \param angle The prefer robot angle.
     */
    void requireAddRobot(QPointF position,
                         qreal angle);

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::showEvent().
     */
    void showEvent(QShowEvent *event);

private slots:
    void retranslate();
    void onActionParameterChange();

private:
    QPushButton *m_okay, *m_cancel;
    QLabel *m_labels[3];
    QSpinBox *m_xData, *m_yData;
    QSlider *m_angleData;
};

#endif // ROBOTADDWIDGET_H
