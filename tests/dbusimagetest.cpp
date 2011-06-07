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
// Qt
#include <QtTestGui>

// Local
#include <dbusimage.h>

// Qt
#include <QImage>
#include <QPainter>
#include <QPixmap>

static QImage qImageFromDBusImage(const DBusImage& dbusImage)
{
    QImage img(dbusImage.width, dbusImage.height, QImage::Format_ARGB32);
    const char* ptr = dbusImage.pixels.constData();
    int byteWidth = dbusImage.width * 4;
    for (int y = 0; y < dbusImage.height; ++y, ptr += byteWidth) {
        memcpy(img.scanLine(y), ptr, byteWidth);
    }
    return img;
}

class DBusImageTest : public QObject
{
Q_OBJECT
private Q_SLOTS:
    void testFromPixmap()
    {
        QImage src(16, 16, QImage::Format_ARGB32);
        {
            QPainter painter(&src);
            painter.setPen(Qt::blue);
            painter.setBrush(Qt::green);
            painter.drawRect(src.rect().adjusted(0, 0, -1, -1));
        }
        QPixmap pixmap = QPixmap::fromImage(src);
        DBusImage dbusImage = DBusImage::createFromPixmap(pixmap);
        QCOMPARE(dbusImage.width, src.width());
        QCOMPARE(dbusImage.height, src.height());

        QImage result = qImageFromDBusImage(dbusImage);
        QCOMPARE(result, src);
    }
};

QTEST_MAIN(DBusImageTest)

#include <dbusimagetest.moc>
