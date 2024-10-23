/**
 * @file spConfig.cpp
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration values and files
 * @version 2.1.2
 * @date 2024-10-23
 * @copyright Copyright (c) 2024
 * 
 */

#include <spConfig.h>


/*    PUBLIC    PUBLIC    PUBLIC    PUBLIC    

      xxxxxxx   xx    xx  xxxxxxx   xx           xx      xxxxxx 
      xx    xx  xx    xx  xx    xx  xx           xx     xx    xx
      xx    xx  xx    xx  xx    xx  xx           xx     xx      
      xxxxxxx   xx    xx  xxxxxxx   xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx    xx
      xx         xxxxxx   xxxxxxx   xxxxxxxx     xx      xxxxxx 
     

      PUBLIC    PUBLIC    PUBLIC    PUBLIC    */

spConfig::~spConfig(){
  if (m_pLoopThread != nullptr)
  {
    setAutosave(false);
  }
}



/*    PRIVATE    PRIVATE    PRIVATE    PRIVATE

      xxxxxxx   xxxxxxx      xx     xx    xx     xx     xxxxxxxx  xxxxxxxx
      xx    xx  xx    xx     xx     xx    xx    xxxx       xx     xx      
      xx    xx  xx    xx     xx     xx    xx   xx  xx      xx     xx      
      xxxxxxx   xxxxxxx      xx      xx  xx   xx    xx     xx     xxxxxxx    
      xx        xx    xx     xx      xx  xx   xxxxxxxx     xx     xx    
      xx        xx    xx     xx       xxxx    xx    xx     xx     xx      
      xx        xx    xx     xx        xx     xx    xx     xx     xxxxxxxx
     

      PRIVATE    PRIVATE    PRIVATE    PRIVATE    */



/**
 * @brief function called when config was changed
 * 
 */
void spConfig::onSetChanged()
{
  // delay autosave as we may have multiple values set
  setNextAutosaveTimeMS(timeSinceEpochMillisec() + 1500);
}

/**
 * @brief reading file content into buffer
 * 
 * @param filename  name of configuration file
 * @param buf       pointer to buffer to read to
 * @param startPos  potiotion in file to start reading from
 * @param maxBytes  maximum number of bytes to read
 * @return size_t   number of bytes read
 */
size_t spConfig::readFile(std::string filename, char* buf, size_t startPos, size_t maxBytes)
{
  FILE *pFile = fopen(filename.c_str(), "r");
  if (!pFile)
  {
    spLOGF_E("spConfig::readFile() failed to get handle for %s", filename.c_str());
    return 0;
  }

  if (startPos > 0)
  {
    fseek(pFile, startPos, SEEK_SET);
  }
  
  size_t len = fread(buf, 1, maxBytes, pFile);
  if ((len != maxBytes) && (!feof(pFile)))
  {
    spLOGF_E("spConfig::readFile() received error when reading %s", filename.c_str());
    len = 0;
  }

  fclose(pFile);
  return len;
}

/**
 * @brief writing file content from buffer
 * 
 * @param filename  name of configuration file
 * @param buf       pointer to buffer to write from
 * @param startPos  potiotion in file to start writing from
 * @param writeBytes  maximum number of bytes to write
 * @return size_t   number of bytes written
 */
size_t spConfig::saveFile(std::string filename, char* buf, size_t startPos, size_t writeBytes)
{
  FILE *pFile = fopen(filename.c_str(), "w");
  if (!pFile)
  {
    spLOGF_E("spConfig::saveFile() failed to get handle for %s", filename.c_str());
    return 0;
  }

  setNextAutosaveTimeMS(0); // stop auto save
 
  if (startPos > 0)
  {
    fseek(pFile, startPos, SEEK_SET);
  }
  
  size_t len = fwrite(buf, 1, writeBytes, pFile);
  if (len != writeBytes)
  {
    spLOGF_E("spConfig::saveFile() received error while writing %s", filename.c_str());
    len = 0;
  }

  fclose(pFile);
  return len;
}

/**
 * @brief create loop task if not exisiting
 * 
 */
void spConfig::ensureLoopTask() {
  if (m_pLoopThread == nullptr)
  {
    std::thread t(config_loop_task, this);
    m_pLoopThread = &t;
    m_pLoopThread->detach();
  }
}

/**
 * @brief milliseconds since epoch
 *        see https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
 * 
 * @return uint64_t 
 */
uint64_t spConfig::timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/**
 * @brief ever running task, used to regularly check on _loopHandler() and other things
 * 
 * @param pConfig   pointer to object given to xTaskCreate()
 */
void spConfig::config_loop_task(spConfig* pConfig) {
  while (pConfig->getAutosave())
  {
    if (pConfig->changed())
    {
      uint64_t timeMS = pConfig->getNextAutosaveTimeMS();
      if ((timeMS > 0) && (pConfig->timeSinceEpochMillisec() > timeMS))
      {
        pConfig->save();
      }
    }
    // sleep for 1 sec
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  pConfig->m_pLoopThread = nullptr;
}
