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
#ifndef DBUSTOOLTIP_H
#define DBUSTOOLTIP_H

#include <dbusimage.h>

// Qt
#include <QString>

class QDBusArgument;

struct DBusToolTip
{
    QString iconName;
    DBusImageList iconData;
    QString title;
    QString description;
};

Q_DECLARE_METATYPE(DBusToolTip)

QDBusArgument& operator<<(QDBusArgument&, const DBusToolTip&);
const QDBusArgument& operator>>(const QDBusArgument&, DBusToolTip&);

#endif /* DBUSTOOLTIP_H */
