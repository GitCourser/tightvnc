// Copyright (C) 2009,2010,2011,2012 GlavSoft LLC.
// All rights reserved.
//
//-------------------------------------------------------------------------
// This file is part of the TightVNC software.  Please visit our Web site:
//
//                       http://www.tightvnc.com/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//-------------------------------------------------------------------------
//

#ifndef _VIEWER_CONNECTION_HISTORY_H_
#define _VIEWER_CONNECTION_HISTORY_H_

#include "config-lib/ConnectionHistory.h"
#include "win-system/RegistryKey.h"

#include <vector>

using namespace std;

// Viewer-specific connection history with dual backends:
// - normal mode: existing registry ConnectionHistory
// - portable mode: [History] section in tvnviewer.ini
class ViewerConnectionHistory
{
public:
  ViewerConnectionHistory(const TCHAR *registryPath, size_t limit);
  ~ViewerConnectionHistory();

  void setLimit(size_t limit);
  size_t getLimit() const;

  void load();
  void save();
  void truncate();
  void clear();

  void addHost(const TCHAR *host);

  size_t getHostCount() const;
  const TCHAR *getHost(size_t i) const;

private:
  void releaseHosts();
  void removeHost(const TCHAR *host);
  void deletePortableConnectionSection(const TCHAR *host);
  void rewritePortableHistorySection();

private:
  vector<StringStorage> m_hosts;
  size_t m_limit;

  // Used only in normal (registry) mode.
  RegistryKey m_conHistoryKey;
  ConnectionHistory *m_registryHistory;
};

#endif
