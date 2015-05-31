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

#include "groundbase.h"
#include "robot.h"

#include "groundpreviewwidget.h"

#include <QDebug>

GroundPreviewWidget::GroundPreviewWidget(QWidget *parent) :
    QWidget(parent),
    m_ground(nullptr),
    m_previewGround(QPolygonF()),
    m_previewBarracks(QPolygonF()),
    m_xOffset(0),
    m_yOffset(0),
    m_groundParameter(1.0),
    m_showPreviewPoint(false),
    m_previewRobot(new Robot)
{
    setFixedSize(200,200);
}

GroundPreviewWidget::~GroundPreviewWidget()
{
    delete m_previewRobot;
}

void GroundPreviewWidget::setGround(GroundBase *ground)
{
    //Save the ground.
    m_ground=ground;
    //Link the ground.
    connect(m_ground, &GroundBase::borderChanged,
            this, &GroundPreviewWidget::onActionBorderChanged);
    connect(m_ground, &GroundBase::barracksChanged,
            this, &GroundPreviewWidget::onActionBarracksChanged);
}

void GroundPreviewWidget::previewRobot(QPointF position, qreal angle)
{
    //Save the preview data.
    m_previewRobot->setPos(pointFromGround(position));
    m_previewRobot->setAngle(angle);
    //Update the preivew.
    update();
}

void GroundPreviewWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    //Check if we have a ground.
    if(m_ground==nullptr)
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the border.
    painter.setPen(m_ground->borderColor());
    painter.drawPolygon(m_previewGround);
    //Paint the barracks.
    painter.setPen(m_ground->barracksColor());
    painter.drawPolygon(m_previewBarracks);
    //If display the preview robot.
    if(m_showPreviewPoint)
    {
        //Paint the preview robot.
        m_previewRobot->paintRobot(&painter);
    }
}

void GroundPreviewWidget::onActionBorderChanged()
{
    //Check if the ground is available.
    if(m_ground==nullptr)
    {
        return;
    }
    QPolygonF groundBorder=m_ground->border(),
                previewGroundBorder;
    //Update the ground height and width.
    qreal groundHeight=m_ground->size().height(),
            groundWidth=m_ground->size().width();
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

void GroundPreviewWidget::onActionBarracksChanged()
{
    //Check if the ground is available.
    if(m_ground==nullptr)
    {
        return;
    }
    QPolygonF barracksBorder=m_ground->barracks(),
                previewBarracksBorder;
    //Gernerate the preview border, will zoom the original ground.
    for(QPointF borderPoint : barracksBorder)
    {
        previewBarracksBorder.append(pointFromGround(borderPoint));
    }
    //Save the preview border.
    m_previewBarracks=previewBarracksBorder;
    //Update the widget.
    update();
}

inline QPointF GroundPreviewWidget::pointFromGround(const QPointF &groundPoint)
{
    return QPointF((groundPoint.x()+m_xOffset)/m_groundParameter*width(),
                   (groundPoint.y()+m_yOffset)/m_groundParameter*height());
}
bool GroundPreviewWidget::showPreviewPoint() const
{
    return m_showPreviewPoint;
}

void GroundPreviewWidget::setShowPreviewPoint(bool showPreviewPoint)
{
    m_showPreviewPoint = showPreviewPoint;
}

