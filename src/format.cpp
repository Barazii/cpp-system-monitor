#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include "format.h"

const std::uint32_t DAY = 86400; // seconds
const std::uint16_t HOUR = 3600; // seconds
const std::uint8_t MIN = 60;     // seconds

// INPUT: Long int measuring seconds
// OUTPUT: DD:HH:MM:SS
std::string Format::ElapsedTime(long seconds) {
  std::uint32_t remaining_sec;
  std::uint8_t dd = seconds / DAY;
  remaining_sec = seconds % DAY;
  std::uint8_t hh = remaining_sec / HOUR;
  remaining_sec = remaining_sec % HOUR;
  std::uint8_t mm = remaining_sec / MIN;
  std::uint8_t ss = remaining_sec % MIN;

  std::ostringstream outputstream;
  outputstream.width(2);
  outputstream.fill('0');
  outputstream << static_cast<std::uint16_t>(dd) << ":";
  outputstream.width(2);
  outputstream.fill('0');
  outputstream << static_cast<std::uint16_t>(hh) << ":";
  outputstream.width(2);
  outputstream.fill('0');
  outputstream << static_cast<std::uint16_t>(mm) << ":";
  outputstream.width(2);
  outputstream.fill('0');
  outputstream << static_cast<std::uint16_t>(ss);

  return outputstream.str();
}