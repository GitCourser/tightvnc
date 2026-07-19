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

#include "ViewerStorage.h"

#include "file-lib/File.h"
#include "win-system/Environment.h"

#include <crtdbg.h>

bool ViewerStorage::s_initialized = false;
bool ViewerStorage::s_portable = false;
StringStorage ViewerStorage::s_executableFolder;
StringStorage ViewerStorage::s_iniFilePath;

void ViewerStorage::initialize(const TCHAR *iniFileName)
{
  if (s_initialized) {
    return;
  }

  s_initialized = true;
  s_portable = false;
  s_executableFolder.setString(_T(""));
  s_iniFilePath.setString(_T(""));

  if (iniFileName == 0 || iniFileName[0] == _T('\0')) {
    return;
  }

  if (!Environment::getCurrentModuleFolderPath(&s_executableFolder)) {
    s_executableFolder.setString(_T(""));
    return;
  }

  s_iniFilePath.format(_T("%s\\%s"),
                       s_executableFolder.getString(),
                       iniFileName);

  File iniFile(s_iniFilePath.getString());
  s_portable = iniFile.exists();
}

bool ViewerStorage::isInitialized()
{
  return s_initialized;
}

bool ViewerStorage::isPortable()
{
  _ASSERT(s_initialized);
  return s_portable;
}

void ViewerStorage::getExecutableFolder(StringStorage *out)
{
  _ASSERT(s_initialized);
  _ASSERT(out != 0);
  *out = s_executableFolder;
}

void ViewerStorage::getIniFilePath(StringStorage *out)
{
  _ASSERT(s_initialized);
  _ASSERT(out != 0);
  *out = s_iniFilePath;
}

void ViewerStorage::getConnectionSectionName(const TCHAR *entryName,
                                             StringStorage *out)
{
  _ASSERT(out != 0);

  if (entryName == 0) {
    out->setString(_T("Connection."));
    return;
  }

  // Keep listen mode on a fixed, readable section name.
  if (_tcscmp(entryName, _T(".listen")) == 0) {
    out->setString(_T("Connection.Listen"));
    return;
  }

  // Encode each TCHAR as 4 hex digits to keep INI section names valid
  // for IPv4/IPv6/domains/Unicode and avoid collisions.
  StringStorage encoded;
  for (const TCHAR *p = entryName; *p != _T('\0'); p++) {
    unsigned int codeUnit;
#if defined(UNICODE) || defined(_UNICODE)
    codeUnit = (unsigned int)(unsigned short)*p;
#else
    codeUnit = (unsigned int)(unsigned char)*p;
#endif
    StringStorage hex;
    hex.format(_T("%04X"), codeUnit);
    encoded.appendString(hex.getString());
  }

  out->format(_T("Connection.%s"), encoded.getString());
}
