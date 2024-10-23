/**
 * @file spConfig.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration values and files
 * @version 2.1.1
 * @date 2024-10-23
 * @copyright Copyright (c) 2024
 * 
 * Version history:
 * v1       original develoment for use in platformio / arduino for an ESP32 project
 * v2.0.0   changed to standard C++ 
 * v2.1.0   updated to use spObjectStore v2.1.0
 * v2.1.1   replaced printf() with spLogHelper
 * v2.1.2   minor updates
 *  
 */


#ifndef SPCONFIG_H
#define SPCONFIG_H


#include <thread>
#include <spLogHelper.h>
#include <spConfigBase.h>


class spConfig : public spConfigBase {
  private:
    std::thread *m_pLoopThread; 
    
    // override virtuals
    void onSetChanged();
    size_t readFile(std::string filename, char* buf, size_t startPos, size_t maxBytes);
    size_t saveFile(std::string filename, char* buf, size_t startPos, size_t writeBytes);
    void ensureLoopTask();

    uint64_t timeSinceEpochMillisec();
    static void config_loop_task(spConfig* pConfig);

  public:
    ~spConfig();

};

#endif // SPCONFIG_H


