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
