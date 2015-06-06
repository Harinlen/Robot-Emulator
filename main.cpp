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
#include <QStyleFactory>

#include "languagemanager.h"
#include "mainwindow.h"

#include <QApplication>

/*!
 * \brief Initial the main window of the application.
 * \param argc Argument count.
 * \param argv Argument values.
 * \return The result of the message loop.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //Set application information.
    QApplication::setApplicationName("Robot Emulator");
    QApplication::setApplicationDisplayName("Robot Emulator");
    QApplication::setApplicationVersion("1.0");
    //Set style.
    QApplication::setStyle(QStyleFactory::create("fusion"));
    //Load languages.
    LanguageManager::instance()->loadLanguages();
    //Conrtruct main window.
    MainWindow mainWindow;
    mainWindow.show();
    //Expand the event loop.
    return app.exec();
}
