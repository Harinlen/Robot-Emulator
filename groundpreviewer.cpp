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

#include "robotbase.h"
#include "groundglobal.h"

#include "groundpreviewer.h"

#include <QDebug>

GroundPreviewer::GroundPreviewer(QWidget *parent) :
    QWidget(parent),
    m_previewGround(QPolygonF()),
    m_previewBarracks(QPolygonF()),
    m_xOffset(0),
    m_yOffset(0),
    m_groundParameter(1.0),
    m_showPreviewPoint(false),
    m_previewRobot(new RobotBase),
    m_previewEnemy(new RobotBase)
{
    //Initial size.
    setFixedSize(200,200);
    //Initial ground global.
    m_groundGlobal=GroundGlobal::instance();
}

GroundPreviewer::~GroundPreviewer()
{
    delete m_previewRobot;
}

void GroundPreviewer::previewRobot(QPointF position, qreal angle)
{
    //Save the preview data.
    m_previewRobot->setPos(pointFromGround(position));
    m_previewRobot->setAngle(angle);
    //Update the preivew.
    update();
}

void GroundPreviewer::previewEnemy(QPointF position)
{
    //Save the preview data.
    m_previewEnemy->setPos(pointFromGround(position));
    //Update the preview.
    update();
}

void GroundPreviewer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Fill the background.
    painter.fillRect(rect(), m_groundGlobal->baseColor());
    //Paint the border.
    painter.setPen(m_groundGlobal->borderColor());
    painter.setPen(Qt::NoPen);
    QBrush borderBrush(Qt::SolidPattern);
    borderBrush.setColor(m_groundGlobal->groundColor());
    painter.setBrush(borderBrush);
    painter.drawPolygon(m_previewGround);
    painter.setPen(m_groundGlobal->borderColor());
    borderBrush.setStyle(Qt::FDiagPattern);
    borderBrush.setColor(m_groundGlobal->borderColor());
    painter.setBrush(borderBrush);
    painter.drawPolygon(m_previewGround);
    //Paint the barracks.
    painter.setPen(m_groundGlobal->barracksColor());
    painter.setBrush(QColor(0,0,0,0));
    painter.drawPolygon(m_previewBarracks);
    //If display the preview robot.
    if(m_showPreviewPoint)
    {
        //Paint the preview robot.
        painter.setPen(Qt::NoPen);
        m_previewRobot->paintRobotDetectArea(&painter);
        painter.setPen(RobotBase::directionLineColor());
        painter.setBrush(Qt::NoBrush);
        m_previewRobot->paintRobotParameter(&painter);
        QPen robotPen(RobotBase::robotBorder());
        robotPen.setWidth(2);
        painter.setPen(robotPen);
        painter.setBrush(RobotBase::robotColor());
        m_previewRobot->paintRobot(&painter);
    }
    //If display the preview enemy.
    if(m_showPreviewEnemy)
    {
        //Paint the preview enemy.
        painter.setPen(Qt::NoPen);
        m_previewEnemy->paintEnemyDetectArea(&painter);
        QPen robotPen(RobotBase::robotBorder());
        robotPen.setWidth(2);
        painter.setPen(robotPen);
        painter.setBrush(RobotBase::robotColor());
        m_previewEnemy->paintRobot(&painter);
    }
}

QPointF GroundPreviewer::pointFromGround(const QPointF &groundPoint)
{
    return QPointF((groundPoint.x()+m_xOffset)/m_groundParameter*width(),
                   (groundPoint.y()+m_yOffset)/m_groundParameter*height());
}

bool GroundPreviewer::showPreviewEnemy() const
{
    return m_showPreviewEnemy;
}

void GroundPreviewer::onActionGroundSizeChanged(const QSize &groundSize)
{
    qreal groundHeight=groundSize.height(), groundWidth=groundSize.width();
    //Update the offset.
    if(groundHeight<groundWidth)
    {
        //Calculate the offset.
        m_xOffset=0;
        m_yOffset=(groundWidth-groundHeight)/2;
        //Set the parameter.
        m_groundParameter=groundWidth;
        //Avoid the divided by 0 bug.
        if(m_groundParameter==0)
        {
            m_groundParameter=1.0;
        }
        return;
    }
    //Calculate the offset.
    m_xOffset=(groundHeight-groundWidth)/2;
    m_yOffset=0;
    //Set the parameter.
    m_groundParameter=groundHeight;
    //Avoid the divided by 0 bug.
    if(m_groundParameter==0)
    {
        m_groundParameter=1.0;
    }
}

void GroundPreviewer::setShowPreviewEnemy(bool showPreviewEnemy)
{
    m_showPreviewEnemy = showPreviewEnemy;
}

bool GroundPreviewer::showPreviewPoint() const
{
    return m_showPreviewPoint;
}

void GroundPreviewer::setPreviewBorder(const QPolygonF &groundBorder)
{
    //Update the ground height and width.
    //Gernerate the preview border, will zoom the original ground.
    QPolygonF previewGroundBorder;
    for(QPointF borderPoint : groundBorder)
    {
        previewGroundBorder.append(pointFromGround(borderPoint));
    }
    //Save the preview border.
    m_previewGround=previewGroundBorder;
    //Update the widget.
    update();
}

void GroundPreviewer::setPreviewBarracks(const QPolygonF &groundBarracks)
{
    QPolygonF previewBarracksBorder;
    //Gernerate the preview border, will zoom the original ground.
    for(QPointF borderPoint : groundBarracks)
    {
        previewBarracksBorder.append(pointFromGround(borderPoint));
    }
    //Save the preview border.
    m_previewBarracks=previewBarracksBorder;
    //Update the widget.
    update();
}

void GroundPreviewer::setShowPreviewPoint(bool showPreviewPoint)
{
    //Save the show point settings.
    m_showPreviewPoint = showPreviewPoint;
    //Update.
    update();
}
