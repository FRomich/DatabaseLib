#pragma once

#include <QtCore/QtGlobal>

#ifdef DATABASELIB_LIBRARY
#define DATABASELIB_EXPORT Q_DECL_EXPORT
#else
#define DATABASELIB_EXPORT Q_DECL_IMPORT
#endif