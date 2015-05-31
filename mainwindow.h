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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QScrollArea;
class Ground;
class PanelDock;
class MenuBar;
class RobotManagement;
class GenerateGround;
/*!
 * \brief The MainWindow class is the top widget of all the visible widget. The
 * only mission of MainWindow is going to show the Ground widget and take the
 * fully management of the Ground widget.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs the MainWindow with the given parent. This class should
     * only be constructed by main() function.
     * \param parent The parent widget.
     */
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private:
    Ground *m_ground;
    PanelDock *m_panel;
    MenuBar *m_menuBar;
    RobotManagement *m_robotManagement;
    GenerateGround *m_groundGenerator;
};

#endif // MAINWINDOW_H
