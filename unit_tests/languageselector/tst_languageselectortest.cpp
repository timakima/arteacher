/****************************************************************************
* AR Physics Teacher is an augmented reality teaching application
*
* Copyright (C) 2012 University of Helsinki
*
* Contact: Timo Makimattila <timo.makimattila@primoceler.com>
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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
* USA.
*
****************************************************************************/


#include <QApplication>
#include <QLocale>
#include <QtCore/QString>
#include <QtTest/QtTest>
#include "languageselector.h"
#include "tool.h"

class LanguageselectorTest : public QObject
{
    Q_OBJECT
    
public:
    LanguageselectorTest();
    
private Q_SLOTS:
    void setLanguage_data();
    void setLanguage();
private:
    LanguageSelector *_selector;
};

LanguageselectorTest::LanguageselectorTest()
{
    int argc = 1;
    char *argv = "foobar";
    QApplication *a = new QApplication(argc, &argv);
    _selector = new LanguageSelector();
}

void LanguageselectorTest::setLanguage_data() {
    int i = 0;
    QTest::addColumn<int>("language");
    QTest::addColumn<int>("result");

    QTest::newRow(rowN()) << (int)QLocale::Finnish << (int)QLocale::Finnish;
    QTest::newRow(rowN()) << (int)QLocale::English << (int)QLocale::English;
    QTest::newRow(rowN()) << (int)QLocale::Kazakhstan << (int)QLocale::English;
    QTest::newRow(rowN()) << (int)QLocale::Finnish << (int)QLocale::Finnish;
}

void LanguageselectorTest::setLanguage() {
    QFETCH(int, language);
    QFETCH(int, result);
    _selector->setLanguage((QLocale::Language)language);
    QLocale locale;
    QVERIFY(locale.language() == result);
}

QTEST_APPLESS_MAIN(LanguageselectorTest)

#include "tst_languageselectortest.moc"
