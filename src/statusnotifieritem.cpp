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
#include "statusnotifieritem.h"

// dbusmenu-qt
#include <dbusmenuexporter.h>

// Qt
#include <QDebug>

StatusNotifierItem::StatusNotifierItem(QSystemTrayIcon* icon)
: QAbstractSystemTrayIconSys(icon)
{}

StatusNotifierItem::~StatusNotifierItem()
{}

QRect StatusNotifierItem::geometry() const
{
    return QRect();
}

void StatusNotifierItem::updateVisibility()
{
    qDebug() << __FUNCTION__;
}

void StatusNotifierItem::updateIcon()
{
    qDebug() << __FUNCTION__ << trayIcon->icon().name();
}

void StatusNotifierItem::updateToolTip()
{
    qDebug() << __FUNCTION__;
}

void StatusNotifierItem::showMessage(const QString &message, const QString &title,
    QSystemTrayIcon::MessageIcon icon, int msecs)
{
    qDebug() << __FUNCTION__ << message;
}

#include <statusnotifieritem.moc>
