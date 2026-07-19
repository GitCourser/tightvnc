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

#include "ConnectionConfigSM.h"
#include "ViewerStorage.h"

#include "config-lib/IniFileSettingsManager.h"
#include "config-lib/RegistrySettingsManager.h"
#include "win-system/Registry.h"

ConnectionConfigSM::ConnectionConfigSM(const TCHAR registryPath[],
                                       const TCHAR entryName[])
: m_backend(0)
{
  if (ViewerStorage::isPortable()) {
    StringStorage iniPath;
    ViewerStorage::getIniFilePath(&iniPath);

    StringStorage sectionName;
    ViewerStorage::getConnectionSectionName(entryName, &sectionName);

    m_backend = new IniFileSettingsManager(iniPath.getString(),
                                           sectionName.getString());
  } else {
    RegistrySettingsManager *registryBackend = new RegistrySettingsManager();
    StringStorage keyName;
    keyName.format(_T("%s\\History\\%s"),
                   registryPath,
                   entryName);
    registryBackend->setRegistryKey(Registry::getCurrentUserKey(), keyName.getString());
    m_backend = registryBackend;
  }
}

ConnectionConfigSM::~ConnectionConfigSM()
{
  if (m_backend != 0) {
    delete m_backend;
    m_backend = 0;
  }
}

bool ConnectionConfigSM::isOk()
{
  return m_backend != 0 && m_backend->isOk();
}

bool ConnectionConfigSM::keyExist(const TCHAR *name)
{
  return m_backend->keyExist(name);
}

bool ConnectionConfigSM::deleteKey(const TCHAR *name)
{
  return m_backend->deleteKey(name);
}

bool ConnectionConfigSM::getString(const TCHAR *name, StringStorage *storage)
{
  return m_backend->getString(name, storage);
}

bool ConnectionConfigSM::setString(const TCHAR *name, const TCHAR *value)
{
  return m_backend->setString(name, value);
}

bool ConnectionConfigSM::getLong(const TCHAR *name, long *value)
{
  return m_backend->getLong(name, value);
}

bool ConnectionConfigSM::setLong(const TCHAR *name, long value)
{
  return m_backend->setLong(name, value);
}

bool ConnectionConfigSM::getBoolean(const TCHAR *name, bool *value)
{
  return m_backend->getBoolean(name, value);
}

bool ConnectionConfigSM::setBoolean(const TCHAR *name, bool value)
{
  return m_backend->setBoolean(name, value);
}

bool ConnectionConfigSM::getUINT(const TCHAR *name, UINT *value)
{
  return m_backend->getUINT(name, value);
}

bool ConnectionConfigSM::setUINT(const TCHAR *name, UINT value)
{
  return m_backend->setUINT(name, value);
}

bool ConnectionConfigSM::getInt(const TCHAR *name, int *value)
{
  return m_backend->getInt(name, value);
}

bool ConnectionConfigSM::setInt(const TCHAR *name, int value)
{
  return m_backend->setInt(name, value);
}

bool ConnectionConfigSM::getByte(const TCHAR *name, char *value)
{
  return m_backend->getByte(name, value);
}

bool ConnectionConfigSM::setByte(const TCHAR *name, char value)
{
  return m_backend->setByte(name, value);
}

bool ConnectionConfigSM::getBinaryData(const TCHAR *name, void *value, size_t *size)
{
  return m_backend->getBinaryData(name, value, size);
}

bool ConnectionConfigSM::setBinaryData(const TCHAR *name, const void *value, size_t size)
{
  return m_backend->setBinaryData(name, value, size);
}
