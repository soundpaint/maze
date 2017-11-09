/*
 * Maze -- A maze / flipper game implementation for RPi with Sense Hat
 * Copyright (C) 2016, 2017  Jürgen Reuter
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <https://www.gnu.org/licenses/>.
 *
 * For updates and more info or contacting the author, visit:
 * <https://github.com/soundpaint/maze>
 *
 * Author's web site: www.juergen-reuter.de
 */

#ifndef LOG_HH
#define LOG_HH

#include <pthread.h>
#include <string>
#include <ostream>
#include <iostream>
#include <sstream>

class Log
{
public:
  static Log _fatal;
  static Log _error;
  static Log _info;
  static void fatal(const std::string msg);
  static void error(const std::string msg);
  static void warn(const std::string msg);
  static void info(const std::string msg);
  static void info(const std::string msg, const bool omitLineFeed);
  static void debug(const std::string msg);
  Log& endl();
  template<class T> Log& operator<<(const T &arg)
  {
    _buffer << arg;
    return *this;
  }
private:
  static pthread_mutex_t _serialize_lock;
  const char *_label;
  std::ostream *_out;
  std::stringstream _buffer;
  bool _abort;
  Log(const char *label, std::ostream *out, const bool abort);
  virtual ~Log();
};

#endif /* LOG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
