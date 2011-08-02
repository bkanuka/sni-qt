/* This file is part of the sni-qt project
   Copyright 2011 Canonical
   Author: Aurelien Gateau <aurelien.gateau@canonical.com>

   sni-qt is free software; you can redistribute it and/or modify it under the
   terms of the GNU Lesser General Public License (LGPL) as published by the
   Free Software Foundation; version 3 of the License.

   sni-qt is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
   more details.

   You should have received a copy of the GNU Lesser General Public License
   along with sni-qt.  If not, see <http://www.gnu.org/licenses/>.
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
#include <sys/types.h>
#include <utime.h>

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
    QByteArray ba = QFile::encodeName(tmpl);
    const char* name = mkdtemp(ba.data());
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

bool touch(const QString& _name)
{
    QByteArray name = QFile::encodeName(_name);
    int ret = utime(name.data(), 0 /* times */);
    if (ret != 0) {
        qCritical("Failed to touch %s: %s", name.data(), strerror(errno));
        return false;
    }
    return true;
}

} // namespace
