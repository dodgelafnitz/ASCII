/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <algorithm>
#include <ctime>
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

  template <auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<EmptyHeader, Element, Count, ExpectedCountType<ExpectedCount>> MakeHeaderlessDataTable(
    std::pair<ExpectedCountType<ExpectedCount>, Element> const (&array)[Count]
  ) {
    return MakeDataTable<EmptyHeader, ExpectedCount, Element, Count>(array);
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

  enum class Emblem {
    Ambusher,
    Anchored,
    Betray,
    Cowardly,
    Defeat,
    Defender,
    Discard,
    Draw,
    Enduring,
    Ephemeral,
    Feint,
    Follower,
    Gift,
    Hidden,
    Invincible,
    Loyal,
    Play,
    Prepare,
    Pull,
    Rally,
    Reactive,
    Recall,
    Recovery,
    Recruit,
    Regeneration,
    Regroup,
    Revive,
    Revolve,
    Sacrifice,
    Send,
    Strong,
    Supplied,
    Unique,
    Weak,

    Count
  };

  enum class Faction {
    GilbrandsArmory,
    CloufaraSeers,
    //Mountaineers,
    //VeiledOnes,
    //Pennywrights,
    //StonewoodBeasts,
    //KingdomOfOwlcliff,

    Count
  };

  enum class CardRequirementType {
    Level,
    Emblem,
    Faction,
    Owner,
    Controller,
    Lane,
    None,
    And,
    Or,
    Not,

    Count
  };

  static int const c_requirementCount = 8;

  struct CardRequirementLevel;
  struct CardRequirementEmblem;
  struct CardRequirementFaction;
  struct CardRequirementOwner;
  struct CardRequirementController;
  struct CardRequirementLane;
  struct CardRequirementNone;
  struct CardRequirementAnd;
  struct CardRequirementOr;
  struct CardRequirementNot;

  using CardRequirement = Variant<
    CardRequirementType,

    CardRequirementLevel,
    CardRequirementEmblem,
    CardRequirementFaction,
    CardRequirementOwner,
    CardRequirementController,
    CardRequirementLane,
    CardRequirementNone,
    CardRequirementAnd,
    CardRequirementOr,
    CardRequirementNot
  >;

  struct CardRequirementLevel {
    CardRequirementLevel(void) = default;
    CardRequirementLevel(int level) :
      level(level)
    {}

    int level = 0;
  };

  struct CardRequirementEmblem {
    CardRequirementEmblem(void) = default;
    CardRequirementEmblem(Emblem emblem) :
      emblem(emblem)
    {}

    Emblem emblem = Emblem::Count;
  };

  struct CardRequirementFaction {
    CardRequirementFaction(void) = default;
    CardRequirementFaction(Faction faction) :
      faction(faction)
    {}

    Faction faction = Faction::Count;
  };

  struct CardRequirementOwner {
  };

  struct CardRequirementController {
  };

  struct CardRequirementLane {
  };

  struct CardRequirementNone {
  };

  struct CardRequirementAnd {
    CardRequirementAnd(void) = default;
    CardRequirementAnd(DynamicArray<std::shared_ptr<CardRequirement>, c_requirementCount> const & requirements) :
      requirements(requirements)
    {}

    DynamicArray<std::shared_ptr<CardRequirement>, c_requirementCount> requirements;
  };

  struct CardRequirementOr {
    CardRequirementOr(void) = default;
    CardRequirementOr(DynamicArray<std::shared_ptr<CardRequirement>, c_requirementCount> const & requirements) :
      requirements(requirements)
    {}

    DynamicArray<std::shared_ptr<CardRequirement>, c_requirementCount> requirements;
  };

  struct CardRequirementNot {
    CardRequirementNot(void) = default;
    CardRequirementNot(std::shared_ptr<CardRequirement> const & requirement) :
      requirement(requirement)
    {}

    std::shared_ptr<CardRequirement> requirement;
  };

  struct EmblemData {
    EmblemData(void) = default;

    EmblemData(
      std::string const &     name,
      int                     generationMod,
      CardRequirement const & cardRequirement
    ) :
      name(name),
      generationMod(generationMod),
      cardRequirement(cardRequirement)
    {}

    std::string     name            = "";
    int             generationMod   = 0;
    CardRequirement cardRequirement = CardRequirementNone();
  };

  static auto const t_emblemData = MakeHeaderlessDataTable<Emblem::Count>({
    std::make_pair(Emblem::Ambusher, EmblemData(
      "Ambusher",
      -4,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Anchored, EmblemData(
      "Anchored",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Betray, EmblemData(
      "Betray",
      -4,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Cowardly, EmblemData(
      "Cowardly",
      +2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Defeat, EmblemData(
      "Defeat",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Defender, EmblemData(
      "Defender",
      -1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Discard, EmblemData(
      "Discard",
      +1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Draw, EmblemData(
      "Draw",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Enduring, EmblemData(
      "Enduring",
      -4,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Ephemeral, EmblemData(
      "Ephemeral",
      +2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Feint, EmblemData(
      "Feint",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Follower, EmblemData(
      "Follower",
      +1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Gift, EmblemData(
      "Gift",
      +2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Hidden, EmblemData(
      "Hidden",
      -1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Invincible, EmblemData(
      "Invincible",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Loyal, EmblemData(
      "Loyal",
      -1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Play, EmblemData(
      "Play",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Prepare, EmblemData(
      "Prepare",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Pull, EmblemData(
      "Pull",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Rally, EmblemData(
      "Rally",
      -4,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Reactive, EmblemData(
      "Reactive",
      -1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Recall, EmblemData(
      "Recall",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Recovery, EmblemData(
      "Recovery",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Recruit, EmblemData(
      "Recruit",
      -4,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Regeneration, EmblemData(
      "Regeneration",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Regroup, EmblemData(
      "Regroup",
      -1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Revive, EmblemData(
      "Revive",
      -4,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Revolve, EmblemData(
      "Revolve",
      -4,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Sacrifice, EmblemData(
      "Sacrifice",
      +2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Send, EmblemData(
      "Send",
      -2,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Strong, EmblemData(
      "Strong",
      -1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Supplied, EmblemData(
      "Supplied",
      -1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Unique, EmblemData(
      "Unique",
      +1,
      CardRequirementNone()
    )),
    std::make_pair(Emblem::Weak, EmblemData(
      "Weak",
      +1,
      CardRequirementNone()
    )),
  });

  static int const c_cardMaxFactions = 2;
  static int const c_cardMaxEmblems  = 5;

  struct CardData {
    CardData(void) = default;

    CardData(
      std::string const &                              name,
      int                                              level,
      bool                                             heroic,
      DynamicArray<Faction, c_cardMaxFactions> const & factions,
      DynamicArray<Emblem, c_cardMaxEmblems> const &   emblems
    ) :
      name(name),
      level(level),
      heroic(heroic),
      factions(factions),
      emblems(emblems)
    {}

    std::string                              name;
    int                                      level    = 0;
    bool                                     heroic   = false;
    DynamicArray<Faction, c_cardMaxFactions> factions;
    DynamicArray<Emblem, c_cardMaxEmblems>   emblems;
  };

  static int const c_factionPrimaryEmblemCount   = 5;
  static int const c_factionSecondaryEmblemCount = 12;

  enum class TitlePartType {
    Adjective,
    Noun,
    FormalName,
    PossesiveName,

    Count
  };

  // Noun of FormalName
  // Adjective Noun
  // PossesiveName Noun

  struct TitlePart {
    TitlePart(void) = default;
    TitlePart(std::string const & titlePart, TitlePartType type, CardRequirement const & requirement) :
      titlePart(titlePart),
      type(type),
      requirement(requirement)
    {}

    std::string     titlePart   = "";
    TitlePartType   type        = TitlePartType::Count;
    CardRequirement requirement = CardRequirementNone();
  };

  static int const c_maxFactionTitleParts = 50;

  struct FactionGenerationData {
    FactionGenerationData(void) = default;

    FactionGenerationData(
      std::string const & name,
      DynamicArray<Emblem, c_factionPrimaryEmblemCount> const &   primaryEmblems,
      DynamicArray<Emblem, c_factionSecondaryEmblemCount> const & secondaryEmblems,
      DynamicArray<TitlePart, c_maxFactionTitleParts> const &     titleParts
    ) :
      name(name),
      primaryEmblems(primaryEmblems),
      secondaryEmblems(secondaryEmblems),
      titleParts(titleParts)
    {}

    std::string                                         name;
    DynamicArray<Emblem, c_factionPrimaryEmblemCount>   primaryEmblems;
    DynamicArray<Emblem, c_factionSecondaryEmblemCount> secondaryEmblems;
    DynamicArray<TitlePart, c_maxFactionTitleParts>     titleParts;
  };

  static auto const t_factionData = MakeHeaderlessDataTable<Faction::Count>({
    std::make_pair(Faction::GilbrandsArmory, FactionGenerationData(
      "Gilbrand's Armory",
      {
        Emblem::Loyal,
        Emblem::Strong,

        Emblem::Invincible,
        Emblem::Recovery,

        Emblem::Rally,
      },
      {
        Emblem::Cowardly,

        Emblem::Follower,
        Emblem::Unique,

        Emblem::Defender,
        Emblem::Reactive,
        Emblem::Supplied,

        Emblem::Anchored,
        Emblem::Prepare,
        Emblem::Send,

        Emblem::Betray,
        Emblem::Recruit,
      },
      {
        TitlePart(
          "Scouthound",
          TitlePartType::Noun,
          CardRequirementEmblem(Emblem::Recovery)
        ),
      }
    )),
    std::make_pair(Faction::CloufaraSeers, FactionGenerationData(
      "Cloufara Seers",
      {
        Emblem::Regroup,
        Emblem::Supplied,

        Emblem::Draw,
        Emblem::Feint,

        Emblem::Revolve,
      },
      {
        Emblem::Sacrifice,

        Emblem::Discard,
        Emblem::Weak,

        Emblem::Defender,
        Emblem::Hidden,
        Emblem::Reactive,

        Emblem::Play,
        Emblem::Prepare,
        Emblem::Recall,

        Emblem::Ambusher,
        Emblem::Revive,
      },
      {
      }
    )),
  });

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

  std::string GetRandomTitle(Faction faction) {
    return ""; //TODO
  };

  static int const c_maxLevel = 5;

  int GetRandomLevel(void) {
    return std::rand() % (c_maxLevel + 1);
  }

  Faction GetRandomFaction(void) {
    return Faction(std::rand() % int(Faction::Count));
  }

  bool EvaluateEmblemRequirements(int level, DynamicArray<Emblem, c_cardMaxEmblems> const & currentEmblems, CardRequirement const & requirement) {
    switch (requirement.GetType()) {
      case CardRequirementType::Level: {
        return level == requirement.Get<CardRequirementType::Level>().level;
      } break;

      case CardRequirementType::Emblem: {
        for (Emblem emblem : currentEmblems) {
          if (emblem == requirement.Get<CardRequirementType::Emblem>().emblem) {
            return true;
          }
        }
        return false;
      } break;

      case CardRequirementType::And: {
        for (std::shared_ptr<CardRequirement> const & require : requirement.Get<CardRequirementType::And>().requirements) {
          if (!require || !EvaluateEmblemRequirements(level, currentEmblems, *require)) {
            return false;
          }
        }
        return true;
      } break;

      case CardRequirementType::Or: {
        for (std::shared_ptr<CardRequirement> const & require : requirement.Get<CardRequirementType::Or>().requirements) {
          if (require && EvaluateEmblemRequirements(level, currentEmblems, *require)) {
            return true;
          }
        }
        return false;
      } break;

      case CardRequirementType::Not: {
        std::shared_ptr<CardRequirement> const & require = requirement.Get<CardRequirementType::Not>().requirement;
        return !(require && EvaluateEmblemRequirements(level, currentEmblems, *require));
      } break;

      default: {
        return true;
      } break;
    }
  }

  template <typename T, int Count>
  bool Contains(DynamicArray<T, Count> const & arr, T const & val) {
    for (T const & arrVal : arr) {
      if (val == arrVal) {
        return true;
      }
    }
    return false;
  }

  CardData CreateCard(int level = GetRandomLevel(), Faction faction = Faction::Count, bool heroic = ((std::rand() % 6) == 0)) {
    CardData result;

    result.name   = GetRandomName();
    result.level  = level;
    result.heroic = heroic;

    int const factionCount = std::min((std::rand() % c_cardMaxFactions) + 1, (std::rand() % c_cardMaxFactions) + 1);

    for (int i = 0; i < factionCount; ++i) {
      if (i == 0 && faction != Faction::Count) {
        result.factions.Emplace(faction);
      }

      Faction newFaction;
      bool    validFaction;
      do {
        newFaction   = GetRandomFaction();
        validFaction = !Contains(result.factions, newFaction);
      } while (!validFaction);

      result.factions.Emplace(newFaction);
    }

    static const int c_maxAllowedEmblems = (c_factionPrimaryEmblemCount + c_factionSecondaryEmblemCount) * c_cardMaxFactions;
    DynamicArray<Emblem, c_maxAllowedEmblems> allowedEmblems;

    static const int c_maxPrimaryEmblems = c_factionPrimaryEmblemCount * c_cardMaxFactions;
    DynamicArray<Emblem, c_maxAllowedEmblems> primaryEmblems;

    for (Faction faction : result.factions) {
      FactionGenerationData const & factionData = t_factionData[faction];

      for (Emblem emblem : factionData.primaryEmblems) {
        if (!Contains(primaryEmblems, emblem)) {
          primaryEmblems.Emplace(emblem);
          allowedEmblems.Emplace(emblem);
        }
      }
    }

    for (Faction faction : result.factions) {
      FactionGenerationData const & factionData = t_factionData[faction];

      for (Emblem emblem : factionData.secondaryEmblems) {
        if (!Contains(allowedEmblems, emblem)) {
          allowedEmblems.Emplace(emblem);
        }
      }
    }

    int currentCost = 0;
    DynamicArray<Emblem, c_cardMaxEmblems> emblems;

    while (result.level != currentCost + result.factions.Count() - (result.heroic ? 5 : 2 )) {
      currentCost = 0;
      emblems.Clear();
      int const emblemCount = std::min((std::rand() % c_cardMaxEmblems) + 1, (std::rand() % c_cardMaxEmblems) + 1);

      for (int i = 0; i < emblemCount; ++i) {
        Emblem newEmblem;
        bool   validEmblem;
        do {
          if (i == 0) {
            newEmblem = primaryEmblems[std::rand() % primaryEmblems.Count()];
          }
          else {
            newEmblem = allowedEmblems[std::rand() % allowedEmblems.Count()];
          }

          validEmblem =
            !Contains(emblems, newEmblem) &&
            EvaluateEmblemRequirements(result.level, emblems, t_emblemData[newEmblem].cardRequirement)
          ;
        } while (!validEmblem);

        emblems.Emplace(newEmblem);
        currentCost -= t_emblemData[newEmblem].generationMod;
      }
    }

    result.emblems = emblems;

    return result;
  };
}

int main(void) {
  std::srand(std::time(nullptr));

  while (true) {
    CardData card = CreateCard();
    //CardData card = CreateCard(0, Faction::GilbrandsArmory);

    std::cout << card.name << ": " << card.level;

    if (card.heroic) {
      std::cout << " Heroic";
    }

    std::cout << std::endl;

    std::cout << "  " << t_factionData[card.factions[0]].name;
    for (int i = 1; i < card.factions.Count(); ++i) {
      std::cout << " | " << t_factionData[card.factions[i]].name;
    }
    std::cout << std::endl;

    if (!card.emblems.Empty()) {
      std::cout << "  " << t_emblemData[card.emblems[0]].name;
      for (int i = 1; i < card.emblems.Count(); ++i) {
        std::cout << ", " << t_emblemData[card.emblems[i]].name;
      }
      std::cout << std::endl;
    }

    std::cin.get();
  }

  return 0;
};
