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

#ifndef ENEMYMANAGEWIDGET_H
#define ENEMYMANAGEWIDGET_H

#include <QWidget>

class QTreeView;
class QPushButton;
class QStandardItemModel;
class QGridLayout;
class QGroupBox;
class Enemy;
class GroundBase;
/*!
 * \brief The EnemyManageWidget class provides a user centered way to manage the
 * initial state data of the robot.
 */
class EnemyManageWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct robot manage widget.
     * \param parent The parent widget.
     */
    explicit EnemyManageWidget(QWidget *parent = 0);

    /*!
     * \brief Get the current managed ground.
     * \return The managed ground class.
     */
    GroundBase *ground() const;

    /*!
     * \brief Get the robot list.
     * \return The robot list from the editor.
     */
    QList<Enemy *> enemyList();

    /*!
     * \brief Get the position list.
     * \return The initial position list from the editor.
     */
    QList<QPointF> positionList();

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
     * \brief When this signal is emiited, the preview will select the robot.
     * \param robot The robot which is going to be selected.
     */
    void requireSelectEnemy(Enemy *robot);

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
    inline void updateEnemyList();
    QPushButton *m_okay, *m_cancel, *m_deletePoint;
    QTreeView *m_enemyInitialDataView;
    QStandardItemModel *m_enemyInitialDataModel;
    QList<Enemy *> m_enemyList;
    GroundBase *m_ground;
    QGroupBox *m_manageBox;
};

#endif // ENEMYMANAGEWIDGET_H
