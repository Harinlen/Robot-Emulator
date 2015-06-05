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
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include "enemyaddwidget.h"

EnemyAddWidget::EnemyAddWidget(QWidget *parent) :
    QWidget(parent),
    m_editArea(new QGroupBox(this)),
    m_okay(new QPushButton(this)),
    m_cancel(new QPushButton(this))
{
    //Intial labels.
    for(int i=0; i<3; i++)
    {
        m_labels[i]=new QLabel(this);
    }
    m_xData=new QLineEdit(this);
    m_yData=new QLineEdit(this);

    //Link edit widget.
    connect(m_xData, SIGNAL(textChanged(QString)),
            this, SLOT(onActionParameterChange()));
    connect(m_yData, SIGNAL(textChanged(QString)),
            this, SLOT(onActionParameterChange()));

    //Initial layouts.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    //Generate the control layout.
    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             mainLayout->widget());
    controlLayout->setSpacing(2);
    m_editArea->setLayout(controlLayout);
    mainLayout->addWidget(m_editArea, 1);

    QBoxLayout *positionLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              mainLayout->widget());
    positionLayout->setSpacing(5);
    positionLayout->addWidget(m_labels[0]);
    positionLayout->addWidget(m_xData, 1);
    positionLayout->addSpacing(10);
    positionLayout->addWidget(m_labels[1]);
    positionLayout->addWidget(m_yData, 1);
    controlLayout->addLayout(positionLayout);
    controlLayout->addStretch();

    //Generate the button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    buttonLayout->setSpacing(2);
    buttonLayout->addWidget(m_okay);
    m_cancel->setShortcut(QKeySequence(Qt::Key_Escape));
    buttonLayout->addWidget(m_cancel);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(m_okay,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]{emit requireAddEnemy(QPointF(m_xData->text().toDouble(),
                                             m_yData->text().toDouble()));});
    connect(m_cancel,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]{emit requireClose();});

    retranslate();
}

void EnemyAddWidget::enabledWidget()
{
    m_okay->setDefault(true);
    setEnabled(true);
}

void EnemyAddWidget::disabledWidget()
{
    m_okay->setDefault(false);
    setEnabled(false);
}

void EnemyAddWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    //Reset the data.
    m_xData->clear();
    m_yData->clear();
    //Update the preview.
    onActionParameterChange();
    //Set the focus to x data edit.
    m_xData->setFocus();
}

void EnemyAddWidget::retranslate()
{
    m_okay->setText(tr("Ok"));
    m_cancel->setText(tr("Cancel"));

    m_labels[0]->setText(tr("X:"));
    m_labels[1]->setText(tr("Y:"));

    m_editArea->setTitle(tr("Robot Initial Status"));
}

void EnemyAddWidget::onActionParameterChange()
{
    emit requirePreviewEnemy(QPointF(m_xData->text().toDouble(),
                                     m_yData->text().toDouble()));
}
