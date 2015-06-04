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
#include <QStaticText>

#include "groundglobal.h"

#include "gridwidget.h"

GridWidget::GridWidget(QWidget *parent) :
    QWidget(parent),
    m_widget(nullptr)
{
    setAutoFillBackground(true);
}

void GridWidget::paintEvent(QPaintEvent *event)
{
    //Paint the widget.
    QWidget::paintEvent(event);
    //Paint the reference line.
    QPainter painter(this);
    painter.setPen(GroundGlobal::instance()->referenceLineColor());
    painter.setBrush(QColor(0,0,0,0));
    QFont textFont=font();
    textFont.setPixelSize(4);
    painter.setFont(textFont);
    int stopWidth=width()+height()+m_gridStep;
    for(int i=0; i<stopWidth; i+=m_gridStep)
    {
        //Draw right top to left bottom lines.
        painter.drawLine(i, 0, i-height(), height());
        //Draw left top to right bottom lines.
        painter.drawLine(i, 0, i+height(), height());
        int horizon=i-m_gridSize+8;
        for(int j=-m_gridSize-2; j<height()+m_gridStep; j+=m_gridSize)
        {
            painter.drawStaticText(horizon,
                                   j,
                                   QStaticText("("+QString::number(horizon)+", "+QString::number(j)+")"));
            horizon-=m_gridSize;
        }
    }
    stopWidth=-height();
    for(int i=0; i>=stopWidth; i-=m_gridStep)
    {
        painter.drawLine(i, 0, i+height(), height());
    }
}

int GridWidget::m_gridStep=100;
int GridWidget::m_gridSize=50;

void GridWidget::setGridStep(int gridStep)
{
    //Update the parameter.
    m_gridStep = gridStep;
    m_gridSize = m_gridStep>>1;
}

QWidget *GridWidget::widget() const
{
    return m_widget;
}

void GridWidget::setWidget(QWidget *widget)
{
    m_widget = widget;
    m_widget->setParent(this);
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //If the widget is not null, resize it.
    if(m_widget)
    {
        m_widget->resize(size());
    }
}

