
#include "DatabaseFactory.h"
#include "QtDatabase.h"

namespace DatabaseLib
{
    std::unique_ptr<IDatabase> DatabaseFactory::create(
        const DatabaseSettings& settings)
    {
        switch (settings.provider)
        {
        case DatabaseProvider::PostgreSQL:
        case DatabaseProvider::MySQL:
            return std::make_unique<QtDatabase>(settings);

        default:
            return nullptr;
        }
    }
}
