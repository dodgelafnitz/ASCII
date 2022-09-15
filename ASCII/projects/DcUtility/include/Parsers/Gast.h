/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_PARSERS_GAST_H
#define DCUTILITY_PARSERS_GAST_H

#include <memory>
#include <string>
#include <vector>

/*
  whitespace        := [(' ' | '\t' | '\r' | '\n' | '\f')...]

  definition_header := identifier whitespace ':='

  identifier        := "[a-zA-Z_][a-zA-Z_0-9]*"
  string_regex      := "\"([^\"]|(\\\"))*\""
  string_literal    := "'([^\"]|(\\\"))*'"

  repeating         := expression whitespace '...'
  grouping          := '(' whitespace expression whitespace ')'
  optional          := '[' whitespace expression whitespace ']'
  sequence          := expression (whitespace expression)...
  options           := expression (whitespace '|' whitespace expression)...

  value             := identifier | string_regex | string_literal
  operation         := repeating | grouping | optional | sequence | options

  expression        := value | operation

  definition        := definition_header whitespace expression
*/

/*

  A ruleset for building abstract syntax trees that is itself built through a string.
  The ruleset used to generate the abstract syntax tree for the ruleset passed in is
  equivalent to the one in the comment above.

  A couple of notes:
  - Currently all rules are processed from left to right.
  - The rules are processed in order: if possible at any stage, ones appearing earlier
    will be processed before ones appearing later.
  - String literals are not stored as children.
  - String regexes are stored as children.
  - Nodes that cannot have any children are not stored as children.
  - Nodes that are entirely optional are not stored as children.
  - It is possible for nodes to have multiple types.
  - A node that can only ever have exactly one child and only consumes exactly one node
    will always be the same node, just with an additional type.
  - A node's content will encompass the content of its children. Non-child node
    consumptions and children without content will be ignored.
  - Nodes with multiple types have the types ordered from highest priority (first) to
    lowest (last).
  - Nodes that match empty strings are optional.
  - Nodes are optional if all of their children are optional.
  - This process is greedy, and will always try to consume as many nodes as possible
  - when building parent nodes.

*/

class GastRuleset {
public:
  struct GastError {
    GastError(void) = default;

    GastError(std::string const & message, int offset) :
      message(message),
      offset(offset)
    {}

    operator bool(void) const {
      return !message.empty();
    }

    std::string message;
    int         offset  = -1;
  };

  struct Node {
    std::vector<std::string> types;
    std::string_view         content;
    std::vector<Node>        children;
  };

  GastRuleset(void) = default;

  GastError BuildRules(std::string const & rules);

  std::vector<Node> ApplyRules(std::string const & content, GastError & o_error);

private:
  struct Rules;

  std::shared_ptr<Rules> m_impl;
};

#endif // DCUTILITY_PARSERS_GAST_H
