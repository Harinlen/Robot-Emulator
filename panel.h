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

#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

class QBoxLayout;
class QPushButton;
class QSignalMapper;
class GroundBase;
class MenuBar;
/*!
 * \brief The Panel class is the control panel of the ground, it can control the
 * time line of the Ground class.
 */
class Panel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a timeline control panel.
     * \param parent The parent widget.
     */
    explicit Panel(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the controlled GroundBase class to the panel, it will
     * automatcially link the the signal and the slots.
     * \param ground The controlled ground class.
     */
    void setGround(GroundBase *ground);

    /*!
     * \brief Set the menu bar to add control actions.
     * \param menuBar The menu bar.
     */
    void setMenuBar(MenuBar *menuBar);

private slots:
    void retranslate();

private:
    enum ControlCommands
    {
        Start,
        Pause,
        Reset,
        NextFrame,
        SaveScreenshot,
        SetSpeedQuarter,
        SetSpeedHalf,
        SetSpeedOriginal,
        SetSpeedDouble,
        SetSpeedTriple,
        SetSpeedQuadra,
        SetSpeedPanta,
        SetSpeedHexa,
        ControlCommandCount,
    };
    QPushButton *m_commands[ControlCommandCount];
    QAction *m_commandActions[ControlCommandCount];
    QSignalMapper *m_speedMapper;

    QBoxLayout *m_mainLayout;
};

#endif // PANEL_H
