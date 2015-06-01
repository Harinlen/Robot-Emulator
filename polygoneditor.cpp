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
#include <QTreeView>
#include <QToolButton>
#include <QStandardItemModel>

#include "pointeditor.h"

#include "polygoneditor.h"

#include <QDebug>

PolygonEditor::PolygonEditor(QWidget *parent) :
    QWidget(parent),
    m_groundPoints(new QTreeView(this)),
    m_groundData(new QStandardItemModel(this))
{
    //Configure the ground points editor.
    m_groundPoints->setSelectionMode(QAbstractItemView::SingleSelection);
    m_groundPoints->setModel(m_groundData);
    m_groundPoints->setDragDropMode(QAbstractItemView::NoDragDrop);
    //Link the model.
    connect(m_groundData, &QStandardItemModel::itemChanged,
            [=]{emit polygonChange(polygon());});
    connect(m_groundData, &QStandardItemModel::rowsInserted,
            [=]{emit polygonChange(polygon());});
    connect(m_groundData, &QStandardItemModel::rowsRemoved,
            [=]{emit polygonChange(polygon());});

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the actions.
    QBoxLayout *actionLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    for(int i=0; i<GroundEditActionsCount; i++)
    {
        m_actions[i]=new QToolButton(this);
        actionLayout->addWidget(m_actions[i]);
    }
    //Set the text.
    m_actions[AddPoint]->setText("+");
    m_actions[RemovePoint]->setText("-");
    actionLayout->addStretch();
    //Set key sequence and actions.
    m_actions[AddPoint]->setShortcut(QKeySequence(Qt::Key_Plus));
    connect(m_actions[AddPoint],
            static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked),
            [=]
            {
                bool getPointResult=false;
                QPointF targetPoint=PointEditor::getPoint(getPointResult);
                //If user want to add this point(click ok).
                if(getPointResult)
                {
                    addPoint(targetPoint);
                }
            });
    connect(m_actions[RemovePoint],
            static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked),
            [=]
            {
                if(m_groundPoints->selectionModel()->currentIndex().isValid())
                {
                    m_groundData->removeRow(m_groundPoints->selectionModel()->currentIndex().row());
                }
            });

    mainLayout->addWidget(m_groundPoints, 1);
    mainLayout->addLayout(actionLayout);

    retranslate();
}

QPolygonF PolygonEditor::polygon()
{
    //Check the point count in the editor widget.
    //If it cannot build a triangle, then return a empty QPolygonF.
    if(m_groundData->rowCount()<3)
    {
        return QPolygonF();
    }
    //Build the polygon.
    QPolygonF border;
    for(int i=0; i<m_groundData->rowCount(); i++)
    {
        border.append(QPointF(m_groundData->item(i, 0)->text().toDouble(),
                              m_groundData->item(i, 1)->text().toDouble()));
    }
    return border;
}

void PolygonEditor::setPolygon(const QPolygonF &polygon)
{
    //Clear all the data in the ground data;
    m_groundData->clear();
    //Set the header data.
    retranslate();
    //Add all the information to ground data.
    for(QPointF point:polygon)
    {
        addPoint(point);
    }
    //Emit the changed signal.
    emit polygonChange(this->polygon());
}

void PolygonEditor::retranslate()
{
    //Update ground points header.
    QStringList groundHeader;
    groundHeader << tr("X") << tr("Y");
    m_groundData->setHorizontalHeaderLabels(groundHeader);
}

void PolygonEditor::addPoint(const QPointF &point)
{
    //Find the if point has been in the model, ignore the same point.
    for(int i=0; i<m_groundData->rowCount(); i++)
    {
        if(point.x()==m_groundData->data(m_groundData->index(i, 0),
                                         Qt::DisplayRole).toString().toDouble()
           && point.y()==m_groundData->data(m_groundData->index(i, 1),
                                            Qt::DisplayRole).toString().toDouble())
        {
            return;
        }
    }

    //Generate the point row.
    QList<QStandardItem *> pointRow;
    pointRow << new QStandardItem(QString::number(point.x()))
             << new QStandardItem(QString::number(point.y()));

    //Append the row.
    if(m_groundPoints->selectionModel()->hasSelection())
    {
        m_groundData->insertRow(m_groundPoints->currentIndex().row(),
                                pointRow);
    }
    else
    {
        m_groundData->appendRow(pointRow);
    }
}
