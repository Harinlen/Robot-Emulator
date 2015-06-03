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

#ifndef GENERATEGROUND_H
#define GENERATEGROUND_H

#include "generategroundbase.h"

class QGroupBox;
class QPushButton;
class QTabWidget;
class PolygonEditor;
class GroundPreviewer;
/*!
 * \brief The GenerateGround class is a default realization of
 * GenerateGroundBase.
 */
class GenerateGround : public GenerateGroundBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct ground generator widget.
     * \param parent The parent widget.
     */
    explicit GenerateGround(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from GenerateGroundBase::border().
     */
    QPolygonF border() const;

    /*!
     * \brief Reimplemented from GenerateGroundBase::barracks().
     */
    QPolygonF barracks() const;

signals:

public slots:
    /*!
     * \brief Reimplemented from GenerateGroundBase::setBorder().
     */
    void setBorder(const QPolygonF &border);

    /*!
     * \brief Reimplemented from GenerateGroundBase::setBarracks().
     */
    void setBarracks(const QPolygonF &barracks);

protected:
    /*!
     * \brief Reimplemented from GenerateGroundBase::showEvent().
     */
    void showEvent(QShowEvent *event);

private slots:
    void retranslate();

private:
    QTabWidget *m_tabManager;
    PolygonEditor *m_borderEditor, *m_barracksEditor;
    GroundPreviewer *m_previewer;
    QGroupBox *m_previewerGroup;
    QPushButton *m_okay, *m_cancel;

    QPolygonF m_border, m_barracks;
};

#endif // GENERATEGROUND_H
