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
#ifndef FSUTILS_H
#define FSUTILS_H

// Qt
#include <QString>

namespace FsUtils {

QString generateTempDir(const QString& prefix);

bool recursiveRm(const QString& dirName);

} // namespace

#endif /* FSUTILS_H */
