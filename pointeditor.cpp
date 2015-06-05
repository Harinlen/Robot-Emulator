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
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QBoxLayout>
#include <QScopedPointer>

#include "pointeditor.h"

#include <QDebug>

QPointF PointEditor::getPoint(bool &accept,
                              const QPointF &point)
{
    //Generate a point editor.
    QScopedPointer<PointEditor> editor(new PointEditor);
    //Display a point.
    editor->setPoint(point.isNull()?QPointF(0,0):point);
    //Launch the point editor.
    accept=(QDialog::Accepted==editor->exec());
    return accept?editor->point():QPointF();
}

void PointEditor::setPoint(const QPointF &point)
{
    //Make the x and y edit to display the point.
    m_x->setText(QString::number(point.x()));
    m_y->setText(QString::number(point.y()));
}

QPointF PointEditor::point() const
{
    return QPointF(m_x->text().toDouble(),
                   m_y->text().toDouble());
}

void PointEditor::showEvent(QShowEvent *event)
{
    //Show the dialog.
    QDialog::showEvent(event);
    //Select all the data in m_x.
    m_x->selectAll();
    m_x->setFocus();
}

PointEditor::PointEditor(QWidget *parent) :
    QDialog(parent),
    m_x(new QLineEdit(this)),
    m_y(new QLineEdit(this)),
    m_ok(new QPushButton(tr("OK"), this)),
    m_cancel(new QPushButton(tr("Cancel"), this))
{
    //Set the number validator.
    QRegExp numberReg("[0-9]+$");
    QValidator *numberValidator=new QRegExpValidator(numberReg,
                                                     this);
    m_x->setValidator(numberValidator);
    m_y->setValidator(numberValidator);
    //Set ok button action.
    connect(m_ok,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                done(QDialog::Accepted);
            });
    connect(m_cancel,
            static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]
            {
                done(QDialog::Rejected);
            });
    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    setLayout(mainLayout);

    //Initial the edit area layout.
    QGridLayout *editAreaLayout=new QGridLayout(mainLayout->widget());
    editAreaLayout->setContentsMargins(0,0,0,0);
    editAreaLayout->setVerticalSpacing(0);
    mainLayout->addLayout(editAreaLayout, 1);

    editAreaLayout->addWidget(new QLabel("X:"), 0, 0, 1, 1, Qt::AlignRight);
    editAreaLayout->addWidget(m_x, 0, 1, 1, 1, Qt::AlignLeft);
    editAreaLayout->addWidget(new QLabel("Y:"), 1, 0, 1, 1, Qt::AlignRight);
    editAreaLayout->addWidget(m_y, 1, 1, 1, 1, Qt::AlignLeft);

    //Initial the button area.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addWidget(m_ok);
    buttonLayout->addWidget(m_cancel);
    buttonLayout->addStretch();
}
