# spConfig Library

This library provides the spConfig class for managing configuration values as well as read them from / save them to configuation files in C++.

It also provides the underlying spConfigBase class, which can be used to develop the same functionality in a context outside of standard C++. As an example of this, see ESPspConfig, which has been developed and written speficically for programming ESP32 MCUs in platformio and Arduino framework.

When using the read() and save() functions, the configuration values will be read from / saved to a 'config.ini' file in the current folder. If available, values from the 'config-default.ini' file will be read before trying to read from 'config.ini'. Such a default configuration allows for an optional method of specifying factory defaults for settings to be changed on user level in the application. 
It also serves as an option to change the value of 'variables' without touching code, i.e. no need to recompile, just amend the config value and restart.


ToDo: complete path setter / getter

Set optional path or change the extension ('ini') or filenames for 'config' and 'config-default' to determine the location and names of files used.
Autosave is disabled by default, but when enbled, the spConfig object will check for changes to the configuration values and saves them automatically.

ToDo: example

The .cpp files in the /examples folder demonstrate the various options to use the functions.


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


#### getConfigValue() Function
```cpp
spConfigValue* getConfigValue(const char* section, const char* key);
```
Return an spConfigValue object, which allows access to the value via its asXYZ() functions, e.g. asString(), asInt32(), .... There is normally no need to work with spConfigValue objects directly, as the following getXYZ() functions of spConfig provide easier access to the values stored.


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

```cpp
bool exists(const char* section, const char* key);
```
To determine whether a value exists with these section and key parameters. 


```cpp
bool changed();
```
```cpp
void reset();
```
Reset the whole config to either 'filename-default.ini' as the factory defaults or - if no default file exists - to an empty list.

#### read() and save() Functions
```cpp
void read();
void save();
```
The config data are read by trying to parse first the 'config-default.ini' file and then the 'config.ini' file, whereby each file is only parsed when it exists and default values with the same section and key will be replaced by those in the 'config.ini' file.

Saving of the config data with save() will only be done when data have changed. Thus, prior checking of changed(); is not needed for that purpose. There is an autosave functionality, which is turned off by default and can be enabled with setAutosave(true);

#### setAutosave() and getAutosave() Functions
```cpp
bool setAutosave(bool autosave);
bool getAutosave();
```
Setting autosave to true will automatically check for any changes made to configuration values and then saves them to file. This means there is no need to call the save() function in the  program code. However, when the program is making many changes in a row, it may be better to set autosave to false during that time. When setting autosave to true again, these values will then be saved to file.

#### setConfigFilename() and getConfigFilename() Functions
```cpp
void setConfigFilename(std::string newName);
std::string getConfigFilename();
```
To change the standard 'config' filename and using a different one and to retrieve the currently used file name. This can also be used to temporarily change the name in order to make a (backup) copy of the configuration values.

#### setConfigDefaultFilename() and getConfigDefaultFilename() Functions
```cpp
void setConfigDefaultFilename(std::string newName);
std::string getConfigDefaultFilename();
```
Setting name for the file with default configuration values and replacing the standard 'config-default' with 'newName'.

#### setConfigFileExtension() and getConfigFileExtension() Functions
```cpp
void setConfigFileExtension(std::string newExtension);
std::string getConfigFileExtension();
```
Replacing the standard 'ini' extention and retrieving the current extension used.



</br>

## Licence
MIT licence  
Copyright &copy; 2024 by krokoreit
