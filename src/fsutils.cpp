/* This file is part of the appmenu-qt project
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
#include <fsutils.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QFile>

// libc
#include <errno.h>
#include <stdlib.h>
#include <string.h>

namespace FsUtils {

QString generateTempDir(const QString& prefix)
{
    QDir dir = QDir::temp();
    if (!dir.mkpath(".")) {
        qCritical("Failed to generate temporary file for prefix %s: could not create %s",
            qPrintable(prefix), qPrintable(dir.path()));
        return QString();
    }

    QString tmpl = QString("%1/%2-XXXXXX")
        .arg(dir.path())
        .arg(prefix);
    const char* name = mkdtemp(QFile::encodeName(tmpl).data());
    if (!name) {
        qCritical("Failed to generate temporary file for prefix %s: %s",
            qPrintable(prefix), strerror(errno));
        return QString();
    }
    return QFile::encodeName(name);
}

bool recursiveRm(const QString& dirName)
{
    bool ok;
    QDir dir(dirName);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden | QDir::System);
    Q_FOREACH(const QFileInfo& info, dir.entryInfoList()) {
        if (info.isDir()) {
            ok = recursiveRm(info.filePath());
            if (!ok) {
                return false;
            }
        } else {
            ok = dir.remove(info.fileName());
            if (!ok) {
                qCritical("Failed to remove file %s", qPrintable(info.filePath()));
                return false;
            }
        }
    }
    ok = dir.rmdir(dir.path());
    if (!ok) {
        qCritical("Failed to remove empty dir %s", qPrintable(dir.path()));
        return false;
    }
    return true;
}

} // namespace
