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
#include "statusnotifieritem.h"

// Local
#include <iconcache.h>
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

static const char* NOTIFICATION_INTERFACE = "org.freedesktop.Notifications";
static const char* NOTIFICATION_SERVICE = "org.freedesktop.Notifications";
static const char* NOTIFICATION_PATH = "/org/freedesktop/Notifications";

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

StatusNotifierItem::StatusNotifierItem(QSystemTrayIcon* icon, IconCache* iconCache)
: QAbstractSystemTrayIconSys(icon)
, m_iconCache(iconCache)
, m_dbusMenuExporter(0)
{
    registerMetaTypes();

    static int id = 1;
    m_objectPath = QString("/org/kde/statusnotifieritem/%1").arg(id++);

    new StatusNotifierItemAdaptor(this);
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.registerObject(m_objectPath, this, QDBusConnection::ExportAdaptors);

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
    NewIcon();
    // ToolTip contains the icon
    NewToolTip();
}

void StatusNotifierItem::updateToolTip()
{
    NewToolTip();
}

void StatusNotifierItem::updateMenu()
{
    delete m_dbusMenuExporter;
    QMenu* menu = trayIcon->contextMenu();
    if (!menu) {
        return;
    }
    m_dbusMenuExporter = new DBusMenuExporter(menuObjectPath(), menu);
}

void StatusNotifierItem::showMessage(const QString &title, const QString &message,
    QSystemTrayIcon::MessageIcon icon, int msecs)
{
    QString iconString;
    switch (icon) {
    case QSystemTrayIcon::NoIcon:
        break;
    case QSystemTrayIcon::Information:
        iconString = "dialog-information";
        break;
    case QSystemTrayIcon::Warning:
        iconString = "dialog-warning";
        break;
    case QSystemTrayIcon::Critical:
        iconString = "dialog-error";
        break;
    }

    QDBusInterface iface(NOTIFICATION_SERVICE, NOTIFICATION_PATH, NOTIFICATION_INTERFACE);
    iface.asyncCall("Notify",
        id(),
        quint32(0),    // replaces_id
        iconString,
        title,
        message,
        QStringList(), // actions
        QVariantMap(), // hints
        msecs
        );
}

void StatusNotifierItem::Activate(int, int)
{
    sendActivated(QSystemTrayIcon::Trigger);
}

void StatusNotifierItem::ContextMenu(int, int)
{
}

void StatusNotifierItem::Scroll(int, const QString&)
{
}

void StatusNotifierItem::SecondaryActivate(int, int)
{
    sendActivated(QSystemTrayIcon::MiddleClick);
}

QString StatusNotifierItem::iconThemePath() const
{
    return m_iconCache->themePath();
}

QString StatusNotifierItem::iconName() const
{
    QIcon icon = trayIcon->icon();
    if (icon.isNull()) {
        return QString();
    }
    QString name = icon.name();
    if (!name.isEmpty()) {
        return name;
    }

    return m_iconCache->nameForIcon(icon);
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

DBusToolTip StatusNotifierItem::toolTip() const
{
    DBusToolTip tip;
    tip.iconName = iconName();
    tip.title = trayIcon->toolTip();
    return tip;
}

#include <statusnotifieritem.moc>
