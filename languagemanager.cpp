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
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QAction>
#include <QTranslator>
#include <QSignalMapper>
#include <QApplication>

#include "menubar.h"

#include "languagemanager.h"

#include <QDebug>

LanguageManager *LanguageManager::m_instance=nullptr;

LanguageManager *LanguageManager::instance()
{
    return m_instance==nullptr?m_instance=new LanguageManager:m_instance;
}

void LanguageManager::loadLanguages()
{
    //Open language list.
    QFile languageListFile("://res/languages.json");
    if(!languageListFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //Get language translations.
    QJsonArray languages=
            QJsonDocument::fromJson(languageListFile.readAll()).array();
    for(QJsonValue languageData : languages)
    {
        QJsonArray langaugeArray=languageData.toArray();
        //Generate the language action.
        QAction *languageAction=new QAction(langaugeArray.at(0).toString(),
                                            this);
        //Link the action with the mapper.
        connect(languageAction, SIGNAL(triggered()),
                m_langaugeMapper, SLOT(map()));
        m_langaugeMapper->setMapping(languageAction,
                                     langaugeArray.at(1).toString());
        //Add to action list.
        m_languageActions.append(languageAction);
    }
}

void LanguageManager::setMenuBar(MenuBar *menuBar)
{
    //Give the action to menubar.
    for(QAction *languageAction : m_languageActions)
    {
        menuBar->addCategoryAction(MenuBar::Language, languageAction);
    }
}

void LanguageManager::onActionLoadLanguage(const QString &languagePath)
{
    qApp->removeTranslator(m_translator);
    //Load new file.
    m_translator->load("://res/"+languagePath);
    //Install the new translator.
    qApp->installTranslator(m_translator);
    //Emit language change signal.
    emit languageChanged();
}

LanguageManager::LanguageManager(QObject *parent) :
    QObject(parent),
    m_langaugeMapper(new QSignalMapper(this)),
    m_translator(new QTranslator(this))
{
    connect(m_langaugeMapper, SIGNAL(mapped(QString)),
            this, SLOT(onActionLoadLanguage(QString)));
}
