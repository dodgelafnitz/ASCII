/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <algorithm>
#include <ctime>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>

#include "Containers/DynamicArray.h"

namespace {
  template <typename Base, typename Element, int Count, typename Indexer = int>
  class DataTable : public Base {
  public:
    DataTable(Element const (&elements)[Count]) {
      for (int i = 0; i < Count; ++i) {
        m_data.Emplace(elements[i]);
      }
    }

    Element const & operator [](Indexer index) const {
      return m_data[int(index)];
    }

    Element const * begin(void) const {
      return m_data.begin();
    }

    Element const * end(void) const {
      return m_data.end();
    }

    constexpr int Size(void) const {
      return Count;
    }

  private:
    DynamicArray<Element, Count> m_data;
  };

  template <auto T>
  using ExpectedCountType = std::conditional_t<std::is_same_v<decltype(T), std::nullptr_t>, int, decltype(T)>;

  template <typename Base, auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<Base, Element, Count, ExpectedCountType<ExpectedCount>> MakeDataTable(
    Element const (&array)[Count]
  ) {
    if constexpr (!std::is_same_v<decltype(ExpectedCount), std::nullptr_t>) {
      static_assert(int(ExpectedCount) == Count);

      return DataTable<Base, Element, int(ExpectedCount), decltype(ExpectedCount)>(array);
    }
    else {
      return DataTable<Base, Element, Count>(array);
    }
  }

  class StaticMessageException : public std::exception {
  public:
    StaticMessageException(char const * message) :
      m_message(message)
    {}

    virtual char const * what(void) const noexcept override {
      return m_message;
    }

  private:
    char const * m_message = "";
  };

  template <typename Base, auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<Base, Element, Count, ExpectedCountType<ExpectedCount>> MakeDataTable(
    std::pair<ExpectedCountType<ExpectedCount>, Element> const (&array)[Count]
  ) {
    Element arr[Count];
    bool    added[Count] = { false };

    for (int i = 0; i < Count; ++i) {
      int const index = int(array[i].first);

      if (added[index]) {
        throw StaticMessageException("Added duplicate index.");
      }

      arr[index] = array[i].second;
      added[index] = true;
    }

    return MakeDataTable<Base, ExpectedCount, Element, Count>(arr);
  }

  struct EmptyHeader {};

  template <auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<EmptyHeader, Element, Count, ExpectedCountType<ExpectedCount>> MakeHeaderlessDataTable(
    Element const (&array)[Count]
  ) {
    return MakeDataTable<EmptyHeader, ExpectedCount, Element, Count>(array);
  }

  template <typename CountType, typename Element, int Count>
  DataTable<EmptyHeader, Element, Count, CountType> MakeHeaderlessDataTable(
    std::pair<CountType, Element> const (&array)[Count]
  ) {
    return MakeDataTable<EmptyHeader, CountType(Count), Element, Count>(array);
  }

  template <typename T, typename ... Params>
  struct MaxSize {
    static const int value = std::max(int(sizeof(T)), MaxSize<Params...>::value);
  };

  template <typename T>
  struct MaxSize<T> {
    static const int value = sizeof(T);
  };

  template <typename Indexer = int, typename ... Types>
  class Variant {
  private:
    static const Indexer Count = Indexer(sizeof...(Types));

    template <int Index, typename FirstParam, typename ... Params>
    struct TypeForIndexInternal {
      using type = TypeForIndexInternal<Index - 1, Params...>::type;
    };

    template <typename FirstParam, typename ... Params>
    struct TypeForIndexInternal<0, FirstParam, Params...> {
      using type = FirstParam;
    };

    template <int Index>
    struct TypeForIndexInternalStart {
      using type = TypeForIndexInternal<Index, Types...>::type;
    };

    template <int Index>
    using TypeForIndex = TypeForIndexInternalStart<Index>::type;

    template <int Index, typename U, typename FirstParam, typename ... Params>
    struct IndexForTypeInternal {
      static const int value = IndexForTypeInternal<Index + 1, U, Params...>::value;
    };

    template <int Index, typename U, typename ... Params>
    struct IndexForTypeInternal<Index, U, U, Params...> {
      static const int value = Index;

      static_assert(IndexForTypeInternal<Index + 1, U, Params...>::value == int(Count));
    };

    template <int Index, typename U>
    struct IndexForTypeInternal<Index, U, U> {
      static const int value = Index;
    };

    template <int Index, typename U, typename V>
    struct IndexForTypeInternal<Index, U, V> {
      static const int value = int(Count);
    };

    template <typename U>
    struct IndexForTypeInternalStart {
      static const int value = IndexForTypeInternal<0, U, Types...>::value;

      static_assert(value != int(Count));
    };

    template <typename U>
    static const Indexer IndexForType = Indexer(IndexForTypeInternalStart<U>::value);

  public:
    Variant(void) = default;

    Variant(Variant const & variant) {
      SetDynamicInternal<Types...>(int(variant.m_type), variant.m_data);
    }

    template <typename U>
    Variant(U const & value) {
      Set(value);
    }

    ~Variant(void) {
      Clear();
    }

    template <typename U>
    Variant operator =(U const & value) {
      Set(value);

      return *this;
    }

    template<typename U>
    void Set(U const & value) {
      if (reinterpret_cast<char const *>(&value) == m_data) {
        return;
      }

      Clear();

      m_type = IndexForType<U>;
      new (reinterpret_cast<U *>(&m_data)) U(value);
    }

    Indexer GetType(void) const {
      return m_type;
    }

    template <Indexer Index>
    TypeForIndex<int(Index)> const & Get(void) const {
      return *reinterpret_cast<TypeForIndex<int(Index)> const *>(&m_data);
    }

    template <Indexer Index>
    TypeForIndex<int(Index)> & Get(void) {
      return *reinterpret_cast<TypeForIndex<int(Index)> *>(&m_data);
    }

    void Clear(void) {
      if (m_type == Count) {
        return;
      }

      ClearInternal<Types...>(int(m_type));
    }

  private:
    Indexer m_type = Count;
    char    m_data[MaxSize<Types...>::value] = { 0 };

    template <typename U, typename ... Params>
    void ClearInternal(int index) {
      if (index == 0) {
        reinterpret_cast<U *>(m_data)->~U();
      }
      else if constexpr (sizeof...(Params) != 0) {
        ClearInternal<Params...>(index - 1);
      }
    }

    template <typename U, typename ... Params>
    void SetDynamicInternal(int index, char const * data) {
      if (index == 0) {
        Set(*reinterpret_cast<U const *>(data));
      }
      else if constexpr (sizeof...(Params) != 0) {
        SetDynamicInternal<Params...>(index - 1, data);
      }
    }
  };

  float GetRandom(void) {
    return (std::rand() % 13091) / 13091.0f;
  }

  enum class CardRequirementType {
    Emblem,
    Faction,
    Style,
    Level,
    Resource,
    Alignment,

    Owner,
    Controller,
    Lane,
    ThisCard,
    Activated,

    None,
    And,
    Or,
    Not,

    Count
  };

  enum class ComparisonOperator {
    LessThan,
    GreaterThan,
    Equal,
    NotEqual,
    LessThanOrEqual,
    GreaterThanOrEqual,

    Count
  };

  static int const c_requirementCount = 8;

  static int const c_invalidIndex = -1;

  struct CardRequirementEmblem;
  struct CardRequirementFaction;
  struct CardRequirementStyle;
  struct CardRequirementLevel;
  struct CardRequirementResource;
  struct CardRequirementAlignment;

  struct CardRequirementOwner;
  struct CardRequirementController;
  struct CardRequirementLane;
  struct CardRequirementThisCard;
  struct CardRequirementActivated;

  struct CardRequirementNone;
  struct CardRequirementAnd;
  struct CardRequirementOr;
  struct CardRequirementNot;

  using CardRequirement = Variant<
    CardRequirementType,

    CardRequirementEmblem,
    CardRequirementFaction,
    CardRequirementStyle,
    CardRequirementLevel,
    CardRequirementResource,
    CardRequirementAlignment,
    CardRequirementOwner,
    CardRequirementController,
    CardRequirementLane,
    CardRequirementThisCard,
    CardRequirementActivated,
    CardRequirementNone,
    CardRequirementAnd,
    CardRequirementOr,
    CardRequirementNot
  >;

  enum class Alignment {
    Fire,
    Water,
    Earth,
    Air,
    Light,

    Count
  };

  std::string GetAlignmentText(Alignment alignment) {
    char const * alignmentStrs[] = {
      "Fire",
      "Water",
      "Earth",
      "Air",
      "Light",
      "",
    };

    return alignmentStrs[int(alignment)];
  }

  enum class CardStyleType {
    Level,
    Resource,
    Alignment,
    Utility,

    Count
  };

  struct CardRequirementEmblem {
    CardRequirementEmblem(void) = default;
    CardRequirementEmblem(int emblem) :
      emblem(emblem)
    {}

    int emblem = c_invalidIndex;
  };

  struct CardRequirementFaction {
    CardRequirementFaction(void) = default;
    CardRequirementFaction(int faction) :
      faction(faction)
    {}

    int faction = c_invalidIndex;
  };

  struct CardRequirementStyle {
    CardRequirementStyle(void) = default;
    CardRequirementStyle(CardStyleType style) :
      style(style)
    {}

    CardStyleType style = CardStyleType::Count;
  };

  struct CardRequirementLevel {
    CardRequirementLevel(void) = default;
    CardRequirementLevel(int level) :
      level(level)
    {}
    CardRequirementLevel(ComparisonOperator op, int level) :
      op(op),
      level(level)
    {}
    
    ComparisonOperator op    = ComparisonOperator::Equal;
    int                level = 0;
  };

  struct CardRequirementResource {
    CardRequirementResource(void) = default;
    CardRequirementResource(int resource) :
      resource(resource)
    {}

    int resource = c_invalidIndex;
  };

  struct CardRequirementAlignment {
    CardRequirementAlignment(void) = default;
    CardRequirementAlignment(Alignment alignment) :
      alignment(alignment)
    {}

    Alignment alignment = Alignment::Count;
  };

  struct CardRequirementOwner {
  };

  struct CardRequirementController {
  };

  struct CardRequirementLane {
  };

  struct CardRequirementThisCard {
  };

  struct CardRequirementActivated {
  };

  struct CardRequirementNone {
  };

  struct CardRequirementAnd {
    CardRequirementAnd(void) = default;
    CardRequirementAnd(DynamicArray<CardRequirement, c_requirementCount> const & i_requirements);

    DynamicArray<std::shared_ptr<CardRequirement>, c_requirementCount> requirements;
  };

  struct CardRequirementOr {
    CardRequirementOr(void) = default;
    CardRequirementOr(DynamicArray<CardRequirement, c_requirementCount> const & i_requirements);

    DynamicArray<std::shared_ptr<CardRequirement>, c_requirementCount> requirements;
  };

  struct CardRequirementNot {
    CardRequirementNot(void) = default;
    CardRequirementNot(CardRequirement const & requirement);

    std::shared_ptr<CardRequirement> requirement;
  };

  CardRequirementAnd::CardRequirementAnd(DynamicArray<CardRequirement, c_requirementCount> const & i_requirements) {
    for (CardRequirement const & requirement : i_requirements) {
      requirements.Emplace(std::make_shared<CardRequirement>(requirement));
    }
  }

  CardRequirementOr::CardRequirementOr(DynamicArray<CardRequirement, c_requirementCount> const & i_requirements) {
    for (CardRequirement const & requirement : i_requirements) {
      requirements.Emplace(std::make_shared<CardRequirement>(requirement));
    }
  }

  CardRequirementNot::CardRequirementNot(CardRequirement const & requirement) :
    requirement(std::make_shared<CardRequirement>(requirement))
  {}

  enum class EmblemTriggerType {
    CardTrigger,
    LaneTrigger,
    GameTrigger,

    Count
  };

  enum class EmblemCardTriggerType {
    OnDestroy,
    OnDraw,
    OnPlay,
    OnDiscard,
    OnCycle,
    OnActivate,

    Count
  };

  enum class EmblemLaneTriggerType {
    OnClaim,
    OnClose,
    OnCycle,

    Count
  };

  enum class EmblemGameTriggerType {
    OnRoundStart,
    OnRoundEnd,
    OnGameEnd,

    Count
  };

  enum class EmblemTargetActionType {
    Destroy,
    Cycle,
    MoveToLane,
    MoveToHand,
    MoveToDeck,
    ChangeControl,

    Count
  };

  struct EmblemTargetActionDestroy {
  };

  struct EmblemTargetActionCycle {
  };

  struct EmblemTargetActionMoveToLane {
    EmblemTargetActionMoveToLane(void) = default;
    EmblemTargetActionMoveToLane(bool sameLane, bool otherLane) :
      sameLane(sameLane),
      otherLane(otherLane)
    {}

    bool sameLane  = false;
    bool otherLane = false;
  };

  struct EmblemTargetActionMoveToHand {
  };

  struct EmblemTargetActionMoveToDeck {
  };

  struct EmblemTargetActionChangeControl {
    EmblemTargetActionChangeControl(void) = default;
    EmblemTargetActionChangeControl(bool controllingPlayer, bool otherPlayer) :
      controllingPlayer(controllingPlayer),
      otherPlayer(otherPlayer)
    {}

    bool controllingPlayer = false;
    bool otherPlayer       = false;
  };

  struct EmblemTargetActionActivate {
  };

  using EmblemTargetActionData = Variant<
    EmblemTargetActionType,

    EmblemTargetActionDestroy,
    EmblemTargetActionCycle,
    EmblemTargetActionMoveToLane,
    EmblemTargetActionMoveToHand,
    EmblemTargetActionMoveToDeck,
    EmblemTargetActionChangeControl,
    EmblemTargetActionActivate
  >;

  /*
  emblem defeat = {
    passiveEffects = [];
    activeEffects = [
      {
        trigger = {
          type        = triggerType.onPlay;
          requirement = {
            type = cardRequirementType.thisCard;
          };
        };
        actions = [
          {
            targetRequirement = {
              type         = cardRequirementType.and;
              requirements = [
                {
                  type = cardRequirementType.sameLane;
                },
                {
                  type        = NOT;
                  requirement = {
                    type = cardRequirementType.thisCard;
                  };
                },
              ];
            };
            action = cardAction.destroy;
          },
        ];
      },
    ];
  };
  */

  struct PassiveEffect {
    PassiveEffect(void) = default;
  };

  struct EmblemCardTrigger {
    EmblemCardTrigger(void) = default;

    EmblemCardTrigger(EmblemCardTriggerType type, CardRequirement const & requirement) :
      type(type),
      requirement(requirement)
    {}

    EmblemCardTriggerType type;
    CardRequirement       requirement;
  };

  struct EmblemLaneTrigger {
    EmblemLaneTrigger(void) = default;

    EmblemLaneTrigger(EmblemLaneTriggerType type) :
      type(type)
    {}

    EmblemLaneTriggerType type;
  };

  struct EmblemGameTrigger {
    EmblemGameTrigger(void) = default;

    EmblemGameTrigger(EmblemGameTriggerType type) :
      type(type)
    {}

    EmblemGameTriggerType type;
  };

  using EmblemTrigger = Variant<
    EmblemTriggerType,

    EmblemCardTrigger,
    EmblemLaneTrigger,
    EmblemGameTrigger
  >;

  struct EmblemTargetAction {
    EmblemTargetAction(void) = default;

    EmblemTargetAction(
      CardRequirement const &        requirement,
      EmblemTargetActionData const & data
    ) :
      requirement(requirement),
      data(data)
    {}

    CardRequirement        requirement;
    EmblemTargetActionData data;
  };

  enum EmblemPlayerActionType {
    DrawCard,
    Discard,
    PlayCard,
    PlayFromDiscard,
    ClaimLane,
    Redraw,

    Count
  };

  struct EmblemPlayerActionDrawCard {
  };

  struct EmblemPlayerActionDiscard {
    EmblemPlayerActionDiscard(void) = default;
    EmblemPlayerActionDiscard(CardRequirement const & requirement) :
      requirement(requirement)
    {}

    CardRequirement requirement;
  };

  struct EmblemPlayerActionPlayCard {
    EmblemPlayerActionPlayCard(void) = default;
    EmblemPlayerActionPlayCard(
      bool                    sameLane,
      bool                    otherLane,
      CardRequirement const & requirement
    ) :
      sameLane(sameLane),
      otherLane(otherLane),
      requirement(requirement)
    {}

    bool            sameLane    = false;
    bool            otherLane   = false;
    CardRequirement requirement;
  };

  struct EmblemPlayerActionPlayFromDiscard {
    EmblemPlayerActionPlayFromDiscard(void) = default;
    EmblemPlayerActionPlayFromDiscard(
      bool                    sameLane,
      bool                    otherLane,
      CardRequirement const & requirement
    ) :
      sameLane(sameLane),
      otherLane(otherLane),
      requirement(requirement)
    {}

    bool            sameLane    = false;
    bool            otherLane   = false;
    CardRequirement requirement;
  };

  struct EmblemPlayerActionClaimLane {
    EmblemPlayerActionClaimLane(void) = default;
    EmblemPlayerActionClaimLane(
      bool sameLane,
      bool otherLane
    ) :
      sameLane(sameLane),
      otherLane(otherLane)
    {}

    bool sameLane  = false;
    bool otherLane = false;
  };

  struct EmblemPlayerActionRedraw {
  };

  using EmblemPlayerAction = Variant<
    EmblemPlayerActionType,

    EmblemPlayerActionDrawCard,
    EmblemPlayerActionDiscard,
    EmblemPlayerActionPlayCard,
    EmblemPlayerActionClaimLane,
    EmblemPlayerActionPlayFromDiscard,
    EmblemPlayerActionRedraw
  >;

  static int const c_activeEffectMaxActionCount = 3;

  enum class EmblemActionType {
    PlayerAction,
    TargetAction,

    Count
  };

  using EmblemAction = Variant<
    EmblemActionType,

    EmblemPlayerAction,
    EmblemTargetAction
  >;

  struct ActiveEffect {
    ActiveEffect(void) = default;

    ActiveEffect(
      EmblemTrigger const &                                            trigger,
      DynamicArray<EmblemAction, c_activeEffectMaxActionCount> const & actions
    ) :
      trigger(trigger),
      actions(actions)
    {}

    EmblemTrigger                                            trigger;
    DynamicArray<EmblemAction, c_activeEffectMaxActionCount> actions;
  };

  static int const c_emblemMaxPassiveEffects = 2;
  static int const c_emblemMaxActiveEffects  = 2;

  struct EmblemData {
    EmblemData(void) = default;

    EmblemData(
      std::string const &                                            name,
      DynamicArray<PassiveEffect, c_emblemMaxPassiveEffects> const & passiveEffects,
      DynamicArray<ActiveEffect, c_emblemMaxActiveEffects> const &   activeEffects
    ) :
      name(name),
      passiveEffects(passiveEffects),
      activeEffects(activeEffects)
    {}

    std::string                                            name           = "";
    DynamicArray<PassiveEffect, c_emblemMaxPassiveEffects> passiveEffects;
    DynamicArray<ActiveEffect, c_emblemMaxActiveEffects>   activeEffects;
  };

  struct CardLevelData {
    CardLevelData(void) = default;
    CardLevelData(int level) :
      level(level)
    {}

    int level = 0;
  };

  enum class CardResourceDataType {
    Produce,
    Consume,

    Count
  };

  struct CardResourceProduceData {
    CardResourceProduceData(void) = default;
    CardResourceProduceData(int resourceIndex0, int resourceIndex1) :
      resourceIndex0(resourceIndex0),
      resourceIndex1(resourceIndex1)
    {}


    int resourceIndex0 = c_invalidIndex;
    int resourceIndex1 = c_invalidIndex;
  };

  struct CardResourceConsumeData {
    CardResourceConsumeData(void) = default;
    CardResourceConsumeData(int resourceIndex) :
      resourceIndex(resourceIndex)
    {}

    int resourceIndex = c_invalidIndex;
  };

  using CardResourceData = Variant<
    CardResourceDataType,

    CardResourceProduceData,
    CardResourceConsumeData
  >;

  struct CardAlignmentData {
    CardAlignmentData(void) = default;
    CardAlignmentData(Alignment alignment) :
      alignment(alignment)
    {}

    Alignment alignment = Alignment::Count;
  };

  struct CardUtilityData {
    CardUtilityData(void) = default;
    CardUtilityData(int emblemIndex) :
      emblemIndex(emblemIndex)
    {}

    int emblemIndex = c_invalidIndex;
  };

  using CardStyleData = Variant<
    CardStyleType,

    CardLevelData,
    CardResourceData,
    CardAlignmentData,
    CardUtilityData
  >;

  struct CardData {
    CardData(void) = default;

    std::string   name;
    int           factionIndex = c_invalidIndex;
    int           emblemIndex  = c_invalidIndex;
    CardStyleData data;
  };

  enum class CardDecisionTreeType {
    Conditional,
    Chain,
    Result,

    Count
  };

  template <typename T>
  struct CardDecisionTreeConditional;
  template <typename T>
  struct CardDecisionTreeChain;
  template <typename T>
  struct CardDecisionTreeWeightedResult;

  template <typename T>
  using CardDecisionTree = Variant<
    CardDecisionTreeType,

    CardDecisionTreeConditional<T>,
    CardDecisionTreeChain<T>,
    CardDecisionTreeWeightedResult<T>
  >;

  template <typename T>
  struct CardDecisionTreeConditional {
    CardDecisionTreeConditional(void) = default;
    CardDecisionTreeConditional(
      CardRequirement const &     requirement,
      CardDecisionTree<T> const & result
    ) :
      requirement(requirement),
      result(std::make_shared<CardDecisionTree<T>>(result))
    {}

    CardRequirement                      requirement = CardRequirementNone();
    std::shared_ptr<CardDecisionTree<T>> result;
  };

  template <typename T>
  struct CardDecisionTreeChain {
    CardDecisionTreeChain(void) = default;
    CardDecisionTreeChain(std::initializer_list<CardDecisionTree<T>> const & i_chain);

    std::vector<CardDecisionTree<T>> chain;
  };

  template <typename T>
  struct CardDecisionTreeWeightedResult {
    CardDecisionTreeWeightedResult(void) = default;
    CardDecisionTreeWeightedResult(float weight, T const & value) :
      weight(weight),
      value(value)
    {}

    float weight = 1.0f;
    T     value  = T();
  };

  template <typename T>
  CardDecisionTreeChain<T>::CardDecisionTreeChain(
    std::initializer_list<CardDecisionTree<T>> const & i_chain
  ) {
    chain.reserve(i_chain.size());

    for (CardDecisionTree<T> const & tree : i_chain) {
      chain.emplace_back(tree);
    }
  }

  static char const c_vowels[] = "aeiou";
  static char const c_vowelCount = sizeof(c_vowels) / sizeof(*c_vowels) - 1;

  static char const c_consonants[] = "bcdfghjklmnprstv";
  static char const c_consonantCount = sizeof(c_consonants) / sizeof(*c_consonants) - 1;

  std::string GetRandomSyllable(void) {
    int const c_maxPreConsonants = 1;
    int const c_maxPostConsonants = 1;
    int const c_maxVowels = 1;

    int const preConsonants = std::rand() % (c_maxPreConsonants + 1);
    int const postConsonants =
      preConsonants == 0 ?
      ((std::rand() % c_maxPostConsonants) + 1) :
      std::rand() % (c_maxPostConsonants + 1)
    ;

    int const vowels = (std::rand() % c_maxVowels) + 1;

    std::string result = "";

    for (int i = 0; i < preConsonants; ++i) {
      result.push_back(c_consonants[std::rand() % c_consonantCount]);
    }

    for (int i = 0; i < vowels; ++i) {
      result.push_back(c_vowels[std::rand() % c_vowelCount]);
    }

    for (int i = 0; i < postConsonants; ++i) {
      result.push_back(c_consonants[std::rand() % c_consonantCount]);
    }

    return result;
  }

  static int const c_minyllablesForWord  = 2;
  static int const c_maxSyllablesForWord = 3;

  std::string GetRandomName(void) {
    int const syllables = (std::rand() % (c_maxSyllablesForWord - c_minyllablesForWord + 1)) + c_minyllablesForWord;

    std::string result = "";

    for (int i = 0; i < syllables; ++i) {
      result.append(GetRandomSyllable());
    }

    result[0] += 'A' - 'a';

    return result;
  }

  static int const c_minLevel = 2;
  static int const c_maxLevel = 5;

  int GetRandomLevel(void) {
    return std::rand() % (c_maxLevel - c_minLevel + 1) + c_minLevel;
  }

  static int const c_factionEmblemCount    = 7;
  static int const c_factionResourceCount  = 4;
  static int const c_factionAlignmentCount = 3;

  struct ResourceTableRow {
    ResourceTableRow(void) = default;
    ResourceTableRow(std::string const & name) :
      name(name)
    {}

    std::string name;
  };

  struct FactionTableRow {
    FactionTableRow(void) = default;
    FactionTableRow(
      std::string const &                                      name,
      DynamicArray<int, c_factionEmblemCount> const &          emblems,
      DynamicArray<int, c_factionResourceCount> const &        resources,
      DynamicArray<Alignment, c_factionAlignmentCount> const & alignments
    ) :
      name(name),
      emblems(emblems),
      resources(resources),
      alignments(alignments)
    {}

    std::string                                      name;
    DynamicArray<int, c_factionEmblemCount>          emblems;
    DynamicArray<int, c_factionResourceCount>        resources;
    DynamicArray<Alignment, c_factionAlignmentCount> alignments;
  };

  enum: int {
    defeatIndex,
    gustIndex,
    hookIndex,
    searchIndex,
    recruitIndex,
    claimIndex,
    reviveIndex,
    recallIndex,
    reevaluateIndex,
    mimicryIndex,

    invincibleIndex,
    defenderIndex,
    anchoredIndex,
    regenerationIndex,
    strongIndex,
    rallyIndex,
    reactiveIndex,
    flightIndex,
    refineIndex,
    assistantIndex,

    emblemCount,
  };

  static auto const t_emblemData = MakeHeaderlessDataTable({
    std::make_pair(int(defeatIndex), EmblemData(
      "Defeat",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemTargetAction(
              CardRequirementLane(),
              EmblemTargetActionDestroy()
            ),
          }
        ),
      }
    )),
    std::make_pair(int(gustIndex), EmblemData(
      "Gust",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemTargetAction(
              CardRequirementLane(),
              EmblemTargetActionMoveToLane(
                false, //sameLane
                true   //otherLane
              )
            ),
          }
        ),
      }
    )),
    std::make_pair(int(hookIndex), EmblemData(
      "Hook",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemTargetAction(
              CardRequirementNot(CardRequirementLane()),
              EmblemTargetActionMoveToLane(
                true, //sameLane
                false //otherLane
              )
            ),
          }
        ),
      }
    )),
    std::make_pair(int(searchIndex), EmblemData(
      "Search",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemPlayerAction(EmblemPlayerActionDrawCard()),
          }
        ),
      }
    )),
    std::make_pair(int(recruitIndex), EmblemData(
      "Recruit",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemPlayerAction(EmblemPlayerActionPlayCard(
              true,                 //sameLane
              false,                //otherLane
              CardRequirementNone()
            )),
          }
        ),
      }
    )),
    std::make_pair(int(claimIndex), EmblemData(
      "Claim",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemPlayerAction(EmblemPlayerActionClaimLane(
              true, //sameLane
              false //otherLane
            )),
          }
        ),
      }
    )),
    std::make_pair(int(reviveIndex), EmblemData(
      "Revive",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemPlayerAction(EmblemPlayerActionPlayFromDiscard(
              true,                 //sameLane
              false,                //otherLane
              CardRequirementNone()
            )),
          }
        ),
      }
    )),
    std::make_pair(int(recallIndex), EmblemData(
      "Recall",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemTargetAction(
              CardRequirementLane(),
              EmblemTargetActionMoveToHand()
            ),
          }
        ),
      }
    )),
    std::make_pair(int(reevaluateIndex), EmblemData(
      "Reevaluate",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemPlayerAction(EmblemPlayerActionRedraw()),
          }
        ),
      }
    )),
    std::make_pair(int(mimicryIndex), EmblemData(
      "Mimicry",
      {},
      {
        ActiveEffect(
          EmblemCardTrigger(
            EmblemCardTriggerType::OnActivate,
            CardRequirementThisCard()
          ),
          {
            EmblemTargetAction(
              CardRequirementAnd({
                CardRequirementLane(),
                CardRequirementNot(CardRequirementActivated())
              }),
              EmblemTargetActionActivate()
            ),
          }
        ),
      }
    )),

    std::make_pair(int(invincibleIndex), EmblemData(
      "Invincible",
      {
        PassiveEffectCannotBeActioned(EmblemTargetActionType::Destroy),
      },
      {}
    )),
    std::make_pair(int(defenderIndex), EmblemData(
      "Defender",
      {
        PassiveEffectForOthers(
          CardRequirementAnd({
            CardRequirementControlled(),
            CardRequirementNot(
              CardRequirementEmblem(defenderIndex)
            ),
          }),
          PassiveEffectCannotBeTargetted()
        ),
      },
      {}
    )),
    std::make_pair(int(anchoredIndex), EmblemData(
      "Anchored",
      {
        PassiveEffectCannotBeActioned(EmblemTargetActionType::MoveToLane),
        PassiveEffectCannotBeActioned(EmblemTargetActionType::MoveToDeck),
        PassiveEffectCannotBeActioned(EmblemTargetActionType::MoveToHand),
      },
      {}
    )),
    std::make_pair(int(regenerationIndex), EmblemData(
      "Regeneration",
      {
        PassiveEffectCanBePlayedFromDiscard(
          CardRequirementOwner()
        ),
      },
      {}
    )),
    std::make_pair(int(strongIndex), EmblemData(
      "Strong",
      {
        PassiveEffectGrantControlPoint(
          CardRequirementNone()
        ),
      },
      {}
    )),
    std::make_pair(int(rallyIndex), EmblemData(
      "Rally",
      {},
      {}
    )),
    std::make_pair(int(reactiveIndex), EmblemData(
      "Reactive",
      {},
      {}
    )),
    std::make_pair(int(flightIndex), EmblemData(
      "Flight",
      {},
      {}
    )),
    std::make_pair(int(refineIndex), EmblemData(
      "Refine",
      {},
      {}
    )),
    std::make_pair(int(assistantIndex), EmblemData(
      "Assistant",
      {},
      {}
    )),
  });

  enum: int {
    goldIndex,
    materialsIndex,
    animalsIndex,
    foodIndex,
    toolsIndex,
    musicIndex,
    shadowIndex,
    plantsIndex,
    treasureIndex,
    peopleIndex,

    resourceCount
  };

  static auto const t_resourceData = MakeHeaderlessDataTable({
    std::make_pair(int(goldIndex),        ResourceTableRow("Gold")),
    std::make_pair(int(materialsIndex),   ResourceTableRow("Materials")),
    std::make_pair(int(animalsIndex),     ResourceTableRow("Animals")),
    std::make_pair(int(foodIndex),        ResourceTableRow("Food")),
    std::make_pair(int(toolsIndex),       ResourceTableRow("Tools")),
    std::make_pair(int(musicIndex),       ResourceTableRow("Music")),
    std::make_pair(int(shadowIndex),      ResourceTableRow("Shadow")),
    std::make_pair(int(plantsIndex),      ResourceTableRow("Plants")),
    std::make_pair(int(treasureIndex),    ResourceTableRow("Treasure")),
    std::make_pair(int(peopleIndex),      ResourceTableRow("People")),
  });

  static auto const t_factionData = MakeHeaderlessDataTable({
    FactionTableRow(
      "Ranthelok Conclave",
      {
        flightIndex,
        anchoredIndex,
        recruitIndex,
        reevaluateIndex,
        searchIndex,
        hookIndex,
        regenerationIndex,
      },
      {
        shadowIndex,
        plantsIndex,
        goldIndex,
        treasureIndex,
      },
      {
        Alignment::Fire,
        Alignment::Earth,
        Alignment::Water,
      }
    ),
    FactionTableRow(
      "Matheran's Seekers",
      {
        recallIndex,
        flightIndex,
        recruitIndex,
        claimIndex,
        reviveIndex,
        invincibleIndex,
        regenerationIndex,
      },
      {
        peopleIndex,
        shadowIndex,
        plantsIndex,
        musicIndex,
      },
      {
        Alignment::Air,
        Alignment::Fire,
        Alignment::Light,
      }
    ),
    FactionTableRow(
      "Antathalyn Seamarket",
      {
        assistantIndex,
        reevaluateIndex,
        rallyIndex,
        regenerationIndex,
        recallIndex,
        reviveIndex,
        claimIndex,
      },
      {
        peopleIndex,
        animalsIndex,
        materialsIndex,
        treasureIndex,
      },
      {
        Alignment::Water,
        Alignment::Earth,
        Alignment::Air,
      }
    ),
    FactionTableRow(
      "Embrial Overcity",
      {
        refineIndex,
        recruitIndex,
        claimIndex,
        anchoredIndex,
        regenerationIndex,
        assistantIndex,
        flightIndex,
      },
      {
        materialsIndex,
        plantsIndex,
        shadowIndex,
        musicIndex,
      },
      {
        Alignment::Air,
        Alignment::Light,
        Alignment::Water,
      }
    ),
    FactionTableRow(
      "Darivaan Cliffkeep",
      {
        regenerationIndex,
        searchIndex,
        recallIndex,
        flightIndex,
        defeatIndex,
        mimicryIndex,
        hookIndex,
      },
      {
        toolsIndex,
        peopleIndex,
        musicIndex,
        goldIndex,
      },
      {
        Alignment::Earth,
        Alignment::Fire,
        Alignment::Light,
      }
    ),
  });

  int GetRandomFactionIndex(void) {
    return std::rand() % t_factionData.Size();
  }

  bool MatchesRequirement(CardData const & currentCard, CardRequirement const & requirement) {
    switch (requirement.GetType()) {
      case CardRequirementType::Level: {
        int const cardLevel =
          (currentCard.data.GetType() == CardStyleType::Level) ?
          (currentCard.data.Get<CardStyleType::Level>().level) :
          0
        ;

        int const reqLevel = requirement.Get<CardRequirementType::Level>().level;

        switch (requirement.Get<CardRequirementType::Level>().op) {
          case ComparisonOperator::LessThan:           return cardLevel <  reqLevel;
          case ComparisonOperator::GreaterThan:        return cardLevel >  reqLevel;
          case ComparisonOperator::Equal:              return cardLevel == reqLevel;
          case ComparisonOperator::NotEqual:           return cardLevel != reqLevel;
          case ComparisonOperator::LessThanOrEqual:    return cardLevel <= reqLevel;
          case ComparisonOperator::GreaterThanOrEqual: return cardLevel >= reqLevel;
          default:                                     return false;
        }
      } break;

      case CardRequirementType::Emblem: {
        DynamicArray<int, 2> emblemIndices;

        emblemIndices.Emplace(currentCard.emblemIndex);

        if (currentCard.data.GetType() == CardStyleType::Utility) {
          emblemIndices.Emplace(currentCard.data.Get< CardStyleType::Utility>().emblemIndex);
        }

        for (int emblemIndex : emblemIndices) {
          if (emblemIndex == requirement.Get<CardRequirementType::Emblem>().emblem) {
            return true;
          }
        }
        return false;
      } break;

      case CardRequirementType::Faction: {
        return currentCard.factionIndex == requirement.Get<CardRequirementType::Faction>().faction;
      } break;

      case CardRequirementType::And: {
        for (std::shared_ptr<CardRequirement> const & require : requirement.Get<CardRequirementType::And>().requirements) {
          if (!require || !MatchesRequirement(currentCard, *require)) {
            return false;
          }
        }
        return true;
      } break;

      case CardRequirementType::Or: {
        for (std::shared_ptr<CardRequirement> const & require : requirement.Get<CardRequirementType::Or>().requirements) {
          if (require && MatchesRequirement(currentCard, *require)) {
            return true;
          }
        }
        return false;
      } break;

      case CardRequirementType::Not: {
        std::shared_ptr<CardRequirement> const & require = requirement.Get<CardRequirementType::Not>().requirement;
        return !(require && MatchesRequirement(currentCard, *require));
      } break;

      default: {
        return true;
      } break;
    }
  }

  template <typename T, int Size>
  T const & RandomElement(DynamicArray<T, Size> const & arr) {
    return arr[std::rand() % arr.Count()];
  }

  template <typename T, int Size>
  T & RandomElement(DynamicArray<T, Size> & arr) {
    return arr[std::rand() % arr.Count()];
  }

  template <typename T, int Size>
  bool Contains(DynamicArray<T, Size> const & arr, T const & value) {
    for (T const & val : arr) {
      if (val == value) {
        return true;
      }
    }
    return false;
  }

  template <typename T, int Size>
  T const & RandomWeightedElement(
    DynamicArray<T, Size> const &           arr,
    std::function<float(T const &)> const & weightFunc
  ) {
    float     total = 0.0f;
    T const * value = nullptr;

    for (T const & val : arr) {
      float const weight = weightFunc;

      total += weight;

      if (GetRandom() * total < weight) {
        value = &val;
      }
    }

    return *value;
  }

  template <typename T, int Size>
  T & RandomWeightedElement(
    DynamicArray<T, Size> &                 arr,
    std::function<float(T const &)> const & weightFunc
  ) {
    return RandomWeightedElement(const_cast<DynamicArray<T, Size> const &>(arr));
  }

  std::string GetRandomTitle(CardData const &) {
      return GetRandomName();
  };

  int GetRandomEmblem(CardData const & card) {
    int newEmblemIndex;
    do {
      newEmblemIndex = RandomElement(t_factionData[card.factionIndex].emblems);
      
      if (MatchesRequirement(card, CardRequirementEmblem(newEmblemIndex))) {
        newEmblemIndex = c_invalidIndex;
      }
    } while (newEmblemIndex == c_invalidIndex);
    
    return newEmblemIndex;
  }

  CardStyleData GetRandomCardStyleData(CardData const & card) {
    CardStyleType const type = CardStyleType(std::rand() % int(CardStyleType::Count));

    switch (type) {
      case CardStyleType::Level: {
        return CardLevelData(std::rand() % (c_maxLevel - c_minLevel + 1) + c_minLevel);
      } break;

      case CardStyleType::Resource: {
        CardResourceDataType const resourceType = CardResourceDataType(std::rand() % int(CardResourceDataType::Count));

        switch (resourceType) {
          case CardResourceDataType::Produce: {
            auto const & resourceTypes = t_factionData[card.factionIndex].resources;

            int resourceIndex0 = RandomElement(resourceTypes);

            int resourceIndex1;
            do {
              resourceIndex1 = RandomElement(resourceTypes);
            } while (resourceIndex1 == resourceIndex0);

            if (resourceIndex0 > resourceIndex1) {
              std::swap(resourceIndex0, resourceIndex1);
            }

            return CardResourceData(CardResourceProduceData(resourceIndex0, resourceIndex1));
          } break;

          case CardResourceDataType::Consume: {
            int const resourceIndex = RandomElement(t_factionData[card.factionIndex].resources);

            return CardResourceData(CardResourceConsumeData(resourceIndex));
          } break;

          default: {
            return CardResourceData();
          } break;
        }

      } break;

      case CardStyleType::Alignment: {
        Alignment const alignment = RandomElement(t_factionData[card.factionIndex].alignments);
        return CardAlignmentData(alignment);
      } break;

      case CardStyleType::Utility: {
        return CardUtilityData(GetRandomEmblem(card));
      } break;
    }

    return CardStyleData();
  }

  CardData CreateCard(int faction = c_invalidIndex) {
    CardData result;

    if (faction == c_invalidIndex) {
      faction = GetRandomFactionIndex();
    }

    result.factionIndex = faction;
    result.emblemIndex  = GetRandomEmblem(result);
    result.data         = GetRandomCardStyleData(result);
    result.name         = GetRandomTitle(result);

    return result;
  };
}

void CreateAndPrintCard(void) {
  CardData card = CreateCard();

  std::cout << card.name << ": " << std::endl;

  std::cout << "  " << t_factionData[card.factionIndex].name << std::endl;

  switch (card.data.GetType()) {
    case CardStyleType::Level: {
      std::cout << "  Level: " << card.data.Get<CardStyleType::Level>().level << std::endl;
    } break;

    case CardStyleType::Resource: {
      CardResourceData const & data = card.data.Get<CardStyleType::Resource>();

      switch (data.GetType()) {
        case CardResourceDataType::Produce: {
          CardResourceProduceData const & produce = data.Get<CardResourceDataType::Produce>();

          std::cout <<
            "  Produce: " <<
            t_resourceData[produce.resourceIndex0].name <<
            ", " <<
            t_resourceData[produce.resourceIndex1].name <<
            std::endl
            ;
        } break;
        case CardResourceDataType::Consume: {
          CardResourceConsumeData const & consume = data.Get<CardResourceDataType::Consume>();
          std::cout << "  Consume: " << t_resourceData[consume.resourceIndex].name << std::endl;
        } break;

        default: {
        }  break;
      }
    } break;

    case CardStyleType::Alignment: {
      std::cout << "  Alignment: " << GetAlignmentText(card.data.Get<CardStyleType::Alignment>().alignment) << std::endl;
    } break;

    case CardStyleType::Utility: {
      std::cout << "  Utility" << std::endl;
    } break;

    default: {
    }  break;
  }

  std::string utilityEmblemText = "";

  if (card.data.GetType() == CardStyleType::Utility) {
    utilityEmblemText = ", " + t_emblemData[card.data.Get<CardStyleType::Utility>().emblemIndex].name;
  }

  std::cout << "  " << t_emblemData[card.emblemIndex].name << utilityEmblemText << std::endl;
}

void CreateAndPrintFaction(void) {

  std::cout << "FactionTableRow(" << std::endl;
  std::cout << "  \"Name\"," << std::endl;

  std::cout << "  {" << std::endl;
  DynamicArray<int, c_factionEmblemCount> emblems;
  while (!emblems.Full()) {
    int const newEmblem = std::rand() % t_emblemData.Size();

    if (!Contains(emblems, newEmblem)) {
      emblems.Emplace(newEmblem);
    }
  }

  for (int emblemIndex : emblems) {
    std::string indexName = t_emblemData[emblemIndex].name + "Index";

    indexName[0] += 'a' - 'A';

    std::cout << "    " << indexName << "," << std::endl;
  }
  std::cout << "  }," << std::endl;

  std::cout << "  {" << std::endl;
  DynamicArray<int, c_factionResourceCount> resources;
  while (!resources.Full()) {
    int const newResource = std::rand() % t_resourceData.Size();

    if (!Contains(resources, newResource)) {
      resources.Emplace(newResource);
    }
  }

  for (int resourceIndex : resources) {
    std::string indexName = t_resourceData[resourceIndex].name + "Index";

    indexName[0] += 'a' - 'A';

    std::cout << "    " << indexName << "," << std::endl;
  }
  std::cout << "  }," << std::endl;

  std::cout << "  {" << std::endl;
  DynamicArray<Alignment, c_factionAlignmentCount> alignments;
  while (!alignments.Full()) {
    Alignment const newAlignment = Alignment(std::rand() % int(Alignment::Count));

    if (!Contains(alignments, newAlignment)) {
      alignments.Emplace(newAlignment);
    }
  }

  for (Alignment alignment : alignments) {
    std::cout << "    " << "Alignment::" << GetAlignmentText(alignment) << "," << std::endl;
  }
  std::cout << "  }" << std::endl;
  std::cout << ")," << std::endl;
}

void CountAndPrintOccurances(void) {
  int emblemOccurances[emblemCount]              = { 0 };
  int resourceOccurances[resourceCount]          = { 0 };
  int alignmentOccurances[int(Alignment::Count)] = { 0 };

  for (auto const & faction : t_factionData) {
    for (int emblemIndex : faction.emblems) {
      ++emblemOccurances[emblemIndex];
    }
    for (int resourceIndex : faction.resources) {
      ++resourceOccurances[resourceIndex];
    }
    for (Alignment alignment : faction.alignments) {
      ++alignmentOccurances[int(alignment)];
    }
  }

  int minAlignment = 0;

  for (auto const & emblem : t_emblemData) {
    if (emblem.name.length() > minAlignment) {
      minAlignment = emblem.name.length();
    }
  }
  for (auto const & resource : t_resourceData) {
    if (resource.name.length() > minAlignment) {
      minAlignment = resource.name.length();
    }
  }
  for (int i = 0; i < int(Alignment::Count); ++i) {
    if (GetAlignmentText(Alignment(i)).length() > minAlignment) {
      minAlignment = GetAlignmentText(Alignment(i)).length();
    }
  }

  minAlignment += 2;



  std::cout << "Emblems:" << std::endl;
  for (int i = 0; i < emblemCount; ++i) {
    int const alignCount = 1 + std::max(0, minAlignment - int(t_emblemData[i].name.length()));
    std::string const alignStr(alignCount, ' ');
    std::string const visual(emblemOccurances[i], 'X');

    std::cout << "  " << t_emblemData[i].name << ":" << alignStr << emblemOccurances[i] << " " << visual << std::endl;
  }

  std::cout << "Resources:" << std::endl;
  for (int i = 0; i < resourceCount; ++i) {
    int const alignCount = 1 + std::max(0, minAlignment - int(t_resourceData[i].name.length()));
    std::string const alignStr(alignCount, ' ');
    std::string const visual(resourceOccurances[i], 'X');

    std::cout << "  " << t_resourceData[i].name << ":" << alignStr << resourceOccurances[i] << " " << visual << std::endl;
  }

  std::cout << "Alignments:" << std::endl;
  for (int i = 0; i < int(Alignment::Count); ++i) {
    int const alignCount = 1 + std::max(0, minAlignment - int(GetAlignmentText(Alignment(i)).length()));
    std::string const alignStr(alignCount, ' ');
    std::string const visual(alignmentOccurances[i], 'X');

    std::cout << "  " << GetAlignmentText(Alignment(i)) << ":" << alignStr << alignmentOccurances[i] << " " << visual << std::endl;
  }

  std::cout << std::endl;
}

int main(void) {
  std::srand(std::time(nullptr));

  CountAndPrintOccurances();
  std::cin.get();

  while (true) {
    //CreateAndPrintCard();
    CreateAndPrintFaction();

    std::cin.get();
  }

  return 0;
};
