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

#ifndef GROUND_H
#define GROUND_H

#include "groundbase.h"

class QTimer;
class GroundGlobal;
/*!
 * \brief The Ground class a default realization of the GroundBase class.
 */
class Ground : public GroundBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct Ground class with the given parent.
     * \param parent The parent widget of the Ground class.
     */
    explicit Ground(QWidget *parent = 0);
    ~Ground();

    /*!
     * \brief Reimplemented from GroundBase::border().
     */
    QPolygonF border() const;

    /*!
     * \brief Reimplemented from GroundBase::barracks().
     */
    QPolygonF barracks() const;

    /*!
     * \brief Reimplemented from GroundBase::addRobot().
     */
    bool addRobot(Robot *robot);

    /*!
     * \brief Reimplemented from GroundBase::addRobots().
     */
    void addRobots(const QList<Robot *> &robots);

    /*!
     * \brief Reimplemented from GroundBase::addEnemy().
     */
    bool addEnemy(Enemy *enemy);

    /*!
     * \brief Reimplemented from GroundBase::addEnemies().
     */
    void addEnemies(const QList<Enemy *> &enemies);

    /*!
     * \brief Reimplemented from GroundBase::setMenuBar().
     */
    void setMenuBar(MenuBar *menuBar);

    /*!
     * \brief Reimplemented from GroundBase::setGenerator().
     */
    void setGenerator(GenerateGroundBase *generator);

    /*!
     * \brief Reimplemented from GroundBase::syncRobotData().
     */
    void syncRobotData(const QList<Robot *> &robots,
                       const QList<QPointF> &initialPosition,
                       const QList<qreal> &initialAngle);

    /*!
     * \brief Reimplemented from GroundBase::syncEnemyData().
     */
    void syncEnemyData(const QList<Enemy *> &enemy,
                       const QList<QPointF> &initialPosition);

    /*!
     * \brief Reimplemented from GroundBase::showCoordinate().
     */
    QAction *showCoordinate();

    /*!
     * \brief Reimplemented from GroundBase::robotList().
     */
    QList<Robot *> robotList() const;

    /*!
     * \brief Reimplemented from GroundBase::enemyList().
     */
    QList<Enemy *> enemyList() const;

    /*!
     * \brief Reimplemented from GroundBase::enemyInitialPosition().
     */
    QList<QPointF> enemyInitialPosition() const;

    /*!
     * \brief Reimplemented from GroundBase::robotInitialPosition().
     */
    QList<QPointF> robotInitialPosition() const;

    /*!
     * \brief Reimplemented from GroundBase::robotInitialAngle().
     */
    QList<qreal> robotInitialAngle() const;

signals:

public slots:
    /*!
     * \brief Reimplemented from GroundBase::setSpeed().
     */
    void setSpeed(const int &speed);

    /*!
     * \brief Reimplemented from GroundBase::setBorder(const QPolygonF &).
     */
    void setBorder(const QPolygonF &border);

    /*!
     * \brief Reimplemented from GroundBase::setBarracks(const QPolygonF &).
     */
    void setBarracks(const QPolygonF &barracks);

    /*!
     * \brief Reimplemented from GroundBase::pause().
     */
    void pause();

    /*!
     * \brief Reimplemented from GroundBase::start().
     */
    void start();

    /*!
     * \brief Reimplemented from GroundBase::nextFrame().
     */
    void nextFrame();

    /*!
     * \brief Reimplemented from GroundBase::reset().
     */
    void reset();

    /*!
     * \brief Close the current open file.
     * \return Success or not.
     */
    bool closeFile();

protected:
    /*!
     * \brief This event handler can be reimplemented in a subclass to receive
     * paint events passed in event. \n
     * Draw all the elements of the ground, included: robots, border and
     * barracks.\n
     * Reimplemented from QWidget::paintEvent().
     * \param event The event handle.
     */
    void paintEvent(QPaintEvent *event);

private slots:
    void retranslate();
    void onActionUpdateRobot();

    void onActionNew();
    bool onActionOpen();
    bool onActionSave();
    bool onActionSaveAs();
    bool onActionClose();

    void onActionChangeBorderColor();
    void onActionChangeBarracksColor();
    void onActionChangeRobotColor();
    void onActionChangeDirectionLineColor();
    void onActionChangeDetectRangeLineColor();

private:
    inline void clearGroundData();
    inline bool readGroundData(const QString &filePath);
    inline bool writeGroundData();
    inline bool inDetectRange(Robot *from, Robot *to);
    inline void appointGuardianLine(Robot *robot);
    inline qreal pointDistance(const QPointF &a, const QPointF &b);
    inline qreal getDistance(const QPointF &point,
                             const QLineF &line,
                             QPointF &footPoint);
    enum GroundActions
    {
        New,
        Open,
        Save,
        SaveAs,
        Close,
        BorderColor,
        BarracksColor,
        RobotColor,
        RobotDirectionLineColor,
        RobotDetectRangeColor,
        ShowDirection,
        ShowDetectRange,
        ShowCoordinate,
        GroundActionsCount
    };
    QAction *m_actions[GroundActionsCount];

    QPolygonF m_border, m_barracks;
    QList<Robot *> m_robotList;
    QList<Enemy *> m_enemyList;

    int m_reachBorderCount, m_minimumMoveEnemyCount;

    //Display options.
    bool m_showDirection, m_showDetectRadius;

    //Project file status.
    QString m_filePath, m_fileName;
    bool m_changed;

    //Initial data recorder.
    QList<QPointF> m_robotInitialPosition, m_enemyInitialPosition;
    QList<qreal> m_robotInitialAngle;

    QList<QLineF> m_borderLines;

    QTimer *m_timeline;
    GenerateGroundBase *m_generator;

    //Ground Global data.
    GroundGlobal *m_groundGlobal;

    //Last folder.
    QString m_lastOpenFolder, m_lastSaveFolder;
};

#endif // GROUND_H
