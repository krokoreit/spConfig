/**
 * @file spConfigBase.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration data
 * @version 2.1.0
 * @date 2024-10-09
 * @copyright Copyright (c) 2024
 * 
 * Version history:
 * v1       original develoment for use in platformio / arduino for an ESP32 project
 * v2.0.0   changed to standard C++ 
 * v2.1.0   updated to use spObjectStore v2.1.0
 *  
 */



#ifndef SPCONFIGBASE_H
#define SPCONFIGBASE_H

#include <stdint.h>
#include <string>
#include <string.h>
#include <functional>

#include <spObjectStore.h>
#include <spConfigValue.h>


// SPCONFIG_FILEPATH_SEPARATOR for Windows and if not already defined
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
  #define SPCONFIG_WINDOWS_OS 1
#endif

#ifndef SPCONFIG_FILEPATH_SEPARATOR
  #ifdef SPCONFIG_WINDOWS_OS
    #define SPCONFIG_FILEPATH_SEPARATOR '\\'
  #else
    #define SPCONFIG_FILEPATH_SEPARATOR '/'
  #endif
#endif

#ifndef SPCONFIG_MAXLINELENGTH
  #define SPCONFIG_MAXLINELENGTH  100
#endif

#ifndef SPCONFIG_FILEBUFSIZE
  #define SPCONFIG_FILEBUFSIZE  1200
#endif


class spConfigBase
{
  private:
    spObjectStore<spConfigValue> m_store;
    spConfigValue m_non_existant_configValue = "non existant";
    bool m_hasChanged = false;
    bool m_autosave = false;
    uint64_t m_autosaveTimeMS = 0;
    std::string m_configFilePath = "";
    std::string m_configFilename = "config";
    std::string m_configDefaultFilename = "config-default";
    std::string m_configFileExtension = "ini";
    std::string m_filenameUsed = "";
    size_t m_fileBufUsed = 0; // size of file buffer used 
    char *m_pFileBuf = nullptr; // pointer to file buffer
    size_t m_fPos = 0; // position inside file 
    std::string m_lastSection;
    // 
    void setChanged();
    std::string makeId(const char* section, const char* key);
    bool ensureFileBuffer();
    void freeFileBuffer();
    bool saveIniEntryCB(const std::string &id, const spConfigValue &cv);
    size_t trimLine(char* buf, size_t len);
    size_t eraseComments(char* buf, size_t len);
    bool parseIniFile(std::string filename);

  protected:
    void setNextAutosaveTimeMS(uint64_t timeMS);
    uint64_t getNextAutosaveTimeMS();
    // virtuals
    virtual bool canUseFS();
    virtual void onSetChanged();
    virtual size_t readFile(std::string filename, char* buf, size_t startPos, size_t maxBytes);
    virtual size_t saveFile(std::string filename, char* buf, size_t startPos, size_t writeBytes);
    virtual void ensureLoopTask();

  public:
    spConfigBase();
    ~spConfigBase();
    void setValue(const char* section, const char* key, const char* value);
    void setValue(const char* section, const char* key, int32_t value);
    void setValue(const char* section, const char* key, uint32_t value);
    void setValue(const char* section, const char* key, int64_t value);
    void setValue(const char* section, const char* key, uint64_t value);
    void setValue(const char* section, const char* key, double value);
    void setValue(const char* section, const char* key, bool value);

    spConfigValue* getConfigValue(const char* section, const char* key);
    const char* getCStr(const char* section, const char* key, const char* defaultValue = "");
    std::string getString(const char* section, const char* key, const char* defaultValue = "");
    int32_t getInt32(const char* section, const char* key, int32_t defaultValue = 0);
    int32_t getUInt32(const char* section, const char* key, uint32_t defaultValue = 0);
    int64_t getInt64(const char* section, const char* key, int64_t defaultValue = 0);
    int64_t getUInt64(const char* section, const char* key, uint64_t defaultValue = 0);
    double getDouble(const char* section, const char* key, double defaultValue = 0.0);
    bool getBool(const char* section, const char* key, bool defaultValue = false);
    bool exists(const char* section, const char* key);
    bool changed();
    void reset();
    void read();
    void save();
    bool setAutosave(bool autosave);
    bool getAutosave();
    void setConfigFilename(std::string newName);
    std::string getConfigFilename();
    void setConfigDefaultFilename(std::string newName);
    std::string getConfigDefaultFilename();
    void setConfigFileExtension(std::string newExtension);
    std::string getConfigFileExtension();
    void setConfigFilePath(std::string newPath);
    std::string getConfigFilePath();

};

#endif // SPCONFIGBASE_H


