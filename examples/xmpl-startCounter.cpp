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

  uint32_t startCounterTotal = 0;
  uint32_t startCounterToday = 0;

  // config
  config.setAutosave(true);
  config.read();

  time_t now = time(nullptr);
  tm* local_tm = localtime(&now);

  char todayString[9];
  sprintf(todayString, "%4d%02d%02d", local_tm->tm_year + 1900, local_tm->tm_mon + 1, local_tm->tm_mday);

  printf("todayString is %s\n", todayString);

  // get config values
  if (strcmp(todayString, config.getCStr("system", "todayString", "none")) == 0)
  {
    startCounterToday = config.getInt32("system", "startCounterToday", startCounterToday);
    startCounterToday++;
  }
  else
  {
    config.setValue("system", "todayString", todayString);
    startCounterToday = 1;
  }
  config.setValue("system", "startCounterToday", startCounterToday);
  printf("Start Counter: %i\n", startCounterToday);
  
  startCounterTotal = config.getInt32("system", "startCounterTotal", startCounterTotal);
  startCounterTotal++;
  config.setValue("system", "startCounterTotal", startCounterTotal);
  

  // ========================================================
  printf("done\n");
  return 0;
}
