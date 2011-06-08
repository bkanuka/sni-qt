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
#include <statusnotifieritemfactory.h>

// Local
#include <iconcache.h>
#include <fsutils.h>
#include <statusnotifieritem.h>

// Qt
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusServiceWatcher>
#include <QDebug>
#include <QtPlugin>

static const char *SNW_SERVICE = "org.kde.StatusNotifierWatcher";
static const char *SNW_IFACE   = "org.kde.StatusNotifierWatcher";
static const char *SNW_PATH    = "/StatusNotifierWatcher";

StatusNotifierItemFactory::StatusNotifierItemFactory()
: m_iconCacheDir(FsUtils::generateTempDir("qt-sni"))
, m_isAvailable(false)
{
    m_iconCache = new IconCache(m_iconCacheDir, this);
    QDBusServiceWatcher* snwWatcher = new QDBusServiceWatcher(this);
    snwWatcher->addWatchedService(SNW_SERVICE);
    connect(snwWatcher, SIGNAL(serviceOwnerChanged(const QString&, const QString&, const QString&)),
        SLOT(slotSnwOwnerChanged(const QString&, const QString&, const QString&)));
    snwWatcher->setConnection(QDBusConnection::sessionBus());

    connectToSnw();
}

StatusNotifierItemFactory::~StatusNotifierItemFactory()
{
    FsUtils::recursiveRm(m_iconCacheDir);
}

void StatusNotifierItemFactory::connectToSnw()
{
    m_isAvailable = false;
    QDBusInterface snw(SNW_SERVICE, SNW_PATH, SNW_IFACE);
    if (!snw.isValid()) {
        return;
    }

    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.connect(SNW_SERVICE, SNW_PATH, SNW_IFACE, "StatusNotifierHostRegistered", "",
        this, SLOT(slotHostRegisteredWithSnw()));

    // FIXME: Make this async?
    QVariant value = snw.property("IsStatusNotifierHostRegistered");
    if (!value.canConvert<bool>()) {
        qWarning() << "IsStatusNotifierHostRegistered returned something which is not a bool:" << value;
        return;
    }
    m_isAvailable = value.toBool();

    Q_FOREACH(StatusNotifierItem* item, m_items) {
        registerItem(item);
    }
}

QAbstractSystemTrayIconSys *StatusNotifierItemFactory::create(QSystemTrayIcon *trayIcon)
{
    StatusNotifierItem* item = new StatusNotifierItem(trayIcon, m_iconCache);
    connect(item, SIGNAL(destroyed(QObject*)), SLOT(slotItemDestroyed(QObject*)));
    m_items.insert(item);
    registerItem(item);
    return item;
}

bool StatusNotifierItemFactory::isAvailable() const
{
    return m_isAvailable;
}

void StatusNotifierItemFactory::slotSnwOwnerChanged(const QString&, const QString&, const QString& newOwner)
{
    bool oldAvailable = m_isAvailable;
    if (newOwner.isEmpty()) {
        m_isAvailable = false;
    } else {
        connectToSnw();
    }

    if (oldAvailable != m_isAvailable) {
        availableChanged(m_isAvailable);
    }
}

void StatusNotifierItemFactory::slotHostRegisteredWithSnw()
{
    if (!m_isAvailable) {
        m_isAvailable = true;
        availableChanged(m_isAvailable);
    }
}

void StatusNotifierItemFactory::slotItemDestroyed(QObject* obj)
{
    m_items.remove(static_cast<StatusNotifierItem*>(obj));
}

void StatusNotifierItemFactory::registerItem(StatusNotifierItem* item)
{
    QDBusInterface snw(SNW_SERVICE, SNW_PATH, SNW_IFACE);
    snw.asyncCall("RegisterStatusNotifierItem", item->objectPath());
}


Q_EXPORT_PLUGIN2(statusnotifieritem, StatusNotifierItemFactory)

#include <statusnotifieritemfactory.moc>
