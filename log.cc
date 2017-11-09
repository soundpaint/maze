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

#include <log.hh>
#include <cstdlib>

pthread_mutex_t Log::_serialize_lock;

Log Log::_fatal("fatal", &std::cerr, true);
Log Log::_error("error", &std::cerr, false);
Log Log::_info("info", &std::cout, false);

void
Log::fatal(const std::string msg)
{
  pthread_mutex_lock(&_serialize_lock);
  std::cerr << "[fatal] " << msg << "\r\n" << std::flush;
  pthread_mutex_unlock(&_serialize_lock);
  //exit(EXIT_FAILURE);
  throw 20;
}

void
Log::error(const std::string msg)
{
  pthread_mutex_lock(&_serialize_lock);
  std::cerr << "[error] " << msg << "\r\n" << std::flush;
  pthread_mutex_unlock(&_serialize_lock);
}

void
Log::warn(const std::string msg)
{
  pthread_mutex_lock(&_serialize_lock);
  std::cerr << "[warn] " << msg << "\r\n" << std::flush;
  pthread_mutex_unlock(&_serialize_lock);
}

void
Log::info(const std::string msg)
{
  info(msg, false);
}

void
Log::info(const std::string msg, const bool omitLineFeed)
{
  pthread_mutex_lock(&_serialize_lock);
  std::cout << "[info] " << msg << "\r";
  if (!omitLineFeed) {
    std::cout << "\n";
  }
  std::cout << std::flush;
  pthread_mutex_unlock(&_serialize_lock);
}

void
Log::debug(const std::string msg)
{
  pthread_mutex_lock(&_serialize_lock);
  std::cout << "[debug] " << msg << "\r\n" << std::flush;
  pthread_mutex_unlock(&_serialize_lock);
}

Log&
Log::endl()
{
  *_out << "[" << _label << "]" << _buffer.str();
  // _buffer = std::stringstream();
  if (_abort) {
    exit(EXIT_FAILURE);
  }
  return *this;
}

Log::Log(const char *label, std::ostream *out, const bool abort)
{
  _label = label;
  _out = out;
  _abort = abort;
  pthread_mutex_init(&_serialize_lock, 0);
  // _buffer = std::stringstream();
}

Log::~Log()
{
  pthread_mutex_destroy(&_serialize_lock);
  _out = 0;
  _abort = false;
  // _buffer = std::stringstream();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
