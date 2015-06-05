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

#ifndef GROUNDREALTIMEPREVIEWER_H
#define GROUNDREALTIMEPREVIEWER_H

#include "groundpreviewer.h"

class Robot;
class Enemy;
class GroundBase;
/*!
 * \brief The GroundRealtimePreview class is a advanced ground preview. It can
 * preview a ground data in real-time. This means you have to set a GroundBase
 * to the previewer.
 */
class GroundRealtimePreviewer : public GroundPreviewer
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a GroundRealtimePreviewer.
     * \param parent The parent widget.
     */
    explicit GroundRealtimePreviewer(QWidget *parent = 0);

    /*!
     * \brief Destory the GroundRealtimePreviewer.
     */
    ~GroundRealtimePreviewer();

    /*!
     * \brief The display exist robots setting.
     * \return If display the exist robot, it will be true, or else false.
     */
    bool displayRobots() const;

    /*!
     * \brief Get the selected robot. The robot will display the parameter.
     * \return The selected robot.
     */
    Robot *selectedRobot() const;

    /*!
     * \brief The display exists enemys setting.
     * \return If display the exist enemy, it will be true, or else false.
     */
    bool displayEnemies() const;


signals:

public slots:
    /*!
     * \brief Set the rendering Ground class.
     * \param ground The render ground.
     */
    void setGround(GroundBase *ground);

    /*!
     * \brief Set the previewer will display or hide the exist enemys.
     * \param displayEnemys To display the exist enemys, set it true, or else
     * false.
     */
    void setDisplayEnemies(bool displayEnemies);

    /*!
     * \brief Set the previewer will display or hide the exist robots.
     * \param displayRobots To display the exist robots, set it true, or else
     * false.
     */
    void setDisplayRobots(bool displayRobots);

    /*!
     * \brief Set the selected enemy.
     * \param selectedEnemy The enemy which will be selected. To cancel the
     * selection please set a nullptr.
     */
    void setSelectedEnemy(Enemy *selectedEnemy);

    /*!
     * \brief Set the selected robot.
     * \param selectedRobot The robot which will be selected. To cancel the
     * selection please set a nullptr.
     */
    void setSelectedRobot(Robot *selectedRobot);

    /*!
     * \brief Update the enemy position list.
     * \param positions The position list.
     */
    void setEnemyPositionList(const QList<QPointF> &positions);

    /*!
     * \brief Update all kinds of preview list info about enemy.
     * \param enemys The enemy list.
     * \param positions The position list.
     */
    void setEnemyPreviewList(const QList<Enemy *> &enemys,
                             const QList<QPointF> &positions);

    /*!
     * \brief Update the position list.
     * \param positions The position list
     */
    void setPositionList(const QList<QPointF> &positions);

    /*!
     * \brief Update the angle list.
     * \param angles The angle list.
     */
    void setAngleList(const QList<qreal> &angles);

    /*!
     * \brief Update all kinds of preview list.
     * \param robots The robot list.
     * \param positions The positions list.
     * \param angles The angle list.
     */
    void setRobotsPreviewList(const QList<Robot *> &robots,
                              const QList<QPointF> &positions,
                              const QList<qreal> &angles);

protected:
    /*!
     * \brief Reimplemented from GroundPreviewer::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionBorderChanged();
    void onActionBarracksChanged();

private:
    Robot *m_proxyRobot;
    Enemy *m_proxyEnemy;
    GroundBase *m_ground;
    bool m_displayRobots, m_displayEnemies;
    Robot *m_selectedRobot;
    Enemy *m_selectedEnemy;

    QList<Enemy *> m_enemies;
    QList<QPointF> m_enemyPositions;
    QList<Robot *> m_robots;
    QList<QPointF> m_positions;
    QList<qreal> m_angles;
};

#endif // GROUNDREALTIMEPREVIEWER_H
