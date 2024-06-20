# spConfig Library

This library provides the spConfig class for managing configuration values as well as read them from / save them to configuration files in C++.

When using the read() and save() functions, the configuration values will be read from / saved to a 'config.ini' file in the current folder. If available, values from the 'config-default.ini' file will be read before trying to read from 'config.ini'. Such a default configuration allows for an optional method of specifying factory defaults for settings to be changed on user level in the application. 
It also serves as an option to change the value of 'variables' without touching code, i.e. no need to recompile, just amend the config value and restart.

Set optional path or change the extension ('ini') or filenames for 'config' and 'config-default' to determine the location and names of files used.
Autosave is disabled by default, but when enabled, the spConfig object will check for changes to the configuration values and saves them automatically.

The .cpp files in the /examples folder demonstrate the various options to use the functions.

This library also contains the spConfigBase class, which can be used to develop the same functionality as spConfig in a context outside of standard C++. As an example of this, see ESPspConfig, which has been developed and written specifically for programming ESP32 MCUs in platformio and Arduino framework.

Both spConfig ans spConfigBase depend on the spObjectStore library ([download here](https://github.com/krokoreit/spObjectStore.git)), which is used under the hood to manage the configuration values.

Enjoy

&emsp;krokoreit  
&emsp;&emsp;&emsp;<img src="assets/krokoreit-01.svg" width="140"/>









## Usage & API

### Configuration Object
Include library and create object
```cpp
#include <spConfig.h>
spConfig config;
```
Typically an application program will start with running the read() function to load configuration data from file. However, this is not a requirement, as you can set() and get() values without any files involved.

In case of using other than the standard file names, extension and location, use the following before calling read() or save():
```cpp
config.setConfigFilename("newName");
config.setConfigDefaultFilename("newDefaultName");
config.setConfigFileExtension("newExtension");
config.setConfigFilePath("newPath");
```

</br>

### Config File Format

The configuration files are regular text files, with each line representing either a section or a key=value pair, e.g.
```cpp
[system]
startCounterToday=6
startCounterTotal=10
todayString=20240615
```

The 'config-default' file allows for
  - comments (starting with '#' or ';')
  - whitespaces before or after section, key or value enties
  - keys without values (as either key only or with '=', but no value)
and it must have at least one [section].

Note that 'config' file will be build from 'config-default' as well as entries made with
application code and therefore does not allow for comments or whitespace, which will be 
stripped out when reading the content. However, other manual changes to the 'config' file
will be preserved until reset() is used.

</br>

### API

#### Functions
* [setValue()](#setvalue-functions)  
* [getConfigValue()](#getconfigvalue-function)  
* [get...()](#get-functions)  
* [exists()](#exists-function)  
* [changed()](#changed-function)  
* [reset()](#reset-function)  
* [read() and save()](#read-and-save-functions)  
* [setAutosave() and getAutosave()](#setautosave-and-getautosave-functions)  
* [setConfigFilename() and getConfigFilename()](#setconfigfilename-and-getconfigfilename-functions)  
* [setConfigDefaultFilename() and getConfigDefaultFilename()](#setconfigdefaultfilename-and-getconfigdefaultfilename-functions)  
* [setConfigFileExtension() and getConfigFileExtension()](#setconfigfileextension-and-getconfigfileextension-functions)  
* [setConfigFilePath() and getConfigFilePath()](#setconfigfilepath-and-getconfigfilepath-functions)

#### setValue() Functions
```cpp
void setValue(const char* section, const char* key, const char* value);
void setValue(const char* section, const char* key, int32_t value);
void setValue(const char* section, const char* key, uint32_t value);
void setValue(const char* section, const char* key, int64_t value);
void setValue(const char* section, const char* key, uint64_t value);
void setValue(const char* section, const char* key, double value);
void setValue(const char* section, const char* key, bool value);
```
All values can be stored via various functions with their section and key parameters followed by the value. The value can be const char*, int32_t, uint32_t, int64_t, uint64_t, double or bool.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


#### getConfigValue() Function
```cpp
spConfigValue* getConfigValue(const char* section, const char* key);
```
Return an spConfigValue object, which allows access to the value via its asXYZ() functions, e.g. asString(), asInt32(), .... There is normally no need to work with spConfigValue objects directly, as the following getXYZ() functions of spConfig provide easier access to the values stored.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### get...() Functions
```cpp
const char* getCStr(const char* section, const char* key, const char* default_value = "");
std::string getString(const char* section, const char* key, const char* default_value = "");
int32_t getInt32(const char* section, const char* key, int32_t default_value = 0);
int32_t getUInt32(const char* section, const char* key, uint32_t default_value = 0);
int64_t getInt64(const char* section, const char* key, int64_t default_value = 0);
int64_t getUInt64(const char* section, const char* key, uint64_t default_value = 0);
double getDouble(const char* section, const char* key, double default_value = 0.0);
bool getBool(const char* section, const char* key, bool default_value = false);
```
Getting values requires to use the getter function suitable for the returned type wanted. While a compiler may be able to cast some return values to the type needed in program code, it is generally best to use the appropriate function matching the type needed.

All function have an optional default_value parameter to specify an return value in case the requested value does not exist, i.e. no entry for the section - key combination. Otherwise, the function will return either "", 0 or false.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### exists() Function
```cpp
bool exists(const char* section, const char* key);
```
To determine whether a value exists with these section and key parameters. 

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### changed() Function
```cpp
bool changed();
```
Returns whether the config object holds any changed and not yet saved values. As such it may act as an indicator for the need to save() the content. If autosave is set to true, the changed() status will automatically return to false after the next autosave is performed.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### reset() Function
```cpp
void reset();
```
Reset the whole config to either 'filename-default.ini' as the factory defaults or - if no default file exists - to an empty list.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### read() and save() Functions
```cpp
void read();
void save();
```
The config data are read by trying to parse first the 'config-default.ini' file and then the 'config.ini' file, whereby each file is only parsed when it exists and default values with the same section and key will be replaced by those in the 'config.ini' file.

Saving of the config data with save() will only be done when data have changed. Thus, prior checking of changed(); is not needed for that purpose. There is an autosave functionality, which is turned off by default and can be enabled with setAutosave(true);

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setAutosave() and getAutosave() Functions
```cpp
bool setAutosave(bool autosave);
bool getAutosave();
```
Setting autosave to true will automatically check for any changes made to configuration values and then saves them to file. This means there is no need to call the save() function in the  program code. However, when the program is making many changes in a row, it may be better to set autosave to false during that time. When setting autosave to true again, these values will then be saved to file.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigFilename() and getConfigFilename() Functions
```cpp
void setConfigFilename(std::string newName);
std::string getConfigFilename();
```
To change the standard 'config' filename and using a different one and to retrieve the currently used file name. This can also be used to temporarily change the name in order to make a (backup) copy of the configuration values.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigDefaultFilename() and getConfigDefaultFilename() Functions
```cpp
void setConfigDefaultFilename(std::string newName);
std::string getConfigDefaultFilename();
```
Setting name for the file with default configuration values and replacing the standard 'config-default' with 'newName'.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigFileExtension() and getConfigFileExtension() Functions
```cpp
void setConfigFileExtension(std::string newExtension);
std::string getConfigFileExtension();
```
Replacing the standard 'ini' extention and retrieving the current extension used.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigFilePath() and getConfigFilePath() Functions
```cpp
void setConfigFilePath(std::string newPath);
std::string getConfigFilePath();
```
Replace the default empty string with a path to be used for read() and save(). The newPath can be absolute or relative and trailing slashes will be added if not present. The only condition is that the folder given by newPath exists and the application has read / write rights.  

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

</br>

## License
MIT license  
Copyright &copy; 2024 by krokoreit
