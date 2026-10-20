#include "QtDatabase.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

namespace DatabaseLib
{
    QtDatabase::QtDatabase(const DatabaseSettings& settings)
        : m_settings(settings)
        , m_connectionName(
            QUuid::createUuid().toString(QUuid::WithoutBraces))
    {
    }

    QtDatabase::~QtDatabase()
    {
        close();
    }

    bool QtDatabase::open()
    {
        if (m_database.isOpen())
            return true;

        QString driver;

        switch (m_settings.provider)
        {
        case DatabaseProvider::PostgreSQL:
            driver = QStringLiteral("QPSQL");
            break;

        case DatabaseProvider::MySQL:
            driver = QStringLiteral("QMYSQL");
            break;

        default:
            m_lastError = QStringLiteral("Unsupported database provider");
            return false;
        }

        if (!QSqlDatabase::isDriverAvailable(driver))
        {
            m_lastError =
                QStringLiteral("Qt SQL driver is not available: %1")
                .arg(driver);
            return false;
        }

        m_database = QSqlDatabase::addDatabase(
            driver, m_connectionName);

        m_database.setHostName(m_settings.host);
        m_database.setDatabaseName(m_settings.database);
        m_database.setUserName(m_settings.user);
        m_database.setPassword(m_settings.password);

        const int port = m_settings.port > 0
            ? m_settings.port
            : (m_settings.provider == DatabaseProvider::PostgreSQL
                ? 5432 : 3306);

        m_database.setPort(port);

        if (!m_database.open())
        {
            m_lastError = m_database.lastError().text();
            m_database = QSqlDatabase();
            QSqlDatabase::removeDatabase(m_connectionName);
            return false;
        }

        m_lastError.clear();
        return true;
    }

    void QtDatabase::close()
    {
        if (!m_database.isValid())
            return;

        m_database.close();
        m_database = QSqlDatabase();
        QSqlDatabase::removeDatabase(m_connectionName);
    }

    bool QtDatabase::isOpen() const
    {
        return m_database.isValid() && m_database.isOpen();
    }

    QString QtDatabase::lastError() const
    {
        return m_lastError;
    }

    bool QtDatabase::prepareQuery(
        QSqlQuery& query,
        const QString& sql,
        const QVariantList& params)
    {
        if (!isOpen())
        {
            m_lastError = QStringLiteral("Database is not open");
            return false;
        }

        query = QSqlQuery(m_database);

        if (!query.prepare(sql))
        {
            m_lastError = query.lastError().text();
            return false;
        }

        for (const QVariant& param : params)
            query.addBindValue(param);

        if (!query.exec())
        {
            m_lastError = query.lastError().text();
            return false;
        }

        m_lastError.clear();
        return true;
    }

    bool QtDatabase::execute(
        const QString& sql,
        const QVariantList& params)
    {
        QSqlQuery query;
        return prepareQuery(query, sql, params);
    }

    QSqlQuery QtDatabase::query(
        const QString& sql,
        const QVariantList& params)
    {
        QSqlQuery result;

        if (!prepareQuery(result, sql, params))
            return QSqlQuery();

        return result;
    }
}
