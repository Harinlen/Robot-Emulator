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
#include "menubar.h"
#include "generategroundbase.h"
#include "groundglobal.h"

#include "ground.h"

#include <QDebug>

Ground::Ground(QWidget *parent) :
    GroundBase(parent),
    m_border(QPolygonF()),
    m_barracks(QPolygonF()),
    m_filePath(QString()),
    m_fileName(QString()),
    m_changed(false),
    m_timeline(new QTimer(this)),
    m_generator(nullptr),
    m_groundGlobal(GroundGlobal::instance()),
    m_lastOpenFolder(QApplication::applicationDirPath()),
    m_lastSaveFolder(QApplication::applicationDirPath())
{
    setContentsMargins(0,0,0,0);
    //Configure the timer.
    m_timeline->setInterval(16); //This will update the image for 60fps.
    connect(m_timeline, &QTimer::timeout,
            this, &Ground::onActionUpdateRobot);

    //Initial the actions.
    for(int i=0; i<GroundActionsCount; i++)
    {
        m_actions[i]=new QAction(this);
    }
    //Set the key sequence.
    m_actions[New]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    m_actions[Open]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    m_actions[Save]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
    m_actions[SaveAs]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
    m_actions[Close]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_W));

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

    retranslate();

    //-----Example-----
//    //Border.
//    debugBorder << QPointF(30, 60) << QPointF(120, 20)
//                << QPointF(400, 45) << QPointF(415, 280)
//                << QPointF(180, 340) << QPointF(15, 210);

//    //Barracks.
//    debugBarracks << QPointF(200, 200) << QPointF(200, 275)
//                  << QPointF(275, 275) << QPointF(275, 200);

//    //Robots.
//    Robot(203, 203));
//    Robot(210, 210));
//    Robot(210, 220));
//    Robot(203, 210));
//    Robot(210, 203));
//    Robot(203, 250));
//    Robot(209, 250));
//    Robot(250, 210));
//    Robot(203, 207));
//    Robot(204, 207));
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
    painter.translate(Robot::detectRadius(), Robot::detectRadius());
    //Draw the reference line.
    painter.setPen(m_groundGlobal->referenceLineColor());
    painter.setBrush(QColor(0,0,0,0));
    for(int i=0; i<=height(); i+=30)
    {
        painter.drawLine(0, i, width(), i);
    }
    for(int i=0; i<=width(); i+=30)
    {
        painter.drawLine(i, 0, i, height());
    }
    //Draw the border.
    painter.setPen(m_groundGlobal->borderColor());
    painter.drawPolygon(m_border);

    //Draw the barracks.
    painter.setPen(m_groundGlobal->barracksColor());
    painter.drawPolygon(m_barracks);

    //Draw all the robot.
    for(Robot *robot : m_robotList)
    {
        robot->paintRobotParameter(&painter);
        robot->paintRobot(&painter);
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
}

void Ground::onActionUpdateRobot()
{
    //If there're more than 1 robot, we will going to detect the robot.
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
    m_timeline->stop();
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
            setBorder(m_generator->border());
            setBarracks(m_generator->barracks());
        }
    }
}

bool Ground::onActionOpen()
{
    //Stop the time line.
    m_timeline->stop();

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
    //Stop the time line.
    m_timeline->stop();

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

    //Hack way to repaint all the things. WTF!
    hide();
    show();

    //Resize ground.
    setFixedSize(0,0);
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
        //If we can go here, then all the data should be ok.
        //Set the border, barracks and robots.
        setBorder(border);
        setBarracks(barracks);
        addRobots(robotList);
        //Change the file information and flags.
        QFileInfo sessionFileInfo(sessionFile);
        m_lastOpenFolder=sessionFileInfo.absoluteDir().absolutePath();
        m_filePath=sessionFileInfo.absoluteFilePath();
        m_fileName=sessionFileInfo.fileName();
        m_changed=false;
        //Update the image.
        update();
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
        QJsonArray border, barracks, robots;
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
    //Update the ground.
    update();
}

QList<Robot *> Ground::robotList() const
{
    return m_robotList;
}

QList<QPointF> Ground::robotInitialPosition() const
{
    return m_robotInitialPosition;
}

QList<qreal> Ground::robotInitialAngle() const
{
    return m_robotInitialAngle;
}
