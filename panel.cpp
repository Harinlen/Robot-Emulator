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
#include <QFileDialog>

#include "panel.h"
#include "menubar.h"
#include "groundbase.h"

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this))
{
    //Set the main layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(2);
    setLayout(m_mainLayout);

    //Add the buttons and actions.
    for(int i=0; i<ControlCommandCount; i++)
    {
        //Initial the button.
        m_commands[i]=new QPushButton(this);
        m_mainLayout->addWidget(m_commands[i]);
        //Initial the actions.
        m_commandActions[i]=new QAction(this);
        //Link the actions to the button.
        connect(m_commandActions[i], SIGNAL(triggered()),
                m_commands[i], SLOT(click()));
    }
    m_mainLayout->addStretch();

    //Set shortcuts.
    m_commandActions[Start]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
    m_commandActions[Pause]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_E));
    m_commandActions[Reset]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_T));
    m_commandActions[NextFrame]->setShortcut(QKeySequence(Qt::CTRL+
                                                          Qt::SHIFT+
                                                          Qt::Key_M));

    retranslate();
}

void Panel::setGround(GroundBase *ground)
{
    connect(m_commands[Start],
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]{ground->start();});
    connect(m_commands[Pause],
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]{ground->pause();});
    connect(m_commands[Reset],
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]{ground->reset();});
    connect(m_commands[NextFrame],
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]{ground->nextFrame();});
    connect(m_commands[SaveScreenshot],
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                if(ground->size().isNull() || ground->size().isEmpty())
                {
                    return;
                }
                //Get the snapshot.
                QPixmap snapshot(ground->size());
                ground->render(&snapshot);
                //If we get nothing, then no image will be save.
                if(snapshot.isNull())
                {
                    return;
                }
                //Get the image path.
                QString snapshotPath=QFileDialog::getSaveFileName(this,
                                                                  tr("Save snapshot"),
                                                                  QString(),
                                                                  tr("Portable Network Graphic Format(*.png)"));
                if(!snapshotPath.isEmpty())
                {
                    //Save the file.
                    snapshot.save(snapshotPath, "png");
                }
            });
}

void Panel::setMenuBar(MenuBar *menuBar)
{
    //Add all the controls to timeline menu.
    for(int i=0; i<ControlCommandCount; i++)
    {
        menuBar->addCategoryAction(MenuBar::TimeLine, m_commandActions[i]);
    }
}

void Panel::retranslate()
{
    QString actionCaptions[ControlCommandCount];
    actionCaptions[Start]=tr("Start");
    actionCaptions[Pause]=tr("Pause");
    actionCaptions[Reset]=tr("Reset");
    actionCaptions[NextFrame]=tr("Next Frame");
    actionCaptions[SaveScreenshot]=tr("Save current status");

    //Update the command buttons and actions.
    for(int i=0; i<ControlCommandCount; i++)
    {
        m_commands[i]->setText(actionCaptions[i]);
        m_commandActions[i]->setText(actionCaptions[i]);
    }
}
