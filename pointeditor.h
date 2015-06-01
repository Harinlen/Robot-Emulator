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

#ifndef POINTEDITOR_H
#define POINTEDITOR_H

#include <QDialog>

class QLineEdit;
class QPushButton;
/*!
 * \brief The PointEditor class is a simple point editor.
 */
class PointEditor : public QDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Get a point using the point editor.
     * \param accept This point is not generate caused by cancel.
     * \param point The default point.
     * \return The new point.
     */
    static QPointF getPoint(bool &accept,
                            const QPointF &point=QPointF());

    /*!
     * \brief Make the editor to display a point value.
     * \param point The display point.
     */
    void setPoint(const QPointF &point);

    /*!
     * \brief Get the edited point data.
     * \return The point data generate from the editor.
     */
    QPointF point() const;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QDialog::showEvent();
     */
    void showEvent(QShowEvent *event);

private:
    explicit PointEditor(QWidget *parent = 0);
    QLineEdit *m_x, *m_y;
    QPushButton *m_ok, *m_cancel;
};

#endif // POINTEDITOR_H
