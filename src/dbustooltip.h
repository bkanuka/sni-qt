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
#ifndef DBUSTOOLTIP_H
#define DBUSTOOLTIP_H

#include <dbusimage.h>

// Qt
#include <QString>

class QDBusArgument;

struct DBusToolTip
{
    QString iconName;
    DBusImageList iconPixmap;
    QString title;
    QString description;
};

Q_DECLARE_METATYPE(DBusToolTip)

QDBusArgument& operator<<(QDBusArgument&, const DBusToolTip&);
const QDBusArgument& operator>>(const QDBusArgument&, DBusToolTip&);

#endif /* DBUSTOOLTIP_H */
