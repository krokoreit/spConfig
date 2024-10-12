/**
 * example code for spConfig library
 * 
 * 
 */

#include <filesystem>

#include <spConfig.h>


#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
  #define USED_OS "Windows"
#else
  #define USED_OS 'not Windows'
#endif


spConfig config;


/**
 * @brief our main function
 * 
 */
int main(int argc, char *argv[])
{
  std::string a = argv[0];
  printf("running %s\n", a.substr(a.rfind(std::filesystem::path::preferred_separator) + 1).c_str());
  // ========================================================

  std::string subDir("sep/new");
  std::filesystem::create_directories(subDir);

  // config
  config.setConfigFilename("seperator");
  config.setConfigFilePath(subDir);
  config.setAutosave(true);
  config.read();

  config.setValue("system", "usedOperatingSystem", USED_OS);
  
  //config.setValue("system", "separator", char(SPCONFIG_FILEPATH_SEPARATOR));
  std::string sep(1, SPCONFIG_FILEPATH_SEPARATOR);
  config.setValue("system", "separator", sep.c_str());


  // ========================================================
  printf("done\n");
  return 0;
}
