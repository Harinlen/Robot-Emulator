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
#include <QMenu>

#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    //Initial all the menu and add to menu bar.
    for(int i=0; i<ActionCategoriesCount; i++)
    {
        m_categoryMenu[i]=new QMenu(this);
        addMenu(m_categoryMenu[i]);
    }

    //Launch the retranslate.
    retranslate();
}

void MenuBar::addCategoryAction(int category, QAction *action)
{
    //Add the action to the category menu.
    if(category>-1 && category<ActionCategoriesCount)
    {
        m_categoryMenu[category]->addAction(action);
    }
}

void MenuBar::retranslate()
{
    m_categoryMenu[File]->setTitle(tr("File"));
    m_categoryMenu[View]->setTitle(tr("View"));
    m_categoryMenu[Ground]->setTitle(tr("Ground"));
    m_categoryMenu[Robot]->setTitle(tr("Robot"));
    m_categoryMenu[TimeLine]->setTitle(tr("Time Line"));
    m_categoryMenu[Help]->setTitle(tr("Help"));
}
