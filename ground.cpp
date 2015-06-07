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
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QFileInfo>
#include <QTimer>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QJsonDocument>
#include <QApplication>

#include "robot.h"
#include "enemy.h"
#include "menubar.h"
#include "generategroundbase.h"
#include "groundglobal.h"
#include "languagemanager.h"

#include "ground.h"

#include <QDebug>

Ground::Ground(QWidget *parent) :
    GroundBase(parent),
    m_border(QPolygonF()),
    m_barracks(QPolygonF()),
    m_reachBorderCount(0),
    m_showDirection(true),
    m_showDetectRadius(true),
    m_filePath(QString()),
    m_fileName(QString()),
    m_changed(false),
    m_timeline(new QTimer(this)),
    m_generator(nullptr),
    m_groundGlobal(GroundGlobal::instance()),
    m_lastOpenFolder(QApplication::applicationDirPath()),
    m_lastSaveFolder(QApplication::applicationDirPath())
{
    //Set parameters.
    setContentsMargins(0,0,0,0);
    //Initial robots.
    Robot::initialRobotPatameters();
    //Configure the timer.
    setSpeed(16); //This will update the image for 60fps.
    connect(m_timeline, &QTimer::timeout,
            this, &Ground::onActionUpdateRobot);

    //Initial the actions.
    for(int i=0; i<GroundActionsCount; i++)
    {
        m_actions[i]=new QAction(this);
    }
    m_actions[ShowDetectRange]->setCheckable(true);
    m_actions[ShowDetectRange]->setChecked(true);

    m_actions[ShowDirection]->setCheckable(true);
    m_actions[ShowDirection]->setChecked(true);

    m_actions[ShowCoordinate]->setCheckable(true);
    m_actions[ShowCoordinate]->setChecked(false);
    //Set the key sequence.
    m_actions[New]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    m_actions[Open]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    m_actions[Save]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
    m_actions[SaveAs]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
    m_actions[Close]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_W));

    //Disable the save, save as and close as default.
    m_actions[Save]->setEnabled(false);
    m_actions[SaveAs]->setEnabled(false);
    m_actions[Close]->setEnabled(false);

    connect(m_actions[New],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionNew();});
    connect(m_actions[Open],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionOpen();});
    connect(m_actions[Save],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionSave();});
    connect(m_actions[SaveAs],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionSaveAs();});
    connect(m_actions[Close],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionClose();});

    connect(m_actions[BorderColor],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionChangeBorderColor();});
    connect(m_actions[BarracksColor],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionChangeBarracksColor();});
    connect(m_actions[RobotColor],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionChangeRobotColor();});
    connect(m_actions[RobotDirectionLineColor],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionChangeDirectionLineColor();});
    connect(m_actions[RobotDetectRangeColor],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{onActionChangeDetectRangeLineColor();});

    connect(m_actions[ShowDetectRange],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{update();});
    connect(m_actions[ShowDirection],
            static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            [=]{update();});

    connect(LanguageManager::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslate()));
    retranslate();
}

Ground::~Ground()
{
    //Recover all the memory of the robot.
    qDeleteAll(m_robotList);
}

QPolygonF Ground::border() const
{
    return m_border;
}

void Ground::setBorder(const QPolygonF &border)
{
    //The border should be at least a triangle, if it's only a line or nothing,
    //ignore the request.
    if(border.size()<3)
    {
        return;
    }
    //Save the border, and clear the barracks.
    m_border = border;
    m_barracks = QPolygonF();
    //Set the changed flag.
    m_changed=true;
    //Update the border lines.
    m_borderLines.clear();
    for(int i=0; i<m_border.size()-1; i++)
    {
        m_borderLines.append(QLineF(m_border.at(i), m_border.at(i+1)));
    }
    m_borderLines.append(QLineF(m_border.at(m_border.size()-1),
                                m_border.at(0)));
    //Get the bounding rect of the border.
    QRect borderBoundingRect=m_border.toPolygon().boundingRect();
    QSize groundSize=QSize(borderBoundingRect.right(),
                           borderBoundingRect.bottom());
    //Bounding revise.
    groundSize+=QSize(2+(Robot::detectRadius()<<1),
                      2+(Robot::detectRadius()<<1));
    //Resize the ground.
    setFixedSize(groundSize);
    emit groundSizeChanged(groundSize);
    //Update the widget.
    update();
    //Emit changed signal.
    emit borderChanged();
    emit barracksChanged();
}

void Ground::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Set translation.
    painter.translate(Robot::detectRadius(), Robot::detectRadius());
    //Draw the ground.
    //Paint the ground base.
    painter.setPen(Qt::NoPen);
    QBrush borderBrush(Qt::SolidPattern);
    borderBrush.setColor(m_groundGlobal->groundColor());
    painter.setBrush(borderBrush);
    painter.drawPolygon(m_border);
    //Paint the ground border.
    painter.setPen(m_groundGlobal->borderColor());
    borderBrush.setStyle(Qt::FDiagPattern);
    borderBrush.setColor(m_groundGlobal->borderColor());
    painter.setBrush(borderBrush);
    painter.drawPolygon(m_border);

    //Draw the barracks.
    painter.setPen(m_groundGlobal->barracksColor());
    borderBrush.setColor(m_groundGlobal->barracksColor());
    borderBrush.setStyle(Qt::DiagCrossPattern);
    painter.setOpacity(0.5);
    painter.setBrush(borderBrush);
    painter.drawPolygon(m_barracks);
    painter.setOpacity(1.0);

    //Draw all the robot.
    if(!m_robotList.isEmpty())
    {
        //First, draw the detect area.
        if(m_actions[ShowDetectRange]->isChecked())
        {
            painter.setPen(Qt::NoPen);
            for(Robot *robot : m_robotList)
            {
                robot->paintRobotDetectArea(&painter);
            }
        }
        //Then, draw the parameter.
        if(m_actions[ShowDirection]->isChecked())
        {
            painter.setPen(Robot::directionLineColor());
            painter.setBrush(Qt::NoBrush);
            for(Robot *robot : m_robotList)
            {
                robot->paintRobotParameter(&painter);
            }
        }
        //Finally, draw the robot.
        QPen robotPen(Robot::robotBorder());
        robotPen.setWidth(2);
        painter.setPen(robotPen);
        painter.setBrush(Robot::robotColor());
        for(Robot *robot : m_robotList)
        {
            robot->paintRobot(&painter);
        }
    }

    //Draw all the enemy.
    if(!m_enemyList.isEmpty())
    {
        //First, draw the detect area.
        if(m_actions[ShowDetectRange]->isChecked())
        {
            painter.setPen(Qt::NoPen);
            for(Enemy *enemy : m_enemyList)
            {
                if(!enemy->destory())
                {
                    enemy->paintEnemyDetectArea(&painter);
                }
            }
        }
//        //Then, draw the parameter.
//        if(m_actions[ShowDirection]->isChecked())
//        {
//            painter.setPen(Robot::enemyRadiusColor());
//            painter.setBrush(Qt::NoBrush);
//            for(Enemy *enemy : m_enemyList)
//            {
//                if(!enemy->destory())
//                {
//                    enemy->paintRobotParameter(&painter);
//                }
//            }
//        }
        //Finally, draw the robot.
        QPen robotPen(Robot::robotBorder());
        robotPen.setWidth(2);
        painter.setPen(robotPen);
        painter.setBrush(Robot::robotColor());
        for(Enemy *enemy : m_enemyList)
        {
            if(!enemy->destory())
            {
                enemy->paintRobot(&painter);
            }
        }
    }
}

void Ground::retranslate()
{
    m_actions[New]->setText(tr("New"));
    m_actions[Open]->setText(tr("Open"));
    m_actions[Save]->setText(tr("Save"));
    m_actions[SaveAs]->setText(tr("Save As"));
    m_actions[Close]->setText(tr("Close"));

    m_actions[BorderColor]->setText(tr("Set border color"));
    m_actions[BarracksColor]->setText(tr("Set barracks color"));
    m_actions[RobotColor]->setText(tr("Set robot color"));
    m_actions[RobotDirectionLineColor]->setText(tr("Set robot direction line color"));
    m_actions[RobotDetectRangeColor]->setText(tr("Set robot detect range color"));
    m_actions[ShowDirection]->setText(tr("Show Robot Direction"));
    m_actions[ShowDetectRange]->setText(tr("Show Detect Range"));
    m_actions[ShowCoordinate]->setText(tr("Show Coordinate"));
}

void Ground::onActionUpdateRobot()
{
    //If there're more than 1 robots, we will going to detect the robot.
    if(m_robotList.size()>1)
    {
        QList<Robot *>::iterator beforeLastRobot=m_robotList.end()-1;
        //Give all the robot the detect information.
        for(QList<Robot *>::iterator robot=m_robotList.begin();
            robot!=beforeLastRobot;
            ++robot)
        {
            for(QList<Robot *>::iterator target=robot+1;
                target!=m_robotList.end();
                ++target)
            {
                //Ignore the current robot.
                if(robot==target)
                {
                    continue;
                }
                //If we have detected one of another robot, add them into the both
                //detect list.
                if(inDetectRange(*robot, *target))
                {
                    (*robot)->addToDetectList(*target);
                    (*target)->addToDetectList(*robot);
                }
                else
                {
                    //Or else remove them from each other's detect list.
                    (*robot)->removeFromDetectList(*target);
                    (*target)->removeFromDetectList(*robot);
                }
            }
        }
    }

    //Move un-destoied unit enemy if 80% of the robots reach the border.
    if(m_reachBorderCount>m_minimumMoveEnemyCount)
    {
        //Get the next position of the enemies.
        for(Enemy *enemy : m_enemyList)
        {
            if(!enemy->destory())
            {
                bool reachTarget=false;
                QPointF nextStep=enemy->nextStep(reachTarget);

                if(reachTarget)
                {
                    if(Enemy::missionComplete())
                    {
                        continue;
                    }
                    //Set mission complete flag.
                    Enemy::setMissionComplete(true);
                    //Generate the four target point.
                    QList<QPointF> targetPoints;
                    targetPoints << QPointF(0, 0)
                                 << QPointF(width(), 0)
                                 << QPointF(0, height())
                                 << QPointF(width(), height());
                    //Set the new target.
                    for(QPointF point : targetPoints)
                    {
                        if(!m_border.containsPoint(point,
                                                   Qt::WindingFill))
                        {
                            Enemy::setTarget(point);
                            break;
                        }
                    }
                    //Get the new next step.
                    nextStep=enemy->nextStep(reachTarget);
                    //Add next step to steps list.
                    enemy->setPos(nextStep);
                }
                enemy->setPos(nextStep);
            }
        }
    }

    //If there'r more than 1 enemies, we will going to detect the enemy.
    if(!m_enemyList.isEmpty() && !m_robotList.isEmpty())
    {
        for(Enemy *enemy : m_enemyList)
        {
            //If current enemy is not destoried.
            if(!enemy->destory())
            {
                //Check the distance to all the robot.
                for(Robot *robot : m_robotList)
                {
                    if(pointDistance(enemy->pos(), robot->pos())
                            < Robot::detectRadius())
                    {
                        enemy->setDestory(true);
                        break;
                    }
                }
            }
        }
    }

    //Update all the robot.
    for(Robot *robot : m_robotList)
    {
        //If the robot is still don't have a line to guard,
        if(!robot->hasGuardianLine())
        {
            //Detect if the robot is in the border, if the robot is out of the
            //border.
            if(!m_border.containsPoint(robot->nextStep(),
                                       Qt::WindingFill))
            {
                //Find the line of the robot should guard, the robot should
                //guard the most recent line.
                //Appoint the robot to guard that line.
                appointGuardianLine(robot);
                //Add the counter.
                m_reachBorderCount++;
            }
        }
        //Update the direction of the robot.
        robot->updateDirection();
    }
    //Ask all the robot to move one step.
    for(Robot *robot : m_robotList)
    {
        //Move the robot.
        robot->moveOneStep();
    }
    //Update the image.
    update();
}

void Ground::onActionNew()
{
    //Stop the time line.
    pause();
    //Close the current file.
    if(!onActionClose())
    {
        return;
    }
    //Should call generate ground widget here, and then judge it's success or
    //failed. If success, set the ground information, or else abandon.
    if(m_generator)
    {
        //Clear the previous data.
        m_generator->setBorder(QPolygonF());
        m_generator->setBarracks(QPolygonF());
        //Get the new data.
        if(QDialog::Accepted==m_generator->exec())
        {
            //Set the border and barracks.
            setBorder(m_generator->border());
            setBarracks(m_generator->barracks());
            //Set changed flag.
            m_changed=true;
            //Update the image.
            update();
            //Enabled save and save as actions.
            m_actions[Save]->setEnabled(true);
            m_actions[SaveAs]->setEnabled(true);
            m_actions[Close]->setEnabled(true);
        }
    }
}

bool Ground::onActionOpen()
{
    //Stop the time line.
    pause();

    //Close the current file.
    if(!onActionClose())
    {
        return false;
    }
    //Get the session file path.
    QString sessionFilePath=QFileDialog::getOpenFileName(this,
                                                         tr("Open session"),
                                                         m_lastOpenFolder,
                                                         tr("Robot Emulator Session") + "(*.sss)");
    if(sessionFilePath.isEmpty())
    {
        return false;
    }
    //Read the session file.
    return readGroundData(sessionFilePath);
}

bool Ground::onActionSave()
{
    //If the file don't need to save, then return false.
    if(!m_changed)
    {
        return false;
    }
    //Check the file path is empty or not, if it's empty, call the save as.
    if(m_filePath.isEmpty())
    {
        return onActionSaveAs();
    }
    //Or else, just write the data to the file.
    m_changed=!writeGroundData();
    return !m_changed;
}

bool Ground::onActionSaveAs()
{
    //Get the new file path.
    QString preferFilePath=QFileDialog::getSaveFileName(this,
                                                        tr("Save session"),
                                                        m_lastSaveFolder,
                                                        tr("Robot Emulator Session") + "(*.sss)");
    if(preferFilePath.isEmpty())
    {
        return false;
    }
    //Set session file information.
    m_filePath=preferFilePath;
    QFileInfo sessionFileInfo(m_filePath);
    m_fileName=sessionFileInfo.fileName();
    //Save the data to session file.
    m_changed=!writeGroundData();
    return !m_changed;
}

bool Ground::onActionClose()
{
    //Stop the timeline.
    pause();

    //Check if the current state is already close.
    if(m_filePath.isEmpty() && !m_changed)
    {
        //Treat this as close successful.
        return true;
    }

    //Check the current session has been saved or not.
    const int buttonSave=0, buttonCancel=1, buttonAbandon=2;
    if(m_changed)
    {
        //There's a session which is modified but not save.
        int result=QMessageBox::question(this,
                                         tr("Close unsaved session"),
                                         tr("Do you want to save the changes you made in the document \"%1\"?").arg(
                                             m_fileName.isEmpty()?tr("Untitled"):m_fileName),
                                         tr("Save"),
                                         tr("Cancel"),
                                         tr("Don't Save"),
                                         buttonSave,
                                         buttonCancel);
        switch(result)
        {
        case buttonSave:
            //If we saved fail, then we can't close the document.
            if(!onActionSave())
            {
                return false;
            }
        case buttonCancel:
            //If user cancel close, then stop to close.
            return false;
        case buttonAbandon:
            //Continue close the file.
            break;
        default:
            //You must kidding me if goes here.
            return false;
        }
    }
    //Clear the ground data.
    clearGroundData();
    //Reset the file status data.
    m_filePath=QString();
    m_fileName=QString();
    m_changed=false;
    //Disable save and save as.
    m_actions[Save]->setEnabled(false);
    m_actions[SaveAs]->setEnabled(false);
    m_actions[Close]->setEnabled(false);
    //Update the panel.
    update();
    return true;
}

void Ground::onActionChangeBorderColor()
{
    //Get the prefer color.
    QColor preferBorderColor=
            QColorDialog::getColor(m_groundGlobal->borderColor(),
                                   this,
                                   tr("Get border color"),
                                   QColorDialog::ShowAlphaChannel |
                                   QColorDialog::DontUseNativeDialog);
    if(!preferBorderColor.isValid())
    {
        return;
    }
    //Set the color.
    m_groundGlobal->setBorderColor(preferBorderColor);
    //Update the panel.
    update();
}

void Ground::onActionChangeBarracksColor()
{
    //Get the prefer color.
    QColor preferBarracksColor=
            QColorDialog::getColor(m_groundGlobal->barracksColor(),
                                   this,
                                   tr("Get barracks color"),
                                   QColorDialog::ShowAlphaChannel |
                                   QColorDialog::DontUseNativeDialog);
    if(!preferBarracksColor.isValid())
    {
        return;
    }
    //Set the color.
    m_groundGlobal->setBarracksColor(preferBarracksColor);
    //Update the panel.
    update();
}

void Ground::onActionChangeRobotColor()
{
    //Get the prefer color.
    QColor preferRobotColor=
            QColorDialog::getColor(Robot::robotColor(),
                                   this,
                                   tr("Get border color"),
                                   QColorDialog::ShowAlphaChannel |
                                   QColorDialog::DontUseNativeDialog);
    if(!preferRobotColor.isValid())
    {
        return;
    }
    //Set the color.
    Robot::setRobotColor(preferRobotColor);
    //Update the panel.
    update();
}

void Ground::onActionChangeDirectionLineColor()
{
    //Get the prefer color.
    QColor preferDirectionLineColor=
            QColorDialog::getColor(Robot::directionLineColor(),
                                   this,
                                   tr("Get border color"),
                                   QColorDialog::ShowAlphaChannel |
                                   QColorDialog::DontUseNativeDialog);
    if(!preferDirectionLineColor.isValid())
    {
        return;
    }
    //Set the color.
    Robot::setDirectionLineColor(preferDirectionLineColor);
    //Update the panel.
    update();
}

void Ground::onActionChangeDetectRangeLineColor()
{
    //Get the prefer color.
    QColor preferDetectRadiusColor=
            QColorDialog::getColor(Robot::detectRadiusColor(),
                                   this,
                                   tr("Get border color"),
                                   QColorDialog::ShowAlphaChannel |
                                   QColorDialog::DontUseNativeDialog);
    if(!preferDetectRadiusColor.isValid())
    {
        return;
    }
    //Set the color.
    Robot::setDetectRadiusColor(preferDetectRadiusColor);
    //Update the panel.
    update();
}

void Ground::clearGroundData()
{
    //Clear the border, border information, and barracks.
    m_border=QPolygon();
    m_borderLines.clear();
    m_barracks=QPolygon();
    //Remove all the robot datas.
    qDeleteAll(m_robotList);
    m_robotList.clear();
    m_robotInitialAngle.clear();
    m_robotInitialPosition.clear();
    //Remove all the enemy datas.
    qDeleteAll(m_enemyList);
    m_enemyList.clear();
    m_enemyInitialPosition.clear();
    Enemy::setTarget(QPointF(0,0));
    //Reset the counters.
    m_reachBorderCount=0;
    m_minimumMoveEnemyCount=0;
    //Hack way to repaint all the things. WTF!
    hide();
    show();

    //Resize ground.
    setFixedSize(0,0);
    emit groundSizeChanged(QSize(0,0));
}

bool Ground::readGroundData(const QString &filePath)
{
    //Try to open the file.
    QFile sessionFile(filePath);
    if(sessionFile.open(QIODevice::ReadOnly))
    {
        //Read the data from the file.
        QTextStream sessionStream(&sessionFile);
        sessionStream.setCodec("UTF-8");
        //Read and parse the json.
        QJsonObject sessionObject=
                QJsonDocument::fromJson(sessionStream.readAll().toUtf8()).object();
        sessionFile.close();

        //Generate the border.
        QJsonArray borderData=sessionObject.value("Border").toArray();
        //Check if the border is vaild or not.
        if(borderData.size()<3) //Simplified than a triangle.
        {
            return false;
        }
        QPolygonF border;
        for(QJsonArray::iterator i=borderData.begin();
            i!=borderData.end();
            ++i)
        {
            QJsonArray pointData=(*i).toArray();
            //The array must contains only x() and y() data of the point.
            if(pointData.size()!=2)
            {
                return false;
            }
            border.append(QPointF(pointData.at(0).toDouble(),
                                  pointData.at(1).toDouble()));
        }

        //Generate the barracks.
        QJsonArray barracksData=sessionObject.value("Barracks").toArray();
        //Check if the barracks is vaild or not.
        if(barracksData.size()<3) //Simlified than a triangle.
        {
            return false;
        }
        QPolygonF barracks;
        for(QJsonArray::iterator i=barracksData.begin();
            i!=barracksData.end();
            ++i)
        {
            QJsonArray pointData=(*i).toArray();
            //The array must contains only x() and y() data of the point.
            if(pointData.size()!=2)
            {
                return false;
            }
            //Check the point is vaild or not.
            QPointF barracksPoint=QPointF(pointData.at(0).toDouble(),
                                          pointData.at(1).toDouble());
            if(!border.containsPoint(barracksPoint, Qt::WindingFill))
            {
                return false;
            }
            barracks.append(barracksPoint);
        }

        //Generate the robot list.
        QList<Robot *> robotList;
        QJsonArray robotsData=sessionObject.value("Robots").toArray();
        for(QJsonArray::iterator i=robotsData.begin();
            i!=robotsData.end();
            ++i)
        {
            //Generate the robot.
            QJsonObject robotData=(*i).toObject();
            QJsonArray robotPosition=robotData.value("Position").toArray();
            if(robotPosition.size()!=2)
            {
                //Clear the robots that has been genereated.
                qDeleteAll(robotList);
                return false;
            }
            Robot *robot=new Robot(robotPosition.at(0).toDouble(),
                                   robotPosition.at(1).toDouble());
            robot->setAngle(robotData.value("Angle").toDouble());

            robotList.append(robot);
        }

        //Generate the enemy list.
        QList<Enemy *> enemyList;
        QJsonArray enemiesData=sessionObject.value("Enemies").toArray();
        for(QJsonArray::iterator i=enemiesData.begin();
            i!=enemiesData.end();
            ++i)
        {
            //Generate enemy.
            QJsonObject enemyData=(*i).toObject();
            QJsonArray enemyPosition=enemyData.value("Position").toArray();
            if(enemyPosition.size()!=2)
            {
                //Clear the enemies that has been generated.
                qDeleteAll(enemyList);
                //Clear the robots that has been generated.
                qDeleteAll(robotList);
                return false;
            }
            Enemy *enemy=new Enemy(enemyPosition.at(0).toDouble(),
                                   enemyPosition.at(1).toDouble());
            enemyList.append(enemy);
        }
        //If we can go here, then all the data should be ok.
        //Set the border, barracks and robots.
        setBorder(border);
        setBarracks(barracks);
        addRobots(robotList);
        addEnemies(enemyList);
        //Change the file information and flags.
        QFileInfo sessionFileInfo(sessionFile);
        m_lastOpenFolder=sessionFileInfo.absoluteDir().absolutePath();
        m_filePath=sessionFileInfo.absoluteFilePath();
        m_fileName=sessionFileInfo.fileName();
        m_changed=false;
        //Update the image.
        update();
        //Enabled save and save as actions.
        m_actions[Save]->setEnabled(true);
        m_actions[SaveAs]->setEnabled(true);
        m_actions[Close]->setEnabled(true);
        return true;
    }
    return false;
}

bool Ground::writeGroundData()
{
    QFile sessionFile(m_filePath);
    if(sessionFile.open(QIODevice::WriteOnly))
    {
        //Generate the json object.
        QJsonObject sessionObject;
        QJsonArray border, barracks, robots, enemies;
        //Write the border data to session object.
        for(QPointF borderPoint : m_border)
        {
            QJsonArray point;
            point.append(borderPoint.x());
            point.append(borderPoint.y());
            border.append(point);
        }
        sessionObject.insert("Border", border);
        //Write barracks data to session object.
        for(QPointF barracksPoint : m_barracks)
        {
            QJsonArray point;
            point.append(barracksPoint.x());
            point.append(barracksPoint.y());
            barracks.append(point);
        }
        sessionObject.insert("Barracks", barracks);
        //Write robot initial data to session object.
        for(int i=0; i<m_robotList.size(); i++)
        {
            QJsonObject robot;
            //Insert the initial position.
            QJsonArray position;
            const QPointF &robotInitialPos=m_robotInitialPosition.at(i);
            position.append(robotInitialPos.x());
            position.append(robotInitialPos.y());
            robot.insert("Position", position);
            //Insert the initial angle.
            robot.insert("Angle", m_robotInitialAngle.at(i));
            //Add robot to robot list.
            robots.append(robot);
        }
        sessionObject.insert("Robots", robots);
        //Write enemy initial data to session object.
        for(int i=0; i<m_enemyList.size(); i++)
        {
            QJsonObject enemy;
            //Insert the initial position.
            QJsonArray position;
            const QPointF &enemyInitialPos=m_enemyInitialPosition.at(i);
            position.append(enemyInitialPos.x());
            position.append(enemyInitialPos.y());
            enemy.insert("Position", position);
            //Add enemy to enemy list.
            enemies.append(enemy);
        }
        sessionObject.insert("Enemies", enemies);
        //Write the object to file using UTF-8 encoding.
        QTextStream sessionStream(&sessionFile);
        sessionStream.setCodec("UTF-8");
        sessionStream << QJsonDocument(sessionObject).toJson() << flush;
        //Close the file.
        sessionFile.close();
        //Get the file info.
        QFileInfo sessionFileInfo(sessionFile);
        m_lastSaveFolder=sessionFileInfo.absoluteDir().absolutePath();
        return true;
    }
    return false;
}

inline bool Ground::inDetectRange(Robot *from, Robot *to)
{
    return QLineF(from->pos(), to->pos()).length()<Robot::detectRadius();
}

inline void Ground::appointGuardianLine(Robot *robot)
{
    qreal minimalDistance=-1.0;
    QLineF guardianLine;
    QPointF footPoint;
    //Get the distance of the robot to all the border line.
    for(QLineF line : m_borderLines)
    {
        QPointF currentFoot;
        //Calculate the distance.
        qreal currentDistance=getDistance(robot->pos(), line, currentFoot);
        //Check the distance.
        if(minimalDistance<0.0 || currentDistance<minimalDistance)
        {
            //Save the minimum distance, and the line's information.
            minimalDistance=currentDistance;
            guardianLine=line;
            footPoint=currentFoot;
        }
    }
    //Set the robot to guard that line.
    robot->setGuardianLine(guardianLine, footPoint);
}

inline qreal Ground::pointDistance(const QPointF &a, const QPointF &b)
{
    return QLineF(a, b).length();
}

inline qreal Ground::getDistance(const QPointF &point,
                                 const QLineF &line,
                                 QPointF &footPoint)
{
    //The line whose angle is 90.0 or 270.0 has no gradient.
    //The distance will be the absolute of the difference of x.
    if(line.angle()==90.0 || line.angle()==270.0)
    {
        footPoint=QPointF(line.p1().x(), point.y());
        return qAbs(line.p1().x()-point.x());
    }
    //Calculate the gradient.
    qreal k=(line.p2().y()-line.p1().y())/(line.p2().x()-line.p1().x());
    //Get the foot point.
    qreal footX=(k*k*line.p1().x()+k*(point.y()-line.p1().y())+point.x())/
            (k*k+1);
    footPoint=QPointF(footX, k*(footX-line.p1().x())+line.p1().y());
    //Get the distance.
    return QLineF(point, footPoint).length();
}

QPolygonF Ground::barracks() const
{
    return m_barracks;
}

bool Ground::addRobot(Robot *robot)
{
    //Check the robot.
    //If the position of the robot is outside barracks, or there's already have
    //a robot, you can't add this robot.
    if(!m_barracks.containsPoint(robot->pos(), Qt::WindingFill) ||
            m_robotInitialPosition.contains(robot->pos()))
    {
        //Delete the robot.
        delete robot;
        return false;
    }
    //Add the available robot to list.
    m_robotList.append(robot);
    //Update the minimum robot conut.
    m_minimumMoveEnemyCount=0.8*(qreal)m_robotList.size();
    //Set the changed flag.
    m_changed=true;
    //Add the robot to initial position.
    m_robotInitialPosition.append(robot->pos());
    m_robotInitialAngle.append(robot->angle());
    return true;
}

void Ground::addRobots(const QList<Robot *> &robots)
{
    //Add the robot one by one.
    for(QList<Robot *>::const_iterator i=robots.begin();
        i!=robots.end();
        ++i)
    {
        addRobot(*i);
    }
}

bool Ground::addEnemy(Enemy *enemy)
{
    //Check the enemy.
    //If the position of the enemy is inside the border, or there's already have
    //a enemy, you can't add this enemy.
    if(m_border.containsPoint(enemy->pos(), Qt::WindingFill) ||
            m_enemyInitialPosition.contains(enemy->pos()))
    {
        //Delete the enemy.
        delete enemy;
        return false;
    }
    //Add the avaliable enemy to list.
    m_enemyList.append(enemy);
    //Set the changed flag.
    m_changed=true;
    //Add the enemy to initial position.
    m_enemyInitialPosition.append(enemy->pos());
    //Resize the ground.
    QSize preferSize=
            QSize(qMax(width(), (int)(enemy->pos().x()+(Enemy::detectRadius()<<1))),
                  qMax(height(), (int)(enemy->pos().y()+(Enemy::detectRadius()<<1))));
    //If the size is not the same as current size, emit size changed signal.
    if(preferSize!=size())
    {
        setFixedSize(preferSize);
        emit groundSizeChanged(preferSize);
    }
    return true;
}

void Ground::addEnemies(const QList<Enemy *> &enemies)
{
    //Add the enemy one by one.
    for(QList<Enemy *>::const_iterator i=enemies.begin();
        i!=enemies.end();
        ++i)
    {
        addEnemy(*i);
    }
}

void Ground::setMenuBar(MenuBar *menuBar)
{
    menuBar->addCategoryAction(MenuBar::File, m_actions[New]);
    menuBar->addCategoryAction(MenuBar::File, m_actions[Open]);
    menuBar->addCategoryAction(MenuBar::File, m_actions[Save]);
    menuBar->addCategoryAction(MenuBar::File, m_actions[SaveAs]);
    menuBar->addCategoryAction(MenuBar::File, m_actions[Close]);

    menuBar->addCategoryAction(MenuBar::Ground, m_actions[BorderColor]);
    menuBar->addCategoryAction(MenuBar::Ground, m_actions[BarracksColor]);
    menuBar->addCategoryAction(MenuBar::Ground, m_actions[RobotColor]);
    menuBar->addCategoryAction(MenuBar::Ground, m_actions[RobotDirectionLineColor]);
    menuBar->addCategoryAction(MenuBar::Ground, m_actions[RobotDetectRangeColor]);
    menuBar->addCategoryAction(MenuBar::Ground, m_actions[ShowDirection]);
    menuBar->addCategoryAction(MenuBar::Ground, m_actions[ShowDetectRange]);
    menuBar->addCategoryAction(MenuBar::Ground, m_actions[ShowCoordinate]);
}

void Ground::setGenerator(GenerateGroundBase *generator)
{
    //Save the generator.
    m_generator=generator;
}

void Ground::syncRobotData(const QList<Robot *> &robots,
                           const QList<QPointF> &initialPosition,
                           const QList<qreal> &initialAngle)
{
    //Search all the robots should be removed.
    while(!m_robotList.isEmpty())
    {
        Robot *currentRobot=m_robotList.takeLast();
        //Check the robot is still in the list.
        if(!robots.contains(currentRobot))
        {
            //Delete the robot.
            delete currentRobot;
        }
    }
    //Save the new robot list, initial position list and initial angle list.
    m_robotList=robots;
    m_robotInitialPosition=initialPosition;
    m_robotInitialAngle=initialAngle;
    //Set the change flag.
    m_changed=true;
    //Reset the ground.
    reset();
}

void Ground::syncEnemyData(const QList<Enemy *> &enemy,
                           const QList<QPointF> &initialPosition)
{
    //Search all the robots should be removed.
    while(!m_enemyList.isEmpty())
    {
        Enemy *currentEnemy=m_enemyList.takeLast();
        //Check the robot is still in the list.
        if(!enemy.contains(currentEnemy))
        {
            //Delete the robot.
            delete currentEnemy;
        }
    }
    //Save the new enemy list and initial positions.
    m_enemyList=enemy;
    m_enemyInitialPosition=initialPosition;
    //Set the change flag.
    m_changed=true;
    //Reset the ground.
    reset();
}

QAction *Ground::showCoordinate()
{
    return m_actions[ShowCoordinate];
}

void Ground::setSpeed(const int &speed)
{
    m_timeline->setInterval(speed);
}

void Ground::setBarracks(const QPolygonF &barracks)
{
    //Check if the barracks is all in the border.
    //If there's any point is not in the border, ignore the set request.
    for(QPolygonF::const_iterator i=barracks.begin();
        i!=barracks.end();
        ++i)
    {
        if(!m_border.containsPoint(*i, Qt::WindingFill))
        {
            return;
        }
    }
    //Save the barracks.
    m_barracks = barracks;
    //Set the enemy target.
    Enemy::setTarget(m_barracks.boundingRect().center());
    //Set the changed flag.
    m_changed=true;
    //Update widget.
    update();
    //Emit the barracks changed signal.
    emit barracksChanged();
}

void Ground::pause()
{
    //Stop the timer.
    m_timeline->stop();
}

void Ground::start()
{
    //Start the time line.
    m_timeline->start();
}

void Ground::nextFrame()
{
    //When the time line is running, ignore the request.
    if(m_timeline->isActive())
    {
        return;
    }
    //Call update robot once.
    onActionUpdateRobot();
}

void Ground::reset()
{
    //Stop the time line.
    m_timeline->stop();
    //Clear the counter.
    m_reachBorderCount=0;
    //Reset all the robot.
    for(int i=0; i<m_robotList.size(); i++)
    {
        Robot *robot=m_robotList.at(i);
        //Clear the guardian line data.
        robot->resetGuardianLine();
        //Clear the robot cache data.
        robot->resetDetectList();
        //Reset the robot position and angle.
        robot->setPos(m_robotInitialPosition.at(i));
        robot->setAngle(m_robotInitialAngle.at(i));
    }
    //Reset all the enemy.
    for(int i=0; i<m_enemyList.size(); i++)
    {
        Enemy *enemy=m_enemyList.at(i);
        //Clear the destory status.
        enemy->setDestory(false);
        //Reset the position.
        enemy->setPos(m_enemyInitialPosition.at(i));
    }
    //Reset the enemy target point and flag.
    Enemy::setMissionComplete(false);
    Enemy::setTarget(m_barracks.boundingRect().center());
    //Update the ground.
    update();
}

bool Ground::closeFile()
{
    return onActionClose();
}

QList<Robot *> Ground::robotList() const
{
    return m_robotList;
}

QList<Enemy *> Ground::enemyList() const
{
    return m_enemyList;
}

QList<QPointF> Ground::enemyInitialPosition() const
{
    return m_enemyInitialPosition;
}

QList<QPointF> Ground::robotInitialPosition() const
{
    return m_robotInitialPosition;
}

QList<qreal> Ground::robotInitialAngle() const
{
    return m_robotInitialAngle;
}
