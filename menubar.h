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

#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

/*!
 * \brief The MenuBar class contains the menu for all the categories.
 */
class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    enum ActionCategories
    {
        File,
        View,
        Ground,
        Robot,
        TimeLine,
        Help,
        ActionCategoriesCount
    };
    /*!
     * \brief Contruct the MenuBar.
     * \param parent The parent widget.
     */
    explicit MenuBar(QWidget *parent = 0);
    /*!
     * \brief Add actions to the specific category menu.
     * \param category The category menu index.
     * \param action The action which is waiting to add.
     */
    void addCategoryAction(int category, QAction *action);

signals:

public slots:

private slots:
    void retranslate();

private:
    QMenu *m_categoryMenu[ActionCategoriesCount];
};

#endif // MENUBAR_H
