#include "DatabaseManager.hpp"
#include <iostream>

DatabaseManager::DatabaseManager() : m_env(nullptr), m_dbc(nullptr) {
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_env);
    SQLSetEnvAttr(m_env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, m_env, &m_dbc);
}

DatabaseManager::~DatabaseManager() {
    if (m_dbc) SQLDisconnect(m_dbc);
    if (m_dbc) SQLFreeHandle(SQL_HANDLE_DBC, m_dbc);
    if (m_env) SQLFreeHandle(SQL_HANDLE_ENV, m_env);
}

bool DatabaseManager::connect(const std::string& dsn, const std::string& user, const std::string& password) {
   std::string connStr = "DSN=" + dsn + ";UID=" + user + ";PWD=" + password + ";DATABASE=WeatherStationDB";
    SQLCHAR outConnStr[1024];
    SQLSMALLINT outLen;

    SQLRETURN ret = SQLDriverConnectA(
        m_dbc,
        nullptr,
        (SQLCHAR*)connStr.c_str(),
        SQL_NTS,
        outConnStr,
        sizeof(outConnStr),
        &outLen,
        SQL_DRIVER_COMPLETE
    );

    if (SQL_SUCCEEDED(ret)) {
        return true;
    }

    printError(SQL_HANDLE_DBC, m_dbc);
    return false;
}

bool DatabaseManager::executePrepared(const std::string& query, const std::function<void(SQLHSTMT)>& binder) {
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, m_dbc, &stmt);

    SQLRETURN ret = SQLPrepareA(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printError(SQL_HANDLE_STMT, stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return false;
    }

    binder(stmt);

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printError(SQL_HANDLE_STMT, stmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return SQL_SUCCEEDED(ret);
}

bool DatabaseManager::insertClient(int socketId, const std::string& filename) {
    std::string sql = "INSERT INTO dbo.CLIENTS (ClientID, NameOfTextFile) VALUES (?, ?)";
    
    if (!executePrepared(sql, [&](SQLHSTMT stmt) {
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&socketId, 0, nullptr);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)filename.c_str(), 0, nullptr);
    })) {
        std::cerr << "[DB ERROR] Failed to insert into CLIENTS\n";
        return false;
    }

    std::cout << "[DB OK] Inserted into CLIENTS: ID = " << socketId 
              << ", File = " << filename << std::endl;
    return true;
}


bool DatabaseManager::insertNotification(int socketId, double lat, double lon, int temp, int aqi) {
    std::string sql = "INSERT INTO dbo.NOTIFICATED_RECORDINGS (latitude, longitude, temperature, AQI, ClientID) VALUES (?, ?, ?, ?, ?)";
    if (!executePrepared(sql, [&](SQLHSTMT stmt) {
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &lat, 0, nullptr);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &lon, 0, nullptr);
        SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp, 0, nullptr);
        SQLBindParameter(stmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &aqi, 0, nullptr);
        SQLBindParameter(stmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &socketId, 0, nullptr);
    })) {
        std::cerr << "[DB ERROR] Failed to insert into NOTIFICATED_RECORDINGS\n";
        return false;
    }
    std::cout << "[DB OK] Inserted into NOTIFICATED_RECORDINGS\n";
    return true;
}

bool DatabaseManager::insertRequestedRecording(int socketId, const std::string& content) {
    std::string sql = "INSERT INTO dbo.REQUESTED_RECORDINGS (content, ClientID) VALUES (?, ?)";
    if (!executePrepared(sql, [&](SQLHSTMT stmt) {
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 4000, 0, (SQLPOINTER)content.c_str(), 0, nullptr);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &socketId, 0, nullptr);
    })) {
        std::cerr << "[DB ERROR] Failed to insert into REQUESTED_RECORDINGS\n";
        return false;
    }
    std::cout << "[DB OK] Inserted into REQUESTED_RECORDINGS\n";
    return true;
}

std::vector<std::string> DatabaseManager::fetchClientData(int clientId) {
    std::vector<std::string> results;
    SQLHSTMT stmt;
    SQLRETURN ret;

    std::string query = "SELECT 'NOTIFICATION: ' + CAST(latitude AS VARCHAR) + '/' + CAST(longitude AS VARCHAR) + '/' + CAST(temperature AS VARCHAR) + '/' + CAST(AQI AS VARCHAR) FROM NOTIFICATED_RECORDINGS WHERE ClientID = ?"
                         " UNION ALL "
                         "SELECT 'REQUESTED: ' + content FROM REQUESTED_RECORDINGS WHERE ClientID = ?";

    if (SQLAllocHandle(SQL_HANDLE_STMT, m_dbc, &stmt) != SQL_SUCCESS) return results;

    if (SQLPrepareA(stmt, (SQLCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        printError(SQL_HANDLE_STMT, stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return results;
    }

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &clientId, 0, nullptr);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &clientId, 0, nullptr);

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printError(SQL_HANDLE_STMT, stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return results;
    }

    char buffer[1024];
    SQLLEN indicator;
    while (SQLFetch(stmt) == SQL_SUCCESS) {
        if (SQLGetData(stmt, 1, SQL_C_CHAR, buffer, sizeof(buffer), &indicator) == SQL_SUCCESS) {
            results.emplace_back(buffer);
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return results;
}

void DatabaseManager::printError(SQLSMALLINT handleType, SQLHANDLE handle) {
    SQLCHAR sqlState[6], msg[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER nativeError;
    SQLSMALLINT msgLen;

    while (SQLGetDiagRecA(handleType, handle, 1, sqlState, &nativeError, msg, sizeof(msg), &msgLen) == SQL_SUCCESS) {
        std::cerr << "ODBC Error: " << msg << " (SQLState: " << sqlState << ")" << std::endl;
        break;
    }
}

std::vector<std::pair<float, int>> DatabaseManager::fetchLastNotificationData(int clientId, int maxEntries) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<std::pair<float, int>> result;

    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, m_dbc, &stmt);

    std::string query = "SELECT TOP " + std::to_string(maxEntries) +
                         " temperature, AQI FROM dbo.NOTIFICATED_RECORDINGS WHERE ClientID = ? ORDER BY ID DESC";
    SQLPrepareA(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &clientId, 0, nullptr);

    SQLRETURN ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printError(SQL_HANDLE_STMT, stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        throw std::runtime_error("Failed to execute SELECT for chart data.");
    }

    float temp;
    int aqi;
    SQLBindCol(stmt, 1, SQL_C_FLOAT, &temp, 0, nullptr);
    SQLBindCol(stmt, 2, SQL_C_LONG, &aqi, 0, nullptr);

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        result.emplace_back(temp, aqi);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    std::reverse(result.begin(), result.end());
    return result;
}

void DatabaseManager::clearAllData() {
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, m_dbc, &stmt);

    const char* deleteRequested = "DELETE FROM dbo.REQUESTED_RECORDINGS";
    const char* deleteNotified = "DELETE FROM dbo.NOTIFICATED_RECORDINGS";
    const char* deleteClients = "DELETE FROM dbo.CLIENTS";

    if (!SQL_SUCCEEDED(SQLExecDirectA(stmt, (SQLCHAR*)deleteRequested, SQL_NTS))) {
        printError(SQL_HANDLE_STMT, stmt);
    }

    if (!SQL_SUCCEEDED(SQLExecDirectA(stmt, (SQLCHAR*)deleteNotified, SQL_NTS))) {
        printError(SQL_HANDLE_STMT, stmt);
    }

    if (!SQL_SUCCEEDED(SQLExecDirectA(stmt, (SQLCHAR*)deleteClients, SQL_NTS))) {
        printError(SQL_HANDLE_STMT, stmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
