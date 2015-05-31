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
#include <QTabWidget>

#include "groundeditor.h"
#include "barrackseditor.h"

#include "generateground.h"

GenerateGround::GenerateGround(QWidget *parent) :
    GenerateGroundBase(parent),
    m_tabManager(new QTabWidget(this)),
    m_groundEditor(new GroundEditor(this)),
    m_barracksEditor(new BarracksEditor(this)),
    m_okay(new QPushButton(this)),
    m_cancel(new QPushButton(this))
{
    //Set properties.
#ifdef Q_OS_MACX
    setWindowFlags(Qt::Sheet);
#endif

    //Configure buttons.
    m_cancel->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(m_cancel,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                close();
            });

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    setLayout(mainLayout);

    mainLayout->addWidget(m_tabManager, 1);

    m_tabManager->addTab(m_groundEditor, "");
    m_tabManager->addTab(m_barracksEditor, "");

    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addWidget(m_okay);
    buttonLayout->addWidget(m_cancel);
    buttonLayout->addStretch();

    retranslate();
}

void GenerateGround::retranslate()
{
    m_okay->setText(tr("Ok"));
    m_cancel->setText(tr("Cancel"));

    m_tabManager->setTabText(0, tr("Ground"));
    m_tabManager->setTabText(1, tr("Barracks"));
}
