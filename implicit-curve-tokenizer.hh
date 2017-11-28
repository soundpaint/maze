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

#ifndef IMPLICIT_CURVE_TOKENIZER_HH
#define IMPLICIT_CURVE_TOKENIZER_HH

#include <string>
#include <exception>
#include <unordered_map>
#include <implicit-curve-parser-token.hh>

class Implicit_curve_tokenizer
{
public:
  class Internal_error : public std::exception
  {
  public:
    Internal_error(const std::string msg);
    virtual ~Internal_error();
    virtual const char *what() const throw();
  private:
    const std::string _msg;
  };

  static const Internal_error UNEXPECTED_EOF;
  static const Internal_error MISSING_TRANSITION_DEFAULT;

  class Lexical_error : public std::exception
  {
  public:
    Lexical_error(const std::string msg);
    virtual ~Lexical_error();
    virtual const char *what() const throw();
  private:
    const std::string _msg;
  };

  Implicit_curve_tokenizer(const std::string expression);
  virtual ~Implicit_curve_tokenizer();
  const bool eof() const;
  const int get_display_position() const;
  const Implicit_curve_parser_token *get_next_token();
  void release_token(const Implicit_curve_parser_token *token) const;

private:
  class State
  {
  public:
    enum Type {
      START, PENDING, SUCCESS_OR_PENDING, SUCCESS, FAILURE
    };
    virtual const Implicit_curve_parser_token *
    create_token(const std::string token_value) const = 0;
    /*
    const bool operator==(const State other) const;
    const std::size_t hash() const;
    */
    const std::string name() const;
    const bool is_start() const;
    const bool is_pending() const;
    const bool requires_stop() const;
    const bool is_sufficient_for_stop() const;
    const bool is_failure() const;
    static const State *Start;
    static const State *Success_parsing_add;
    static const State *Success_parsing_minus;
    static const State *Success_parsing_mul;
    static const State *Success_parsing_x;
    static const State *Success_parsing_y;
    static const State *Pending_parsing_int_part;
    static const State *Pending_parsing_fract_part_start;
    static const State *Pending_parsing_fract_part;
    static const State *Success_parsing_double;
    static const State *Failure_sign_or_var_or_digit_expected;
    static const State *Failure_digit_expected;
  protected:
    State(const std::string name, const Type type);
    virtual ~State();
  private:
    const std::string _name;
    const Type _type;
  };

  class Constant_symbol_state : public State
  {
  public:
    virtual const Implicit_curve_parser_token *
    create_token(const std::string token_value) const;
    Constant_symbol_state(const std::string name,
                          const Type type,
                          const Implicit_curve_parser_token *constant_symbol);
    virtual ~Constant_symbol_state();
  private:
    const Implicit_curve_parser_token *_constant_symbol;
  };

  class Double_symbol_state : public State
  {
  public:
    virtual const Implicit_curve_parser_token *
    create_token(const std::string token_value) const;
    Double_symbol_state(const std::string name,
                        const Type type);
    virtual ~Double_symbol_state();
  };

  /*
  struct StateHash
  {
    std::size_t operator()(State const& state) const;
  };
  */

  struct Transition_tail {
    const bool consume_character;
    const State *state_out;
  };

  struct Transition {
    const State *state_in;
    const char character_in;
    const Transition_tail transition_tail;
  };

  static
  std::unordered_map<const State *, const std::unordered_map<char, const Transition_tail>/*, const StateHash*/>
  TRANSITIONS;

  std::string *_expression;
  uint64_t _token_start_pos, _current_pos;
  static const bool is_white_space_char(const char);
  void mark_token_start();
  const char look_ahead() const;
  void next_char();
  std::string get_token_value();
  void consume_white_space();
  static const bool pending_state(const State *state);

  /**
   * @return True if the look-ahead character has been
   * accepted.
   */
  static const bool try_accept_character(const State *state_in,
                                         const State **state_out,
                                         const char look_ahead);
};

#endif /* IMPLICIT_CURVE_TOKENIZER_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
