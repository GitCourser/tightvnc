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

#ifndef _VIEWER_STORAGE_H_
#define _VIEWER_STORAGE_H_

#include "util/StringStorage.h"

// Process-wide viewer storage mode and portable INI paths.
// Mode is fixed at initialize() and is not re-detected later.
class ViewerStorage
{
public:
  // Detect portable mode by presence of <exeDir>\iniFileName.
  // Must be called once before any viewer settings access.
  static void initialize(const TCHAR *iniFileName);

  static bool isInitialized();
  static bool isPortable();

  // Absolute path of the directory that contains tvnviewer.exe.
  static void getExecutableFolder(StringStorage *out);

  // Absolute path of tvnviewer.ini next to the executable.
  static void getIniFilePath(StringStorage *out);

  // Build INI section name for a host or special entry.
  // ".listen" maps to "Connection.Listen".
  // Other hosts map to "Connection.<hex-encoded host>".
  static void getConnectionSectionName(const TCHAR *entryName, StringStorage *out);

private:
  ViewerStorage();
  ~ViewerStorage();

  static bool s_initialized;
  static bool s_portable;
  static StringStorage s_executableFolder;
  static StringStorage s_iniFilePath;
};

#endif
