/* This file is part of the sni-qt project
   Copyright 2011 Canonical
   Author: Aurelien Gateau <aurelien.gateau@canonical.com>

   sni-qt is free software; you can redistribute it and/or modify it under the
   terms of the GNU Lesser General Public License (LGPL) as published by the
   Free Software Foundation; version 3 of the License.

   sni-qt is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
   more details.

   You should have received a copy of the GNU Lesser General Public License
   along with sni-qt.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef DEBUG_H
#define DEBUG_H

// Qt
#include <QDebug>
#include <QTime>

inline QDebug _sniTrace(bool isDebug, const char* function)
{
    QDebug debug = isDebug ? qDebug() : qWarning();
    debug << "sni-qt" << (isDebug ? "DBG" : "WRN");
    debug << QTime::currentTime().toString("HH:mm:ss.zzz").toUtf8().data();
    debug << function;
    return debug;
}

// Simple macros to get KDebug like support
#ifdef SNI_DEBUG_OUTPUT
    #define SNI_DEBUG  _sniTrace(true, __PRETTY_FUNCTION__)
#else
    #define SNI_DEBUG  while (false) qDebug()
#endif
#define SNI_WARNING _sniTrace(false, __PRETTY_FUNCTION__)

// Log a variable name and value
#define SNI_VAR(var) SNI_DEBUG << #var ":" << var

#define SNI_RETURN_IF_FAIL(cond) if (!(cond)) { \
    SNI_WARNING << "Condition failed: " #cond; \
    return; \
}

#define SNI_RETURN_VALUE_IF_FAIL(cond, value) if (!(cond)) { \
    SNI_WARNING << "Condition failed: " #cond; \
    return (value); \
}

#endif /* DEBUG_H */
