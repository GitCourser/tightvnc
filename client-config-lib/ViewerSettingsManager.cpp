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

#include "ViewerSettingsManager.h"
#include "ViewerStorage.h"

#include "config-lib/IniFileSettingsManager.h"
#include "config-lib/RegistrySettingsManager.h"
#include "util/Exception.h"
#include "win-system/Registry.h"

SettingsManager *ViewerSettingsManager::s_instance = 0;

ViewerSettingsManager::ViewerSettingsManager(const TCHAR registryName[])
: m_backend(0)
{
  if (ViewerStorage::isPortable()) {
    StringStorage iniPath;
    ViewerStorage::getIniFilePath(&iniPath);
    m_backend = new IniFileSettingsManager(iniPath.getString(), _T("Settings"));
  } else {
    RegistrySettingsManager *registryBackend = new RegistrySettingsManager();
    StringStorage keyName;
    keyName.format(_T("%s\\Settings\\"), registryName);
    registryBackend->setRegistryKey(Registry::getCurrentUserKey(), keyName.getString());
    m_backend = registryBackend;
  }
}

ViewerSettingsManager::~ViewerSettingsManager()
{
  if (m_backend != 0) {
    delete m_backend;
    m_backend = 0;
  }
}

void ViewerSettingsManager::initInstance(const TCHAR registryName[])
{
  if (s_instance != 0)
    delete s_instance;
  s_instance = new ViewerSettingsManager(registryName);
}

SettingsManager *ViewerSettingsManager::getInstance()
{
  if (s_instance == 0) {
    throw Exception(_T("Instance of viewer settings manager is 0"));
  }
  return s_instance;
}

bool ViewerSettingsManager::isOk()
{
  return m_backend != 0 && m_backend->isOk();
}

bool ViewerSettingsManager::keyExist(const TCHAR *name)
{
  return m_backend->keyExist(name);
}

bool ViewerSettingsManager::deleteKey(const TCHAR *name)
{
  return m_backend->deleteKey(name);
}

bool ViewerSettingsManager::getString(const TCHAR *name, StringStorage *storage)
{
  return m_backend->getString(name, storage);
}

bool ViewerSettingsManager::setString(const TCHAR *name, const TCHAR *value)
{
  return m_backend->setString(name, value);
}

bool ViewerSettingsManager::getLong(const TCHAR *name, long *value)
{
  return m_backend->getLong(name, value);
}

bool ViewerSettingsManager::setLong(const TCHAR *name, long value)
{
  return m_backend->setLong(name, value);
}

bool ViewerSettingsManager::getBoolean(const TCHAR *name, bool *value)
{
  return m_backend->getBoolean(name, value);
}

bool ViewerSettingsManager::setBoolean(const TCHAR *name, bool value)
{
  return m_backend->setBoolean(name, value);
}

bool ViewerSettingsManager::getUINT(const TCHAR *name, UINT *value)
{
  return m_backend->getUINT(name, value);
}

bool ViewerSettingsManager::setUINT(const TCHAR *name, UINT value)
{
  return m_backend->setUINT(name, value);
}

bool ViewerSettingsManager::getInt(const TCHAR *name, int *value)
{
  return m_backend->getInt(name, value);
}

bool ViewerSettingsManager::setInt(const TCHAR *name, int value)
{
  return m_backend->setInt(name, value);
}

bool ViewerSettingsManager::getByte(const TCHAR *name, char *value)
{
  return m_backend->getByte(name, value);
}

bool ViewerSettingsManager::setByte(const TCHAR *name, char value)
{
  return m_backend->setByte(name, value);
}

bool ViewerSettingsManager::getBinaryData(const TCHAR *name, void *value, size_t *size)
{
  return m_backend->getBinaryData(name, value, size);
}

bool ViewerSettingsManager::setBinaryData(const TCHAR *name, const void *value, size_t size)
{
  return m_backend->setBinaryData(name, value, size);
}
