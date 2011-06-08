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
#ifndef STATUSNOTIFIERITEM_H
#define STATUSNOTIFIERITEM_H

// Local
#include <dbusimage.h>
#include <dbustooltip.h>

#include <private/qabstractsystemtrayiconsys_p.h>

// Qt
#include <QObject>
#include <QScopedPointer>

class QDBusObjectPath;

class DBusMenuExporter;

class IconCache;

class StatusNotifierItem : public QObject, public QAbstractSystemTrayIconSys
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.StatusNotifierItem")
    Q_PROPERTY(QString Category READ category)
    Q_PROPERTY(QString Id READ id)
    Q_PROPERTY(QString Title READ title)
    Q_PROPERTY(QString Status READ status)
    Q_PROPERTY(quint32 WindowId READ windowId)
    Q_PROPERTY(QString IconThemePath READ iconThemePath)
    Q_PROPERTY(QString IconName READ iconName)
    Q_PROPERTY(DBusImageList IconPixmap READ iconPixmap)
    Q_PROPERTY(QString OverlayIconName READ overlayIconName)
    Q_PROPERTY(DBusImageList OverlayIconPixmap READ overlayIconPixmap)
    Q_PROPERTY(QString AttentionIconName READ attentionIconName)
    Q_PROPERTY(DBusImageList AttentionIconPixmap READ attentionIconPixmap)
    Q_PROPERTY(QString AttentionMovieName READ attentionMovieName)
    Q_PROPERTY(DBusToolTip ToolTip READ toolTip)
    Q_PROPERTY(QDBusObjectPath Menu READ menu)
public:
    StatusNotifierItem(QSystemTrayIcon*, IconCache*);
    ~StatusNotifierItem();

    QString objectPath() const;

    // QAbstractSystemTrayIconSys
    QRect geometry() const;
    void updateVisibility();
    void updateIcon();
    void updateToolTip();
    void updateMenu();
    void showMessage(const QString &message, const QString &title,
        QSystemTrayIcon::MessageIcon icon, int msecs);
    /// QAbstractSystemTrayIconSys

    // StatusNotifierItem
    void Activate(int, int);
    void ContextMenu(int, int);
    void Scroll(int, const QString&);
    void SecondaryActivate(int, int);

    QString category() const;
    QString id() const;
    QString title() const;
    QString status() const;
    quint32 windowId() const { return 0; }
    QString iconThemePath() const;
    QString iconName() const;
    DBusImageList iconPixmap() const { return DBusImageList(); }
    QString overlayIconName() const { return QString(); }
    DBusImageList overlayIconPixmap() const { return DBusImageList(); }
    QString attentionIconName() const { return QString(); }
    DBusImageList attentionIconPixmap() const { return DBusImageList(); }
    QString attentionMovieName() const { return QString(); }
    DBusToolTip toolTip() const;
    QDBusObjectPath menu() const;

Q_SIGNALS:
    void NewStatus(const QString &status);
    void NewIcon();
    void NewToolTip();
    // /StatusNotifierItem

private:
    QString m_objectPath;
    IconCache* m_iconCache;
    DBusMenuExporter* m_dbusMenuExporter;

    QString menuObjectPath() const;
};

#endif // STATUSNOTIFIERITEM_H
