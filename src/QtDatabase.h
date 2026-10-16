
#pragma once

#include "IDatabase.h"
#include "DatabaseSettings.h"

#include <QSqlDatabase>

namespace DatabaseLib
{
    class QtDatabase final : public IDatabase
    {
    public:
        explicit QtDatabase(const DatabaseSettings& settings);
        ~QtDatabase() override;

        bool open() override;
        void close() override;
        bool isOpen() const override;

        QString lastError() const override;

        bool execute(
            const QString& sql,
            const QVariantList& params = {}) override;

        QSqlQuery query(
            const QString& sql,
            const QVariantList& params = {}) override;

    private:
        DatabaseSettings m_settings;
        QString m_connectionName;
        QString m_lastError;
        QSqlDatabase m_database;

        bool prepareQuery(
            QSqlQuery& query,
            const QString& sql,
            const QVariantList& params);
    };
}
