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

void About::showAbout(QWidget *parent)
{
    QScopedPointer<About> aboutWindow(new About(parent));
    aboutWindow->exec();
}

About::About(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("About Robot Emulator"));
    //Set the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    setLayout(mainLayout);

    QLabel *icon=new QLabel(this);
    icon->setPixmap(QPixmap("://res/icon.png"));
    icon->setFixedSize(256, 256);
    icon->setScaledContents(true);
    mainLayout->addWidget(icon, 1, Qt::AlignCenter);
    mainLayout->addWidget(new QLabel(tr("Robot Emulator"), this),
                          0,
                          Qt::AlignHCenter);
    mainLayout->addWidget(new QLabel(QApplication::applicationVersion(), this),
                          0,
                          Qt::AlignHCenter);
    QLabel *author=new QLabel(this);
    author->setAlignment(Qt::AlignHCenter);
    author->setText(tr("Author:\n"
                       "Haolei Ye, 1120132191, Class: 08111302\n"
                        "Han Wang, 1120132065, Class: 08211301\n"
                     "Quanquan Li, 1120132192, Class: 08211302\n"));
    mainLayout->addWidget(author,
                          0,
                          Qt::AlignHCenter);
}
