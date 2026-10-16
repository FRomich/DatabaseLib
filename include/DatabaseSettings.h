#pragma once

#include "DatabaseProvider.h"

#include <QString>

namespace DatabaseLib
{
    struct DatabaseSettings
    {
        DatabaseProvider provider = DatabaseProvider::PostgreSQL;

        QString host = "localhost";
        int port = 0;
        QString database;
        QString user;
        QString password;
    };
}