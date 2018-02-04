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

#ifndef XML_STRING_HH
#define XML_STRING_HH

#include <xercesc/dom/DOM.hpp>
#include <log.hh>

class Xml_string
{
public:
  //Xml_string();
  Xml_string(const Xml_string& other);
  Xml_string(const XMLCh *char_array);
  Xml_string(const XMLCh *char_array, const std::size_t hash);
  virtual ~Xml_string();
  const XMLSize_t length() const;
  //Xml_string& operator=(const Xml_string& other);
  bool operator <(const Xml_string& other) const;
  bool operator ==(const Xml_string& other) const;
  std::size_t hash() const;
  char *transcode() const;
  static void release(char **transcoded);
  static bool equals(const Xml_string * const & a,
                     const Xml_string * const & b);

  struct hashing_functor
  {
    unsigned long operator()(const Xml_string * const & key) const
    {
      return key->hash();
    }
  };

  struct equal_functor
  {
    bool operator()(const Xml_string * const & t1,
                    const Xml_string * const & t2) const
    {
      return Xml_string::equals(t1, t2);
    }
  };

private:
  const XMLCh *_char_array;
  const std::size_t _hash;
  static const XMLCh *replicate_char_array(const XMLCh *char_array);
  static const std::size_t compute_hash(const XMLCh *char_array);
  //Xml_string& copy(const Xml_string &other);
};

struct Xml_string_star_hash
{
  std::size_t operator()(const Xml_string * const &xml_string) const
  {
    return xml_string->hash();
  }
};
  
namespace std {
  template <>
  struct hash<Xml_string>
  {
    std::size_t operator()(const Xml_string& k) const
    {
      Log::debug("XXXXX");
      char *transcoded = k.transcode();
      std::size_t result = hash<char *>()(transcoded);
      k.release(&transcoded);
      return result;
    }
  };
}

/*
namespace std {
  template <>
  struct hash<Xml_string>
  {
    std::size_t operator()(const Xml_string& k) const
    {
      char *transcoded = k.transcode();
      std::size_t result = hash<char *>()(transcoded);
      k.release(&transcoded);
      return result;
    }
  };
}
*/

#endif /* XML_STRING_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
