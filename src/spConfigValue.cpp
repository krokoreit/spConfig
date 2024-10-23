/**
 * @file spConfigValue.cpp
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration data
 * @version 2.0.1
 * @date 2024-10-22
 * @copyright Copyright (c) 2024
 * 
 */

#include <spConfigValue.h>


/*    PUBLIC    PUBLIC    PUBLIC    PUBLIC    

      xxxxxxx   xx    xx  xxxxxxx   xx           xx      xxxxxx 
      xx    xx  xx    xx  xx    xx  xx           xx     xx    xx
      xx    xx  xx    xx  xx    xx  xx           xx     xx      
      xxxxxxx   xx    xx  xxxxxxx   xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx    xx
      xx         xxxxxx   xxxxxxx   xxxxxxxx     xx      xxxxxx 
     

      PUBLIC    PUBLIC    PUBLIC    PUBLIC    */


/**
 * @brief Construct a new config value from char*
 * 
 * @param cstr 
 */
spConfigValue::spConfigValue(const char *cstr)
{
  init();
  *this = cstr;
}

/**
 * @brief Construct a new config value from another config value
 * 
 * @param configValue 
 */
spConfigValue::spConfigValue(const spConfigValue &configValue)
{
  init();
  *this = configValue;
}

/**
 * @brief Construct a new config value from std::string
 * 
 * @param strValue 
 */
spConfigValue::spConfigValue(std::string strValue)
{
  init();
  *this = strValue.c_str();
}

/**
 * @brief Construct a new config value from char
 * 
 * @param charValue 
 */
spConfigValue::spConfigValue(char charValue)
{
  init();
  *this = charValue;
}

/**
 * @brief Construct a new config value from int32_t
 * 
 * @param int32Value 
 */
spConfigValue::spConfigValue(int32_t int32Value)
{
  init();
  *this = int32Value;
}

/**
 * @brief Construct a new config value from uint32_t
 * 
 * @param uint32Value 
 */
spConfigValue::spConfigValue(uint32_t uint32Value)
{
  init();
  *this = uint32Value;
}

/**
 * @brief Construct a new config value from int64_t
 * 
 * @param int64Value 
 */
spConfigValue::spConfigValue(int64_t int64Value)
{
  init();
  *this = int64Value;
}

/**
 * @brief Construct a new config value from uint64_t
 * 
 * @param uint64Value 
 */
spConfigValue::spConfigValue(uint64_t uint64Value)
{
  init();
  *this = uint64Value;
}

/**
 * @brief Construct a new config value from double
 * 
 * @param doubleValue 
 * @param decimalPlaces 
 */
spConfigValue::spConfigValue(double doubleValue, uint32_t decimalPlaces)
{
  init();
  m__decimalPlaces = decimalPlaces;
  *this = doubleValue;
}

/**
 * @brief Construct a new config value from bool
 * 
 * @param boolValue 
 */
spConfigValue::spConfigValue(bool boolValue)
{
  init();
  *this = boolValue;
}

/**
 * @brief Destroy the sp Config Value::sp Config Value object
 * 
 */
spConfigValue::~spConfigValue()
{
  invalidate();
}

/**
 * @brief return char pointer to config value
 * 
 * @return const char* 
 */
const char* spConfigValue::c_str() const
{ 
  return m_buffer;
}

/**
 * @brief return config value as std::string
 * 
 * @return const std::string 
 */
const std::string spConfigValue::asString() const
{
  std::string ret(m_buffer);
  return ret;
}

/**
 * @brief return config value as int32_t
 * 
 * @return int32_t 
 */
int32_t spConfigValue::asInt32() const
{
  if (m_buffer)
      return atol(m_buffer);
  return 0;
}

/**
 * @brief return config value as uint32_t
 * 
 * @return uint32_t 
 */
uint32_t spConfigValue::asUInt32() const
{
  if (m_buffer)
      return strtoul(m_buffer, nullptr, 10);
  return 0;
}

/**
 * @brief return config value as int64_t
 * 
 * @return int64_t 
 */
int64_t spConfigValue::asInt64() const
{
  if (m_buffer)
      return atoll(m_buffer);
  return 0;
}

/**
 * @brief return config value as uint64_t
 * 
 * @return uint64_t 
 */
uint64_t spConfigValue::asUInt64() const
{
  if (m_buffer)
      return strtoull(m_buffer, nullptr, 10);
  return 0;
}

/**
 * @brief return config value as double
 * 
 * @return double 
 */
double spConfigValue::asDouble() const
{
  if (m_buffer)
      return atof(m_buffer);
  return 0.0;
}

/**
 * @brief return config value as bool
 * 
 * @return true for boolean true / false for everything else
 */
bool spConfigValue::asBool() const
{
  return (strcmp(m_buffer, "true") == 0);
}


/**
 * @brief assign value from char* and return pointer to config value
 * 
 * @param cstr 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(const char *cstr)
{
  setBufferValue(cstr);
  return *this;
}

/**
 * @brief assign value of another config value and return pointer to this config value
 * 
 * @param configValue 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(const spConfigValue &configValue)
{
  setBufferValue(configValue.c_str());
  return *this;
}

/**
 * @brief assign value from std::string and return pointer to config value
 * 
 * @param strValue 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(const std::string &strValue)
{
  setBufferValue(strValue.c_str());
  return *this;
}

/**
 * @brief assign value from char and return pointer to config value
 * 
 * @param charValue 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(char charValue)
{
  char buf[] = { charValue, '\0' };
  setBufferValue(buf);
  return *this;
}

/**
 * @brief assign value from int32_t and return pointer to config value
 * 
 * @param int32Value 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(int32_t int32Value)
{
  size_t size = 2 + 8 * sizeof(int32_t); 
  char buf[size];
  snprintf(buf, size, "%d", int32Value);
  setBufferValue(buf);
  return *this;
}

/**
 * @brief assign value from uint32_t and return pointer to config value
 * 
 * @param uint32Value 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(uint32_t uint32Value)
{
  size_t size = 2 + 8 * sizeof(uint32_t); 
  char buf[size];
  snprintf(buf, size, "%u", uint32Value);
  setBufferValue(buf);
  return *this;
}

/**
 * @brief assign value from int64_t and return pointer to config value
 * 
 * @param int64Value 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(int64_t int64Value)
{
  size_t size = 2 + 8 * sizeof(int64_t); 
  char buf[size];
  snprintf(buf, size, "%lld", int64Value);
  setBufferValue(buf);
  return *this;
}

/**
 * @brief assign value from uint64_t and return pointer to config value
 * 
 * @param uint64Value 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(uint64_t uint64Value)
{
  size_t size = 2 + 8 * sizeof(uint64_t); 
  char buf[size];
  snprintf(buf, size, "%llu", uint64Value);
  setBufferValue(buf);
  return *this;
}

/**
 * @brief assign value from double and return pointer to config value
 * 
 * @param doubleValue 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(double doubleValue)
{
  char *buf = (char*)malloc(m__decimalPlaces + 312);
  if (buf)
  {
    // f for double, L for long double
    sprintf(buf, "%.*f", m__decimalPlaces, doubleValue);
    setBufferValue(buf);
    free(buf);
  }
  else
  {
    //*this = "nan";
    setBufferValue("nan");
  }
  return *this;
}

/**
 * @brief assign value from bool and return pointer to config value
 * 
 * @param boolValue 
 * @return spConfigValue& 
 */
spConfigValue& spConfigValue::operator =(bool boolValue)
{
  if (boolValue)
  {
    //*this = "true";
    setBufferValue("true");
  }
  else
  {
    //*this = "false";
    setBufferValue("false");
  }
  return *this;
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
 * @brief initialize config value variables
 * 
 */
inline void spConfigValue::init(void)
{
  m_buffer = nullptr;
  m_capacity = 0;
  m_len = 0;
}

/**
 * @brief invalidate value
 * 
 */
void spConfigValue::invalidate(void)
{
  if(m_buffer)
    free(m_buffer);
  init();
}

/**
 * @brief reserve at least the number of bytes requested, expand the buffer is needed.
 *        will return false if memory space could not be made available.
 * 
 * @param size 
 * @return true / false
 */
bool spConfigValue::reserve(unsigned int size)
{
  if(m_buffer && (m_capacity >= size))
  {
    return true;
  }
  size_t newSize = (size + 16) & (~0xf);
  // Make sure we can fit newsize in the buffer
  if (newSize > UINT16_MAX)
  {
    return false;
  }

  uint16_t oldLen = m_len;
  char *newbuffer = (char *) realloc(m_buffer, newSize);
  if (newbuffer)
  {
    size_t oldSize = m_capacity + 1; // include NULL.
    if (newSize > oldSize)
    {
        memset(newbuffer + oldSize, 0, newSize - oldSize);
    }
    m_capacity = newSize - 1;
    m_buffer = newbuffer;
    setLen(oldLen); 
    return true;
  }
  return false;
}


/**
 * @brief set length of buffer and zero terminate it
 * 
 * @param len 
 */
void spConfigValue::setLen(int len)
{
  m_len = len;
  if (m_buffer)
  {
      m_buffer[len] = 0;
  }
}

/**
 * @brief copy the given char string to buffer after checking for available space of length bytes
 * 
 * @param cstr 
 * @param length 
 */
void spConfigValue::copy(const char *cstr, unsigned int length)
{
  if(!reserve(length))
  {
      invalidate();
      char content[11];
      strncpy(content, cstr, 10);
      content[10] = '\0';
  
      // error log
      spLOGF_E("spConfigValue failed to copy value %s, not enough space for %i bytes\n", content, length);

      return;
  }
  memmove(m_buffer, cstr, length);
  setLen(length);
}

/**
 * @brief set the buffer to char string value
 * 
 * @param cstr 
 */
void spConfigValue::setBufferValue(const char *cstr)
{
  if(cstr)
  {
    copy(cstr, strlen(cstr));
  }
  else
  {
    invalidate();
  }
}
