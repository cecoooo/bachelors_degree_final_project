#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <functional>
#include <sqltypes.h>
#include <mutex>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect(const std::string& dsn, const std::string& user, const std::string& password);

    bool insertClient(int socketId, const std::string& filename);
    bool insertNotification(int socketId, double lat, double lon, int temp, int aqi);
    bool insertRequestedRecording(int socketId, const std::string& content);
    void clearAllData();

    std::vector<std::string> fetchClientData(int clientId);
    std::vector<std::pair<float, int>> fetchLastNotificationData(int clientId, int maxEntries);

private:
    SQLHENV m_env;
    SQLHDBC m_dbc;
    std::mutex m_mutex;

    bool executePrepared(const std::string& query, const std::function<void(SQLHSTMT)>& binder);
    void printError(SQLSMALLINT handleType, SQLHANDLE handle);
};
