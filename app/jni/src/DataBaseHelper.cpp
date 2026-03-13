#include "DataBaseHelper.h"
#include "EnumsAndVariables.h"

namespace DataBaseHelper
{
    bool getIsSettingsTableEmpty()
    {
        try
        {
            Beryll::DataBase::openDataBase(dataBaseName);
            Beryll::DataBase::setSqlQuery(selectSettingsAll);
            auto rows = Beryll::DataBase::executeSelectQuery();

            return rows.empty();
        }
        catch(const Beryll::DataBaseException& e)
        {
            std::string what = e.what();
            BR_ERROR("DataBaseException %s", what.c_str());
        }
        catch(const std::exception& e)
        {
            std::string what = e.what();
            BR_ERROR("std::exception %s", what.c_str());
        }

        return true;
    }

    void prepareDatabase()
    {
        // Table Settings is main table.
        // If it does not exist app is launched first time on device.
        // Create all tables we need for app.
        if(getIsSettingsTableEmpty())
        {
            BR_INFO("%s", "Create database tables + first rows. Looks like first app launch.");

            try
            {
                // Create table Settings and insert 1 row.
                executeSql(createTableSettings);
                executeSql(insertSettings);
                storeSettingsFPSLimit(EnumsAndVars::SettingsMenu::FPSLimit);

                // MapsProgress.
                executeSql(createTableMapsProgress);
                executeSql(insertMapsProgress);
                storeMapsProgressCurrentMapIndex(EnumsAndVars::MapsProgress::currentMapIndex);
                storeMapsProgressLastOpenedMapIndex(EnumsAndVars::MapsProgress::lastOpenedMapIndex);
            }
            catch(const Beryll::DataBaseException& e)
            {
                std::string what = e.what();
                BR_ASSERT(false, "DataBaseException %s", what.c_str());
            }
            catch(const std::exception& e)
            {
                std::string what = e.what();
                BR_ASSERT(false, "std::exception %s", what.c_str());
            }
        }
        else
        {
            BR_INFO("%s", "Read database tables at app launch.");

            readSettings();
            readMapsProgress();
        }
    }

    void readSettings()
    {
        std::vector<std::vector<std::variant<long long int, double, std::string, Beryll::SqliteNULL>>> rows = executeSqlSelect(selectSettingsAll);
        BR_ASSERT((!rows.empty() && !rows[0].empty()), "%s", "readSettings() rows are empty.");

        BR_INFO("readSettings() rows: %d columns: %d", rows.size(), rows[0].size());

        BR_ASSERT((std::holds_alternative<long long int>(rows[0][0])), "%s", "ID INTEGER PRIMARY KEY contains wrong data.");

        BR_ASSERT((std::holds_alternative<long long int>(rows[0][1])), "%s", "FPSLimit contains wrong data.");
        if(std::holds_alternative<long long int>(rows[0][1]))
            EnumsAndVars::SettingsMenu::FPSLimit = std::get<long long int>(rows[0][1]);
        BR_INFO("SettingsMenu::FPSLimit after read: %d", EnumsAndVars::SettingsMenu::FPSLimit);
    }

    void readMapsProgress()
    {
        std::vector<std::vector<std::variant<long long int, double, std::string, Beryll::SqliteNULL>>> rows = executeSqlSelect(selectMapsProgressAll);
        BR_ASSERT((!rows.empty() && !rows[0].empty()), "%s", "readMapsProgress() rows are empty.");

        BR_INFO("readMapsProgress() rows: %d columns: %d", rows.size(), rows[0].size());

        BR_ASSERT((std::holds_alternative<long long int>(rows[0][0])), "%s", "ID INTEGER PRIMARY KEY contains wrong data.");

        BR_ASSERT((std::holds_alternative<long long int>(rows[0][1])), "%s", "CurrentMapIndex contains wrong data.");
        if(std::holds_alternative<long long int>(rows[0][1]))
            EnumsAndVars::MapsProgress::currentMapIndex = std::get<long long int>(rows[0][1]);
        BR_INFO("MapsProgress::currentMapIndex after read: %d", EnumsAndVars::MapsProgress::currentMapIndex);

        BR_ASSERT((std::holds_alternative<long long int>(rows[0][2])), "%s", "LastOpenedMapIndex contains wrong data.");
        if(std::holds_alternative<long long int>(rows[0][2]))
            EnumsAndVars::MapsProgress::lastOpenedMapIndex = std::get<long long int>(rows[0][2]);
        BR_INFO("MapsProgress::lastOpenedMapIndex after read: %d", EnumsAndVars::MapsProgress::lastOpenedMapIndex);

        // EnumsAndVars::MapsProgress::maxMapIndex is hardcoded and const. Should not be changed during game.
    }

    void executeSql(const std::string& sql)
    {
        BR_INFO("executeSql() %s", sql.c_str());
        BR_ASSERT((sql.find("SELECT") == std::string::npos), "%s", "That is select query. Use executeSqlSelect().");
        try
        {
            Beryll::DataBase::setSqlQuery(sql);
            Beryll::DataBase::executeNotSelectQuery();
        }
        catch(const Beryll::DataBaseException& e)
        {
            std::string what = e.what();
            BR_ASSERT(false, "DataBaseException %s", what.c_str());
        }
        catch(const std::exception& e)
        {
            std::string what = e.what();
            BR_ASSERT(false, "std::exception %s", what.c_str());
        }
    }

    std::vector<std::vector<std::variant<long long int, double, std::string, Beryll::SqliteNULL>>> executeSqlSelect(const std::string& sql)
    {
        BR_INFO("executeSqlSelect() %s", sql.c_str());
        BR_ASSERT((sql.find("SELECT") != std::string::npos), "%s", "That is not select query.");
        try
        {
            Beryll::DataBase::setSqlQuery(sql);
            return Beryll::DataBase::executeSelectQuery();
        }
        catch(const Beryll::DataBaseException& e)
        {
            std::string what = e.what();
            BR_ASSERT(false, "DataBaseException %s", what.c_str());
        }
        catch(const std::exception& e)
        {
            std::string what = e.what();
            BR_ASSERT(false, "std::exception %s", what.c_str());
        }

        return {};
    }
}
