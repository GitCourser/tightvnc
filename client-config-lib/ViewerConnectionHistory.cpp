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

#include "ViewerConnectionHistory.h"
#include "ViewerStorage.h"

#include "config-lib/IniFileSettingsManager.h"
#include "win-system/Registry.h"

#include <crtdbg.h>
#include <algorithm>

ViewerConnectionHistory::ViewerConnectionHistory(const TCHAR *registryPath,
                                                 size_t limit)
: m_limit(limit),
  m_registryHistory(0)
{
  if (!ViewerStorage::isPortable()) {
    StringStorage registryKey;
    registryKey.format(_T("%s\\History"), registryPath);
    m_conHistoryKey.open(Registry::getCurrentUserKey(),
                         registryKey.getString(),
                         true);
    m_registryHistory = new ConnectionHistory(&m_conHistoryKey, m_limit);
  }
}

ViewerConnectionHistory::~ViewerConnectionHistory()
{
  if (m_registryHistory != 0) {
    delete m_registryHistory;
    m_registryHistory = 0;
  }
  releaseHosts();
}

void ViewerConnectionHistory::setLimit(size_t limit)
{
  bool truncationNeeded = limit < m_limit;
  m_limit = limit;

  if (m_registryHistory != 0) {
    m_registryHistory->setLimit(limit);
    return;
  }

  if (truncationNeeded) {
    truncate();
  }
}

size_t ViewerConnectionHistory::getLimit() const
{
  if (m_registryHistory != 0) {
    return m_registryHistory->getLimit();
  }
  return m_limit;
}

void ViewerConnectionHistory::load()
{
  if (m_registryHistory != 0) {
    m_registryHistory->load();
    return;
  }

  releaseHosts();

  StringStorage iniPath;
  ViewerStorage::getIniFilePath(&iniPath);

  IniFileSettingsManager sm(iniPath.getString(), _T("History"));

  StringStorage valueName;
  StringStorage value;
  for (size_t i = 0; i < m_limit; i++) {
    valueName.format(_T("%d"), i);
    if (!sm.getString(valueName.getString(), &value)) {
      break;
    }
    m_hosts.push_back(value);
  }
}

void ViewerConnectionHistory::save()
{
  if (m_registryHistory != 0) {
    m_registryHistory->save();
    return;
  }

  if (m_hosts.size() > m_limit) {
    truncate();
    return;
  }

  rewritePortableHistorySection();
}

void ViewerConnectionHistory::truncate()
{
  if (m_registryHistory != 0) {
    m_registryHistory->truncate();
    return;
  }

  // Delete connection config sections for hosts that fall beyond the limit,
  // matching registry ConnectionHistory::truncate() semantics.
  while (m_hosts.size() > m_limit) {
    size_t last = m_hosts.size() - 1;
    deletePortableConnectionSection(m_hosts.at(last).getString());
    m_hosts.erase(m_hosts.begin() + last);
  }

  rewritePortableHistorySection();
}

void ViewerConnectionHistory::clear()
{
  if (m_registryHistory != 0) {
    m_registryHistory->clear();
    return;
  }

  // Remove per-host connection sections, but keep Settings and Connection.Listen.
  for (size_t i = 0; i < m_hosts.size(); i++) {
    deletePortableConnectionSection(m_hosts.at(i).getString());
  }

  releaseHosts();

  StringStorage iniPath;
  ViewerStorage::getIniFilePath(&iniPath);
  IniFileSettingsManager sm(iniPath.getString(), _T("History"));
  sm.deleteSection();
}

void ViewerConnectionHistory::addHost(const TCHAR *host)
{
  if (m_registryHistory != 0) {
    m_registryHistory->addHost(host);
    return;
  }

  StringStorage hostS(host);

  for (vector<StringStorage>::iterator it = m_hosts.begin();
       it != m_hosts.end();
       it++) {
    if (it->isEqualTo(&hostS)) {
      m_hosts.erase(it);
      break;
    }
  }

  m_hosts.insert(m_hosts.begin(), hostS);
}

size_t ViewerConnectionHistory::getHostCount() const
{
  if (m_registryHistory != 0) {
    return m_registryHistory->getHostCount();
  }
  return m_hosts.size();
}

const TCHAR *ViewerConnectionHistory::getHost(size_t i) const
{
  if (m_registryHistory != 0) {
    return m_registryHistory->getHost(i);
  }
  return m_hosts.at(i).getString();
}

void ViewerConnectionHistory::releaseHosts()
{
  m_hosts.clear();
}

void ViewerConnectionHistory::removeHost(const TCHAR *host)
{
  StringStorage hostS(host);

  for (vector<StringStorage>::iterator it = m_hosts.begin();
       it != m_hosts.end();
       it++) {
    if (it->isEqualTo(&hostS)) {
      m_hosts.erase(it);
      break;
    }
  }
}

void ViewerConnectionHistory::deletePortableConnectionSection(const TCHAR *host)
{
  StringStorage iniPath;
  ViewerStorage::getIniFilePath(&iniPath);

  StringStorage sectionName;
  ViewerStorage::getConnectionSectionName(host, &sectionName);

  IniFileSettingsManager sm(iniPath.getString(), sectionName.getString());
  sm.deleteSection();
}

void ViewerConnectionHistory::rewritePortableHistorySection()
{
  StringStorage iniPath;
  ViewerStorage::getIniFilePath(&iniPath);

  IniFileSettingsManager sm(iniPath.getString(), _T("History"));
  sm.deleteSection();

  size_t count = min(m_hosts.size(), m_limit);
  StringStorage valueName;
  for (size_t i = 0; i < count; i++) {
    valueName.format(_T("%u"), i);
    sm.setString(valueName.getString(), m_hosts.at(i).getString());
  }
}
