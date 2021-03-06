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

#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>

/*!
 * \brief The GridWidget class is a class provide the grid background for UI
 * enhancement.
 */
class GridWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the GridWidget.
     * \param parent
     */
    explicit GridWidget(QWidget *parent = 0);

    /*!
     * \brief Get the central widget.
     * \return The central widget. It will be nullptr if you don't set.
     */
    QWidget *widget() const;

    /*!
     * \brief The grid size parameter.
     * \return The grid size.
     */
    static int gridSize();

    /*!
     * \brief Change the grid size.
     * \param gridSize The new grid size.
     */
    static void setGridSize(int gridSize);

    /*!
     * \brief Get the grid step parameter.
     * \return The grid step.
     */
    static int gridStep();

    /*!
     * \brief Change the grid step size.
     * \param gridStep The grid step.
     */
    static void setGridStep(int gridStep);

    /*!
     * \brief Show or hide coordinate effect.
     * \return The status of display the coordinate effect.
     */
    bool coordinate() const;

signals:

public slots:
    /*!
     * \brief Set the central widget. It will change the parentship of the
     * widget.
     * \param widget The prefer central widget.
     */
    void setWidget(QWidget *widget);

    /*!
     * \brief Set to display or hide the coordinate effect.
     * \param coordinate To enabled coordinate effect, true.
     */
    void setCoordinate(bool coordinate);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

private:
    struct CoordinateItem
    {
        int x;
        int y;
        QString text;
    };
    QList<CoordinateItem> m_captions;

    QWidget *m_widget;
    bool m_coordinate;
    static int m_gridSize;
    static int m_gridStep;
};

#endif // GRIDWIDGET_H
