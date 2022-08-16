/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include <map>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <variant>
#include <vector>

#include "Containers/DynamicArray.h"
#include "General/Color.h"
#include "Window/Window.h"
#include "Systems/Input/InputManager.h"
#include "Systems/UpdateManager.h"

namespace {
  static ivec2 const s_screenSize = ivec2(80, 80);

  int GetLinesForString(std::string const & str, int width) {
    int lines = 1;
    int lastBreak = -1;

    for (int i = 0; i < str.length(); ++i) {
      if (str[i] == '\n') {
        ++lines;
        lastBreak = i;
      }
      else if (str[i] == '\r') {
        ++lines;
        if (i + 1 < int(str.length()) && str[size_t(i) + 1] == '\n') {
          ++i;
        }
        lastBreak = i;
      }
      else if (i - lastBreak > width) {
        ++lines;
        lastBreak = i;
      }
    }

    return lines;
  }

  void PrintTextToScreen(Grid<AsciiCell, 2> & io_grid, ivec2 const & offset, ivec2 const & size, std::string const & text) {
    ivec2 cursor;
    int   index = 0;

    while (index < text.length() && cursor.y < size.y) {
      if (text[index] == '\n') {
        cursor.x = 0;
        ++cursor.y;
      }
      else if (text[index] == '\r') {
        cursor.x = 0;
        ++cursor.y;

        if (index + 1 < int(text.length()) && text[size_t(index) + 1] == '\n') {
          ++index;
        }
      }
      else if (offset.y + cursor.y >= 0) {
        io_grid[offset + cursor].backgroundColor = 0;
        io_grid[offset + cursor].foregroundColor = 1;
        io_grid[offset + cursor].character = text[index];

        ++cursor.x;
      }

      if (cursor.x >= size.x) {
        cursor.x = 0;
        ++cursor.y;
      }
      ++index;
    }
  }

  Grid<AsciiCell, 2> GetScreenFromBuffer(int startRow, std::vector<std::string> const & textBuffer, std::string const & currentInput) {
    Grid<AsciiCell, 2> result(s_screenSize);

    int const inputLines = GetLinesForString(currentInput, s_screenSize.x);

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

      int const   lineDepth = GetLinesForString(textBuffer[i], s_screenSize.x);
      ivec2 const offset = ivec2(0, currentY - lineDepth);

      PrintTextToScreen(result, offset, ivec2(s_screenSize.x, lineDepth), textBuffer[i]);

      currentY -= lineDepth + 1;
    }

    return result;
  }

  enum class CommunicationType {
    Query,
    Inform,
    Smalltalk,
    Request,
    Command,
    Response,

    Count
  };

  enum class ResponseType {
    Reject,
    Disaffirm,
    Clarify,
    Acknowledge,
    Affirm,

    Count
  };

  enum class CommunicationObjectType {
    You,
    Me,
    Filter,

    Count
  };

  enum class DescriptorType {
    Name,
    Location,
    Color,
    Size,
    Mood,
    State,
    Speed,
    Material,
    Nature,
    Opinion,
    Relation,

    Count
  };

  enum class ColorType {
    Red,
    Green,
    Blue,
    Yellow,
    Cyan,
    Purple,
    Orange,
    Pink,
    Brown,
    Black,
    White,
    Grey,
    Clear,

    Count
  };

  enum class SizeType {
    Tiny,
    Small,
    Medium,
    Large,
    Huge,

    Count
  };

  enum class MoodType {
    Neutral,
    Sad,
    Happy,
    Angry,
    Afraid,
    Confused,
    Curious,
    Joyous,
    Relaxed,
    Nervous,

    Count
  };

  enum class StateType {
    Enabled,
    Disabled,
    Powerless,
    Malfunctioning,
    Broken,
    Fine,
    Sleeping,
    Unconscious,
    Sick,
    Injured,

    Count
  };

  enum class SpeedType {
    Immobile,
    Slow,
    Medium,
    Quick,
    Fast,
    Instantaneous,

    Count
  };

  enum class MaterialType {
    Wood,
    Dirt,
    Grass,
    Stone,
    Metal,
    Plastic,
    Glass,
    Cloth,
    Meat,
    Bone,
    Paper,
    Rubber,
    Water,
    Oil,
    Gel,

    Count
  };

  enum class NatureType {
    Location,
    Object,
    Person,
    Creature,
    Structure,
    AI,
    Robot,
    Vehicle,

    Count
  };

  enum class OpinionType {
    Hate,
    Dislike,
    Neutral,
    Like,
    Love,

    Count
  };

  enum class RelationType {
    Direction,
    Presence,
    Relationship,
    Destination,
    Target,

    Count
  };

  struct Context {
    Context(void) = default;
    Context(std::string const & name, std::string const & type) :
      name(name),
      type(type)
    {}

    bool operator ==(Context const &) const = default;

    std::string name;
    std::string type;
  };

  std::string NameFromVarName(std::string const & name) {
    std::string result;

    bool lastNotCharacter = true;
    for (char character : name) {
      bool const isUpper = character >= 'A' && character <= 'Z';
      bool const isLower = character >= 'a' && character <= 'z';
      char const upper = isLower ? character + ('A' - 'a') : character;
      char const lower = isUpper ? character + ('a' - 'A') : character;

      if (lastNotCharacter) {
        result.append(1, upper);
      }
      else if (isUpper) {
        result.append(1, ' ');
        result.append(1, upper);
      }
      else {
        result.append(1, lower);
      }

      lastNotCharacter = !(isUpper || isLower);
    }

    return result;
  };

#define CREATE_CONTEXT(type, name) Context const c_##name = Context(NameFromVarName(#name), NameFromVarName(#type))
#define CREATE_ATTRIBUTE_CONTEXT(type, name) Context const c_##type##_##name = Context(NameFromVarName(#name), NameFromVarName(#type "Attribute"))
#define CREATE_CONTEXT_VALUE(type, name) std::string const c_##type##_##name = NameFromVarName(#name)

  struct ContextTag {
    ContextTag(Context const & context, int value = 1) :
      context(context),
      value(value)
    {}

    ContextTag(Context const & context, std::string const & strValue) :
      context(context),
      value(1),
      strValue(strValue)
    {}

    bool operator ==(ContextTag const &) const = default;

    Context     context;
    int         value;
    std::string strValue;
  };

  enum class RewardType {
    AddTag,
    RemoveTag,
    IncrementTag,
    DecrementTag,
    SetTagString,

    Count
  };

  struct PathReward {
    PathReward(RewardType type, Context const & context) :
      type(type),
      context(context),
      delta(1)
    {}

    PathReward(RewardType type, Context const & context, std::string const & strValue) :
      type(type),
      context(context),
      strValue(strValue),
      delta(1)
    {}

    PathReward(RewardType type, Context const & context, int delta) :
      type(type),
      context(context),
      delta(delta)
    {}

    RewardType  type;
    Context     context;
    std::string strValue;
    int         delta;
  };

  enum class ContextRequirementType {
    Present,
    NotPresent,

    ContextEqual,
    ContextNotEqual,
    ContextGreaterThan,
    ContextLessThan,
    ContextGreaterThanOrEqual,
    ContextLessThanOrEqual,

    ValueEqual,
    ValueNotEqual,
    ValueGreaterThan,
    ValueLessThan,
    ValueGreaterThanOrEqual,
    ValueLessThanOrEqual,

    ContextStringEqual,
    ContextStringNotEqual,
    ContextStringContains,
    ContextStringNotContains,

    ValueStringEqual,
    ValueStringNotEqual,
    ValueStringContains,
    ValueStringNotContains,

    Count
  };

  struct ContextRequirement {
    ContextRequirement(ContextRequirementType type, Context const & context) :
      type(type),
      context(context)
    {}

    ContextRequirement(ContextRequirementType type, Context const & context, Context const & comparisonContext) :
      type(type),
      context(context),
      comparisonContext(comparisonContext)
    {}

    ContextRequirement(ContextRequirementType type, Context const & context, int comparisonValue) :
      type(type),
      context(context),
      comparisonValue(comparisonValue)
    {}

    ContextRequirement(ContextRequirementType type, Context const & context, std::string const & comparisonStrValue) :
      type(type),
      context(context),
      comparisonStrValue(comparisonStrValue)
    {}

    ContextRequirementType type;
    Context                context;
    Context                comparisonContext;
    int                    comparisonValue    = 0;
    std::string            comparisonStrValue;
  };

  enum class PathChildType {
    Sequence,
    Choose,
    Generated,

    Count
  };

  struct Path {
    using PathTemplate  = std::function<void(Path &)>;
    using PathGenerator = std::function<Path(std::vector<ContextTag> const &)>;

    Path(std::string const & description = std::string()) :
      description(description),
      childType(PathChildType::Count),
      baseWeight(1),
      priority(false)
    {}

    Path & BaseWeight(int weight) {
      baseWeight = weight;

      return *this;
    }

    Path & Priority(void) {
      priority = true;

      return *this;
    }

    Path & RepeatUntil(ContextRequirement const & requirement) {
      repeatUntil.emplace_back(requirement);

      return *this;
    }

    Path & RepeatUntil(ContextRequirementType type, Context const & context) {
      repeatUntil.emplace_back(type, context);

      return *this;
    }

    Path & RepeatUntil(ContextRequirementType type, Context const & context, Context const & comparisonContext) {
      repeatUntil.emplace_back(type, context, comparisonContext);

      return *this;
    }

    Path & RepeatUntil(ContextRequirementType type, Context const & context, int comparisonValue) {
      repeatUntil.emplace_back(type, context, comparisonValue);

      return *this;
    }

    Path & RepeatUntil(ContextRequirementType type, Context const & context, std::string const & strValue) {
      repeatUntil.emplace_back(type, context, strValue);

      return *this;
    }

    Path & Requirement(ContextRequirement const & requirement) {
      requirements.emplace_back(requirement);

      return *this;
    }

    Path & Requirement(ContextRequirementType type, Context const & context) {
      requirements.emplace_back(type, context);

      return *this;
    }

    Path & Requirement(ContextRequirementType type, Context const & context, Context const & comparisonContext) {
      requirements.emplace_back(type, context, comparisonContext);

      return *this;
    }

    Path & Requirement(ContextRequirementType type, Context const & context, int comparisonValue) {
      requirements.emplace_back(type, context, comparisonValue);

      return *this;
    }

    Path & Requirement(ContextRequirementType type, Context const & context, std::string const & strValue) {
      requirements.emplace_back(type, context, strValue);

      return *this;
    }

    Path & WeightModifier(Context const & context) {
      weightModifiers.emplace_back(context);

      return *this;
    }

    Path & Reward(PathReward const & reward) {
      rewards.emplace_back(reward);

      return *this;
    }

    Path & Reward(RewardType type, Context const & context) {
      rewards.emplace_back(type, context);

      return *this;
    }

    Path & Reward(RewardType type, Context const & context, std::string const & strValue) {
      rewards.emplace_back(type, context, strValue);

      return *this;
    }

    Path & Reward(RewardType type, Context const & context, int delta) {
      rewards.emplace_back(type, context, delta);

      return *this;
    }

    Path & Sequence(Path const & child) {
      if (childType == PathChildType::Count) {
        childType = PathChildType::Sequence;
      }

      if (childType != PathChildType::Sequence) {
        return **static_cast<Path **>(nullptr);
      }

      children.emplace_back(child);

      return *this;
    }

    Path & Sequence(std::vector<Path> const & children) {
      for (Path const & child : children) {
        Sequence(child);
      }

      return *this;
    }

    Path & Choice(Path const & child) {
      if (childType == PathChildType::Count) {
        childType = PathChildType::Choose;
      }

      if (childType != PathChildType::Choose) {
        return **static_cast<Path **>(nullptr);
      }

      children.emplace_back(child);

      return *this;
    }

    Path & Choice(std::vector<Path> const & children) {
      for (Path const & child : children) {
        Choice(child);
      }

      return *this;
    }

    Path & Template(PathTemplate const & func) {
      func(*this);

      return *this;
    }

    Path & Generator(PathGenerator const & func) {
      if (childType == PathChildType::Count) {
        childType = PathChildType::Generated;
      }

      if (childType != PathChildType::Generated) {
        return **static_cast<Path **>(nullptr);
      }

      generator   = func;

      return *this;
    }

    std::string                     description;
    PathChildType                   childType;
    int                             baseWeight;
    bool                            priority;
    PathGenerator                   generator;
    std::vector<ContextRequirement> repeatUntil;
    std::vector<ContextRequirement> requirements;
    std::vector<Context>            weightModifiers;
    std::vector<PathReward>         rewards;
    std::vector<Path>               children;
  };

  struct GameInfo {
    using AttributeFunc = std::function<std::string(std::vector<ContextTag> const &, std::string const &)>;

    GameInfo(Path const & path) :
      path(path)
    {}

    GameInfo & AddAttribute(std::string const & code, AttributeFunc const & func) {
      attributes.emplace(code, func);

      return *this;
    }

    Path                                 path;
    std::map<std::string, AttributeFunc> attributes;
  };

  struct GameData {
    GameInfo *              gameInfo;
    std::vector<ContextTag> tags;
  };

  CREATE_ATTRIBUTE_CONTEXT(player, name);
  CREATE_ATTRIBUTE_CONTEXT(player, namePossessive);
  CREATE_ATTRIBUTE_CONTEXT(player, gender);

  CREATE_CONTEXT_VALUE(gender, masculine);
  CREATE_CONTEXT_VALUE(gender, feminine);
  CREATE_CONTEXT_VALUE(gender, neutral);
  CREATE_CONTEXT_VALUE(gender, object);

  /*
  std::vector<Event> const c_damageEvents = {
    Event("The ship lurches slightly. Something is wrong. A glance through the automatic systems check shows that a small meteor has bypassed the shields and impacted {system.n}"
  };
  */

  GameInfo::AttributeFunc GetNamedAttributeFunc(std::string const & name) {
    return [name](std::vector<ContextTag> const & context, std::string const & value) {
      Context const contextTag = Context(name, NameFromVarName(value + "Attribute"));

      for (ContextTag const & tag : context) {
        if (tag.context == contextTag) {
          return tag.strValue;
        }
      }

      return std::string();
    };
  }

  GameInfo::AttributeFunc GetGenderedAttributeFunc(
    std::string const & masculineResult,
    std::string const & feminineResult,
    std::string const & neutralResult,
    std::string const & objectResult
  ) {
    return [=](std::vector<ContextTag> const & context, std::string const & value) -> std::string {
      Context const genderContext = Context("Gender", NameFromVarName(value + "Attribute"));

      std::string gender;
      for (ContextTag const & tag : context) {
        if (tag.context == genderContext) {
          gender = tag.strValue;
          break;
        }
      }

      if (gender == c_gender_masculine) {
        return masculineResult;
      }
      else if (gender == c_gender_feminine) {
        return feminineResult;
      }
      else if (gender == c_gender_neutral) {
        return neutralResult;
      }
      else if (gender == c_gender_object) {
        return objectResult;
      }

      return std::string();
    };
  }

  static GameInfo s_info =
    GameInfo(
      Path()
    )
    .AddAttribute("n",  GetNamedAttributeFunc("Name"))
    .AddAttribute("np", GetNamedAttributeFunc("Name Possessive"))
    .AddAttribute("o",  GetGenderedAttributeFunc("he", "she", "they", "it"))
    .AddAttribute("co", GetGenderedAttributeFunc("He", "She", "They", "It"))
    .AddAttribute("s",  GetGenderedAttributeFunc("him", "her", "them", "it"))
    .AddAttribute("cs", GetGenderedAttributeFunc("Him", "Her", "Them", "It"))
    .AddAttribute("p",  GetGenderedAttributeFunc("his", "her", "their", "its"))
    .AddAttribute("cp", GetGenderedAttributeFunc("His", "Her", "Their", "Its"))
    .AddAttribute("d",  GetGenderedAttributeFunc("is", "is", "are", "is"))
    .AddAttribute("dp", GetGenderedAttributeFunc("was", "was", "were", "was"))
    .AddAttribute("df", GetGenderedAttributeFunc("will be", "will be", "will be", "will be"))
  ;

  std::vector<ContextTag> GetCurrentContext(GameData const & data) {
    return data.tags;
  }

  bool ContextSatisfied(ContextRequirement const & requirement, std::vector<ContextTag> const & context) {
    ContextTag const * tag           = nullptr;
    ContextTag const * comparisonTag = nullptr;

    for (ContextTag const & contextTag : context) {
      if (contextTag.context == requirement.context) {
        tag = &contextTag;
      }
      if (contextTag.context == requirement.comparisonContext) {
        comparisonTag = &contextTag;
      }

      if (tag && comparisonTag) {
        break;
      }
    }

    int const         tagValue         = tag ? tag->value : 0;
    int const         comparisonValue  = comparisonTag ? comparisonTag->value : 0;
    std::string const tagString        = tag ? tag->strValue : "";
    std::string const comparisonString = comparisonTag ? comparisonTag->strValue : "";

    switch (requirement.type)
    {
      case ContextRequirementType::Present:
        return tag;

      case ContextRequirementType::NotPresent:
        return tag == nullptr;

      case ContextRequirementType::ContextEqual:
        return tagValue == comparisonValue;

      case ContextRequirementType::ContextNotEqual:
        return tagValue != comparisonValue;

      case ContextRequirementType::ContextGreaterThan:
        return tagValue > comparisonValue;

      case ContextRequirementType::ContextLessThan:
        return tagValue < comparisonValue;

      case ContextRequirementType::ContextGreaterThanOrEqual:
        return tagValue >= comparisonValue;

      case ContextRequirementType::ContextLessThanOrEqual:
        return tagValue <= comparisonValue;

      case ContextRequirementType::ValueEqual:
        return tagValue == requirement.comparisonValue;

      case ContextRequirementType::ValueNotEqual:
        return tagValue != requirement.comparisonValue;

      case ContextRequirementType::ValueGreaterThan:
        return tagValue > requirement.comparisonValue;

      case ContextRequirementType::ValueLessThan:
        return tagValue < requirement.comparisonValue;

      case ContextRequirementType::ValueGreaterThanOrEqual:
        return tagValue >= requirement.comparisonValue;

      case ContextRequirementType::ValueLessThanOrEqual:
        return tagValue <= requirement.comparisonValue;

      case ContextRequirementType::ContextStringEqual:
        return tagString == comparisonString;

      case ContextRequirementType::ContextStringNotEqual:
        return tagString != comparisonString;

      case ContextRequirementType::ContextStringContains:
        return tagString.find(comparisonString) != std::string::npos;

      case ContextRequirementType::ContextStringNotContains:
        return tagString.find(comparisonString) == std::string::npos;

      case ContextRequirementType::ValueStringEqual:
        return tagString == requirement.comparisonStrValue;

      case ContextRequirementType::ValueStringNotEqual:
        return tagString != requirement.comparisonStrValue;

      case ContextRequirementType::ValueStringContains:
        return tagString.find(requirement.comparisonStrValue) != std::string::npos;

      case ContextRequirementType::ValueStringNotContains:
        return tagString.find(requirement.comparisonStrValue) == std::string::npos;
    }

    return false;
  }

  bool ContextSatisfied(std::vector<ContextRequirement> const & requirements, std::vector<ContextTag> const & context) {
    for (ContextRequirement const & requirement : requirements) {
      if (!ContextSatisfied(requirement, context)) {
        return false;
      }
    }

    return true;
  }

  int EvaluateWeightModifiers(std::vector<Context> const & modifiers, std::vector<ContextTag> const & context) {
    int total = 0;

    for (Context const & modifier : modifiers) {
      for (ContextTag const & tag : context) {
        if (modifier == tag.context) {
          total += tag.value;
          break;
        }
      }
    }

    return total;
  }

  Path * GetNextPath(std::vector<ContextTag> const & context, std::stack<Path *> & io_currentPaths) {
    Path * chosenPath = nullptr;

    if (io_currentPaths.empty()) {
      return chosenPath;
    }

    int nextSequentialChildIndex = 0;

    // pop up to the next path's parent if we don't have children
    if (io_currentPaths.top()->children.empty()) {
      while (true) {
        Path * currentPath = io_currentPaths.top();

        io_currentPaths.pop();

        if (io_currentPaths.empty()) {
          return chosenPath;
        }

        Path * parentPath = io_currentPaths.top();

        if (parentPath->childType == PathChildType::Sequence) {
          int const startIndex = currentPath - parentPath->children.data();

          // next path is next available child of this one
          bool nextPathIsInSequence = false;
          for (int i = startIndex + 1; i < parentPath->children.size(); ++i) {
            if (ContextSatisfied(parentPath->children[i].requirements, context)) {
              nextSequentialChildIndex = i;
              nextPathIsInSequence = true;
              break;
            }
          }

          if (nextPathIsInSequence) {
            break;
          }
        }

        // repeat this path
        if (!ContextSatisfied(parentPath->repeatUntil, context)) {
          break;
        }
      }
    }

    Path * parentPath = io_currentPaths.top();

    switch (parentPath->childType) {
      case PathChildType::Sequence: {
        chosenPath = &parentPath->children[nextSequentialChildIndex];
      } break;

      case PathChildType::Choose: {
        int totalWeight = 0;

        for (Path & childPath : parentPath->children) {
          if (ContextSatisfied(childPath.requirements, context)) {
            int const pathWeight = childPath.baseWeight + EvaluateWeightModifiers(childPath.weightModifiers, context);

            totalWeight += pathWeight;
            if (std::rand() % totalWeight < pathWeight) {
              chosenPath = &childPath;
            }
          }
        }
      } break;

      case PathChildType::Generated: {
        parentPath->children.clear();
        parentPath->children.emplace_back(parentPath->generator(context));
        chosenPath = &parentPath->children[0];
      } break;
    }

    io_currentPaths.push(chosenPath);
    return chosenPath;
  }

  void UpdateReward(PathReward const & reward, std::vector<ContextTag> & io_context) {
    ContextTag * tag = nullptr;

    for (ContextTag & contextTag : io_context) {
      if (contextTag.context == reward.context) {
        tag = &contextTag;
      }
    }

    switch (reward.type) {
      case RewardType::AddTag: {
        if (tag == nullptr) {
          io_context.emplace_back(reward.context);
        }
      } break;
      case RewardType::RemoveTag: {
        if (tag) {
          *tag = io_context.back();
          io_context.pop_back();
        }
      } break;
      case RewardType::IncrementTag: {
        if (tag == nullptr) {
          io_context.emplace_back(reward.context, reward.delta);
        }
        else {
          tag->value += reward.delta;
        }
      } break;
      case RewardType::DecrementTag: {
        if (tag) {
          tag->value -= reward.delta;

          if (tag->value <= 0) {
            *tag = io_context.back();
            io_context.pop_back();
          }
        }
      } break;
      case RewardType::SetTagString: {
        if (tag == nullptr) {
          io_context.emplace_back(reward.context, reward.strValue);
        }
        else {
          tag->strValue = reward.strValue;
        }
      }
    }
  }

  void UpdateRewards(std::vector<PathReward> const & rewards, std::vector<ContextTag> & io_context) {
    for (PathReward const & reward : rewards) {
      UpdateReward(reward, io_context);
    }
  }

  std::string DecsriptionClarification(std::string const & base, GameData const & gameData, std::vector<ContextTag> const & context) {
    std::string result = base;

    while (true) {
      size_t const attrStart = result.find_first_of('{', 0);
      size_t const attrEnd   = result.find_first_of('}', 0);

      if (attrStart == std::string::npos) {
        break;
      }

      if (attrEnd == std::string::npos) {
        return "<<ERROR WITH STRING: " + result + ">>";
      }

      std::string const attrFull = result.substr(attrStart + 1, attrEnd - attrStart - 1);

      size_t const attrSplit = attrFull.find(".");

      if (attrSplit == std::string::npos) {
        return "<<ERROR WITH ATTRIBUTE: " + attrFull + ">>";
      }

      std::string const attrType = attrFull.substr(0, attrSplit);
      std::string const attrCode = attrFull.substr(attrSplit + 1, attrFull.size());

      if (gameData.gameInfo->attributes.count(attrCode) != 0) {
        std::string const attr = gameData.gameInfo->attributes.at(attrCode)(context, attrType);

        result = result.substr(0, attrStart) + attr + result.substr(attrEnd + 1);
      }
      else {
        result = result.substr(0, attrStart) + "<<" + attrFull + ">>" + result.substr(attrEnd + 1);
      }
    }

    return result;
  }


}

namespace SpaceGame {
  CREATE_CONTEXT(playerState, shipForm);
  CREATE_CONTEXT(playerState, location);

  CREATE_CONTEXT_VALUE(faction, human);
  CREATE_CONTEXT_VALUE(faction, orider);
  CREATE_CONTEXT_VALUE(faction, brak);
  CREATE_CONTEXT_VALUE(faction, androre);
  CREATE_CONTEXT_VALUE(faction, unaligned);

  CREATE_CONTEXT(playerState, civilizationLevel);
  CREATE_CONTEXT_VALUE(civilizationLevel, rim);
  CREATE_CONTEXT_VALUE(civilizationLevel, controlled);
  CREATE_CONTEXT_VALUE(civilizationLevel, core);
  CREATE_CONTEXT_VALUE(civilizationLevel, capital);

  CREATE_CONTEXT(playerState, humanRelationship);
  CREATE_CONTEXT(playerState, oriderRelationship);
  CREATE_CONTEXT(playerState, brakRelationship);
  CREATE_CONTEXT(playerState, androreRelationship);

  CREATE_CONTEXT_VALUE(relationship, adored);
  CREATE_CONTEXT_VALUE(relationship, allied);
  CREATE_CONTEXT_VALUE(relationship, friendly);
  CREATE_CONTEXT_VALUE(relationship, neutral);
  CREATE_CONTEXT_VALUE(relationship, unknown);
  CREATE_CONTEXT_VALUE(relationship, rival);
  CREATE_CONTEXT_VALUE(relationship, enemy);
  CREATE_CONTEXT_VALUE(relationship, hated);

  CREATE_CONTEXT(playerState, shipSize);
  CREATE_CONTEXT_VALUE(size, tiny);
  CREATE_CONTEXT_VALUE(size, small);
  CREATE_CONTEXT_VALUE(size, medium);
  CREATE_CONTEXT_VALUE(size, large);
  CREATE_CONTEXT_VALUE(size, huge);
  CREATE_CONTEXT_VALUE(size, massive);

  CREATE_CONTEXT(playerState, crewSize);
  CREATE_CONTEXT(playerState, credits);

  CREATE_CONTEXT(playerState, weaponLevel);
  CREATE_CONTEXT(playerState, shieldLevel);
  CREATE_CONTEXT(playerState, navigationLevel);
  CREATE_CONTEXT(playerState, labratoryLevel);
  CREATE_CONTEXT(playerState, facilitiesLevel);

  CREATE_CONTEXT(playerState, shipDamage);
  CREATE_CONTEXT_VALUE(damage, undamaged);
  CREATE_CONTEXT_VALUE(damage, cosmetic);
  CREATE_CONTEXT_VALUE(damage, light);
  CREATE_CONTEXT_VALUE(damage, medium);
  CREATE_CONTEXT_VALUE(damage, heavy);
  CREATE_CONTEXT_VALUE(damage, critical);

  CREATE_ATTRIBUTE_CONTEXT(player, faction);

  CREATE_CONTEXT(mission, intro);

  Path CreateNewPlayerChoice(std::string const & name, std::string const & gender, std::string const & faction) {
    std::string const namePossessive = name.back() == 's' ? name + "'" : name + "'s";

    return Path()
      .Reward(RewardType::SetTagString, c_player_name, name)
      .Reward(RewardType::SetTagString, c_player_namePossessive, namePossessive)
      .Reward(RewardType::SetTagString, c_player_gender, gender)
      .Reward(RewardType::SetTagString, c_player_faction, faction)
    ;
  }

  Path CreateNewPlayerLaunchChoice(std::string const & description, std::string const & faction, std::string const & rivalFaction) {
    Context const factionRelationship = Context(NameFromVarName(faction      + "Relationship"), NameFromVarName("PlayerState"));
    Context const rivalRelationship   = Context(NameFromVarName(rivalFaction + "Relationship"), NameFromVarName("PlayerState"));

    return Path(description)
      .Requirement(ContextRequirementType::ValueStringNotEqual, c_player_faction, rivalFaction)
      .Reward(RewardType::SetTagString, c_shipForm, faction)
      .Reward(RewardType::SetTagString, factionRelationship, c_relationship_friendly)
      .Reward(RewardType::SetTagString, rivalRelationship,   c_relationship_rival)
    ;
  }

  Path const c_introEvent =
    Path()
    .Sequence(
      Path()
      .Choice(CreateNewPlayerChoice("Jeff",     c_gender_masculine, c_faction_human))
      .Choice(CreateNewPlayerChoice("Avery",    c_gender_feminine,  c_faction_human))
      .Choice(CreateNewPlayerChoice("K'listic", c_gender_neutral,   c_faction_orider))
      .Choice(CreateNewPlayerChoice("Vilt'rit", c_gender_neutral,   c_faction_orider))
      .Choice(CreateNewPlayerChoice("C3P-8",    c_gender_object,    c_faction_androre))
      .Choice(CreateNewPlayerChoice("BT-6-a",   c_gender_neutral,   c_faction_androre))
      .Choice(CreateNewPlayerChoice("Gree",     c_gender_object,    c_faction_brak))
      .Choice(CreateNewPlayerChoice("Markelo",  c_gender_feminine,  c_faction_brak))
    )
    .Sequence(
      Path("At long last, {player.n} has gotten together a small ship and a crew for it.")
      .RepeatUntil(ContextRequirementType::ValueStringEqual, c_player_name, "")
      .Priority()
      .Requirement(ContextRequirementType::NotPresent, c_intro)
      .Reward(RewardType::IncrementTag, c_intro)
      .Reward(RewardType::SetTagString, c_shipSize, c_size_tiny)
      .Reward(RewardType::SetTagString, c_civilizationLevel, c_civilizationLevel_capital)
      .Choice(CreateNewPlayerLaunchChoice("{player.co} {player.d} launching from Earth, the homeworld of the humans.",     c_faction_human,   c_faction_androre))
      .Choice(CreateNewPlayerLaunchChoice("{player.co} {player.d} launching from Arachnas, the homeworld of the oriders.", c_faction_orider,  c_faction_brak))
      .Choice(CreateNewPlayerLaunchChoice("{player.co} {player.d} launching from Mecharus, the homeworld of the androre.", c_faction_androre, c_faction_human))
      .Choice(CreateNewPlayerLaunchChoice("{player.co} {player.d} launching from Basalt, the homeworld of the brak.",      c_faction_brak,    c_faction_orider))
    )
  ;

  Path GetPath(void) {
    return c_introEvent;
  }
}

namespace FantasyGame {
  CREATE_CONTEXT(playerState, money);

  CREATE_CONTEXT(playerState, location);
  CREATE_CONTEXT_VALUE(location, city);
  CREATE_CONTEXT_VALUE(location, desert);
  CREATE_CONTEXT_VALUE(location, forest);
  CREATE_CONTEXT_VALUE(location, mountains);
  CREATE_CONTEXT_VALUE(location, ocean);

  CREATE_CONTEXT(playerState, job);
  CREATE_CONTEXT_VALUE(job, rogue);
  CREATE_CONTEXT_VALUE(job, warrior);
  CREATE_CONTEXT_VALUE(job, wizard);
  CREATE_CONTEXT_VALUE(job, ranger);
  CREATE_CONTEXT_VALUE(job, cleric);
  CREATE_CONTEXT_VALUE(job, bard);

  CREATE_CONTEXT(playerState, stealth);
  CREATE_CONTEXT(playerState, combat);
  CREATE_CONTEXT(playerState, magic);
  CREATE_CONTEXT(playerState, tracking);
  CREATE_CONTEXT(playerState, faith);
  CREATE_CONTEXT(playerState, charm);

  CREATE_ATTRIBUTE_CONTEXT(player, race);
  CREATE_CONTEXT_VALUE(race, human);
  CREATE_CONTEXT_VALUE(race, dwarf);
  CREATE_CONTEXT_VALUE(race, elf);
  CREATE_CONTEXT_VALUE(race, goblin);
  CREATE_CONTEXT_VALUE(race, orc);
  CREATE_CONTEXT_VALUE(race, lizardfolk);

  CREATE_ATTRIBUTE_CONTEXT(partner, name);
  CREATE_ATTRIBUTE_CONTEXT(partner, namePossessive);
  CREATE_ATTRIBUTE_CONTEXT(partner, gender);
  CREATE_ATTRIBUTE_CONTEXT(partner, race);

  CREATE_ATTRIBUTE_CONTEXT(enemy, name);
  CREATE_ATTRIBUTE_CONTEXT(enemy, namePossessive);
  CREATE_ATTRIBUTE_CONTEXT(enemy, gender);
  CREATE_ATTRIBUTE_CONTEXT(enemy, race);

  Path GetPath(void) {
    return Path();
  }
}

int main(void) {
  std::srand(std::time(nullptr));

  auto window = std::make_shared<AsciiWindow>();

  window->SetTitle("Meta");

  AsciiFont font;

  font.size = ivec2(8, 8);
  font.colors[0] = Color::Black;
  font.colors[1] = Color::White;

  window->SetFont(font);

  auto updateManager = std::make_shared<UpdateManager>(window, 0.1f, 1.0f / 60.0f);
  auto inputManager = std::make_shared<InputManager>(window);

  bool                     shouldEnd = false;
  std::string              currentInput;
  std::vector<std::string> textBuffer;
  int                      startRow = -1;


  GameData gameData;

  gameData.gameInfo = &s_info;

  gameData.gameInfo->path = SpaceGame::GetPath();

  Delegator<std::string> onInput;

  auto moveInputToOutput = onInput.AddDelegate([&currentInput, &textBuffer](std::string const & input) {
    textBuffer.emplace_back(std::string("< ") + currentInput);
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

  float const c_waitTime                     = 0.5f;
  float const c_delayPerDescriptionCharacter = 0.025f;

  float currentTime = 0.0f;

  std::stack<Path *> currentPath;

  bool started = false;
  auto updateFunc = updateManager->AddOnFixedUpdate([&](float dt) {
    currentTime += dt;

    if (currentTime > c_waitTime) {
      Path * path = nullptr;

      if (started) {
        path = GetNextPath(gameData.tags, currentPath);
      }
      else {
        currentPath.push(&gameData.gameInfo->path);
        path = currentPath.top();

        started = true;
      }

      if (path) {
        UpdateRewards(path->rewards, gameData.tags);

        if (!path->description.empty()) {
          textBuffer.emplace_back(DecsriptionClarification(path->description, gameData, gameData.tags));
          currentTime -= path->description.size() * c_delayPerDescriptionCharacter + c_waitTime;
        }
      }
      else {
        textBuffer.emplace_back("<<no path>>");
      }
    }
  });

  while (!shouldEnd) {
    updateManager->Update();
  }

  return 0;
}

