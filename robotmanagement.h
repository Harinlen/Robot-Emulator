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

#ifndef ROBOTMANAGEMENT_H
#define ROBOTMANAGEMENT_H

#include <QWidget>

class QStackedLayout;
class GroundBase;
class MenuBar;
class RobotAddWidget;
class GroundPreviewWidget;
/*!
 * \brief The RobotManagement class is the dialog for robot add, modify and
 * multi-robot remove.
 */
class RobotManagement : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a robot management dialog.
     * \param parent The parent widget.
     */
    explicit RobotManagement(QWidget *parent = 0);

    /*!
     * \brief Get the managing Ground class.
     * \return The managing Ground class.
     */
    GroundBase *ground();

signals:

public slots:
    /*!
     * \brief Show add robot dialog.
     */
    void addRobot();

    /*!
     * \brief Show the robots management dialog.
     */
    void manageRobot();

    /*!
     * \brief Set the Ground class for signal robot management.
     * \param ground The Ground class.
     */
    void setGround(GroundBase *ground);

    /*!
     * \brief Set the menu bar for action adding.
     * \param menuBar The menu bar.
     */
    void setMenuBar(MenuBar *menuBar);

private slots:
    void retranslate();
    void onActionAddRobot(const QPointF &position,
                          const qreal &angle);

private:
    enum RobotManagementActions
    {
        AddRobot,
        ManageRobot,
        RobotManagementActionsCount
    };
    QAction *m_actions[RobotManagementActionsCount];

    GroundBase *m_ground;
    GroundPreviewWidget *m_groundPreview;
    QStackedLayout *m_stackLayout;
    RobotAddWidget *m_robotAdd;
};

#endif // ROBOTMANAGEMENT_H
