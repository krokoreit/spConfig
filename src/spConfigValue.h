/**
 * @file spConfigValue.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to hold a config value
 * @version 2.0.0
 * @date 2024-06-10
 * @copyright Copyright (c) 2024
 * 
 */


/**
 * Version history:
 * v1 original develoment for use in platformio / arduino on an ESP32 project
 * v2 changed to standard C++ 
 *  
 */



#ifndef SPCONFIGVALUE_H
#define SPCONFIGVALUE_H

#include <stdint.h>
#include <string>
#include <string.h>


class spConfigValue{
private:
  char* m_buffer;
  uint32_t m_capacity;
  uint32_t m_len;
  uint32_t m__decimalPlaces = 2;
  void init(void);
  void invalidate(void);
  bool reserve(unsigned int size);
  void setLen(int len);
  void copy(const char *cstr, unsigned int length);
  void setBufferValue(const char *cstr);

public:
  spConfigValue(const char *cstr = "");
  spConfigValue(const spConfigValue &configValue);
  spConfigValue(std::string strValue);  
  spConfigValue(char charValue);
  spConfigValue(int32_t int32Value);
  spConfigValue(uint32_t uint32Value);
  spConfigValue(int64_t int64Value);
  spConfigValue(uint64_t uint64Value);
  spConfigValue(double doubleValue, uint32_t decimalPlaces = 2);
  spConfigValue(bool boolValue);
  ~spConfigValue();
  const char* c_str() const;
  const std::string asString() const;
  int32_t asInt32() const;
  uint32_t asUInt32() const;
  int64_t asInt64() const;
  uint64_t asUInt64() const;
  double asDouble() const;
  bool asBool() const;
  spConfigValue& operator =(const char *cstr);
  spConfigValue& operator =(const spConfigValue &configValue);
  spConfigValue& operator =(const std::string &strValue);
  spConfigValue& operator =(char charvalue);
  spConfigValue& operator =(int32_t int32Value);
  spConfigValue& operator =(uint32_t uint32Value);
  spConfigValue& operator =(int64_t int64Value);
  spConfigValue& operator =(uint64_t uint64Value);
  spConfigValue& operator =(double doubleValue);
  spConfigValue& operator =(bool boolValue);

};





#endif // SPCONFIGVALUE_H


