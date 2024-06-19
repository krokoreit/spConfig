/**
 * @file spConfig.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration values and files
 * 
 * @version 2.0.0
 * @date 2024-06-19
 * 
 * @copyright Copyright (c) 2024
 * 
 * 
 */

#ifndef SPCONFIG_H
#define SPCONFIG_H

#include <thread>
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


