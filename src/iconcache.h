/* This file is part of the sni-qt project
   Copyright 2011 Canonical
   Author: Aurelien Gateau <aurelien.gateau@canonical.com>

   sni-qt is free software; you can redistribute it and/or modify it under the
   terms of the GNU General Public License (GPL) as published by the Free
   Software Foundation; version 3 of the License.

   sni-qt is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License
   along with sni-qt.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ICONCACHE_H
#define ICONCACHE_H

// Qt
#include <QDateTime>
#include <QHash>
#include <QObject>

class QIcon;

/**
 * This class will save pixmaps from icons in a temporary dir on the disk,
 * making it possible to pass filenames for icons without names.
 */
class IconCache : public QObject
{
    Q_OBJECT
public:
    IconCache(const QString& baseDir, QObject* parent=0);

    QString themePath() const;

    QString nameForIcon(const QIcon& icon) const;

private:
    QString m_themePath;
    typedef QHash<qint64, QDateTime> Cache;
    Cache m_cache;

    void cacheIcon(const QIcon&);
};

#endif /* ICONCACHE_H */
