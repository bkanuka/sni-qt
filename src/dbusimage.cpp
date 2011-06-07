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
#include <dbusimage.h>

// Qt
#include <QDBusArgument>
#include <QDebug>
#include <QIcon>
#include <QImage>
#include <QPixmap>

QDBusArgument& operator<<(QDBusArgument& argument, const DBusImage& image)
{
    argument.beginStructure();
    argument << image.width << image.height << image.pixels;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, DBusImage& image)
{
    argument.beginStructure();
    argument >> image.width >> image.height >> image.pixels;
    argument.endStructure();
    return argument;
}

DBusImageList DBusImage::createListFromIcon(const QIcon& icon)
{
    if (icon.isNull()) {
        qWarning("qt-sni: DBusImage::createListFromIcon() icon is null");
        return DBusImageList();
    }

    DBusImageList list;
    QList<QSize> sizes = icon.availableSizes();
    if (sizes.isEmpty()) {
        // sizes can be empty if icon is an SVG. In this case generate images for a few sizes
        #define SIZE(x) QSize(x, x)
        sizes
            << SIZE(16)
            << SIZE(22)
            << SIZE(24)
            << SIZE(32)
            << SIZE(48)
            ;
        #undef SIZE
    }

    Q_FOREACH(const QSize& size, sizes) {
        list << createFromPixmap(icon.pixmap(size));
    }
    return list;
}

DBusImage DBusImage::createFromPixmap(const QPixmap& pixmap)
{
    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    DBusImage dbusImage;
    dbusImage.width = pixmap.width();
    dbusImage.height = pixmap.height();

    dbusImage.pixels.resize(dbusImage.width * dbusImage.height * 4);
    char* ptr = dbusImage.pixels.data();
    const int byteWidth = dbusImage.width * 4;
    for (int y=0; y < dbusImage.height; ++y, ptr += byteWidth) {
        memcpy(ptr, image.constScanLine(y), byteWidth);
    }

    return dbusImage;
}
