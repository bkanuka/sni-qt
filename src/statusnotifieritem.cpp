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

// Local
#include <statusnotifieritemadaptor.h>

// dbusmenu-qt
#include <dbusmenuexporter.h>

// Qt
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusMetaType>
#include <QDebug>

void registerMetaTypes()
{
    static bool registered = false;
    if (registered) {
        return;
    }
    qDBusRegisterMetaType<DBusImage>();
    qDBusRegisterMetaType<DBusImageList>();
    qDBusRegisterMetaType<DBusToolTip>();
}

StatusNotifierItem::StatusNotifierItem(QSystemTrayIcon* icon)
: QAbstractSystemTrayIconSys(icon)
{
    registerMetaTypes();

    static int id = 1;
    m_objectPath = QString("/org/kde/statusnotifieritem/%1").arg(id++);

    StatusNotifierItemAdaptor* adaptor = new StatusNotifierItemAdaptor(this);
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.registerObject(m_objectPath, adaptor, QDBusConnection::ExportAllContents);

    updateMenu();
}

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

void StatusNotifierItem::updateMenu()
{
    qDebug() << __FUNCTION__;
    QMenu* menu = trayIcon->contextMenu();
    if (!menu) {
        m_dbusMenuExporter.reset();
        return;
    }
    m_dbusMenuExporter.reset(new DBusMenuExporter(menuObjectPath(), menu));
}

void StatusNotifierItem::showMessage(const QString &message, const QString &title,
    QSystemTrayIcon::MessageIcon icon, int msecs)
{
    qDebug() << __FUNCTION__ << message;
}

void StatusNotifierItem::Activate(int, int)
{
}

void StatusNotifierItem::ContextMenu(int, int)
{
}

void StatusNotifierItem::Scroll(int, const QString&)
{
}

void StatusNotifierItem::SecondaryActivate(int, int)
{
}

QString StatusNotifierItem::iconName() const
{
    return "konqueror";
}

QString StatusNotifierItem::objectPath() const
{
    return m_objectPath;
}

QString StatusNotifierItem::id() const
{
    return QCoreApplication::applicationName();
}

QString StatusNotifierItem::title() const
{
    return QCoreApplication::applicationName();
}

QDBusObjectPath StatusNotifierItem::menu() const
{
    return m_dbusMenuExporter
        ? QDBusObjectPath(menuObjectPath())
        : QDBusObjectPath("/invalid");
}

QString StatusNotifierItem::menuObjectPath() const
{
    return objectPath() + "/menu";
}

#include <statusnotifieritem.moc>
