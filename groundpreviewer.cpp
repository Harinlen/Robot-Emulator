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

#include "robot.h"
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
    m_previewRobot(new Robot)
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

void GroundPreviewer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the border.
    painter.setPen(m_groundGlobal->borderColor());
    painter.drawPolygon(m_previewGround);
    //Paint the barracks.
    painter.setPen(m_groundGlobal->barracksColor());
    painter.drawPolygon(m_previewBarracks);
    //If display the preview robot.
    if(m_showPreviewPoint)
    {
        //Paint the preview robot.
        m_previewRobot->paintRobotParameter(&painter);
        m_previewRobot->paintRobot(&painter);
    }
}

QPointF GroundPreviewer::pointFromGround(const QPointF &groundPoint)
{
    return QPointF((groundPoint.x()+m_xOffset)/m_groundParameter*width(),
                   (groundPoint.y()+m_yOffset)/m_groundParameter*height());
}


bool GroundPreviewer::showPreviewPoint() const
{
    return m_showPreviewPoint;
}

void GroundPreviewer::setPreviewBorder(const QPolygonF &groundBorder)
{
    QPolygonF previewGroundBorder;
    //Update the ground height and width.
    QRect borderBoundingRect=groundBorder.toPolygon().boundingRect();
    QSize groundSize=QSize(borderBoundingRect.right(),
                           borderBoundingRect.bottom());
    //Bounding revise.
    groundSize+=QSize(2+(Robot::detectRadius()<<1),
                      2+(Robot::detectRadius()<<1));
    qreal groundHeight=groundSize.height(),
            groundWidth=groundSize.width();
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
    }
    else
    {
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
    //Gernerate the preview border, will zoom the original ground.
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
