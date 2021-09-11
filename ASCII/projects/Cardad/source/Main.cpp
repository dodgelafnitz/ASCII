/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <array>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>

namespace {
}
  template <typename T, int Cap>
  class DynamicArr {
  public:
    DynamicArr(void) = default;
    DynamicArr(int count) {
      while (Size() < count) {
        EmplaceBack();
      }
    }

    DynamicArr(int count, T const & value) {
      while (Size() < count) {
        EmplaceBack(value);
      }
    }

    ~DynamicArr(void) {
      while (Size() > 0) {
        PopBack();
      }
    }

    bool Empty(void) const {
      return Size() == 0;
    }

    bool Full(void) const {
      return Size() == Capacity();
    }

    int Size(void) const {
      return m_count;
    }

    int Capacity(void) const {
      return Cap;
    }

    template <typename ... Params>
    int EmplaceBack(Params &&... params) {
      if (Full()) {
        return -1;
      }

      new(m_data + m_count * sizeof(T)) T(std::forward<Params>(params)...);

      return m_count++;
    }

    void PopBack(void) {
      if (Empty()) {
        return;
      }

      reinterpret_cast<T *>(m_data + (--m_count) * sizeof(T))->~T();
    }

    T const & operator [] (int index) const {
      return *reinterpret_cast<T const *>(m_data + index * sizeof(T));
    }

    T & operator [] (int index) {
      return *reinterpret_cast<T *>(m_data + index * sizeof(T));
    }

    T const * begin(void) const {
      return &operator [](0);
    }

    T * begin(void) {
      return &operator [](0);
    }

    T const * end(void) const {
      return &operator [](Size());
    }

    T * end(void) {
      return &operator [](Size());
    }

  private:
    char m_data[Cap * sizeof(T)];
    int  m_count = 0;
  };

  template <typename Base, typename Element, int Count, typename Indexer = int>
  class DataTable : public Base {
  public:
    DataTable(Element const (& elements)[Count]) {
      for (int i = 0; i < Count; ++i) {
        m_data.EmplaceBack(elements[i]);
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
    DynamicArr<Element, Count> m_data;
  };

  template <auto T>
  using ExpectedCountType = std::conditional_t<std::is_same_v<decltype(T), nullptr_t>, int, decltype(T)>;

  template <typename Base, auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<Base, Element, Count, ExpectedCountType<ExpectedCount>> MakeDataTable(
    Element const (& array)[Count]
  ) {
    if constexpr (!std::is_same_v<decltype(ExpectedCount), nullptr_t>) {
      static_assert(int(ExpectedCount) == Count);

      return DataTable<Base, Element, int(ExpectedCount), decltype(ExpectedCount)>(array);
    }
    else {
      return DataTable<Base, Element, Count>(array);
    }
  }

  template <typename Base, auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<Base, Element, Count, ExpectedCountType<ExpectedCount>> MakeDataTable(
    std::pair<ExpectedCountType<ExpectedCount>, Element> const (& array)[Count]
  ) {
    Element arr[Count];
    bool    added[Count] = { false };

    for (int i = 0; i < Count; ++i) {
      int const index = int(array[i].first);

      if (added[index]) {
        throw std::exception("Added duplicate index.");
      }

      arr[index] = array[i].second;
      added[index] = true;
    }

    return MakeDataTable<Base, ExpectedCount, Element, Count>(arr);
  }

  struct EmptyHeader {};

  template <auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<EmptyHeader, Element, Count, ExpectedCountType<ExpectedCount>> MakeHeaderlessDataTable(
    Element const (& array)[Count]
  ) {
    return MakeDataTable<EmptyHeader, ExpectedCount, Element, Count>(array);
  }

  template <auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<EmptyHeader, Element, Count, ExpectedCountType<ExpectedCount>> MakeHeaderlessDataTable(
    std::pair<ExpectedCountType<ExpectedCount>, Element> const (&array)[Count]
  ) {
    return MakeDataTable<EmptyHeader, ExpectedCount, Element, Count>(array);
  }

  enum class Tier {
    Copper,
    Bronze,
    Silver,
    Gold,
    Platinum,
    Rhodium,

    Count
  };

  struct TierInfoHeader {
    static Tier const defualtTier         = Tier::Bronze;
    static int const  staringTierCount    = 3;
    static Tier const minimumStartingTier = Tier::Bronze;
  };

  struct TierInfoRow {
    char const * name;
  };

  auto const c_tierTable = MakeDataTable<TierInfoHeader, Tier::Count>({
    std::make_pair(Tier::Copper,   TierInfoRow({ "Copper"   })),
    std::make_pair(Tier::Bronze,   TierInfoRow({ "Bronze"   })),
    std::make_pair(Tier::Silver,   TierInfoRow({ "Silver"   })),
    std::make_pair(Tier::Gold,     TierInfoRow({ "Gold"     })),
    std::make_pair(Tier::Platinum, TierInfoRow({ "Platinum" })),
    std::make_pair(Tier::Rhodium,  TierInfoRow({ "Rhodium"  })),
  });


  char const * GetTierName(Tier tier) {
    return c_tierTable[tier].name;
  };

  enum class CardType {
    Location,
    Character,
    Modifier,
    Event,

    Count
  };

  struct CardInfoRow {
    char const * name;
  };

  auto const c_cardTypeTable = MakeHeaderlessDataTable<CardType::Count>({
    std::make_pair(CardType::Location,  CardInfoRow({ "Location"  })),
    std::make_pair(CardType::Character, CardInfoRow({ "Character" })),
    std::make_pair(CardType::Modifier,  CardInfoRow({ "Modifier"  })),
    std::make_pair(CardType::Event,     CardInfoRow({ "Event"     })),
  });

  char const * GetCardTypeName(CardType type) {
    return c_cardTypeTable[type].name;
  };

  enum class ActiveEffectType {
    Defeat,
    Send,
    Pull,
    Refresh,
    Revive,
    Betray,
    Surge,
    Recall,
    Action,
    Draw,
    Play,
    Silence,
    Shatter,
    Steal,
    Discover,
    Improve,
    Recruit,
    Encounter,

    Count
  };

  struct ActiveEffectInfoRow {
    char const * name;
    bool         isTiered;
  };

  auto const c_activeEffectTable = MakeHeaderlessDataTable<ActiveEffectType::Count>({
    std::make_pair(ActiveEffectType::Defeat,    ActiveEffectInfoRow({ "Defeat",    true  })),
    std::make_pair(ActiveEffectType::Send,      ActiveEffectInfoRow({ "Send",      true  })),
    std::make_pair(ActiveEffectType::Pull,      ActiveEffectInfoRow({ "Pull",      true  })),
    std::make_pair(ActiveEffectType::Refresh,   ActiveEffectInfoRow({ "Refresh",   true  })),
    std::make_pair(ActiveEffectType::Revive,    ActiveEffectInfoRow({ "Revive",    true  })),
    std::make_pair(ActiveEffectType::Betray,    ActiveEffectInfoRow({ "Betray",    true  })),
    std::make_pair(ActiveEffectType::Surge,     ActiveEffectInfoRow({ "Surge",     false })),
    std::make_pair(ActiveEffectType::Recall,    ActiveEffectInfoRow({ "Recall",    true  })),
    std::make_pair(ActiveEffectType::Action,    ActiveEffectInfoRow({ "Action",    false })),
    std::make_pair(ActiveEffectType::Draw,      ActiveEffectInfoRow({ "Draw",      false })),
    std::make_pair(ActiveEffectType::Play,      ActiveEffectInfoRow({ "Play",      false })),
    std::make_pair(ActiveEffectType::Silence,   ActiveEffectInfoRow({ "Silence",   true  })),
    std::make_pair(ActiveEffectType::Shatter,   ActiveEffectInfoRow({ "Shatter",   true  })),
    std::make_pair(ActiveEffectType::Steal,     ActiveEffectInfoRow({ "Steal",     true  })),
    std::make_pair(ActiveEffectType::Discover,  ActiveEffectInfoRow({ "Discover",  true  })),
    std::make_pair(ActiveEffectType::Improve,   ActiveEffectInfoRow({ "Improve",   true  })),
    std::make_pair(ActiveEffectType::Recruit,   ActiveEffectInfoRow({ "Recruit",   true  })),
    std::make_pair(ActiveEffectType::Encounter, ActiveEffectInfoRow({ "Encounter", true  })),
  });

  bool IsActiveEffectTiered(ActiveEffectType type) {
    return c_activeEffectTable[type].isTiered;
  }

  char const * GetActiveEffectTypeName(ActiveEffectType type) {
    return c_activeEffectTable[type].name;
  };

  enum class PassiveEffectType {
    Rally,
    Defender,
    Invisible,
    Enduring,
    Regeneration,
    Defile,
    Weak,
    Invincible,
    Anchored,
    Pure,

    Count
  };

  struct PassiveEffectInfoRow {
    char const * name;
    bool         isTiered;
  };

  auto const c_passiveEffectTable = MakeHeaderlessDataTable<PassiveEffectType::Count>({
    std::make_pair(PassiveEffectType::Rally,        PassiveEffectInfoRow({ "Rally",        true  })),
    std::make_pair(PassiveEffectType::Defender,     PassiveEffectInfoRow({ "Defender",     false })),
    std::make_pair(PassiveEffectType::Invisible,    PassiveEffectInfoRow({ "Invisible",    true  })),
    std::make_pair(PassiveEffectType::Enduring,     PassiveEffectInfoRow({ "Enduring",     false })),
    std::make_pair(PassiveEffectType::Regeneration, PassiveEffectInfoRow({ "Regeneration", false })),
    std::make_pair(PassiveEffectType::Defile,       PassiveEffectInfoRow({ "Defile",       true  })),
    std::make_pair(PassiveEffectType::Weak,         PassiveEffectInfoRow({ "Weak",         false })),
    std::make_pair(PassiveEffectType::Invincible,   PassiveEffectInfoRow({ "Invincible",   false })),
    std::make_pair(PassiveEffectType::Anchored,     PassiveEffectInfoRow({ "Anchored",     false })),
    std::make_pair(PassiveEffectType::Pure,         PassiveEffectInfoRow({ "Pure",         false })),
  });

  bool IsPassiveEffectTiered(PassiveEffectType type) {
    return c_passiveEffectTable[type].isTiered;
  }

  char const * GetPassiveEffectTypeName(PassiveEffectType type) {
    return c_passiveEffectTable[type].name;
  };

  enum class ModifierType {
    AddNew,
    EnhancePrimary,
    ReducePrimary,
    RemovePrimary,
    DuplicatePrimary,
    EnhanceSecondaries,
    ReduceSecondaries,
    RemoveSecondaries,

    Count
  };

  struct ModifierInfoRow {
    char const * name;
    bool         containsEffect;
  };

  auto const c_modifierTable = MakeHeaderlessDataTable<ModifierType::Count>({
    std::make_pair(ModifierType::AddNew,             ModifierInfoRow({ "Add New",             true  })),
    std::make_pair(ModifierType::EnhancePrimary,     ModifierInfoRow({ "Enhance Primary",     false })),
    std::make_pair(ModifierType::ReducePrimary,      ModifierInfoRow({ "Reduce Primary",      false })),
    std::make_pair(ModifierType::RemovePrimary,      ModifierInfoRow({ "Remove Primary",      false })),
    std::make_pair(ModifierType::DuplicatePrimary,   ModifierInfoRow({ "Duplicate Primary",   false })),
    std::make_pair(ModifierType::EnhanceSecondaries, ModifierInfoRow({ "Enhance Secondaries", false })),
    std::make_pair(ModifierType::ReduceSecondaries,  ModifierInfoRow({ "Reduce Secondaries",  false })),
    std::make_pair(ModifierType::RemoveSecondaries,  ModifierInfoRow({ "Remove Secondaries",  false })),
  });

  bool DoesModifierTypeContainEffect(ModifierType type) {
    return c_modifierTable[type].containsEffect;
  }

  char const * GetModifierTypeName(ModifierType type) {
    return c_modifierTable[type].name;
  };

  enum class EffectType {
    Active,
    Passive,

    Count
  };
  int const c_effectTypes = int(EffectType::Count);

  struct ActiveEffect {
    ActiveEffectType type;
    Tier             tier;
  };

  struct PassiveEffect {
    PassiveEffectType type;
    Tier              tier;
  };

  struct Effect {
    static_assert(c_effectTypes == 2);
    EffectType type;
    union {
      ActiveEffect  active;
      PassiveEffect passive;
    };
  };

  bool IsEffectTiered(Effect const & effect) {
    static_assert(c_effectTypes == 2);
    switch (effect.type) {
      case EffectType::Active:  return IsActiveEffectTiered(effect.active.type);
      case EffectType::Passive: return IsPassiveEffectTiered(effect.passive.type);
      default:                  return false;
    }
  }

  struct Modifier {
    ModifierType type;
    Effect       effect;
  };

  enum class LocationEffectType {
    Active,
    Modifier,

    Count
  };
  int const c_locationEffectTypes = int(LocationEffectType::Count);

  struct LocationEffect {
    static_assert(c_locationEffectTypes == 2);
    LocationEffectType type;
    union {
      ActiveEffect active;
      Modifier     modifier;
    };
  };

  int const c_maxSymbols = 3;

  struct LocationDef {
    char const *                             subtype;
    DynamicArr<LocationEffect, c_maxSymbols> effects;
  };

  struct CharacterDef {
    Effect                               primaryEffect;
    DynamicArr<Effect, c_maxSymbols - 1> secondaryEffects;
  };

  struct ModifierDef {
    char const *                       subtype;
    DynamicArr<Modifier, c_maxSymbols> modifiers;
  };

  struct EventDef {
    DynamicArr<ActiveEffect, c_maxSymbols> effects;
  };

  struct CardDef {
    CardDef(void) :
      type(CardType::Location),
      location()
    {}

    ~CardDef(void) {
      static_assert(c_cardTypeTable.Size() == 4);
      switch (type) {
        case CardType::Location: {
          location.~LocationDef();
        } break;
        case CardType::Character: {
          character.~CharacterDef();
        } break;
        case CardType::Modifier: {
          modifier.~ModifierDef();
        } break;
        case CardType::Event: {
          event.~EventDef();
        } break;
      }
    }

    char const * faction = "";
    Tier         tier    = c_tierTable.defualtTier;

    static_assert(c_cardTypeTable.Size() == 4);
    CardType     type;
    union {
      LocationDef  location;
      CharacterDef character;
      ModifierDef  modifier;
      EventDef     event;
    };
  };

  int const c_maxHandSize    = 30;
  int const c_maxDeckSize    = 30;
  int const c_maxPlayerCount = 5;
  int const c_maxCardsOnField = c_maxPlayerCount * c_maxHandSize;

  int const c_maxCardCount = 600;

  struct PlayerData {
    int                            victoryPoints;
    DynamicArr<int, c_maxHandSize> handCardDefIndices;
    DynamicArr<int, c_maxDeckSize> deckCardDefIndices;
    DynamicArr<int, c_maxDeckSize> discardCardDefIndices;
    bool                           hasSurge;
  };

  struct CardPlayData {
    int cardDefIndex;
    int location;
  };

  struct GameBoard {
    int                                         currentRound;
    int                                         currentPlayerTurn;
    DynamicArr<PlayerData, c_maxPlayerCount>    players;
    DynamicArr<CardPlayData, c_maxCardsOnField> cardsOnField;
  };

  struct GameData {
    DynamicArr<CardDef, c_maxCardCount> cards;
    GameBoard                           game;
  };

  char const * GetRandomFaction(void) {
    static char const * factions[] = {
      "Children of the Queen",
      "Mercantile Guild",
      "Mechanosis",
      "Xenophet",
      "Starwise Drifters",
      "Beleen Unifiers",
    };

    return factions[std::rand() % (sizeof(factions) / sizeof(*factions))];
  }

  char const * GetRandomLocationSubtype(void) {
    static char const * subtypes[] = {
      "Structure",
      "Weather",
      "Crowd",
      "Aura",
    };

    return subtypes[std::rand() % (sizeof(subtypes) / sizeof(*subtypes))];
  }

  char const * GetRandomModifierSubtype(void) {
    static char const * subtypes[] = {
      "Transformation",
      "Gear",
      "Attitude",
      "Device",
    };

    return subtypes[std::rand() % (sizeof(subtypes) / sizeof(*subtypes))];
  }

  Tier GetRandomStartingTier(void) {
    return Tier(std::rand() % c_tierTable.staringTierCount + int(c_tierTable.minimumStartingTier));
  }

  ActiveEffect CreateRandomActiveEffect(void) {
    ActiveEffect result;

    result.tier = GetRandomStartingTier();
    result.type = ActiveEffectType(std::rand() % c_activeEffectTable.Size());

    return result;
  }

  PassiveEffect CreateRandomPassiveEffect(void) {
    PassiveEffect result;

    result.tier = GetRandomStartingTier();
    result.type = PassiveEffectType(std::rand() % c_passiveEffectTable.Size());

    return result;
  }

  Effect CreateRandomEffect(void) {
    Effect result;

    result.type = EffectType(std::rand() % c_effectTypes);

    static_assert(c_effectTypes == 2);
    switch (result.type) {
      case EffectType::Active: {
        result.active = CreateRandomActiveEffect();
      } break;
      case EffectType::Passive: {
        result.passive = CreateRandomPassiveEffect();
      } break;
    }

    return result;
  }

  Modifier CreateRandomModifier(void) {
    Modifier result;

    result.type = ModifierType(std::rand() % c_modifierTable.Size());

    if (DoesModifierTypeContainEffect(result.type)) {
      result.effect = CreateRandomEffect();
    }

    return result;
  }

  LocationEffect CreateRandomLocationEffect(void) {
    LocationEffect result;

    result.type = LocationEffectType(std::rand() % c_locationEffectTypes);

    static_assert(c_locationEffectTypes == 2);
    switch (result.type) {
      case LocationEffectType::Active: {
        result.active = CreateRandomActiveEffect();
      } break;
      case LocationEffectType::Modifier: {
        result.modifier = CreateRandomModifier();
      } break;
    }

    return result;
  }

  CardDef CreateCompletelyRandomCard(void) {
    CardDef result;

    result.faction = GetRandomFaction();
    result.tier    = GetRandomStartingTier();

    result.type = CardType(std::rand() % c_cardTypeTable.Size());

    static_assert(c_cardTypeTable.Size() == 4);
    switch (result.type) {
      case CardType::Character: {
        result.character.primaryEffect = CreateRandomEffect();

        // intentional call of rand each loop
        for (int i = 1; i <= std::rand() % c_maxSymbols; ++i) {
          result.character.secondaryEffects.EmplaceBack(CreateRandomEffect());
        }
      } break;
      case CardType::Location: {
        // intentional call of rand each loop
        for (int i = 0; i <= std::rand() % c_maxSymbols; ++i) {
          result.location.effects.EmplaceBack(CreateRandomLocationEffect());
        }
        result.location.subtype = GetRandomLocationSubtype();
      } break;
      case CardType::Modifier: {
        // intentional call of rand each loop
        for (int i = 0; i <= std::rand() % c_maxSymbols; ++i) {
          result.modifier.modifiers.EmplaceBack(CreateRandomModifier());
        }
        result.modifier.subtype = GetRandomModifierSubtype();
      } break;
      case CardType::Event: {
        // intentional call of rand each loop
        for (int i = 0; i <= std::rand() % c_maxSymbols; ++i) {
          result.event.effects.EmplaceBack(CreateRandomActiveEffect());
        }
      } break;
    }

    return result;
  }

  std::string GetActiveEffectText(ActiveEffect const & effect) {
    return
      GetActiveEffectTypeName(effect.type) + (
        IsActiveEffectTiered(effect.type) ? (
          std::string("(") + GetTierName(effect.tier) + ")"
        ) : (
          ""
        )
      )
    ;
  }

  std::string GetPassiveEffectText(PassiveEffect const & effect) {
    return
      GetPassiveEffectTypeName(effect.type) + (
        IsPassiveEffectTiered(effect.type) ? (
          std::string("(") + GetTierName(effect.tier) + ")"
        ) : (
          ""
        )
      )
    ;
  }

  std::string GetEffectTypeName(Effect const & effect) {
    switch (effect.type) {
      case EffectType::Active:  return GetActiveEffectText(effect.active);
      case EffectType::Passive: return GetPassiveEffectText(effect.passive);
      default:                  return "";
    }
  }

  std::string GetModifierText(Modifier const & modifier) {
    return
      GetModifierTypeName(modifier.type) + (
        DoesModifierTypeContainEffect(modifier.type) ? (
          " " + GetEffectTypeName(modifier.effect)
        ) : (
          ""
        )
      )
    ;
  }


int main(void) {
  std::srand(std::time(nullptr));

  while (true) {
    CardDef card = CreateCompletelyRandomCard();

    std::cout << "Faction: " << card.faction << std::endl;
    std::cout << "Tier:    " << GetTierName(card.tier) << std::endl;
    std::cout << "Type:    " << GetCardTypeName(card.type) << std::endl;

    static_assert(c_cardTypeTable.Size() == 4);
    switch (card.type) {
      case CardType::Character: {
        DynamicArr<ActiveEffect, c_maxSymbols>  activeEffects;
        DynamicArr<PassiveEffect, c_maxSymbols> passiveEffects;

        static_assert(c_effectTypes == 2);
        for (auto & effect : card.character.secondaryEffects) {
          switch (effect.type) {
            case EffectType::Active: {
              activeEffects.EmplaceBack(effect.active);
            } break;
            case EffectType::Passive: {
              passiveEffects.EmplaceBack(effect.passive);
            } break;
          }
        }

        if (!activeEffects.Empty() || card.character.primaryEffect.type == EffectType::Active) {
          std::cout << "Active Effects:" << std::endl;
          if (card.character.primaryEffect.type == EffectType::Active) {
            std::cout << "  " << GetActiveEffectText(card.character.primaryEffect.active) << "*" << std::endl;
          }

          for (auto & effect : activeEffects) {
            std::cout << "  " << GetActiveEffectText(effect) << std::endl;
          }
        }

        if (!passiveEffects.Empty() || card.character.primaryEffect.type == EffectType::Passive) {
          std::cout << "Passive Effects:" << std::endl;
          if (card.character.primaryEffect.type == EffectType::Passive) {
            std::cout << "  " << GetPassiveEffectText(card.character.primaryEffect.passive) << "*" << std::endl;
          }

          for (auto & effect : passiveEffects) {
            std::cout << "  " << GetPassiveEffectText(effect) << std::endl;
          }
        }

        std::cout << std::endl;

      } break;
      case CardType::Location: {
        std::cout << "Subtype: " << card.location.subtype << std::endl;

        DynamicArr<ActiveEffect, c_maxSymbols> activeEffects;
        DynamicArr<Modifier, c_maxSymbols>     modifiers;

        for (auto & effect : card.location.effects) {
          switch (effect.type) {
            case LocationEffectType::Active: {
              activeEffects.EmplaceBack(effect.active);
            } break;
            case LocationEffectType::Modifier: {
              modifiers.EmplaceBack(effect.modifier);
            } break;
          }
        }

        if (!activeEffects.Empty()) {
          std::cout << "Surge Effects:" << std::endl;
          for (auto & effect : activeEffects) {
            std::cout << "  " << GetActiveEffectText(effect) << std::endl;
          }
        }

        if (!modifiers.Empty()) {
          std::cout << "Modifiers:" << std::endl;
          for (auto & modifier : modifiers) {
            std::cout << "  " << GetModifierText(modifier) << std::endl;
          }
        }

        std::cout << std::endl;
      } break;
      case CardType::Modifier: {
        std::cout << "Subtype: " << card.modifier.subtype << std::endl;

        std::cout << "Modifiers:" << std::endl;
        for (auto & modifier : card.modifier.modifiers) {
          std::cout << "  " << GetModifierText(modifier) << std::endl;
        }

        std::cout << std::endl;
      } break;
      case CardType::Event: {
        std::cout << "Effects:" << std::endl;
        for (auto & effect : card.event.effects) {
          std::cout << "  " << GetActiveEffectText(effect) << std::endl;
        }

        std::cout << std::endl;
      } break;
    }

    std::cin.get();
  };

  return 0;
};
