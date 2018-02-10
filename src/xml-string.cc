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

#include <xml-string.hh>
#include <log.hh>

/*
Xml_string::Xml_string() : Xml_string(0)
{
  Log::warn("creating empty Xml_string");
}
*/

Xml_string::Xml_string(const Xml_string& other) :
  Xml_string(other._char_array, other._hash)
{
}

Xml_string::Xml_string(const XMLCh *char_array) :
  _char_array(replicate_char_array(char_array)),
  _hash(compute_hash(char_array))
{
}

Xml_string::Xml_string(const XMLCh *char_array, const std::size_t hash) :
  _char_array(replicate_char_array(char_array)),
  _hash(hash)
{
}

Xml_string::~Xml_string()
{
  XMLCh *no_const_char_array = (XMLCh *)_char_array;
  xercesc::XMLString::release(&no_const_char_array);
}

const XMLSize_t
Xml_string::length() const
{
  return xercesc::XMLString::stringLen(_char_array);
}

const XMLCh *
Xml_string::replicate_char_array(const XMLCh *char_array)
{
  /*
  if (!char_array) {
    return 0;
  }
  */
  const XMLCh *copy_of_char_array =
    xercesc::XMLString::replicate(char_array);
  if (!copy_of_char_array) {
    Log::fatal("not enough memory");
  }
  return copy_of_char_array;
}

const std::size_t
Xml_string::compute_hash(const XMLCh *char_array)
{
  if (!char_array) {
    Log::fatal("unexpected null char_array");
  }
  char *char_array_as_c_star =
    xercesc::XMLString::transcode(char_array);
  const std::string str_char_array(char_array_as_c_star);
  const std::size_t hash = std::hash<std::string>{}(str_char_array);
  release(&char_array_as_c_star);
  return hash;
}

bool
Xml_string::equals(const Xml_string * const & a,
                   const Xml_string * const & b)
{
  if (!a && !b) {
    return true;
  } else if (!a || !b) {
    return false;
  } else {
    return !(*a < *b) && !(*b < *a);
  }
}

/*
Xml_string&
Xml_string::copy(const Xml_string &other)
{
  (void) other;
  return *this;
}
*/

/*
Xml_string&
Xml_string::operator=(const Xml_string& other)
{
  return copy(other);
}
*/

bool
Xml_string::operator<(const Xml_string& other) const
{
  const XMLSize_t this_len = xercesc::XMLString::stringLen(_char_array);
  const XMLSize_t other_len = xercesc::XMLString::stringLen(other._char_array);
  const XMLSize_t min_len = this_len <= other_len ? this_len : other_len;
  bool done = false;
  bool result;
  for (XMLSize_t i = 0; i < min_len; i++) {
    XMLCh this_ch = _char_array[i];
    XMLCh other_ch = other._char_array[i];
    if (this_ch < other_ch) {
      result = true;
      done = true;
      break;
    } else if (this_ch > other_ch) {
      result = false;
      done = true;
      break;
    }
    if (done) break;
  }
  if (!done) {
    if (this_len < other_len) {
      result = true;
    } else if (this_len > other_len) {
      result = false;
    } else {
      result = false;
    }
  }
  return result;
}

bool
Xml_string::operator==(const Xml_string& other) const
{
  const XMLSize_t this_len = xercesc::XMLString::stringLen(_char_array);
  const XMLSize_t other_len = xercesc::XMLString::stringLen(other._char_array);
  bool result;
  if (this_len == other_len) {
    result = true;
    const XMLSize_t min_len = this_len <= other_len ? this_len : other_len;
    for (XMLSize_t i = 0; i < min_len; i++) {
      XMLCh this_ch = _char_array[i];
      XMLCh other_ch = _char_array[i];
      if (this_ch != other_ch) {
        result = false;
        break;
      }
    }
  } else {
    result = false;
  }
  return result;
}

std::size_t
Xml_string::hash() const
{
  return _hash;
}

char *
Xml_string::transcode() const
{
  char *transcoded = xercesc::XMLString::transcode(_char_array);
  return transcoded;
}

void
Xml_string::release(char **transcoded)
{
  xercesc::XMLString::release(transcoded);
  *transcoded = 0;
}

const std::string
Xml_string::to_string() const
{
  std::stringstream str;
  char *transcoded = transcode();
  str << transcoded;
  release(&transcoded);
  return std::string(str.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
