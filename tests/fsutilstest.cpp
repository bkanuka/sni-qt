/* This file is part of the dbusmenu-qt library
   Copyright 2011 Canonical
   Author: Aurelien Gateau <aurelien.gateau@canonical.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License (LGPL) as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
// Local
#include <fsutils.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QtTest>

// libc
#include <stdlib.h>

static void touchFile(const QString& name)
{
    QFile file(name);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    QVERIFY(QFile::exists(name));
}

class FsUtilsTest : public QObject
{
Q_OBJECT
private Q_SLOTS:
    void init()
    {
        m_sandBoxDirName = QDir::currentPath() + "/sandbox";
        setenv("TMPDIR", m_sandBoxDirName.toLocal8Bit().constData(), 1 /*overwrite*/);
    }

    void cleanup()
    {
        QProcess::execute("/bin/rm", QStringList() << "-r" << m_sandBoxDirName);
    }

    void testGenerateTempDir()
    {
        QString prefix = "fsutilstest";
        QString dirName = FsUtils::generateTempDir(prefix);
        QFileInfo info(dirName);
        QVERIFY(info.exists());
        QVERIFY(info.isDir());
        QVERIFY(info.isAbsolute());
        QVERIFY(info.fileName().startsWith(prefix + "-"));
    }

    void testRecursiveRm()
    {
        QDir dir(m_sandBoxDirName);
        QVERIFY(dir.mkpath("dir/dir1"));
        QVERIFY(dir.mkpath("dir/dir2"));
        QVERIFY(dir.mkpath("dir/.hiddendir"));
        QString testDir = dir.path() + "/dir";
        touchFile(testDir + "/dir1/f1");
        touchFile(testDir + "/dir2/f2");
        touchFile(testDir + "/dir2/.hidden");
        touchFile(testDir + "/.hiddendir/f1");
        touchFile(testDir + "/.hiddendir/.hidden");

        QVERIFY(FsUtils::recursiveRm(testDir));
        QVERIFY(!QFile::exists(testDir));
    }

private:
    QString m_sandBoxDirName;
};

QTEST_MAIN(FsUtilsTest)

#include <fsutilstest.moc>
