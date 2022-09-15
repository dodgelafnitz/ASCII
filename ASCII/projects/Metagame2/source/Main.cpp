/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <variant>
#include <vector>

#include "Containers/DynamicArray.h"
#include "Containers/Grid.h"
#include "General/Color.h"
#include "Parsers/Gast.h"
#include "Window/Window.h"
#include "Systems/Input/InputManager.h"
#include "Systems/UpdateManager.h"

namespace {
  static ivec2 const s_screenSize = ivec2(80, 80);

  std::vector<std::string> GetLinesForString(std::string const & str, int width) {
    std::vector< std::string> lines;

    int  lastBreak = -1;
    int  lastSpace = -1;
    bool isSpace = true;

    for (int i = 0; i < str.length(); ++i) {
      if (str[i] == '\n') {
        lines.emplace_back(str.substr(lastBreak + 1, i - lastBreak));
        lastBreak = i;
        lastSpace = -1;
      }
      else if (str[i] == '\r') {
        lines.emplace_back(str.substr(lastBreak + 1, i - lastBreak));
        if (i + 1 < int(str.length()) && str[size_t(i) + 1] == '\n') {
          ++i;
        }
        lastBreak = i;
        lastSpace = -1;
      }
      else if (i - lastBreak > width) {
        if (lastSpace != -1 && !(str[i] == ' ' || str[i] == '\t')) {
          lines.emplace_back(str.substr(lastBreak + 1, lastSpace - lastBreak));
          lastBreak = lastSpace;
        }
        else {
          lines.emplace_back(str.substr(lastBreak + 1, i - lastBreak));
          lastBreak = i;
        }
        lastSpace = -1;
        isSpace = true;
      }
      else if (!isSpace && (str[i] == ' ' || str[i] == '\t')) {
        lastSpace = i;
        isSpace = true;
      }
      else {
        isSpace = false;
      }
    }

    lines.emplace_back(str.substr(lastBreak + 1));


    return lines;
  }

  void PrintTextToScreen(Grid<AsciiCell, 2> & io_grid, ivec2 const & offset, ivec2 const & size, std::string const & text) {
    ivec2 cursor;
    int   index = 0;

    std::vector<std::string> const lines = GetLinesForString(text, size.x);

    for (int i = 0; i < lines.size(); ++i) {
      if (offset.y + i < 0) {
        continue;
      }
      if (offset.y + i >= io_grid.GetSize().y) {
        break;
      }

      for (int j = 0; j < lines[i].size(); ++j) {
        if (offset.x + j < 0) {
          continue;
        }
        if (offset.x + j >= io_grid.GetSize().x) {
          break;
        }

        ivec2 const cursor = offset + ivec2(j, i);

        io_grid[cursor].backgroundColor = 0;
        io_grid[cursor].foregroundColor = 1;
        io_grid[cursor].character = lines[i][j];
      }
    }
  }

  enum class TextType {
    Input,
    Output,

    Count
  };

  struct TextInfo {
    TextType    type;
    std::string text;
  };

  Grid<AsciiCell, 2> GetScreenFromBuffer(int startRow, std::vector<TextInfo> const & textBuffer, std::string const & currentInput) {
    Grid<AsciiCell, 2> result(s_screenSize);

    int const inputLines = GetLinesForString(currentInput, s_screenSize.x).size();

    PrintTextToScreen(result, ivec2(0, s_screenSize.y - inputLines), ivec2(s_screenSize.x, inputLines), currentInput);

    if (inputLines < s_screenSize.y) {
      for (int i = 0; i < s_screenSize.x; ++i) {
        AsciiCell & cell = result[ivec2(i, s_screenSize.y - inputLines - 1)];
        cell.backgroundColor = 0;
        cell.foregroundColor = 1;
        cell.character = '-';
      }
    }

    int currentY = s_screenSize.y - inputLines - 2;

    for (int i = textBuffer.size() - 1; i >= 0; --i) {
      if (currentY < 0) {
        break;
      }

      int const   lineDepth = GetLinesForString(textBuffer[i].text, s_screenSize.x - 2).size();
      ivec2 const typeOffset = ivec2(0, currentY - lineDepth);
      ivec2 const offset = typeOffset + ivec2(2, 0);

      char typeChar = ' ';
      switch (textBuffer[i].type) {
        case TextType::Input: {
          typeChar = '<';
        } break;
        case TextType::Output: {
          typeChar = '>';
        } break;
      }

      result[typeOffset].backgroundColor = 0;
      result[typeOffset].foregroundColor = 1;
      result[typeOffset].character = typeChar;

      PrintTextToScreen(result, offset, ivec2(s_screenSize.x - 2, lineDepth), textBuffer[i].text);

      currentY -= lineDepth + 1;
    }

    return result;
  }

  class Variable {
  public:
    Variable(std::string const & value) :
      m_data(value)
    {}

    Variable(float value) :
      m_data(value)
    {}

    bool IsString(void) const {
      return std::holds_alternative<std::string>(m_data);
    }

    bool IsNumber(void) const {
      return std::holds_alternative<float>(m_data);
    }

    std::string GetString(void) const {
      if (IsString()) {
        return GetStringData();
      }
      else if (IsNumber()) {
        return std::to_string(GetNumberData());
      }
      else {
        return "";
      }
    }

    float GetNumber(void) const {
      if (IsString()) {
        return GetStringData().empty() ? 0.0f : 1.0f;
      }
      else if (IsNumber()) {
        return GetNumberData();
      }
      else {
        return 0;
      }
    }

  private:
    std::string const & GetStringData(void) const {
      return std::get<std::string>(m_data);
    }

    float const & GetNumberData(void) const {
      return std::get<float>(m_data);
    }

    std::variant<
      std::string,
      float
    > m_data;
  };

  using Context = std::map<std::string, Variable>;

  class Expression {
  public:
    Expression(void) = default;

    static Expression And(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::And, left, right);
    }

    static Expression Or(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Or, left, right);
    }

    static Expression Not(Expression const & value) {
      return Expression(ExpressionType::Not, value);
    }

    static Expression Equal(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Equal, left, right);
    }

    static Expression NotEqual(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::NotEqual, left, right);
    }

    static Expression LessThan(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::LessThan, left, right);
    }

    static Expression GreaterThan(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::GreaterThan, left, right);
    }

    static Expression LessThanOrEqual(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::LessThanOrEqual, left, right);
    }

    static Expression GreaterThanOrEqual(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::GreaterThanOrEqual, left, right);
    }

    static Expression Append(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Append, left, right);
    }

    static Expression Negation(Expression const & value) {
      return Expression(ExpressionType::Negation, value);
    }

    static Expression Addition(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Addition, left, right);
    }

    static Expression Subtraction(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Subtraction, left, right);
    }

    static Expression Multiplication(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Multiplication, left, right);
    }

    static Expression Division(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Division, left, right);
    }

    static Expression Modulo(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Modulo, left, right);
    }

    static Expression Exponent(Expression const & left, Expression const & right) {
      return Expression(ExpressionType::Exponent, left, right);
    }

    static Expression Grouping(Expression const & value) {
      return Expression(ExpressionType::Grouping, value);
    }

    static Expression TagValue(Expression const & value) {
      return Expression(ExpressionType::TagValue, value);
    }

    static Expression Evaluate(Expression const & value) {
      return Expression(ExpressionType::Evaluate, value);
    }

    static Expression FromString(std::string const & expressionString) {
    }

  private:
    enum class ExpressionType {
      And,
      Or,
      Not,

      Equal,
      NotEqual,
      LessThan,
      GreaterThan,
      LessThanOrEqual,
      GreaterThanOrEqual,

      Append,

      Negation,
      Addition,
      Subtraction,
      Multiplication,
      Division,
      Modulo,
      Exponent,

      Grouping,
      TagValue,
      Evaluate,

      Count
    };

    /*
      And:                a & b
      Or:                 a | b
      Not:                ! a
      Equal:              a = b
      NotEqual:           a != b
      LessThan:           a < b
      GreaterThan:        a > b
      LessThanOrEqual:    a <= b
      GreaterThanOrEqual: a >= b
      Append:             a .. b
      Negation:           - a
      Addition:           a + b
      Subtraction:        a - b
      Multiplication:     a * b
      Division:           a / b
      Modulo:             a % b
      Exponent:           a ^ b
      Grouping:           ( a )
      TagValue:           { a }
      Evaluate:           [ a ]
    */

    Expression(ExpressionType type) :
      m_type(type)
    {}

    Expression(ExpressionType type, Expression const & first) :
      m_type(type)
    {
      GetSubexpressions().emplace_back(first);
    }

    Expression(ExpressionType type, Expression const & first, Expression const & second) :
      m_type(type)
    {
      GetSubexpressions().emplace_back(first);
      GetSubexpressions().emplace_back(second);
    }

    Expression(ExpressionType type, Variable const & value) :
      m_type(type)
    {
      GetVariable() = value;
    }

    Expression(ExpressionType type, std::string const & value) :
      m_type(type)
    {
      GetVariable() = value;
    }

    Expression(ExpressionType type, int value) :
      m_type(type)
    {
      GetVariable() = value;
    }

    Expression(ExpressionType type, bool value) :
      m_type(type)
    {
      GetVariable() = value;
    }

    bool IsVariable(void) const {
      return std::holds_alternative<Variable>(m_data);
    }

    bool IsSubexpressions(void) const {
      return std::holds_alternative<std::vector<Expression>>(m_data);
    }

    Variable const & GetVariable(void) const {
      return std::get<Variable>(m_data);
    }

    Variable & GetVariable(void) {
      return std::get<Variable>(m_data);
    }

    std::vector<Expression> const & GetSubexpressions(void) const {
      return std::get<std::vector<Expression>>(m_data);
    }

    std::vector<Expression> & GetSubexpressions(void) {
      return std::get<std::vector<Expression>>(m_data);
    }

    ExpressionType m_type = ExpressionType::Count;

    std::variant<
      std::vector<Expression>,
      Variable
    > m_data;
  };

  /*
    TODO: Work on getting tuples/functions from strings

    EventName name := Sequence: {
      Description: "Event: " .. name
    }

    RepeatUntil: [{new_value} > 250] Sequence: {
      Description: "I am " .. {character.name} .. " and I am " .. [{character.age} / 365] .. " years old."
      Choice: {
        "Do something useful." [4] | Description: "Blob."
        "Do nothing."          [2] | Description: "Bleb."
      }
      EventName: "test"
      Sequence: {
        Description: "Test A."
        Description: "Test B."
      }
      Random: {
        [15] | Description: "Blob."
        [2]  | Description: "Bleb."
      }
      SetValue: "new_value" [14 + {old_value}]
      ClearValue: "old_name"
    }

    identifier
    string_literal
    number
    ..
    {
    }
    [
    ]
    :
    |

  */
}

int main(void) {
  std::srand(std::time(nullptr));

  auto window = std::make_shared<AsciiWindow>();

  /*
    And:                a & b
    Or:                 a | b
    Not:                ! a
    Equal:              a = b
    NotEqual:           a != b
    LessThan:           a < b
    GreaterThan:        a > b
    LessThanOrEqual:    a <= b
    GreaterThanOrEqual: a >= b
    Append:             a .. b
    Negation:           - a
    Addition:           a + b
    Subtraction:        a - b
    Multiplication:     a * b
    Division:           a / b
    Modulo:             a % b
    Exponent:           a ^ b
    Grouping:           ( a )
    TagValue:           { a }
    Evaluate:           [ a ]
  */

  std::string const testDefinitions =
    " whitespace               := \"[ \\t\\r\\n\\f]*\" "

    " repeat_header            := \"Repeat\"      whitespace ':' "
    " random_header            := \"Random\"      whitespace ':' "
    " sequence_header          := \"Sequence\"    whitespace ':' "
    " choice_header            := \"Choice\"      whitespace ':' "
    " set_value_header         := \"SetValue\"    whitespace ':' "
    " clear_value_header       := \"ClearValue\"  whitespace ':' "
    " description_header       := \"Description\" whitespace ':' "

    " identifier               := \"[a-zA-Z._]+\"                    "
    " string_literal           := \"\\\"([^\\\"\\\\]|(\\\\.))*\\\"\" "
    " number_literal           := \"[0-9]+(\\.[0-9]+)?\"             "

    " operation                := boolean_operation | comparison_operation | append_operation | exponential_operation | multiplicative_operation | additive_operation "

    " value                    := string_literal | number_literal | identifier | evaluation | value_access | grouping | operation "

    " evaluation               := '[' whitespace value whitespace ']' "
    " value_access             := '{' whitespace value whitespace '}' "
    " grouping                 := '(' whitespace value whitespace ')' "

    " boolean_operation        := value whitespace \"&|\\|\"                whitespace value "
    " comparison_operation     := value whitespace \"=|(!=)|>|<|(<=)|(>=)\" whitespace value "
    " append_operation         := value whitespace '..'                     whitespace value "
    " exponential_operation    := value whitespace '^'                      whitespace value "
    " multiplicative_operation := value whitespace \"\\*|/|%\"              whitespace value "
    " additive_operation       := value whitespace \"\\+|-\"                whitespace value "

    " unary_operation          := \"!|-\" whitespace value "

    " repeat_event             := repeat_header      whitespace event                                                                                     "
    " random_event             := random_header      whitespace '{' (whitespace                  value whitespace '|' whitespace event)... whitespace '}' "
    " sequence_event           := sequence_header    whitespace '{' (whitespace                                                  event)... whitespace '}' "
    " choice_event             := choice_header      whitespace '{' (whitespace value whitespace value whitespace '|' whitespace event)... whitespace '}' "
    " set_value_event          := set_value_header   whitespace value whitespace value                                                                    "
    " clear_value_event        := clear_value_header whitespace value                                                                                     "
    " description_event        := description_header whitespace value                                                                                     "

    " event                    := custom_event_definition | repeat_event | sequence_event | choice_event | set_value_event | clear_value_event | description_event "

    " event_set                := whitespace event whitespace "
  ;


  std::string const testContent =
    " 2 + 5 * 3 * (2 + 1)^2 "
  ;

  std::string const testDefinitions0 =
    " test := 'a' "
  ;

  GastRuleset ruleset;

  GastRuleset::GastError errorMessage = ruleset.BuildRules(testDefinitions);

  /*
    whitespace     := ["[ \t\r\n\f]+"]

    definition_haeder := identifier whitesapce ':='

    identifier     := "[a-zA-Z_][a-zA-Z_0-9]*"
    string_regex   := "\"([^\"]|(\\\"))*\""
    string_literal := "'([^\"]|(\\\"))*'"

    repeating      := expression whitespace '...'
    grouping       := '(' whitespace expression whitespace ')'
    optional       := '[' whitespace expression whitespace ']'
    sequence       := expression (whitespace expression)...
    options        := expression (whitespace '|' whitespace expression)...

    value          := identifier | string_regex | string_literal
    operation      := repeating | grouping | optional | sequence | options

    expression     := value | operation

    definition     := definition_haeder whitespace expression
  */


  std::vector<GastRuleset::Node> rules = ruleset.ApplyRules(testContent, errorMessage);

  window->SetTitle("Meta");

  AsciiFont font;

  font.size = ivec2(8, 8);
  font.colors[0] = Color::Black;
  font.colors[1] = Color::White;

  window->SetFont(font);

  auto updateManager = std::make_shared<UpdateManager>(window, 0.1f, 1.0f / 60.0f);
  auto inputManager  = std::make_shared<InputManager>(window);

  bool                  shouldEnd = false;
  std::string           currentInput;
  std::vector<TextInfo> textBuffer;
  int                   startRow = -1;

  Delegator<std::string> onInput;

  auto moveInputToOutput = onInput.AddDelegate([&currentInput, &textBuffer](std::string const & input) {
    TextInfo newText;
    newText.type = TextType::Input;
    newText.text = currentInput;
    textBuffer.emplace_back(newText);
    currentInput.clear();
  });

  auto escToExit = inputManager->GetButtonManager()->AddButtonEvent(AsciiButton::Escape, [&](bool) {
    shouldEnd = true;
  });

  auto inputFunc = updateManager->AddOnDynamicUpdate([&](float dt, float progress) {
    inputManager->ProcessInput();
  });

  auto onText = inputManager->GetTextManager()->AddTextEvent([&](TextEvent const & text) {
    switch (text.type) {
      case TextEventType::Completion: {
        std::string input = currentInput;
        onInput.Trigger(input);
      } break;
      case TextEventType::Write: {
        currentInput.push_back(text.character);
      } break;
      case TextEventType::Delete: {
        if (!currentInput.empty()) {
          currentInput.pop_back();
        }
      } break;
    }
  });

  auto drawFunc = updateManager->AddOnDynamicUpdate([&](float dt, float progress) {
    Grid<AsciiCell, 2> screen = GetScreenFromBuffer(startRow, textBuffer, currentInput);

    window->Draw(screen);
  });

  while (!shouldEnd) {
    updateManager->Update();
  }

  return 0;
}

