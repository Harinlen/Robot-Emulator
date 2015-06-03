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
#include <QBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QMessageBox>
#include <QTreeView>
#include <QGroupBox>

#include "robot.h"
#include "groundbase.h"

#include "robotmanagewidget.h"

#include <QDebug>

RobotManageWidget::RobotManageWidget(QWidget *parent) :
    QWidget(parent),
    m_okay(new QPushButton(this)),
    m_cancel(new QPushButton(this)),
    m_deletePoint(new QPushButton(this)),
    m_robotInitialDataView(new QTreeView(this)),
    m_robotInitialDataModel(new QStandardItemModel(this)),
    m_ground(nullptr),
    m_manageBox(new QGroupBox(this))
{
    //Set the palette.
    QPalette pal=m_deletePoint->palette();
    pal.setColor(QPalette::Button, QColor(255,0,0));
    pal.setColor(QPalette::ButtonText, QColor(255,255,255));
    m_deletePoint->setPalette(pal);

    //Configure robot list.
    m_robotInitialDataView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_robotInitialDataView->setModel(m_robotInitialDataModel);
    connect(m_robotInitialDataView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            [=](const QModelIndex &current)
            {
                emit requireSelectRobot(m_robotList.at(current.row()));
            });
    connect(m_robotInitialDataModel, &QStandardItemModel::itemChanged,
            [=](QStandardItem *item)
            {
                //Check item is valid first.
                if(item!=nullptr)
                {
                    //If the item is from angle row.
                    if(2==item->column())
                    {
                        emit requireUpdateAngleList();
                    }
                    else
                    {
                        //Or else, it will be position list.
                        emit requireUpdatePositionList();
                    }
                }
            });

    //Initial layouts.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);


    //Generate the editor layout.
    QBoxLayout *robotViewLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                         mainLayout->widget());
    m_manageBox->setLayout(robotViewLayout);
    robotViewLayout->addWidget(m_robotInitialDataView, 1);
    robotViewLayout->addWidget(m_deletePoint);
    mainLayout->addWidget(m_manageBox, 1);

    //Generate the button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    buttonLayout->setSpacing(2);
    buttonLayout->addWidget(m_okay);
    m_cancel->setShortcut(QKeySequence(Qt::Key_Escape));
    buttonLayout->addWidget(m_cancel);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(m_deletePoint,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                //Check if current index is vaild.
                if(m_robotInitialDataView->selectionModel()->hasSelection() &&
                        m_robotInitialDataView->currentIndex().isValid())
                {
                    int robotIndex=m_robotInitialDataView->currentIndex().row();
                    //Remove the robot from the list.
                    m_robotList.removeAt(robotIndex);
                    //Remove the initial data from the model.
                    m_robotInitialDataModel->removeRow(robotIndex);

                    //Update all.
                    emit requireUpdateAllList();
                }
            });
    connect(m_okay,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                //Check if ground is not nullptr.
                if(m_ground)
                {
                    //Check the position of all the robots.
                    QList<QPointF> positions=positionList();
                    for(int i=0; i<positions.size(); i++)
                    {
                        if(!m_ground->barracks().containsPoint(positions.at(i),
                                                               Qt::WindingFill))
                        {
                            QMessageBox::information(this,
                                                     tr("Invalid positions"),
                                                     tr("The selected positions is outside of the barracks, please change the position."));
                            return;
                        }
                    }
                    //Now, we should ask the ground to sync the data.
                    m_ground->syncRobotData(m_robotList,
                                            positions,
                                            angleList());
                    //Close the management dialog.
                    emit requireClose();
                }
            });
    connect(m_cancel,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]{emit requireClose();});

    retranslate();
}

void RobotManageWidget::showEvent(QShowEvent *event)
{
    //Show the widget.
    QWidget::showEvent(event);
    //Update the robot list.
    updateRobotList();
}

void RobotManageWidget::retranslate()
{
    m_okay->setText(tr("Ok"));
    m_cancel->setText(tr("Cancel"));
    m_deletePoint->setText(tr("Delete"));

    m_manageBox->setTitle(tr("Robots"));

    updateHeader();
}

inline qreal RobotManageWidget::getModelData(int row, int column, int role)
{
    return m_robotInitialDataModel->data(
                m_robotInitialDataModel->index(row, column), role).toDouble();
}

void RobotManageWidget::updateHeader()
{
    QStringList header;
    header << tr("X") << tr("Y") << tr("Angle");
    m_robotInitialDataModel->setHorizontalHeaderLabels(header);
}

void RobotManageWidget::updateRobotList()
{
    //Reset the model.
    m_robotInitialDataModel->clear();
    //Reset all the cache information.
    m_robotList.clear();
    updateHeader();
    //Check ground first.
    if(m_ground==nullptr)
    {
        //Update all the information.
        emit requireUpdateAllList();
        return;
    }
    //Get the robots information.
    m_robotList=m_ground->robotList();
    QList<QPointF> positionList=m_ground->robotInitialPosition();
    QList<qreal> angleList=m_ground->robotInitialAngle();
    //Generate the robot row.
    for(int i=0; i<m_robotList.size(); i++)
    {
        //Get the robot info.
        const QPointF &robotPos=positionList.at(i);
        //Generate the robot row.
        QList<QStandardItem *> robotRow;
        QStandardItem *item=new QStandardItem(QString::number(robotPos.x()));
        robotRow.append(item);
        item=new QStandardItem(QString::number(robotPos.y()));
        robotRow.append(item);
        item=new QStandardItem(QString::number(angleList.at(i)));
        robotRow.append(item);
        //Add the robot row to model.
        m_robotInitialDataModel->appendRow(robotRow);
    }
    //Check the row count.
    if(m_robotInitialDataModel->rowCount()>0)
    {
        m_robotInitialDataView->setCurrentIndex(
                    m_robotInitialDataModel->index(0,0));
    }
    //Update all the information.
    emit requireUpdateAllList();
}

GroundBase *RobotManageWidget::ground() const
{
    return m_ground;
}

QList<Robot *> RobotManageWidget::robotList()
{
    return m_robotList;
}

QList<QPointF> RobotManageWidget::positionList()
{
    //Generate the position list.
    QList<QPointF> pointList;
    for(int i=0; i<m_robotInitialDataModel->rowCount(); i++)
    {
        pointList.append(QPointF(getModelData(i, 0),
                                 getModelData(i, 1)));
    }
    return pointList;
}

QList<qreal> RobotManageWidget::angleList()
{
    //Generate the angle list.
    QList<qreal> angleList;
    for(int i=0; i<m_robotInitialDataModel->rowCount(); i++)
    {
        angleList.append(getModelData(i, 2));
    }
    return angleList;
}

void RobotManageWidget::enabledWidget()
{
    m_okay->setDefault(true);
    setEnabled(true);
}

void RobotManageWidget::disabledWidget()
{
    m_okay->setDefault(false);
    setEnabled(false);
}

void RobotManageWidget::setGround(GroundBase *ground)
{
    //Save the new ground.
    m_ground = ground;
}
