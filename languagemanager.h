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

#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QStringList>

class QAction;
class QTranslator;
class QSignalMapper;
class MenuBar;
/*!
 * \brief The LanguageManager class is the manager of the emulator language.
 */
class LanguageManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the instance of language manager.
     * \return The instance of language manager.
     */
    static LanguageManager *instance();

    /*!
     * \brief Load all the languages. Generate the language menu.
     */
    void loadLanguages();

signals:
    /*!
     * \brief When the language changed, this signal will be emitted.
     */
    void languageChanged();

public slots:
    /*!
     * \brief Set the MenuBar class to langauge manager.
     * \param menuBar The menubar class.
     */
    void setMenuBar(MenuBar *menuBar);

private slots:
    void onActionLoadLanguage(const QString &languagePath);

private:
    static LanguageManager *m_instance;
    explicit LanguageManager(QObject *parent = 0);

    QSignalMapper *m_langaugeMapper;
    QList<QAction *> m_languageActions;
    QTranslator *m_translator;
};

#endif // LANGUAGEMANAGER_H
