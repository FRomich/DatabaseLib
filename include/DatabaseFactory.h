
#pragma once

#include "DatabaseLibExport.h"
#include "DatabaseSettings.h"
#include "IDatabase.h"

#include <memory>

namespace DatabaseLib
{
    class DATABASELIB_EXPORT DatabaseFactory
    {
    public:
        static std::unique_ptr<IDatabase> create(
            const DatabaseSettings& settings);
    };
}
