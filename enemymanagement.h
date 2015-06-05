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

#ifndef ENEMYMANAGEMENT_H
#define ENEMYMANAGEMENT_H

#include <QDialog>

class QStackedLayout;
class QGroupBox;
class GroundBase;
class MenuBar;
class EnemyAddWidget;
class EnemyManageWidget;
class GroundRealtimePreviewer;
/*!
 * \brief The EnemyManagement class is the dialog for robot add, modify and
 * multi-robot remove.
 */
class EnemyManagement : public QDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a robot management dialog.
     * \param parent The parent widget.
     */
    explicit EnemyManagement(QWidget *parent = 0);

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
    void addEnemy();

    /*!
     * \brief Show the robots management dialog.
     */
    void manageEnemy();

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
    void onActionAddEnemy(const QPointF &position);

private:
    enum EnemyManagementActions
    {
        AddEnemy,
        ManageEnemy,
        EnemyManagementActionsCount
    };
    QAction *m_actions[EnemyManagementActionsCount];

    QGroupBox *m_previewGroup;
    GroundBase *m_ground;
    GroundRealtimePreviewer *m_groundPreview;
    QStackedLayout *m_stackLayout;
    EnemyAddWidget *m_enemyAdd;
    EnemyManageWidget *m_enemyManage;
};

#endif // ENEMYMANAGEMENT_H
