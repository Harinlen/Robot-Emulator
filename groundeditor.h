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

#ifndef GROUNDEDITOR_H
#define GROUNDEDITOR_H

#include <QWidget>

class QToolButton;
class QTableWidget;
/*!
 * \brief The GroundEditor class can edit a ground-guard-bordor, and set the new
 * border to that ground.
 */
class GroundEditor : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct Ground Editor.
     * \param parent The parent widget.
     */
    explicit GroundEditor(QWidget *parent = 0);

signals:

public slots:

private slots:
    void retranslate();

private:
    enum GroundEditActions
    {
        AddPoint,
        RemovePoint,
        GroundEditActionsCount
    };
    QToolButton *m_actions[GroundEditActionsCount];

    QTableWidget *m_groundPoints;
};

#endif // GROUNDEDITOR_H
