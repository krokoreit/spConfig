/**
 * @file spConfigBase.cpp
 * 
 */

#include <spConfigBase.h>
#include <filesystem>


/*    PUBLIC    PUBLIC    PUBLIC    PUBLIC    

      xxxxxxx   xx    xx  xxxxxxx   xx           xx      xxxxxx 
      xx    xx  xx    xx  xx    xx  xx           xx     xx    xx
      xx    xx  xx    xx  xx    xx  xx           xx     xx      
      xxxxxxx   xx    xx  xxxxxxx   xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx    xx
      xx         xxxxxx   xxxxxxx   xxxxxxxx     xx      xxxxxx 
     

      PUBLIC    PUBLIC    PUBLIC    PUBLIC    */


spConfigBase::spConfigBase() : m_store(true)
{
}


spConfigBase::~spConfigBase()
{
  if (m_autosave)
  {
    save();
  }
}

void spConfigBase::setValue(const char* section, const char* key, const char* value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key), true, value);
  if (!m_store.isAdded())
  {
    if (value == cv->c_str())
    {
      return;
    }
    *cv = value;
  }
  setChanged();
}

void spConfigBase::setValue(const char* section, const char* key, int32_t value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key), true, value);
  if (!m_store.isAdded())
  {
    if (value == cv->asInt32())
    {
      return;
    }
    *cv = value;
  }
  setChanged();
}

void spConfigBase::setValue(const char* section, const char* key, uint32_t value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key), true, value);
  if (!m_store.isAdded())
  {
    if (value == cv->asUInt32())
    {
      return;
    }
    *cv = value;
  }
  setChanged();
}

void spConfigBase::setValue(const char* section, const char* key, int64_t value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key), true, value);
  if (!m_store.isAdded())
  {
    if (value == cv->asInt64())
    {
      return;
    }
    *cv = value;
  }
  setChanged();
}

void spConfigBase::setValue(const char* section, const char* key, uint64_t value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key), true, value);
  if (!m_store.isAdded())
  {
    if (value == cv->asUInt64())
    {
      return;
    }
    *cv = value;
  }
  setChanged();
}

void spConfigBase::setValue(const char* section, const char* key, double value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key), true, value);
  if (!m_store.isAdded())
  {
    if (value == cv->asDouble())
    {
      return;
    }
    *cv = value;
  }
  setChanged();
}

void spConfigBase::setValue(const char* section, const char* key, bool value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key), true, value);
  if (!m_store.isAdded())
  {
    if (value == cv->asBool())
    {
      return;
    }
    *cv = value;
  }
  setChanged();
}


spConfigValue* spConfigBase::getConfigValue(const char* section, const char* key)
{
  spConfigValue* cv = m_store.getObj(makeId(section, key));
  if (cv == nullptr)
  {
    return &m_non_existant_configValue;
  }
  return cv;
}

const char* spConfigBase::getCStr(const char* section, const char* key, const char* default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->c_str();
  } 
  return default_value;
}

std::string spConfigBase::getString(const char* section, const char* key, const char* default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->asString();
  } 
  return std::string(default_value);
}

int32_t spConfigBase::getInt32(const char* section, const char* key, int32_t default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->asInt32();
  } 
  return default_value;
}

int32_t spConfigBase::getUInt32(const char* section, const char* key, uint32_t default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->asUInt32();
  } 
  return default_value;
}

int64_t spConfigBase::getInt64(const char* section, const char* key, int64_t default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->asInt64();
  } 
  return default_value;  
}

int64_t spConfigBase::getUInt64(const char* section, const char* key, uint64_t default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->asUInt64();
  } 
  return default_value;
}

double spConfigBase::getDouble(const char* section, const char* key, double default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->asDouble();
  } 
  return default_value;
}

bool spConfigBase::getBool(const char* section, const char* key, bool default_value)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  if (cv)
  {
    return cv->asBool();
  } 
  return default_value;
}


bool spConfigBase::exists(const char* section, const char* key)
{
  spConfigValue *cv = m_store.getObj(makeId(section, key));
  return (cv != nullptr);
}


bool spConfigBase::changed()
{
  return m_hasChanged;
}



void spConfigBase::reset()
{
  if (!canUseFS())
  {
    return;
  }
  m_store.reset();
  save();

  // read defaults
  if (parseIniFile(m_configDefaultFilename))
  {
    m_hasChanged = true; // force save
    save();
  }
  m_hasChanged = false;
}



void spConfigBase::read(){
 
//ToDo
printf("reading configuration\n");

  if (!canUseFS())
  {
    return;
  }

  m_store.reset();
  parseIniFile(m_configDefaultFilename);
  if (!parseIniFile(m_configFilename))
  {
    m_hasChanged = true; // force save
    save();
  }
  m_hasChanged = false;
}


void spConfigBase::save()
{
  if (!m_hasChanged || !canUseFS())
  {
    return;
  }

  if (m_pFileBuf == nullptr)
  {
    m_pFileBuf = (char*)malloc(SPCONFIG_FILEBUFSIZE);
    if (m_pFileBuf == nullptr)
    {

//ToDo
printf("spConfigBase::save() could not allocate file buffer of size %d\n", SPCONFIG_FILEBUFSIZE);

      return;
    }
  }
  // start of file & buffer
  m_fPos = 0;
  m_fileBufUsed = 0;

  m_filenameUsed = m_configFilePath + m_configFilename;
  if (m_configFileExtension.length() > 0)
  {
    m_filenameUsed.append(".");
    m_filenameUsed.append(m_configFileExtension);
  }


// ToDo external print
printf("saving %s\n", m_filenameUsed.c_str());

  // init collection vars
  m_lastSection = "";

  // callback, link non-static function to have access to non-static members, i.e. the spConfigBase object
  m_store.forEach(std::bind(&spConfigBase::saveIniEntryCB, this, std::placeholders::_1, std::placeholders::_2));

  // final write
  saveFile(m_filenameUsed, m_pFileBuf, m_fPos, m_fileBufUsed);

  // release buffer and return success
  free(m_pFileBuf);


  m_hasChanged = false;
}


/**
 * @brief set autosave mode and return previous mode
 * 
 * @param autosave true or false
 * @return true or false
 */
bool spConfigBase::setAutosave(bool autosave)
{
  bool oldAutosave = m_autosave;
  if (m_autosave != autosave)
  {
    m_autosave = autosave;
    if (oldAutosave)
    {
      // ensure we have no unsaved stuff
      save();
    }
    else 
    {
      // ensure we have a looptask
      ensureLoopTask();
    }
  }
  return oldAutosave;
}


/**
 * @brief return status of autosave
 * 
 * @return true / false 
 */
bool spConfigBase::getAutosave()
{
  return m_autosave;
}


void spConfigBase::setConfigFilename(std::string newName)
{
  if (newName.length() > 0)
  {
    m_configFilename = newName;
  }
}

std::string spConfigBase::getConfigFilename()
{
  return m_configFilename;
}

void spConfigBase::setConfigDefaultFilename(std::string newName)
{
  if (newName.length() > 0)
  {
    m_configDefaultFilename = newName;
  }
}

std::string spConfigBase::getConfigDefaultFilename()
{
  return m_configDefaultFilename;
}

void spConfigBase::setConfigFileExtension(std::string newExtension)
{
  if (newExtension.length() > 0)
  {
    m_configFileExtension = newExtension;
  }
}

std::string spConfigBase::getConfigFileExtension()
{
  return m_configFileExtension;
}

void spConfigBase::setConfigFilePath(std::string newPath)
{
  // remove any trailing slashes, Windows or Linux style
  size_t len = newPath.length();
  if (len > 0)
  {
    std::filesystem::path path(newPath);
    newPath = path.make_preferred().string();
    len = newPath.length();
    if ((len > 0) && (newPath.back() != path.preferred_separator))
    {
      newPath += path.preferred_separator;
    }
  }
  m_configFilePath = newPath;
}

std::string spConfigBase::getConfigFilePath()
{
  return m_configFilePath;
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
 * @brief set changed to true and call onSetChanged()
 * 
 */
void spConfigBase::setChanged()
{
  m_hasChanged = true;
  onSetChanged();
}

/**
 * @brief combine section and key to ID string
 * 
 * @param section 
 * @param key 
 * @return std::string  ID used for store 
 */
std::string spConfigBase::makeId(const char* section, const char* key)
{
  std::string ret(section);
  ret.append("#/#");
  ret.append(key);
  return ret;
}


/**
 * @brief callback function for each entry in store
 * 
 * @param id  ID used in store
 * @param cv  configvalue in store
 * @return true  for continue loop (as we want all values)
 */
bool spConfigBase::saveIniEntryCB(std::string id, spConfigValue *cv)
{

  if (m_pFileBuf == nullptr)
  {
   
//ToDo
printf("spConfigBase::saveIniEntryCB() failed because no file buffer allocated\n");

    return false;
  }

  int sIdx = id.find("#/#", 0);
  if (sIdx == -1) // should not happen, but ...
  {

//ToDo
printf("spConfigBase::saveIniEntryCB called with bad id %s\n", id.c_str());

    return true;
  }
  std::string section = id.substr(0, sIdx);
  std::string key = id.substr(sIdx + 3);

  // line string
  std::string lineString = "";
  char lineBuf[SPCONFIG_MAXLINELENGTH + 1];
  size_t len;


  // write new section
  if (section.compare(m_lastSection) != 0)
  {
    if (m_lastSection.length() > 0)
    {
      lineString = "\n";
    }
    m_lastSection = section;
    len = snprintf(lineBuf, SPCONFIG_MAXLINELENGTH + 1, "[%s]\n", section.c_str());
    if (len > SPCONFIG_MAXLINELENGTH)
    {

//ToDo
      printf("spConfigBase::saveIniEntryCB: section entry %s exceeds maximum line length of %i\n", lineBuf, SPCONFIG_MAXLINELENGTH);
    }
    if (len > 0)
    {
      lineString.append(lineBuf);
    }
  }

  // write key = value
  len = snprintf(lineBuf, SPCONFIG_MAXLINELENGTH + 1, "%s=%s\n", key.c_str(), cv->c_str());
  if (len > SPCONFIG_MAXLINELENGTH)
  {

//ToDo    
    printf("spConfigBase::saveIniEntryCB: entry %s exceeds maximum line length of %i\n", lineBuf, SPCONFIG_MAXLINELENGTH);
  }
  if (len > 0)
  {
    lineString.append(lineBuf);
  }


  len = lineString.length();
  if (len > 0)
  {
    // exceeding file buffer size?
    if (m_fileBufUsed + len > SPCONFIG_FILEBUFSIZE)
    {
      saveFile(m_filenameUsed, m_pFileBuf, m_fPos, m_fileBufUsed);
      m_fPos += m_fileBufUsed;
      m_fileBufUsed = 0;
    }
    
    // copy line to buffer
    memcpy(m_pFileBuf + m_fileBufUsed, lineString.c_str(), len);
    m_fileBufUsed += len;
  }
  
  return true;
}

/**
 * @brief remove white space from text in buffer 
 * 
 * @param buf  buffer
 * @param len  len of text in buffer
 * @return size_t  new length of trimmed text
 */
size_t spConfigBase::trimLine(char* buf, size_t len)
{
  int sLen = len;
  // from end
  while (sLen > 0)
  {
    if (!isspace(buf[sLen-1]))
    {
      break;
    }
    sLen--;
  }
  // from start
  if (sLen > 0)
  {
    int i = 0;
    while (i < sLen)
    {
      if (!isspace(buf[i]))
      {
        if (i > 0)
        {
          sLen -= i;
          memmove(buf, buf + i, sLen);
        }
        break;
      }
      i++;
    }
  }
  return sLen;
}

/**
 * @brief remove comments from text in buffer
 * 
 * @param buf  buffer
 * @param len  len of text in buffer
 * @return size_t  new length of text
 */
size_t spConfigBase::eraseComments(char* buf, size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    if ((buf[i] == '#') || (buf[i] == ';'))
    {
      if ((i > 0) && buf[i-1] == '\\')
      {
        return i-1;
      }
      return i;
    }
  }
  return len;
}

/**
 * @brief parse file into store
 * 
 * @param filename  file to parse
 * @return true / false  for success 
 */
bool spConfigBase::parseIniFile(std::string filename)
{

  if (m_pFileBuf == nullptr)
  {
    m_pFileBuf = (char*)malloc(SPCONFIG_FILEBUFSIZE);
    if (m_pFileBuf == nullptr)
    {
      
//ToDo
      printf("spConfigBase::parseIniFile() could not allocate file buffer of size %d\n", SPCONFIG_FILEBUFSIZE);
      return false;
    }
  }

  m_filenameUsed = m_configFilePath + filename;
  if (m_configFileExtension.length() > 0)
  {
    m_filenameUsed.append(".");
    m_filenameUsed.append(m_configFileExtension);
  }


// ToDo external print
printf("spConfigBase::parseIniFile() for %s\n", m_filenameUsed.c_str());    


  // attempt to read file
  int32_t received = readFile(m_filenameUsed, m_pFileBuf, 0, SPCONFIG_FILEBUFSIZE);
  // return if nothing received
  if (received < 1)
  {
    return false;
  }
  
  // line
  char lineBuf[SPCONFIG_MAXLINELENGTH + 1];
  size_t lineLength = 0;
  // read char
  char c;
  // pos in file processed
  m_fPos = 0;
  // pos in buffer
  size_t lPos = 0;
  size_t bPos = 0;
  // string for content
  std::string section = "";
  std::string key = "";
  std::string value = "";
  // position of equal sign in key = value
  size_t equalPos = -1;
  // len of string
  size_t sLen = -1;

  // with content in file
  while (received > 0)
  {
    // new chunk to process
    bPos = 0;
    lPos = 0;
    
    while (bPos < received)
    {
      // line by line
      lineLength = 0;
      c = m_pFileBuf[bPos++];
      while (c != '\n')
      {
        if (lineLength < SPCONFIG_MAXLINELENGTH)
        {
          lineBuf[lineLength++] = c;
          c = m_pFileBuf[bPos++];
        }
        else
        {
          
//ToDo
          printf("spConfigBase::parseIniFile(): line in %s exceeds maximum length of %d\n", m_filenameUsed.c_str(), SPCONFIG_MAXLINELENGTH);
          c = '\n';
        }        
      }

      // new line end
      if ((c == '\n') && (lineLength > 0))
      {
        // pos in buffer after full line completed
        lPos = bPos;
        // parse line
        lineLength = eraseComments(lineBuf, lineLength);
        lineLength = trimLine(lineBuf, lineLength);
        if (lineLength > 0)
        {
          // section
          if (lineBuf[0] == '['){
            if (lineLength > 2 && lineBuf[lineLength-1] == ']')
            {
              memmove(lineBuf, lineBuf+1, lineLength-2);
              lineLength = trimLine(lineBuf, lineLength-2);
            }
            else
            {
              lineLength = 0; // empty or not ] closed, skip this section
            }
            lineBuf[lineLength] = 0;
            section = std::string(lineBuf);
          // key = value text within a section
          } 
          else if(section.length() > 0)
          {
            equalPos = -1;
            sLen = -1;
            for (size_t i = 0; i < lineLength; i++)
            {
              if(lineBuf[i] == '=')
              {
                equalPos = i;
                break;
              }
            }

            //  abc=de    lineLength  6
            //  ...e      equalPos    3
            //  abcdef    lineLength  6
            //  ......    equalPos    -1 -> 6
            //  abcde=    lineLength  6
            //  .....e    equalPos    5

            // equalPos could be -1 for no value entry
            if (equalPos == -1)
            {
              equalPos = lineLength;
            }
            sLen = trimLine(lineBuf, equalPos); // possible right trim (before '=')
            if (sLen > 0) // only with key existing = chars left to use
            {
              key = std::string(lineBuf, sLen);
              value = "";
              if (equalPos < lineLength)
              {
                sLen = lineLength - equalPos - 1;
                for (size_t i = equalPos+1; i < lineLength; i++)
                {
                  if (!isspace(lineBuf[i])) // possible left trim (after '=')
                  {
                    break;
                  }
                  sLen--;
                }
                if (sLen > 0)
                {
                  value = std::string(lineBuf + lineLength - sLen, sLen);
                }
              }
              // good to store with addObj to overwrite existing entry
              m_store.addObj(makeId(section.c_str(), key.c_str()), value.c_str());
            }
          
          }
        }

      } // if ((c == '\n') && (lineLength > 0))

    } // while (bPos < received)

    //done with file?
    if (received < SPCONFIG_FILEBUFSIZE)
    {
      received = 0;
    }
    else
    {
      m_fPos += lPos;
      received = readFile(m_filenameUsed, m_pFileBuf, m_fPos, SPCONFIG_FILEBUFSIZE);
    }


  } // while (received > 0)
  
  // release buffer and return success
  free(m_pFileBuf);
  return true;
}


/**
 * @brief set the time for next autosave
 * 
 * @param timeMS  tim ein ms since epoch or 0 to pause autosave
 */
void spConfigBase::setNextAutosaveTimeMS(uint64_t timeMS)
{
  m_autosaveTimeMS = timeMS;
}

/**
 * @brief return time of next autosave in ms since epoch
 * 
 * @return uint64_t 
 */
uint64_t spConfigBase::getNextAutosaveTimeMS()
{
  return m_autosaveTimeMS;
}

/**
 * @brief base function to return usability of the file system
 *        can override in derived class, where this needs to be verified
 * 
 */
bool spConfigBase::canUseFS()
{
  return true;
}


/**
 * @brief base function to act on changed event
 *        can override in derived class 
 * 
 */
void spConfigBase::onSetChanged()
{  
}

/**
 * @brief base function to read a file
 *        should be overriden by derived class
 * 
 * @param filename  name of file
 * @param buf   pointer to buffer to hold read content
 * @param startPos  position in file to start reading
 * @param maxBytes  max number of bytes to read
 * @return size_t  actual number of bytes read
 */
size_t spConfigBase::readFile(std::string filename, char* buf, size_t startPos, size_t maxBytes)
{
  return 0;
}

/**
 * @brief base function to save a file
 *        should be overriden by derived class
 * 
 * @param filename  name of file
 * @param buf   pointer to buffer holding content to write
 * @param startPos  position in file to start writing
 * @param writeBytes  max number of bytes to read
 * @return size_t  actual number of bytes written
 */
size_t spConfigBase::saveFile(std::string filename, char* buf, size_t startPos, size_t writeBytes)
{
  return 0;
}


/**
 * @brief base function to create a loop task if needed
 *        should be overriden by derived class
 * 
 */
void spConfigBase::ensureLoopTask()
{
  // for derived class to do
}
