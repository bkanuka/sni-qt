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
// Local
#include <fsutils.h>
#include <iconcache.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QImage>
#include <QPainter>
#include <QtTestGui>

static QImage createTestImage(int width, int height)
{
    QImage img(width, height, QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::green);
    painter.drawRect(img.rect().adjusted(0, 0, -1, -1));
    return img;
}

class IconCacheTest : public QObject
{
Q_OBJECT
private Q_SLOTS:
    void init()
    {
        QDir dir = QDir::current();
        m_sandBoxDirName = dir.path() + "/sandbox";
        dir.mkdir("sandbox");
    }

    void cleanup()
    {
        FsUtils::recursiveRm(m_sandBoxDirName);
    }

    void testThemePath()
    {
        IconCache cache(m_sandBoxDirName);
        QString themePath = cache.themePath();
        QCOMPARE(themePath, m_sandBoxDirName + "/icons");

        QFileInfo info(themePath);
        QVERIFY(info.isDir());
    }

    void testPixmapIcon()
    {
        IconCache cache(m_sandBoxDirName);

        QList<int> sizes = QList<int>() << 16 << 22 << 32;
        QList<QImage> images;
        QIcon icon;
        Q_FOREACH(int size, sizes) {
            QImage image = createTestImage(size, size);
            images << image;
            icon.addPixmap(QPixmap::fromImage(image));
        }

        QString name = cache.nameForIcon(icon);
        Q_FOREACH(int size, sizes) {
            QString dirName = cache.themePath() + QString("/hicolor/%1x%1/apps").arg(size);
            QVERIFY(QFile::exists(dirName));
            QImage image;
            QVERIFY(image.load(dirName + "/" + name + ".png"));
        }
    }

private:
    QString m_sandBoxDirName;
};

QTEST_MAIN(IconCacheTest)

#include <iconcachetest.moc>
