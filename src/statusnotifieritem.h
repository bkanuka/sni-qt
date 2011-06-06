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

class StatusNotifierItem : public QObject, public QAbstractSystemTrayIconSys
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.StatusNotifierItem")
    Q_PROPERTY(QString IconName READ iconName)
public:
    StatusNotifierItem(QSystemTrayIcon*);
    ~StatusNotifierItem();

    // QAbstractSystemTrayIconSys
    QRect geometry() const;
    void updateVisibility();
    void updateIcon();
    void updateToolTip();
    void showMessage(const QString &message, const QString &title,
        QSystemTrayIcon::MessageIcon icon, int msecs);
    /// QAbstractSystemTrayIconSys

    // StatusNotifierItem
    void Activate(int, int);
    void ContextMenu(int, int);
    void Scroll(int, const QString&);
    void SecondaryActivate(int, int);

    QString iconName() const;
    // /StatusNotifierItem
};

#endif // STATUSNOTIFIERITEM_H
