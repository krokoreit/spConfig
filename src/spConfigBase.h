/**
 * @file spConfigBase.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration data
 * @version 2.0.0
 * @date 2024-06-10
 * @copyright Copyright (c) 2024
 * 
 */


/**
 * Version history:
 * v1 original develoment for use in platformio / arduino on an ESP32 project
 * v2 changed to standard C++ 
 *  
 */



/*

Set optional path or change extension ('ini') and filenames for 'config' and 
'config-default' to determine the location and names of files used to read / create 
the configuration and - if available - read the default configuration before that. 
The default configuration provides for an optional method of specifying factory defaults
for settings to be changed in the app. It also serves as an option to change the value of
'variables' without touching code, i.e. no need to recompile, just amend the config value
and restart.

Typically a program will start with running the read() function to load configuration data from file.
However, this is not a requirement, as 



The 'config-default' file allows for
  - comments (starting with '#' or ';')
  - whitespaces before or after section, key or value enties
  - keys without values (as either key only or with '=', but no value)
and it must have at least one [section].

Note that 'config' file will be build from 'config-default' as well as entries made with
application code and therefore does not allow for comments or whitespace, which will be 
stripped out when reading the content. However, other manual changes to the 'config' file
will be preserved until reset() is used to rebuild from 'config-default', if available.



All values can be stored and retirved via various functions and with their section and key parameters:
  -   confObj.setValue(section ,key, value);
      whereby value can be const char*, int32_t, uint32_t, int64_t, uint64_t, double or bool
  -   confObj.getString(section, key, default_value);
      or the equivalent getInt32, getUInt32, getInt64, getUInt64, getDouble or getBool functions 
      whereby default_value is optional to specify an alternative return value to "", 0 or false in case
      the requested value does not exist.
  -   confObj.getConfigValue(section, key);
      to return an spConfigValue object

To determine if a value exists use confObj.exists(section, key);

To reset the whole config (i.e. to filename-default.ini as the factory defaults or to an empty list if such 
file does not exist), use confObj.reset();

The config data are automatically read from file when the spConfigBase object is created. In case (re)reading needs
to be invoked manually, confObj.read(); can be used.

Saving of the config data can be done with confObj.save();, which is only done when data have changed. Thus,
prior checking of confObj.changed(); is not needed for that purpose.
There is an autosave functionality, which is turned off by default.

*/


/*

ToDo
=====

  m_non_existant_configValue = "non existant";
  is this a good return value for cv?



*/

#ifndef SPCONFIGBASE_H
#define SPCONFIGBASE_H

#include <stdint.h>
#include <string>
#include <string.h>
#include <functional>

#include <spObjectStore.h>
#include <spConfigValue.h>


#ifndef SPCONFIG_MAXLINELENGTH
  #define SPCONFIG_MAXLINELENGTH  100
#endif

#ifndef SPCONFIG_FILEBUFSIZE
  #define SPCONFIG_FILEBUFSIZE  120
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
    bool saveIniEntryCB(std::string id, spConfigValue *cv);
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
    const char* getCStr(const char* section, const char* key, const char* default_value = "");
    std::string getString(const char* section, const char* key, const char* default_value = "");
    int32_t getInt32(const char* section, const char* key, int32_t default_value = 0);
    int32_t getUInt32(const char* section, const char* key, uint32_t default_value = 0);
    int64_t getInt64(const char* section, const char* key, int64_t default_value = 0);
    int64_t getUInt64(const char* section, const char* key, uint64_t default_value = 0);
    double getDouble(const char* section, const char* key, double default_value = 0.0);
    bool getBool(const char* section, const char* key, bool default_value = false);
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

};


#endif // SPCONFIGBASE_H


