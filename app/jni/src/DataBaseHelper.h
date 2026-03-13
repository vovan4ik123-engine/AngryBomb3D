#pragma once

#include "EngineHeaders.h"

namespace DataBaseHelper
{
    // Data types:
    // NULL - The value is a NULL value.
    // INTEGER - The value is a signed integer, stored in 0, 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value.
    // REAL - The value is a floating point value, stored as an 8-byte IEEE floating point number.
    // TEXT - The value is a text string, stored using the database encoding (UTF-8, UTF-16BE or UTF-16LE).
    // BLOB - The value is a blob of data, stored exactly as it was input.
    // Use INTEGER 0 or 1 For bool. Sqlite don't store bool.

    // Database schema part 1.
    // Tables:  Settings            |   MapsProgress        |
    // -----------------------------------------------------------------------------------------------------------------------------------------------
    // Columns: ID                  |   ID                  |
    //          FPSLimit            |   CurrentMapIndex     |
    //                              |   LastOpenedMapIndex  |
    //                              |                       |
    //

    // Common data.
    const inline std::string dataBaseName = "AngryBomb3D.sqlite";
    void prepareDatabase(); // Always call at app launch.
    // const inline std::string deleteAllSettings = "DELETE FROM Settings;";
    //const inline std::string insertIntoBattlesProgress = "INSERT INTO BattlesProgress(ID, MaxAvailableBattle, AllBattlesFinished) "
    //                                                     "VALUES(NULL, :::battleNumber, :::allFinished);";

    void executeSql(const std::string& sql);
    std::vector<std::vector<std::variant<long long int, double, std::string, Beryll::SqliteNULL>>> executeSqlSelect(const std::string& sql);

    // Settings.
    const inline std::string createTableSettings = "CREATE TABLE IF NOT EXISTS "
                                                   "Settings( "
                                                   "ID INTEGER PRIMARY KEY NOT NULL, "
                                                   "FPSLimit INTEGER "
                                                   ");";

    const inline std::string insertSettings = "INSERT INTO Settings(ID, FPSLimit) VALUES(NULL, NULL);";
    const inline std::string selectSettingsAll = "SELECT * FROM Settings LIMIT 1;";
    const inline std::string updateSettingsFPSLimit = "UPDATE Settings SET FPSLimit = :::FPS;";

    bool getIsSettingsTableEmpty();
    void readSettings();
    inline void storeSettingsFPSLimit(long long int value)
    {
        executeSql(std::regex_replace(updateSettingsFPSLimit, std::regex(":::FPS"), std::to_string(value)));
    }

    // MapsProgress.
    const inline std::string createTableMapsProgress = "CREATE TABLE IF NOT EXISTS "
                                                       "MapsProgress( "
                                                       "ID INTEGER PRIMARY KEY NOT NULL, "
                                                       "CurrentMapIndex INTEGER, "
                                                       "LastOpenedMapIndex INTEGER "
                                                       ");";

    const inline std::string insertMapsProgress = "INSERT INTO MapsProgress(ID, CurrentMapIndex, LastOpenedMapIndex) VALUES(NULL, NULL, NULL);";
    const inline std::string selectMapsProgressAll = "SELECT * FROM MapsProgress LIMIT 1;";
    const inline std::string updateMapsProgressCurrentMapIndex = "UPDATE MapsProgress SET CurrentMapIndex = :::currentMapIndex;";
    const inline std::string updateMapsProgressLastOpenedMapIndex = "UPDATE MapsProgress SET LastOpenedMapIndex = :::lastOpenedIndex;";

    void readMapsProgress();
    inline void storeMapsProgressCurrentMapIndex(long long int value)
    {
        executeSql(std::regex_replace(updateMapsProgressCurrentMapIndex, std::regex(":::currentMapIndex"), std::to_string(value)));
    }
    inline void storeMapsProgressLastOpenedMapIndex(long long int value)
    {
        executeSql(std::regex_replace(updateMapsProgressLastOpenedMapIndex, std::regex(":::lastOpenedIndex"), std::to_string(value)));
    }


}
