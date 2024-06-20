/**
 * example code for spConfig library
 * 
 * 
 */

#include <filesystem>

#include <spConfig.h>

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

  // config
  config.setConfigFilename("application");
  config.setConfigDefaultFilename("default");
  config.setConfigFileExtension("conf");
  config.read();

  printf("configuration values read:\n");
  int32_t maybeInt32 = config.getInt32("maybe-overridden", "maybeInt32", 1111);
  printf("   maybeInt32: %i\n", maybeInt32);
  std::string maybeString = config.getString("maybe-overridden", "maybeString", "no maybeString found");
  printf("   maybeString: %s\n", maybeString.c_str());
  int32_t stableInt32 = config.getInt32("not-overridden", "stableInt32", 2222);
  printf("   stableInt32: %i\n", stableInt32);
  std::string stableString = config.getString("not-overridden", "stableString", "no stableString found");
  printf("   stableString: %s\n", stableString.c_str());
  printf("\nrename 'application.conf' and then 'default.conf' and see how the output changes.\n...\n");

  // ========================================================
  printf("done\n");
  return 0;
}
