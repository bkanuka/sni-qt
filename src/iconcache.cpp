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

// Local
#include <debug.h>
#include <fsutils.h>

// Qt
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QList>

const int IconCache::MaxIconCount = 20;

static QByteArray hashForPixmap(const QList<int>& extents, const QPixmap& pixmap)
{
    QCryptographicHash hash(QCryptographicHash::Md4);
    Q_FOREACH(int extent, extents) {
        hash.addData(QByteArray::number(extent));
    }
    QImage image = pixmap.toImage();
    hash.addData(reinterpret_cast<const char*>(image.constBits()), image.byteCount());
    return hash.result().toHex();
}

static QString computeKeyForIcon(const QIcon& icon)
{
    static QString prefix = QString("%1_%2_")
        .arg(QCoreApplication::applicationFilePath().section('/', -1))
        .arg(QCoreApplication::applicationPid());

    // Get a sorted list of extents
    QList<int> extents;
    Q_FOREACH(const QSize& size, icon.availableSizes()) {
        extents << qMax(size.width(), size.height());
    }
    qSort(extents);
    if (extents.isEmpty()) {
        extents << 16 << 22 << 32 << 48;
    }

    // Determine the test extent: for our purpose we define it as either the
    // biggest extent which is <= 32 or, if there is none, the smallest
    // available
    int testExtent;
    auto it = extents.begin(), end = extents.end();
    do {
        testExtent = *it;
        ++it;
    } while (testExtent <= 32 && it != end);

    QByteArray hash = hashForPixmap(extents, icon.pixmap(testExtent));

    return prefix + QString::fromAscii(hash);
}

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

    QString key = computeKeyForIcon(icon);
    QStringList::iterator it = qFind(m_cacheKeys.begin(), m_cacheKeys.end(), key);
    if (it == m_cacheKeys.end()) {
        cacheIcon(key, icon);
        trimCache();
    } else {
        // Place key at the end of list as it is the most recently accessed
        m_cacheKeys.erase(it);
        m_cacheKeys.append(key);
    }

    return key;
}

void IconCache::trimCache() const
{
    QDir dir(m_themePath + "/hicolor");
    dir.setFilter(QDir::Dirs);

    while (m_cacheKeys.count() > MaxIconCount) {
        QString cacheKey = m_cacheKeys.takeFirst();

        Q_FOREACH(const QString& sizeDir, dir.entryList()) {
            QString iconSubPath = QString("%1/apps/%2.png").arg(sizeDir).arg(cacheKey);
            if (dir.exists(iconSubPath)) {
                dir.remove(iconSubPath);
            }
        }
    }
}

void IconCache::cacheIcon(const QString& key, const QIcon& icon) const
{
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

    m_cacheKeys << key;

    // Touch the theme path: GTK icon loading system checks the mtime of the
    // dir to decide whether it should look for new icons in the theme dir.
    //
    // Note: We do not use QDateTime::currentDateTime() as the new time because
    // if the icon is updated in less than one second, GTK won't notice it.
    // See https://bugs.launchpad.net/sni-qt/+bug/812884
    QFileInfo info(m_themePath);
    QDateTime mtime = info.lastModified();
    FsUtils::touch(m_themePath, mtime.addSecs(1));
}

#include <iconcache.moc>
