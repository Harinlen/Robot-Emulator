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
    for(int i=30; i<=height(); i+=30)
    {
        painter.drawLine(0, i, width(), i);
    }
    for(int i=30; i<=width(); i+=30)
    {
        painter.drawLine(i, 0, i, height());
    }
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

