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
#include <iconcache.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QIcon>

IconCache::IconCache(const QString& baseDir, QObject* parent)
: QObject(parent)
, m_themePath(baseDir + "/icons")
{
    QDir dir(baseDir);
    bool ok = dir.mkdir("icons");
    if (!ok) {
        qCritical("Could not create '%s' dir for SNI icon cache", qPrintable(m_themePath));
        m_themePath = QString();
        return;
    }
}

QString IconCache::themePath() const
{
    return m_themePath;
}

QString IconCache::nameForIcon(const QIcon& icon) const
{
    if (icon.isNull()) {
        return QString();
    }

    qint64 key = icon.cacheKey();
    if (!m_cache.contains(key)) {
        const_cast<IconCache*>(this)->cacheIcon(icon);
    }

    return QString::number(key);
}

void IconCache::cacheIcon(const QIcon& icon)
{
    qint64 key = icon.cacheKey();
    QList<QSize> sizes = icon.availableSizes();
    if (sizes.isEmpty()) {
        // sizes can be empty if icon is an SVG. In this case generate images for a few sizes
        #define SIZE(x) QSize(x, x)
        sizes
            << SIZE(16)
            << SIZE(22)
            << SIZE(32)
            << SIZE(48)
            ;
        #undef SIZE
    }

    QDir dir(m_themePath);
    Q_FOREACH(const QSize& size, sizes) {
        QPixmap pix = icon.pixmap(size);
        QString dirName = QString("hicolor/%1x%1/apps").arg(size.width());
        if (!dir.exists(dirName)) {
            if (!dir.mkpath(dirName)) {
                qWarning("Could not create '%s' dir in '%s'",
                    qPrintable(m_themePath), qPrintable(dirName));
                continue;
            }
        }
        QString pixPath = QString("%1/%2/%3.png")
            .arg(m_themePath).arg(dirName).arg(key);
        if (!pix.save(pixPath, "png")) {
            qWarning("Could not save icon as '%s'", qPrintable(pixPath));
        }
    }

    m_cache.insert(key, QDateTime::currentDateTime());
}

#include <iconcache.moc>
