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
: m_isAvailable(false)
{
    qDebug() << __FUNCTION__;
    QDBusServiceWatcher* snwWatcher = new QDBusServiceWatcher(this);
    snwWatcher->addWatchedService(SNW_SERVICE);
    connect(snwWatcher, SIGNAL(serviceOwnerChanged(const QString&, const QString&, const QString&)),
        SLOT(slotSnwOwnerChanged(const QString&, const QString&, const QString&)));
    snwWatcher->setConnection(QDBusConnection::sessionBus());

    connectToSnw();
}

void StatusNotifierItemFactory::connectToSnw()
{
    m_isAvailable = false;
    QDBusInterface snw(SNW_SERVICE, SNW_PATH, SNW_IFACE);
    if (!snw.isValid()) {
        qDebug() << "!snw.isValid";
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

    qDebug() << "m_isAvailable" << m_isAvailable;
}

QAbstractSystemTrayIconSys *StatusNotifierItemFactory::create(QSystemTrayIcon *trayIcon)
{
    return new StatusNotifierItem(trayIcon);
}

bool StatusNotifierItemFactory::isAvailable() const
{
    return m_isAvailable;
}

void StatusNotifierItemFactory::slotSnwOwnerChanged(const QString&, const QString&, const QString& newOwner)
{
    qDebug() << __FUNCTION__ << newOwner;
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

Q_EXPORT_PLUGIN2(statusnotifieritem, StatusNotifierItemFactory)

#include <statusnotifieritemfactory.moc>
