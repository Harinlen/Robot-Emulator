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
#include <QScopedPointer>
#include <QBoxLayout>
#include <QApplication>
#include <QLabel>

#include "about.h"

#include <QDebug>

void About::showAbout(QWidget *parent)
{
    QScopedPointer<About> aboutWindow(new About(parent));
    aboutWindow->exec();
}

About::About(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(325, 356);
    setWindowTitle(tr("About Robot Emulator"));
    //Set the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(20,20,20,20);
    mainLayout->setSpacing(15);
    setLayout(mainLayout);

    QLabel *label=new QLabel(this);
    label->setPixmap(QPixmap("://res/icon.png"));
    label->setFixedSize(128, 128);
    label->setScaledContents(true);
    mainLayout->addWidget(label, 1, Qt::AlignCenter);
    label=new QLabel(tr("Robot Emulator"), this);
    QFont labelFont=label->font();
    labelFont.setBold(true);
    label->setFont(labelFont);
    mainLayout->addWidget(label,
                          0,
                          Qt::AlignHCenter);
    mainLayout->addWidget(new QLabel(tr("New robot simulation experience")),
                          0,
                          Qt::AlignHCenter);

    mainLayout->addWidget(new QLabel(tr("Emulator version ")+
                                     QApplication::applicationVersion(),
                                     this),
                          0,
                          Qt::AlignHCenter);
    QLabel *author=new QLabel(this);
    author->setAlignment(Qt::AlignHCenter);
    author->setText(tr("Haolei Ye, Han Wang, and Quanquan Li\n"
                       "All rights reserved."));
    mainLayout->addWidget(author,
                          0,
                          Qt::AlignHCenter);
}
