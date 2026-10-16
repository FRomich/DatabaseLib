#pragma once

#include "DatabaseLibExport.h"

#include <QString>
#include <QVariant>
#include <QVariantList>
#include <QSqlQuery>

namespace DatabaseLib
{
    class DATABASELIB_EXPORT IDatabase
    {
    public:
        virtual ~IDatabase() = default;

        virtual bool open() = 0;
        virtual void close() = 0;
        virtual bool isOpen() const = 0;

        virtual QString lastError() const = 0;

        virtual bool execute(
            const QString& sql,
            const QVariantList& params = {}) = 0;

        virtual QSqlQuery query(
            const QString& sql,
            const QVariantList& params = {}) = 0;
    };
}