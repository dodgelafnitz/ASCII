/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include "Parsers/Gast.h"

#include <algorithm>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <variant>

struct GastRuleset::Rules {
  struct RuleNode;

  struct RuleNodeRepeat {
    std::shared_ptr<RuleNode> node;
  };

  struct RuleNodeOptional {
    std::shared_ptr<RuleNode> node;
  };

  struct RuleNodeSequence {
    std::vector<RuleNode> nodes;
  };

  struct RuleNodeOptions {
    std::vector<RuleNode> nodes;
  };

  struct RuleNodeRegex {
    std::string matchStr;
  };

  struct RuleNodeLiteral {
    std::string matchStr;
  };

  struct RuleNodeIdentifier {
    std::string type;
  };

  struct RuleNodeDefinition {
    std::string               type;
    std::shared_ptr<RuleNode> definition;
  };

  struct RuleNode {
    using DataVariant = std::variant<
      RuleNodeRepeat,
      RuleNodeOptional,
      RuleNodeSequence,
      RuleNodeOptions,
      RuleNodeRegex,
      RuleNodeLiteral,
      RuleNodeIdentifier,
      RuleNodeDefinition
    >;

    RuleNode(void) = default;

    template <typename T>
    requires std::constructible_from<DataVariant, T>
      RuleNode(T const & node) :
      data(node)
    {}

    bool IsRepeat(void) const {
      return std::holds_alternative<RuleNodeRepeat>(data);
    }

    bool IsOptional(void) const {
      return std::holds_alternative<RuleNodeOptional>(data);
    }

    bool IsSequence(void) const {
      return std::holds_alternative<RuleNodeSequence>(data);
    }

    bool IsOptions(void) const {
      return std::holds_alternative<RuleNodeOptions>(data);
    }

    bool IsRegex(void) const {
      return std::holds_alternative<RuleNodeRegex>(data);
    }

    bool IsLiteral(void) const {
      return std::holds_alternative<RuleNodeLiteral>(data);
    }

    bool IsIdentifier(void) const {
      return std::holds_alternative<RuleNodeIdentifier>(data);
    }

    bool IsDefinition(void) const {
      return std::holds_alternative<RuleNodeDefinition>(data);
    }

    RuleNodeRepeat const & GetRepeat(void) const {
      return std::get<RuleNodeRepeat>(data);
    }

    RuleNodeOptional const & GetOptional(void) const {
      return std::get<RuleNodeOptional>(data);
    }

    RuleNodeSequence const & GetSequence(void) const {
      return std::get<RuleNodeSequence>(data);
    }

    RuleNodeOptions const & GetOptions(void) const {
      return std::get<RuleNodeOptions>(data);
    }

    RuleNodeRegex const & GetRegex(void) const {
      return std::get<RuleNodeRegex>(data);
    }

    RuleNodeLiteral const & GetLiteral(void) const {
      return std::get<RuleNodeLiteral>(data);
    }

    RuleNodeIdentifier const & GetIdentifier(void) const {
      return std::get<RuleNodeIdentifier>(data);
    }

    RuleNodeDefinition const & GetDefinition(void) const {
      return std::get<RuleNodeDefinition>(data);
    }

    DataVariant data;
  };

  enum class TokenType {
    Literal,
    Regex,
  };

  struct RuleToken {
    RuleToken(TokenType type, std::string const & content) :
      type(type),
      content(content)
    {}

    bool operator ==(RuleToken const & token) const = default;

    std::string ToString(void) const {
      char typeChar = '.';

      switch (type) {
        case TokenType::Literal: {
          typeChar = '\'';
        } break;
        case TokenType::Regex: {
          typeChar = '"';
        } break;
      }

      return typeChar + content + typeChar;
    }

    TokenType   type;
    std::string content;
  };

  struct IdentifierInfoEntry {
    bool isOptional;
    bool continuable;
    bool consumesOnlyOne;
    int  minConsumed;

    std::vector<std::string> dependants;
    std::vector<std::string> dependantForms;
  };

  struct IdentifierInfo {
    bool IsOptional(std::string const & type) const {
      if (data.contains(type)) {
        return data.at(type).isOptional;
      }
      return false;
    }

    bool Continuable(std::string const & type) const {
      if (data.contains(type)) {
        return data.at(type).continuable;
      }
      return false;
    }

    bool ConsumesOnlyOne(std::string const & type) const {
      if (data.contains(type)) {
        return data.at(type).consumesOnlyOne;
      }
      return std::numeric_limits<int>::max();
    }

    int MinConsumed(std::string const & type) const {
      if (data.contains(type)) {
        return data.at(type).minConsumed;
      }
      return 0;
    }

    std::vector<std::string> const * GetDependants(std::string const & type) const {
      if (data.contains(type)) {
        return &data.at(type).dependants;
      }
      return nullptr;
    }

    std::vector<std::string> const * GetDependantForms(std::string const & type) const {
      if (data.contains(type)) {
        return &data.at(type).dependantForms;
      }
      return nullptr;
    }

    std::map<std::string, IdentifierInfoEntry> data;
  };

  static Rules s_baseRules;

  static RuleNode Repeat(RuleNode const & node) {
    return RuleNodeRepeat(std::make_shared<RuleNode>(node));
  }

  static RuleNode Optional(RuleNode const & node) {
    return RuleNodeOptional(std::make_shared<RuleNode>(node));
  }

  static RuleNode Sequence(std::vector<RuleNode> const & nodes) {
    return RuleNodeSequence(nodes);
  }

  static RuleNode Options(std::vector<RuleNode> const & nodes) {
    return RuleNodeOptions(nodes);
  }

  static RuleNode Regex(std::string const & matchStr) {
    return RuleNodeRegex(matchStr);
  }

  static RuleNode Literal(std::string const & matchStr) {
    return RuleNodeLiteral(matchStr);
  }

  static RuleNode Identifier(std::string const & type) {
    return RuleNodeIdentifier(type);
  }

  static RuleNodeDefinition DefinitionRule(std::string const & type, RuleNode const & node) {
    return RuleNodeDefinition(type, std::make_shared<RuleNode>(node));
  }

  static RuleNode Definition(std::string const & type, RuleNode const & node) {
    return DefinitionRule(type, node);
  }

  static RuleToken LiteralToken(std::string const & content) {
    return RuleToken(TokenType::Literal, content);
  }

  static RuleToken RegexToken(std::string const & content) {
    return RuleToken(TokenType::Regex, content);
  }

  static Rules FromString(std::string const & input, GastError & o_errorMessage) {
    Rules result;

    std::vector<Node> const resultNodes = s_baseRules.Apply(input, o_errorMessage);

    if (o_errorMessage) {
      return result;
    }

    result.BuildRuleNodes(resultNodes);
    result.BuildRuleTokens(resultNodes);
    result.BuildInfo();

    return result;
  };

  static RuleNode GetRuleNodeFromNode(Node const & node) {
    RuleNode result;

    std::string const & type = node.types.front();

    if (DoesNodeHaveType(node, "identifier")) {
      std::string const identifier = std::string(node.content);
      result = Identifier(identifier);
    }
    else if (DoesNodeHaveType(node, "string_regex")) {
      std::string const regex = std::string(node.content).substr(1, node.content.size() - 2);
      result = Regex(regex);
    }
    else if (DoesNodeHaveType(node, "string_literal")) {
      std::string const literal = std::string(node.content).substr(1, node.content.size() - 2);
      result = Literal(literal);
    }
    else if (DoesNodeHaveType(node, "repeating")) {
      RuleNode const repeatingExpression = GetRuleNodeFromNode(node.children.front());
      result = Repeat(repeatingExpression);
    }
    else if (DoesNodeHaveType(node, "grouping")) {
      RuleNode const expression = GetRuleNodeFromNode(node.children.front());
      result = expression;
    }
    else if (DoesNodeHaveType(node, "optional")) {
      RuleNode const optionalExpression = GetRuleNodeFromNode(node.children.front());
      result = Optional(optionalExpression);
    }
    else if (DoesNodeHaveType(node, "sequence")) {
      std::vector<RuleNode> const expressions = GetRulesFromNodes(node.children);
      result = Sequence(expressions);
    }
    else if (DoesNodeHaveType(node, "options")) {
      std::vector<RuleNode> const options = GetRulesFromNodes(node.children);
      result = Options(options);
    }
    else if (DoesNodeHaveType(node, "definition")) {
      std::string const name       = std::string(node.children[0].content);
      RuleNode const    expression = GetRuleNodeFromNode(node.children[1]);
      result = Definition(name, expression);
    }

    return result;
  }

  static std::vector<RuleNode> GetRulesFromNodes(std::vector<Node> const & nodes) {
    std::vector<RuleNode> result;
    result.reserve(nodes.size());

    for (Node const & node : nodes) {
      result.emplace_back(GetRuleNodeFromNode(node));
    }

    return result;
  }

  static std::vector<RuleNodeDefinition> GetDefinitionsFromNodes(std::vector<Node> const & nodes) {
    std::vector<RuleNodeDefinition> result;

    std::vector<RuleNode> rules = GetRulesFromNodes(nodes);
    for (RuleNode const & rule : rules) {
      if (rule.IsDefinition()) {
        result.emplace_back(rule.GetDefinition());
      }
    }

    return result;
  }

  static void AddTokenIfUnique(RuleToken const & token, std::vector<RuleToken> & io_tokens) {
    for (RuleToken const & currentToken : io_tokens) {
      if (currentToken == token) {
        return;
      }
    }

    io_tokens.emplace_back(token);
  }

  static void GetTokensFromNode(Node const & node, std::vector<RuleToken> & io_tokens) {
    std::string const & type = node.types.front();

    if (DoesNodeHaveType(node, "string_regex")) {
      std::string const regex = std::string(node.content).substr(1, node.content.size() - 2);
      AddTokenIfUnique(RegexToken(regex), io_tokens);
    }
    else if (DoesNodeHaveType(node, "string_literal")) {
      std::string const literal = std::string(node.content).substr(1, node.content.size() - 2);
      AddTokenIfUnique(LiteralToken(literal), io_tokens);
    }

    GetTokensFromNodes(node.children, io_tokens);
  }

  static void GetTokensFromNodes(std::vector<Node> const & nodes, std::vector<RuleToken> & io_tokens) {
    for (Node const & node : nodes) {
      GetTokensFromNode(node, io_tokens);
    }
  }

  static bool TryMatchToken(std::string const & input, int currentPos, RuleToken const & token, int & o_matchLength) {
    o_matchLength = 0;

    switch (token.type) {
      case TokenType::Literal: {
        for (int i = 0; i < token.content.length(); ++i) {
          if (input[currentPos + i] != token.content[i]) {
            return false;
          }
        }
        o_matchLength = token.content.length();
        return true;
      } break;

      case TokenType::Regex: {
        std::regex matchExpr(token.content, std::regex_constants::nosubs | std::regex_constants::optimize);

        std::cmatch regexMatch;
        std::regex_search(input.c_str() + currentPos, regexMatch, matchExpr, std::regex_constants::match_continuous);

        if (regexMatch.empty()) {
          return false;
        }

        o_matchLength = regexMatch[0].length();
        return true;
      } break;

      default: {
        return false;
      }
    }
  }

  static bool DoesNodeHaveType(Node const & node, std::string const & type) {
    for (std::string const & nodeType : node.types) {
      if (type == nodeType) {
        return true;
      }
    }
    return false;
  }

  static std::string_view GetContentFromNodes(std::vector<Node> const & nodes) {
    char const * contentBegin = nullptr;
    for (Node const & node : nodes) {
      if (!node.content.empty()) {
        contentBegin = node.content.data();
        break;
      }
    }

    char const * contentEnd = nullptr;
    for (auto i = nodes.rbegin(); i != nodes.rend(); ++i) {
      Node const & node = *i;

      if (!node.content.empty()) {
        contentEnd = node.content.data() + node.content.length();
        break;
      }
    }

    if (contentBegin && contentEnd) {
      return std::string_view(contentBegin, contentEnd);
    }

    return std::string_view();
  }

  static bool TryBuildChildrenForRule(IdentifierInfo const & info, RuleNode const & rule, std::vector<Node> const & nodes, int & io_offset, std::vector<Node> & o_children) {
    if (io_offset >= nodes.size()) {
      return false;
    }

    if (rule.IsRepeat()) {
      bool              atLeastOnce = false;
      int               current     = io_offset;
      std::vector<Node> children;
      while (TryBuildChildrenForRule(info, *rule.GetRepeat().node, nodes, current, children)) {
        o_children.insert(o_children.end(), children.begin(), children.end());
        children.clear();

        atLeastOnce = true;
      }

      if (atLeastOnce) {
        io_offset = current;

        return true;
      }
      return false;
    }
    else if (rule.IsOptional()) {
      TryBuildChildrenForRule(info, *rule.GetOptional().node, nodes, io_offset, o_children);

      return true;
    }
    else if (rule.IsSequence()) {
      int               current  = io_offset;
      std::vector<Node> children;
      for (RuleNode const & childRule : rule.GetSequence().nodes) {
        if (!TryBuildChildrenForRule(info, childRule, nodes, current, children)) {
          return false;
        }
      }

      io_offset  = current;
      o_children = children;
      return true;
    }
    else if (rule.IsOptions()) {
      for (RuleNode const & childRule : rule.GetOptions().nodes) {
        if (TryBuildChildrenForRule(info, childRule, nodes, io_offset, o_children)) {
          return true;
        }
      }
      return false;
    }
    else if (rule.IsRegex()) {
      std::string const type = "\"" + rule.GetRegex().matchStr + "\"";
      if (DoesNodeHaveType(nodes[io_offset], type)) {
        if (!info.IsOptional(type)) {
          o_children.emplace_back(nodes[io_offset]);
        }
        io_offset++;

        return true;
      }
      return false;
    }
    else if (rule.IsLiteral()) {
      if (DoesNodeHaveType(nodes[io_offset], "'" + rule.GetLiteral().matchStr + "'")) {
        io_offset++;
        return true;
      }
      return false;
    }
    else if (rule.IsIdentifier()) {
      if (DoesNodeHaveType(nodes[io_offset], rule.GetIdentifier().type)) {
        if (!info.IsOptional(rule.GetIdentifier().type)) {
          o_children.emplace_back(nodes[io_offset]);
        }
        io_offset++;

        return true;
      }
      else if (info.IsOptional(rule.GetIdentifier().type)) {
        return true;
      }
      return false;
    }
    else if (rule.IsDefinition()) {
      return TryBuildChildrenForRule(info, *rule.GetDefinition().definition, nodes, io_offset, o_children);
    }

    return false;
  }

  static void ReplaceNodes(std::vector<Node> & io_nodes, int offset, Node const & node, int consumedNodes) {
    if (consumedNodes > 1) {
      io_nodes.erase(io_nodes.begin() + offset + 1, io_nodes.begin() + offset + consumedNodes);
    }

    io_nodes[offset] = node;
  }

  static bool IsRuleCompleted(IdentifierInfo const & info, RuleNode const & definition, std::vector<Node> const & nodes, std::set<std::string> const & completedRules, bool optionalCompletes) {
    if (definition.IsIdentifier()) {
      std::string const & type = definition.GetIdentifier().type;

      if (completedRules.contains(type) && !info.IsOptional(type)) {
        for (Node const & node : nodes) {
          if (DoesNodeHaveType(node, type)) {
            if (info.GetDependantForms(type)->empty()) {
              return false;
            }

            for (std::string const & dependantType : *info.GetDependantForms(type)) {
              if (!DoesNodeHaveType(node, dependantType)) {
                return false;
              }
            }
          }
        }

        return true;
      }

      return false;
    }
    else if (definition.IsLiteral()) {
      std::string const type = "'" + definition.GetLiteral().matchStr + "'";

      for (Node const & node : nodes) {
        if (DoesNodeHaveType(node, type)) {
          if (info.GetDependantForms(type)->empty()) {
            return false;
          }

          for (std::string const & dependantType : *info.GetDependantForms(type)) {
            if (!DoesNodeHaveType(node, dependantType)) {
              return false;
            }
          }
        }
      }

      return true;
    }
    else if (definition.IsOptional()) {
      if (optionalCompletes) {
        return IsRuleCompleted(info, *definition.GetOptional().node, nodes, completedRules, optionalCompletes);
      }

      return false;
    }
    else if (definition.IsOptions()) {
      for (RuleNode const & node : definition.GetOptions().nodes) {
        if (!IsRuleCompleted(info, node, nodes, completedRules, optionalCompletes)) {
          return false;
        }
      }

      return true;
    }
    else if (definition.IsRegex()) {
      std::string const type = "\"" + definition.GetRegex().matchStr + "\"";

      for (Node const & node : nodes) {
        if (DoesNodeHaveType(node, type)) {
          if (info.GetDependantForms(type)->empty()) {
            return false;
          }

          for (std::string const & dependantType : *info.GetDependantForms(type)) {
            if (!DoesNodeHaveType(node, dependantType)) {
              return false;
            }
          }
        }
      }

      return true;
    }
    else if (definition.IsRepeat()) {
      return IsRuleCompleted(info, *definition.GetRepeat().node, nodes, completedRules, optionalCompletes);
    }
    else if (definition.IsSequence()) {
      for (RuleNode const & node : definition.GetSequence().nodes) {
        if (IsRuleCompleted(info, node, nodes, completedRules, false)) {
          return true;
        }
      }

      return false;
    }

    return false;
  }

  static bool EvaluateContinuable(IdentifierInfo const & info, std::vector<RuleNodeDefinition> const & rules, std::string const & type) {
    std::vector<std::string> const & dependants = *info.GetDependants(type);

    for (RuleNodeDefinition const & rule : rules) {
      for (std::string const & dependant : dependants) {
        if (rule.type == dependant) {
          return false;
        }
      }

      if (rule.type == type) {
        break;
      }
    }

    return true;
  }

  static int EvaluateMinConsumed(IdentifierInfo const & info, RuleNode const & definition) {
    if (definition.IsIdentifier()) {
      return info.IsOptional(definition.GetIdentifier().type) ? 0 : 1;
    }
    else if (definition.IsLiteral()) {
      return 1;
    }
    else if (definition.IsOptional()) {
      return 0;
    }
    else if (definition.IsOptions()) {
      int min = std::numeric_limits<int>::max();

      for (RuleNode const & node : definition.GetOptions().nodes) {
        min = std::min(min, EvaluateMinConsumed(info, node));
      }

      if (min != std::numeric_limits<int>::max()) {
        return min;
      }
    }
    else if (definition.IsRegex()) {
      return 1;
    }
    else if (definition.IsRepeat()) {
      return EvaluateMinConsumed(info, *definition.GetRepeat().node);
    }
    else if (definition.IsSequence()) {
      int total = 0;

      for (RuleNode const & node : definition.GetSequence().nodes) {
        total += EvaluateMinConsumed(info, node);
      }

      return total;
    }

    return 0;
  }

  static bool EvaluateConsumesOnlyOne(IdentifierInfo const & info, RuleNode const & definition) {
    if (definition.IsIdentifier()) {
      return info.IsOptional(definition.GetIdentifier().type) ? false : true;
    }
    else if (definition.IsLiteral()) {
      return true;
    }
    else if (definition.IsOptional()) {
      return false;
    }
    else if (definition.IsOptions()) {
      for (RuleNode const & node : definition.GetOptions().nodes) {
        if (!EvaluateConsumesOnlyOne(info, node)) {
          return false;
        }
      }

      return true;
    }
    else if (definition.IsRegex()) {
      return true;
    }
    else if (definition.IsRepeat()) {
      return false;
    }
    else if (definition.IsSequence()) {
      return definition.GetSequence().nodes.size() == 1;
    }

    return false;
  }

  static bool EvaluateIsOptional(IdentifierInfo const & info, RuleNode const & definition) {
    if (definition.IsIdentifier()) {
      return info.IsOptional(definition.GetIdentifier().type);
    }
    else if (definition.IsLiteral()) {
      int matchLength;
      return TryMatchToken("", 0, RuleToken(TokenType::Literal, definition.GetLiteral().matchStr), matchLength);
    }
    else if (definition.IsOptional()) {
      return true;
    }
    else if (definition.IsOptions()) {
      for (RuleNode const & node : definition.GetOptions().nodes) {
        if (EvaluateIsOptional(info, node)) {
          return true;
        }
      }

      return false;
    }
    else if (definition.IsRegex()) {
      int matchLength;
      return TryMatchToken("", 0, RuleToken(TokenType::Regex, definition.GetRegex().matchStr), matchLength);
    }
    else if (definition.IsRepeat()) {
      return EvaluateIsOptional(info, *definition.GetRepeat().node);
    }
    else if (definition.IsSequence()) {
      for (RuleNode const & node : definition.GetSequence().nodes) {
        if (!EvaluateIsOptional(info, node)) {
          return false;
        }
      }

      return true;
    }

    return false;
  }

  static bool AddStringIfUnique(std::string const & value, std::vector<std::string> & strings) {
    for (std::string const & string : strings) {
      if (value == string) {
        return false;
      }
    }

    strings.emplace_back(value);

    return true;
  }

  static void GetDirectDependencies(RuleNode const & definition, std::vector<std::string> & io_dependancies) {
    if (definition.IsIdentifier()) {
      AddStringIfUnique(definition.GetIdentifier().type, io_dependancies);
    }
    else if (definition.IsLiteral()) {
      AddStringIfUnique("'" + definition.GetLiteral().matchStr + "'", io_dependancies);
    }
    else if (definition.IsOptional()) {
      GetDirectDependencies(*definition.GetOptional().node, io_dependancies);
    }
    else if (definition.IsOptions()) {
      for (RuleNode const & node : definition.GetOptions().nodes) {
        GetDirectDependencies(node, io_dependancies);
      }
    }
    else if (definition.IsRegex()) {
      AddStringIfUnique("\"" + definition.GetRegex().matchStr + "\"", io_dependancies);
    }
    else if (definition.IsRepeat()) {
      GetDirectDependencies(*definition.GetRepeat().node, io_dependancies);
    }
    else if (definition.IsSequence()) {
      for (RuleNode const & node : definition.GetSequence().nodes) {
        GetDirectDependencies(node, io_dependancies);
      }
    }
  }

  static bool EvaluateIsDependantForm(RuleNode const & definition) {
    if (definition.IsIdentifier()) {
      return true;
    }
    else if (definition.IsLiteral()) {
      return true;
    }
    else if (definition.IsOptional()) {
      return EvaluateIsDependantForm(*definition.GetOptional().node);
    }
    else if (definition.IsOptions()) {
      for (RuleNode const & node : definition.GetOptions().nodes) {
        if (!EvaluateIsDependantForm(node)) {
          return false;
        }
      }

      return true;
    }
    else if (definition.IsRegex()) {
      return true;
    }
    else if (definition.IsRepeat()) {
      return EvaluateIsDependantForm(*definition.GetRepeat().node);
    }
    else if (definition.IsSequence()) {
      if (definition.GetSequence().nodes.size() == 1) {
        return EvaluateIsDependantForm(definition.GetSequence().nodes.front());
      }

      return false;
    }

    return false;
  }

  static void EvaluateInitialDependants(std::vector<RuleNodeDefinition> const & nodeDefinitions, IdentifierInfo & io_info) {
    for (RuleNodeDefinition const & definition : nodeDefinitions) {
      std::vector<std::string> dependancies;
      GetDirectDependencies(*definition.definition, dependancies);

      for (std::string const & dependancy : dependancies) {
        AddStringIfUnique(definition.type, io_info.data[dependancy].dependants);
      }

      bool const isDependantForm = EvaluateIsDependantForm(*definition.definition);

      if (isDependantForm) {
        for (std::string const & dependancy : dependancies) {
          AddStringIfUnique(definition.type, io_info.data[dependancy].dependantForms);
        }
      }
    }
  }

  static bool EvaluateDependants(IdentifierInfo const & info, std::vector<std::string> & io_dependants) {
    bool changed = false;

    for (int i = 0; i < io_dependants.size(); ++i) {
      std::vector<std::string> const & dependants = *info.GetDependants(io_dependants[i]);

      for (std::string const & dependant : dependants) {
        if (AddStringIfUnique(dependant, io_dependants)) {
          changed = true;
        }
      }
    }

    return changed;
  }

  static bool UpdateDependantInfo(IdentifierInfo & io_info, std::string const & type, std::vector<RuleNodeDefinition> const & nodeDefinitions) {
    if (!io_info.data.contains(type)) {
      return false;
    }

    IdentifierInfoEntry & entry = io_info.data.at(type);

    std::shared_ptr<RuleNode> rule;
    {
      RuleNodeDefinition const * definition = nullptr;
      for (RuleNodeDefinition const & nodeDef : nodeDefinitions) {
        if (nodeDef.type == type) {
          rule = nodeDef.definition;
          break;
        }
      }
    }

    if (!rule) {
      if (type.front() == type.back()) {
        switch (type.front()) {
          case '"': {
            rule = std::make_shared<RuleNode>(RuleNodeRegex(type.substr(1, type.length() - 2)));
          } break;
          case '\'': {
            rule = std::make_shared<RuleNode>(RuleNodeLiteral(type.substr(1, type.length() - 2)));
          } break;
        }
      }
    }

    if (!rule) {
      return false;
    }

    bool changed = false;

    {
      int const oldMinConsumed = entry.minConsumed;

      entry.minConsumed = EvaluateMinConsumed(io_info, *rule);

      if (oldMinConsumed != entry.minConsumed) {
        changed = true;
      }
    }

    {
      bool const oldConsumesOnlyOne = entry.consumesOnlyOne;

      entry.consumesOnlyOne = entry.minConsumed == 1 && EvaluateConsumesOnlyOne(io_info, *rule);

      if (oldConsumesOnlyOne != entry.consumesOnlyOne) {
        changed = true;
      }
    }

    {
      bool const oldContinuable = entry.continuable;

      entry.continuable = EvaluateContinuable(io_info, nodeDefinitions, type);

      if (oldContinuable != entry.continuable) {
        changed = true;
      }
    }

    {
      bool const oldIsOptional = entry.isOptional;

      entry.isOptional = EvaluateIsOptional(io_info, *rule);

      if (oldIsOptional != entry.isOptional) {
        changed = true;
      }
    }

    {
      bool const dependantsChanged = EvaluateDependants(io_info, entry.dependants);

      if (dependantsChanged) {
        changed = true;
      }
    }

    return changed;
  }

  static void EvaluateDependantInfo(std::vector<RuleNodeDefinition> const & nodeDefinitions, IdentifierInfo & io_info) {
    std::vector<std::string> entriesToUpdate;
    entriesToUpdate.reserve(io_info.data.size());

    for (std::pair<std::string const, IdentifierInfoEntry> const & entry : io_info.data) {
      entriesToUpdate.emplace_back(entry.first);
    }

    bool changed;

    do {
      changed = false;

      for (std::string const & type : entriesToUpdate) {
        if (UpdateDependantInfo(io_info, type, nodeDefinitions)) {
          changed = true;
        }
      }
    } while (changed);
  }

  Rules(void) = default;

  Rules(
    std::vector<RuleNodeDefinition> const & nodeDefinitions,
    std::vector<RuleToken> const &          tokenDefinitions
  ) :
    nodeDefinitions(nodeDefinitions),
    tokenDefinitions(tokenDefinitions)
  {
    BuildInfo();
  }

  void BuildRuleNodes(std::vector<Node> const & nodes) {
    nodeDefinitions = GetDefinitionsFromNodes(nodes);
  }

  void BuildRuleTokens(std::vector<Node> const & nodes) {
    tokenDefinitions.clear();
    GetTokensFromNodes(nodes, tokenDefinitions);
  }

  void BuildInfo(void) {
    identifierInfo.data.clear();

    for (RuleNodeDefinition const & definition : nodeDefinitions) {
      identifierInfo.data[definition.type];
    }

    EvaluateInitialDependants(nodeDefinitions, identifierInfo);

    EvaluateDependantInfo(nodeDefinitions, identifierInfo);
  }

  std::vector<Node> Tokenize(std::string const & input, GastError & o_errorMessage) {
    std::vector<Node> result;

    int currentPosition = 0;

    while (currentPosition < input.size()) {
      std::vector<std::string> matches;
      int                      matchLength = 0;

      for (RuleToken const & token : tokenDefinitions) {
        int        length;
        bool const matchToken = TryMatchToken(input, currentPosition, token, length);

        if (matchToken && length >= matchLength) {
          if (length > matchLength) {
            matches.clear();
            matchLength = length;
          }

          matches.emplace_back(token.ToString());
        }
      }

      if (matches.empty()) {
        o_errorMessage.message = "Could not find match for token start position.";
        o_errorMessage.offset  = currentPosition;

        return result;
      }

      Node newNode;

      newNode.types   = matches;
      newNode.content = std::string_view(input.c_str() + currentPosition, matchLength);

      result.emplace_back(newNode);

      currentPosition += matchLength;
    }

    return result;
  }

  bool ReduceNodes(std::vector<Node> & io_nodes, std::set<std::string> & io_completedRules, GastError & o_errorMessage) {
    bool processed;

    do {
      processed = false;

      for (RuleNodeDefinition const & rule : nodeDefinitions) {
        int ruleIndex = &rule - nodeDefinitions.data();

        if (io_completedRules.contains(rule.type)) {
          continue;
        }

        int const minConsumed = identifierInfo.MinConsumed(rule.type);

        bool const newNodesShouldNotStartWithSelf = identifierInfo.ConsumesOnlyOne(rule.type);

        for (int i = 0; i < int(io_nodes.size()) - minConsumed + 1; ++i) {
          int const startOffset = i;
          int       offset      = startOffset;
          std::vector<Node> children;

          if (newNodesShouldNotStartWithSelf && DoesNodeHaveType(io_nodes[offset], rule.type)) {
            continue;
          }

          if (TryBuildChildrenForRule(identifierInfo, rule, io_nodes, offset, children)) {
            if (offset == i) {
              continue;
            }

            //if (identifierInfo.IsOptional(rule.type)) {
            //  io_nodes.erase(io_nodes.begin() + startOffset, io_nodes.begin() + offset);
            //}
            //else
            if (children.size() == 1 && offset == startOffset + 1) {
              if (DoesNodeHaveType(io_nodes[startOffset], rule.type)) {
                continue;
              }

              io_nodes[startOffset].types.emplace_back(rule.type);
            }
            else {
              Node newNode;

              newNode.types.emplace_back(rule.type);
              newNode.content  = GetContentFromNodes(children);
              newNode.children = children;

              ReplaceNodes(io_nodes, startOffset, newNode, offset - startOffset);
            }

            processed = true;

            if (!identifierInfo.Continuable(rule.type)) {
              break;
            }
          }
        }

        if (processed) {
          return true;
        }

        bool const optionalCompletes = true;
        if (IsRuleCompleted(identifierInfo, *rule.definition, io_nodes, io_completedRules, optionalCompletes)) {
          io_completedRules.emplace(rule.type);
        }
      }

    } while (processed);

    return false;
  }

  std::vector<Node> Apply(std::string const & input, GastError & o_errorMessage) {
    std::vector<Node> result = Tokenize(input, o_errorMessage);

    if (o_errorMessage) {
      return result;
    }

    std::set<std::string> completedRules;
    while (ReduceNodes(result, completedRules, o_errorMessage)) {
      if (o_errorMessage) {
        return result;
      }
    }

    return result;
  }

  std::vector<RuleNodeDefinition> nodeDefinitions;
  std::vector<RuleToken>          tokenDefinitions;
  IdentifierInfo                  identifierInfo;
};

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
  whitespace        := "[ \\t\\r\\n\\f]*"

  definition_header := identifier ':='

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

GastRuleset::Rules GastRuleset::Rules::s_baseRules = GastRuleset::Rules(
  {
    DefinitionRule("whitespace",
      Optional(
        Regex("[ \\r\\n\\t\\f]+")
      )
    ),
    DefinitionRule("definitionHeader",
      Sequence({
        Identifier("identifier"),
        Identifier("whitespace"),
        Literal(":="),
      })
    ),
    DefinitionRule("identifier",
      Regex("[a-zA-Z_][a-zA-Z_0-9]*")
    ),
    DefinitionRule("string_regex",
      Regex("\"([^\"\\\\]|(\\\\.))*\"")
    ),
    DefinitionRule("string_literal",
      Regex("'([^'\\\\]|(\\\\.))*'")
    ),
    DefinitionRule("repeating",
      Sequence({
        Identifier("expression"),
        Identifier("whitespace"),
        Literal("..."),
      })
    ),
    DefinitionRule("grouping",
      Sequence({
        Literal("("),
        Identifier("whitespace"),
        Identifier("expression"),
        Identifier("whitespace"),
        Literal(")"),
      })
    ),
    DefinitionRule("optional",
      Sequence({
        Literal("["),
        Identifier("whitespace"),
        Identifier("expression"),
        Identifier("whitespace"),
        Literal("]"),
      })
    ),
    DefinitionRule("sequence",
      Sequence({
        Identifier("expression"),
        Repeat(Sequence({
          Identifier("whitespace"),
          Identifier("expression"),
        })),
      })
    ),
    DefinitionRule("options",
      Sequence({
        Identifier("expression"),
        Repeat(Sequence({
          Identifier("whitespace"),
          Literal("|"),
          Identifier("whitespace"),
          Identifier("expression"),
        })),
      })
    ),
    DefinitionRule("value",
      Options({
        Identifier("identifier"),
        Identifier("string_regex"),
        Identifier("string_literal"),
      })
    ),
    DefinitionRule("operation",
      Options({
        Identifier("repeating"),
        Identifier("grouping"),
        Identifier("optional"),
        Identifier("sequence"),
        Identifier("options"),
      })
    ),
    DefinitionRule("expression",
      Options({
        Identifier("value"),
        Identifier("operation"),
      })
    ),
    DefinitionRule("definition",
      Sequence({
        Identifier("definitionHeader"),
        Identifier("whitespace"),
        Identifier("expression"),
      })
    ),
  },
  {
    RegexToken("[ \\r\\n\\t\\f]+"),
    LiteralToken(":="),
    LiteralToken("..."),
    LiteralToken("("),
    LiteralToken(")"),
    LiteralToken("["),
    LiteralToken("]"),
    LiteralToken("|"),
    RegexToken("[a-zA-Z_][a-zA-Z_0-9]*"),
    RegexToken("\"([^\"\\\\]|(\\\\.))*\""),
    RegexToken("'([^'\\\\]|(\\\\.))*'"),
  }
);


GastRuleset::GastError GastRuleset::BuildRules(std::string const & rules) {
  if (m_impl) {
    m_impl.reset();
  }

  GastRuleset::GastError error;
  m_impl = std::make_shared<Rules>(Rules::FromString(rules, error));

  return error;
}

std::vector<GastRuleset::Node> GastRuleset::ApplyRules(std::string const & content, GastError & o_error) {
  if (!m_impl) {
    return std::vector<Node>();
  }

  return m_impl->Apply(content, o_error);
}
