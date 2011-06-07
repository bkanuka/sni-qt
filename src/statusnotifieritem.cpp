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

static const char* SNI_CATEGORY_PROPERTY = "_qt_sni_category";
static const char* DEFAULT_CATEGORY = "ApplicationStatus";

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

    new StatusNotifierItemAdaptor(this);
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.registerObject(m_objectPath, this, QDBusConnection::ExportAllContents);

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
    NewStatus(status());
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
    return QCoreApplication::applicationFilePath().section('/', -1);
}

QString StatusNotifierItem::title() const
{
    QString title = QCoreApplication::applicationName();
    return title.isEmpty() ? id() : title;
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

QString StatusNotifierItem::category() const
{
    static QStringList validCategories = QStringList()
        << "ApplicationStatus"
        << "Communications"
        << "SystemServices"
        << "Hardware"
        ;

    QVariant value = trayIcon->property(SNI_CATEGORY_PROPERTY);
    if (!value.canConvert<QString>()) {
        return DEFAULT_CATEGORY;
    }

    QString category = value.toString();
    if (!validCategories.contains(category)) {
        qWarning("\"%s\" is not a valid value for \"%s\" property. Valid values are: %s",
            qPrintable(category), SNI_CATEGORY_PROPERTY,
            qPrintable(validCategories.join(", ")));
    }
    return category;
}

QString StatusNotifierItem::status() const
{
    return trayIcon->isVisible() ? "Active" : "Passive";
}

#include <statusnotifieritem.moc>
