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

#include <stdlib.h>
#include <implicit-curve-tokenizer.hh>
#include <log.hh>

Implicit_curve_tokenizer::Internal_error::Internal_error(const std::string msg)
  : _msg(msg)
{
}

Implicit_curve_tokenizer::Internal_error::~Internal_error()
{
}

const char *
Implicit_curve_tokenizer::Internal_error::what() const throw()
{
  return _msg.c_str();
};

Implicit_curve_tokenizer::Lexical_error::Lexical_error(const std::string msg)
  : _msg(msg)
{
}

Implicit_curve_tokenizer::Lexical_error::~Lexical_error()
{
}

const char *
Implicit_curve_tokenizer::Lexical_error::what() const throw()
{
  return _msg.c_str();
};

Implicit_curve_tokenizer::State::State(const std::string name,
                                       const Type type) :
  _name(name),
  _type(type)
{
}

Implicit_curve_tokenizer::State::~State()
{
}

const Implicit_curve_tokenizer::Internal_error
Implicit_curve_tokenizer::UNEXPECTED_EOF =
  Internal_error("unexpected EOF");

const Implicit_curve_tokenizer::Internal_error
Implicit_curve_tokenizer::MISSING_TRANSITION_DEFAULT =
  Internal_error("missing transition default");

/*
const bool
Implicit_curve_tokenizer::State::operator==(const State other) const
{
  return hash() == other.hash();
}

const std::size_t
Implicit_curve_tokenizer::State::hash() const
{
  return (size_t)this;
}
*/

const std::string
Implicit_curve_tokenizer::State::name() const
{
  return _name;
}

const bool
Implicit_curve_tokenizer::State::is_start() const
{
  return _type == START;
}

const bool
Implicit_curve_tokenizer::State::is_pending() const
{
  return _type == PENDING;
}

const bool
Implicit_curve_tokenizer::State::requires_stop() const
{
  return
    (_type == SUCCESS) ||
    (_type == FAILURE);
}

const bool
Implicit_curve_tokenizer::State::is_sufficient_for_stop() const
{
  return
    (_type == SUCCESS) ||
    (_type == SUCCESS_OR_PENDING) ||
    (_type == FAILURE);
}

const bool
Implicit_curve_tokenizer::State::is_failure() const
{
  return _type == FAILURE;
}

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Start =
  new Constant_symbol_state("start parsing", START,
                            &Implicit_curve_parser_token::None);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Success_parsing_add =
  new Constant_symbol_state("parsed '+'", SUCCESS,
                            &Implicit_curve_parser_token::Add);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Success_parsing_minus =
  new Constant_symbol_state("parsed '-'", SUCCESS,
                            &Implicit_curve_parser_token::Minus);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Success_parsing_mul =
  new Constant_symbol_state("parsed '*'", SUCCESS,
                            &Implicit_curve_parser_token::Mul);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Success_parsing_x =
  new Constant_symbol_state("parsed 'x'", SUCCESS,
                            &Implicit_curve_parser_token::Var_x);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Success_parsing_y =
  new Constant_symbol_state("parsed 'y'", SUCCESS,
                            &Implicit_curve_parser_token::Var_y);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Pending_parsing_int_part =
  new Double_symbol_state("parsing double int part", SUCCESS_OR_PENDING);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Pending_parsing_fract_part_start =
  new Double_symbol_state("parsing double fract part (first digit)", SUCCESS_OR_PENDING);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Pending_parsing_fract_part =
  new Double_symbol_state("parsing double fract part", SUCCESS_OR_PENDING);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Success_parsing_double =
  new Double_symbol_state("parsed double", SUCCESS);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Failure_sign_or_var_or_digit_expected =
  new Constant_symbol_state("'+', '-', 'x', 'y' or '0'..'9' expected", FAILURE,
                            &Implicit_curve_parser_token::None);

const Implicit_curve_tokenizer::State *
Implicit_curve_tokenizer::State::Failure_digit_expected =
  new Constant_symbol_state("'0'..'9' expected", FAILURE,
                            &Implicit_curve_parser_token::None);

/*
std::size_t
Implicit_curve_tokenizer::StateHash::operator()(State const& state) const
{
  //return std::hash<std::shared_ptr<State>>(state);
  return state.hash();
}
*/

std::unordered_map<const Implicit_curve_tokenizer::State *, const std::unordered_map<char, const Implicit_curve_tokenizer::Transition_tail>/*, const Implicit_curve_tokenizer::StateHash*/>
Implicit_curve_tokenizer::TRANSITIONS = {
  {State::Start,
   {
     {'+', {true, State::Success_parsing_add}},
     {'-', {true, State::Success_parsing_minus}},
     {'*', {true, State::Success_parsing_mul}},
     {'x', {true, State::Success_parsing_x}},
     {'y', {true, State::Success_parsing_y}},
     {'0', {true, State::Pending_parsing_int_part}},
     {'1', {true, State::Pending_parsing_int_part}},
     {'2', {true, State::Pending_parsing_int_part}},
     {'3', {true, State::Pending_parsing_int_part}},
     {'4', {true, State::Pending_parsing_int_part}},
     {'5', {true, State::Pending_parsing_int_part}},
     {'6', {true, State::Pending_parsing_int_part}},
     {'7', {true, State::Pending_parsing_int_part}},
     {'8', {true, State::Pending_parsing_int_part}},
     {'9', {true, State::Pending_parsing_int_part}},
     {'\000', {false, State::Failure_sign_or_var_or_digit_expected}}
   }
  },
  {State::Pending_parsing_int_part,
   {
     {'0', {true, State::Pending_parsing_int_part}},
     {'1', {true, State::Pending_parsing_int_part}},
     {'2', {true, State::Pending_parsing_int_part}},
     {'3', {true, State::Pending_parsing_int_part}},
     {'4', {true, State::Pending_parsing_int_part}},
     {'5', {true, State::Pending_parsing_int_part}},
     {'6', {true, State::Pending_parsing_int_part}},
     {'7', {true, State::Pending_parsing_int_part}},
     {'8', {true, State::Pending_parsing_int_part}},
     {'9', {true, State::Pending_parsing_int_part}},
     {'.', {true, State::Pending_parsing_fract_part}},
     {'\000', {false, State::Success_parsing_double}}
   }
  },
  {State::Pending_parsing_fract_part_start,
   {
     {'0', {true, State::Pending_parsing_fract_part}},
     {'1', {true, State::Pending_parsing_fract_part}},
     {'2', {true, State::Pending_parsing_fract_part}},
     {'3', {true, State::Pending_parsing_fract_part}},
     {'4', {true, State::Pending_parsing_fract_part}},
     {'5', {true, State::Pending_parsing_fract_part}},
     {'6', {true, State::Pending_parsing_fract_part}},
     {'7', {true, State::Pending_parsing_fract_part}},
     {'8', {true, State::Pending_parsing_fract_part}},
     {'9', {true, State::Pending_parsing_fract_part}},
     {'\000', {false, State::Failure_digit_expected}}
   }
  },
  {State::Pending_parsing_fract_part,
   {
     {'0', {true, State::Pending_parsing_fract_part}},
     {'1', {true, State::Pending_parsing_fract_part}},
     {'2', {true, State::Pending_parsing_fract_part}},
     {'3', {true, State::Pending_parsing_fract_part}},
     {'4', {true, State::Pending_parsing_fract_part}},
     {'5', {true, State::Pending_parsing_fract_part}},
     {'6', {true, State::Pending_parsing_fract_part}},
     {'7', {true, State::Pending_parsing_fract_part}},
     {'8', {true, State::Pending_parsing_fract_part}},
     {'9', {true, State::Pending_parsing_fract_part}},
     {'\000', {false, State::Success_parsing_double}}
   }
  }
};

Implicit_curve_tokenizer::Constant_symbol_state::
Constant_symbol_state(const std::string name,
                      const Type type,
                      const Implicit_curve_parser_token *constant_symbol) :
  State(name, type),
  _constant_symbol(constant_symbol)
{
}

Implicit_curve_tokenizer::Constant_symbol_state::~Constant_symbol_state()
{
}

const Implicit_curve_parser_token *
Implicit_curve_tokenizer::Constant_symbol_state::
create_token(const std::string token_value) const
{
  return _constant_symbol;
}

Implicit_curve_tokenizer::Double_symbol_state::
Double_symbol_state(const std::string name,
                    const Type type) :
  State(name, type)
{
}

Implicit_curve_tokenizer::Double_symbol_state::~Double_symbol_state()
{
}

const Implicit_curve_parser_token *
Implicit_curve_tokenizer::Double_symbol_state::
create_token(const std::string token_value) const
{
  return Implicit_curve_parser_token::create_double_token(token_value);
}

Implicit_curve_tokenizer::Implicit_curve_tokenizer(const std::string expression)
{
  _expression = new std::string(expression);
  if (!_expression) {
    Log::fatal("not enough memory");
  }
  _token_start_pos = 0;
  _current_pos = 0;
}

Implicit_curve_tokenizer::~Implicit_curve_tokenizer()
{
  if (!_saved_positions.empty()) {
    Log::error("programming error: saved positions left upon destruction of tokenizer");
  }
  _token_start_pos = 0;
  _current_pos = 0;
  delete _expression;
  _expression = 0;
}

const bool
Implicit_curve_tokenizer::eof() const
{
  return _current_pos >= _expression->length();
}

void
Implicit_curve_tokenizer::save_position()
{
  _saved_positions.push(_current_pos);
}

// TODO: Performance issue: Cache the least, say 5-10 (assuming a
// grammar with left look-ahead < 10), recently scanned token (e.g. in
// a hashtable as pairs (_current_pos, token)), such that after
// calling drop_saved_position() or reset_to_saved_position(),
// recently scanned tokens do not need to be scanned again.

void
Implicit_curve_tokenizer::drop_saved_position()
{
  if (_saved_positions.empty()) {
    Log::fatal("tokenizer: no position left to reset");
  }
  _saved_positions.pop();
}

void
Implicit_curve_tokenizer::reset_to_saved_position()
{
  if (_saved_positions.empty()) {
    Log::fatal("tokenizer: no position left to reset");
  }
  _current_pos = _saved_positions.top();
  _saved_positions.pop();
}

const int
Implicit_curve_tokenizer::get_display_position() const
{
  return _current_pos + 1; // humans start counting 1-based, not 0-based
}

const bool
Implicit_curve_tokenizer::is_white_space_char(const char c)
{
  switch (c) {
  case ' ':
  case '\n':
  case '\r':
  case '\t':
    return true;
  default:
    return false;
  }
}

void
Implicit_curve_tokenizer::mark_token_start()
{
  _token_start_pos = _current_pos;
}

const char
Implicit_curve_tokenizer::look_ahead() const
{
  if (eof()) {
    throw UNEXPECTED_EOF;
  }
  return _expression->at(_current_pos);
}

void
Implicit_curve_tokenizer::next_char()
{
  if (eof()) {
    throw UNEXPECTED_EOF;
  }
  _current_pos++;
}

std::string
Implicit_curve_tokenizer::get_token_value()
{
  const size_t count = _current_pos - _token_start_pos;
  return
    _expression->substr(_token_start_pos, count);
}

void
Implicit_curve_tokenizer::consume_white_space()
{
  while (!eof() && is_white_space_char(look_ahead())) {
    next_char();
  }
}

const bool
Implicit_curve_tokenizer::try_accept_character(const State *state_in,
                                               const State **state_out,
                                               const char look_ahead)
{
  const std::unordered_map<char, const Implicit_curve_tokenizer::Transition_tail>
    state_transition = TRANSITIONS[state_in];
  const Transition_tail *transition_tail;
  // std::unordered_map<std::string,double>::const_iterator got
  auto search = state_transition.find(look_ahead);
  if (!(search == state_transition.end())) {
    transition_tail = &search->second;
  } else {
    auto search_default = state_transition.find('\000');
    if (!(search_default == state_transition.end())) {
      transition_tail = &search_default->second;
    } else {
      throw MISSING_TRANSITION_DEFAULT;
    }
  }
  const State *state = transition_tail->state_out;
  *state_out = state;
  return transition_tail->consume_character;
}

const Implicit_curve_parser_token *
Implicit_curve_tokenizer::get_next_token()
{
  consume_white_space();
  if (eof()) {
    return &Implicit_curve_parser_token::Eof;
  }
  mark_token_start();
  const State *state = State::Start;
  std::string value;
  while (!state->requires_stop() && !eof()) {
    const bool accepted = try_accept_character(state, &state, look_ahead());
    if (accepted) {
      next_char();
    }
  }
  if (!state->is_sufficient_for_stop()) {
    std::stringstream msg;
    msg << "lexical error in expression '" << *_expression <<
      "': premature end at position " << (_current_pos + 1);
    msg << " (last state: " << state->name() << ")";
    Log::fatal(msg.str());
  }
  if (state->is_failure()) {
    std::stringstream msg;
    msg << "lexical error in expression '" << *_expression <<
      "': " << state->name() << " at position " << (_current_pos + 1);
    Log::fatal(msg.str());
  }
  const std::string token_value = get_token_value();
  return state->create_token(token_value);
  /*
  if (state == State::Success_parsing_add) {
    return Implicit_curve_parser_token::Add;
  } else if (state == State::Success_parsing_minus) {
    return Implicit_curve_parser_token::Minus;
  } else if (state == State::Success_parsing_mul) {
    return Implicit_curve_parser_token::Mul;
  } else if (state == State::Success_parsing_x) {
    return Implicit_curve_parser_token::Var_x;
  } else if (state == State::Success_parsing_y) {
    return Implicit_curve_parser_token::Var_y;
  } else if (state == State::Success_parsing_double) {
    return Implicit_curve_parser_token::create_double_token(token_value);
  } else {
    return Implicit_curve_parser_token::None;
  }
  */
}

void
Implicit_curve_tokenizer::release_token(const Implicit_curve_parser_token *token) const
{
  if (token->get_type() == Implicit_curve_parser_token::DOUBLE) {
    delete token;
  } else {
    // this is not a dynamically allocated token => nothing to release
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
