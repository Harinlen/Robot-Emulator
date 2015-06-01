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

#ifndef ROBOTMANAGEWIDGET_H
#define ROBOTMANAGEWIDGET_H

#include <QWidget>

class QTreeView;
class QPushButton;
class QStandardItemModel;
class QGridLayout;
class Robot;
class GroundBase;
/*!
 * \brief The RobotManageWidget class provides a user centered way to manage the
 * initial state data of the robot.
 */
class RobotManageWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct robot manage widget.
     * \param parent The parent widget.
     */
    explicit RobotManageWidget(QWidget *parent = 0);

    /*!
     * \brief Get the current managed ground.
     * \return The managed ground class.
     */
    GroundBase *ground() const;

    /*!
     * \brief Get the robot list.
     * \return The robot list from the editor.
     */
    QList<Robot *> robotList();

    /*!
     * \brief Get the position list.
     * \return The initial position list from the editor.
     */
    QList<QPointF> positionList();

    /*!
     * \brief Get the angle list.
     * \return The initial angle list from the editor.
     */
    QList<qreal> angleList();

signals:
    /*!
     * \brief When this signal is emitted, the robot management dialog will be
     * closed.
     */
    void requireClose();

    /*!
     * \brief When this signal is emiited, the preview will select the robot.
     * \param robot The robot which is going to be selected.
     */
    void requireSelectRobot(Robot *robot);

    /*!
     * \brief When this signal is emitted, the preview will be asked to update
     * all the robots preview information.
     */
    void requireUpdateAllList();

    /*!
     * \brief When this signal is emitted, the preview will be asked to update
     * all the initial position information.
     */
    void requireUpdatePositionList();

    /*!
     * \brief When this signal is emitted, the preview will be asked to update
     * all the initial angle information.
     */
    void requireUpdateAngleList();

public slots:
    /*!
     * \brief Set the managed ground class.
     * \param ground The managed ground class.
     */
    void setGround(GroundBase *ground);

protected:
    /*!
     * \brief Reimplemented from QWidget::showEvent().
     */
    void showEvent(QShowEvent *event);

private slots:
    void retranslate();

private:
    inline qreal getModelData(int row, int column, int role=Qt::DisplayRole);
    inline void updateHeader();
    inline void updateRobotList();
    QPushButton *m_okay, *m_cancel, *m_deletePoint;
    QTreeView *m_robotInitialDataView;
    QStandardItemModel *m_robotInitialDataModel;
    QList<Robot *> m_robotList;
    GroundBase *m_ground;
};

#endif // ROBOTMANAGEWIDGET_H
