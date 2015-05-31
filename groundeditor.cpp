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
#include <QTableWidget>
#include <QToolButton>

#include "groundeditor.h"

GroundEditor::GroundEditor(QWidget *parent) :
    QWidget(parent),
    m_groundPoints(new QTableWidget(this))
{
    //Configure the ground points editor.
    m_groundPoints->setColumnCount(2);

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
    //Set key sequence.
    m_actions[AddPoint]->setShortcut(QKeySequence(Qt::Key_Plus));
    actionLayout->addStretch();

    mainLayout->addWidget(m_groundPoints, 1);
    mainLayout->addLayout(actionLayout);

    retranslate();
}

void GroundEditor::retranslate()
{
    //Update ground points header.
    QStringList groundHeader;
    groundHeader << tr("X") << tr("Y");
    m_groundPoints->setHorizontalHeaderLabels(groundHeader);

    //Set the text.
    m_actions[AddPoint]->setText("+");
    m_actions[RemovePoint]->setText("-");
}
