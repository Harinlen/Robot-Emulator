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
#include <QMessageBox>

#include "groundbase.h"
#include "polygoneditor.h"
#include "groundpreviewer.h"

#include "generateground.h"

GenerateGround::GenerateGround(QWidget *parent) :
    GenerateGroundBase(parent),
    m_tabManager(new QTabWidget(this)),
    m_borderEditor(new PolygonEditor(this)),
    m_barracksEditor(new PolygonEditor(this)),
    m_previewer(new GroundPreviewer(this)),
    m_okay(new QPushButton(this)),
    m_cancel(new QPushButton(this))
{
    //Set properties.
#ifdef Q_OS_MACX
    setWindowFlags(Qt::Sheet);
#endif

    //Link the previewer.
    connect(m_borderEditor, &PolygonEditor::polygonChange,
            m_previewer, &GroundPreviewer::setPreviewBorder);
    connect(m_barracksEditor, &PolygonEditor::polygonChange,
            m_previewer, &GroundPreviewer::setPreviewBarracks);

    //Configure buttons.
    m_cancel->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(m_okay,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                //Check is the ground and barracks polygon available.
                QPolygonF ground=m_borderEditor->polygon();
                if(ground.size()<3) //Simplified than a triangle.
                {
                    //Set the tab to ground editor.
                    m_tabManager->setCurrentWidget(m_borderEditor);
                    //Raise the information.
                    QMessageBox::information(this,
                                             tr("Ground border invalid"),
                                             tr("The ground border must be at least a triangle."));
                    return;
                }
                QPolygonF barracks=m_barracksEditor->polygon();
                if(barracks.size()<3) //Simplified than a triangle.
                {
                    //Set the tab to barracks editor.
                    m_tabManager->setCurrentWidget(m_barracksEditor);
                    //Raise the information.
                    QMessageBox::information(this,
                                             tr("Barracks border invalid"),
                                             tr("The barracks border must be at least a triangle."));
                    return;
                }
                //Check the barracks is valid or not.
                for(QPointF point : barracks)
                {
                    //If there's one point which is not in the ground,
                    //Ask user to change the barracks.
                    if(!ground.containsPoint(point, Qt::OddEvenFill))
                    {
                        //Set the tab to barracks editor.
                        m_tabManager->setCurrentWidget(m_barracksEditor);
                        //Raise the information.
                        QMessageBox::information(this,
                                                 tr("Barracks border invalid"),
                                                 tr("The barracks border must be inside the border.\n"
                                                    "You may:\n"
                                                    "    1. Enlarge the border."
                                                    "    2. Reduce the barracks."));
                        return;
                    }
                }
                //Now all the polygon should be okay, save the data.
                m_border=ground;
                m_barracks=barracks;
                //Finished.
                done(QDialog::Accepted);
            });
    connect(m_cancel,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                done(QDialog::Rejected);
            });

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    setLayout(mainLayout);

    mainLayout->addWidget(m_tabManager, 1);
    mainLayout->addWidget(m_previewer, 0, Qt::AlignCenter);

    m_tabManager->addTab(m_borderEditor, "");
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

QPolygonF GenerateGround::barracks() const
{
    return m_barracks;
}

void GenerateGround::setBarracks(const QPolygonF &barracks)
{
    //Save the barracks first.
    m_barracks = barracks;
    //Set the polygon to the editor.
    m_barracksEditor->setPolygon(m_barracks);
}

void GenerateGround::showEvent(QShowEvent *event)
{
    //Show the widget.
    GenerateGroundBase::showEvent(event);
    //Set default widget to border editor.
    m_tabManager->setCurrentWidget(m_borderEditor);
}

QPolygonF GenerateGround::border() const
{
    return m_border;
}

void GenerateGround::setBorder(const QPolygonF &border)
{
    //Save the border first.
    m_border = border;
    //Set the polygon to the editor.
    m_borderEditor->setPolygon(m_border);
}

