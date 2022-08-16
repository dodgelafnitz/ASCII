/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace {
  std::vector<std::string> SplitString(std::string const & str, char character) {
    std::vector<std::string> results;

    if (str.empty()) {
      return results;
    }

    size_t currentStart = 0;

    while (currentStart != std::string::npos) {
      size_t const currentEnd = str.find_first_of(character, currentStart);

      if (currentEnd == std::string::npos) {
        results.emplace_back(str.substr(currentStart));
        break;
      }
      else {
        results.emplace_back(str.substr(currentStart, currentEnd - currentStart));
      }

      currentStart = currentEnd + 1;
    }

    return results;
  }

  bool IsNumber(std::string const & str) {
    for (char character : str) {
      if (!std::isdigit(character)) {
        return false;
      }
    }

    return true;
  }

  enum class SigilType {
    Attribute,
    Action,

    Count
  };

  enum class Attribute {
    Wonder,
    Sparkle,
    Burst,

    Count
  };

  enum class Action {
    Rally,
    Prepare,
    Retire,
    Repurpose,
    Reorganize,
    Reveal,

    Count
  };

  enum class Kind {
    Skeleton,
    Squirrel,
    Cat,
    Plant,
    Wisp,
    Golem,
    Crystal,
    Bird,

    Count
  };

  enum class Role {
    Soldier,
    Scholar,
    Artisan,
    Citizen,

    Count
  };

  enum class CardType {
    Character,
    Structure,
    Action,

    Count
  };

  CardType GetCardTypeFromString(std::string const & str) {
    static_assert(int(CardType::Count) == 3);

    if (str == "Character") {
      return CardType::Character;
    }
    if (str == "Structure") {
      return CardType::Structure;
    }
    if (str == "Action") {
      return CardType::Action;
    }

    return CardType::Count;
  }

  enum class DescriptorType {
    Kind,
    Role,

    Count
  };

  struct Sigil {
    Sigil(void) = default;

    Sigil(Attribute attribute) :
      type(SigilType::Attribute),
      attribute(attribute)
    {}

    Sigil(Action action) :
      type(SigilType::Action),
      action(action)
    {}

    bool operator <(Sigil const & sigil) const {
      if (int(type) < int(sigil.type)) {
        return true;
      }

      if (int(type) > int(sigil.type)) {
        return false;
      }

      switch (type) {
        case SigilType::Attribute: return attribute < sigil.attribute;
        case SigilType::Action:    return action    < sigil.action;
      }

      return false;
    }

    bool operator ==(Sigil const & sigil) const {
      if (type != sigil.type) {
        return false;
      }

      switch (type) {
        case SigilType::Attribute: return attribute == sigil.attribute;
        case SigilType::Action:    return action    == sigil.action;
      }

      return false;
    }

    std::string ToString(void) const {
      static_assert(int(SigilType::Count) == 2);
      switch (type) {
        static_assert(int(Attribute::Count) == 3);
        case SigilType::Attribute: {
          switch (attribute) {
            case Attribute::Wonder:  return "Wonder";
            case Attribute::Sparkle: return "Sparkle";
            case Attribute::Burst:   return "Burst";
          }
        } break;

          static_assert(int(Action::Count) == 6);
        case SigilType::Action: {
          switch (action) {
            case Action::Rally:      return "Rally";
            case Action::Prepare:    return "Prepare";
            case Action::Retire:     return "Retire";
            case Action::Repurpose:  return "Repurpose";
            case Action::Reorganize: return "Reorganize";
            case Action::Reveal:     return "Reveal";
          }
        }
      }

      return "";
    }

    static Sigil FromString(std::string const & str) {
      static_assert(int(SigilType::Count) == 2);

      static_assert(int(Attribute::Count) == 3);
      if (str == "Wonder") {
        return Attribute::Wonder;
      }
      if (str == "Sparkle") {
        return Attribute::Sparkle;
      }
      if (str == "Burst") {
        return Attribute::Burst;
      }

      static_assert(int(Action::Count) == 6);
      if (str == "Rally") {
        return Action::Rally;
      }
      if (str == "Prepare") {
        return Action::Prepare;
      }
      if (str == "Retire") {
        return Action::Retire;
      }
      if (str == "Repurpose") {
        return Action::Repurpose;
      }
      if (str == "Reorganize") {
        return Action::Reorganize;
      }
      if (str == "Reveal") {
        return Action::Reveal;
      }

      return Sigil();
    }

    SigilType type = SigilType::Count;

    union {
      Attribute attribute;
      Action    action;
    };
  };

  struct Descriptor {
    Descriptor(void) = default;

    Descriptor(Kind kind) :
      type(DescriptorType::Kind),
      kind(kind)
    {}

    Descriptor(Role role) :
      type(DescriptorType::Role),
      role(role)
    {}

    bool operator <(Descriptor const & descriptor) const {
      if (int(type) < int(descriptor.type)) {
        return true;
      }

      if (int(type) > int(descriptor.type)) {
        return false;
      }

      switch (type) {
        case DescriptorType::Kind: return kind < descriptor.kind;
        case DescriptorType::Role: return role < descriptor.role;
      }

      return false;
    }

    bool operator ==(Descriptor const & descriptor) const {
      if (type != descriptor.type) {
        return false;
      }

      switch (type) {
        case DescriptorType::Kind: return kind == descriptor.kind;
        case DescriptorType::Role: return role == descriptor.role;
      }

      return false;
    }

    std::string ToString(void) const {
      switch (type) {
        case DescriptorType::Kind: {
          static_assert(int(Kind::Count) == 8);
          switch (kind) {
            case Kind::Skeleton: return "Skeleton";
            case Kind::Squirrel: return "Squirrel";
            case Kind::Cat:      return "Cat";
            case Kind::Plant:    return "Plant";
            case Kind::Wisp:     return "Wisp";
            case Kind::Golem:    return "Golem";
            case Kind::Crystal:  return "Crystal";
            case Kind::Bird:     return "Bird";
          }
        } break;

        static_assert(int(Role::Count) == 4);
        case DescriptorType::Role: {
          switch (role) {
            case Role::Soldier: return "Soldier";
            case Role::Scholar: return "Scholar";
            case Role::Artisan: return "Artisan";
            case Role::Citizen: return "Citizen";
          }
        } break;
      }

      return "";
    }

    static Descriptor FromString(std::string const & str) {
      static_assert(int(Kind::Count) == 8);
      if (str == "Skeleton") {
        return Kind::Skeleton;
      }
      if (str == "Squirrel") {
        return Kind::Squirrel;
      }
      if (str == "Cat") {
        return Kind::Cat;
      }
      if (str == "Plant") {
        return Kind::Plant;
      }
      if (str == "Wisp") {
        return Kind::Wisp;
      }
      if (str == "Golem") {
        return Kind::Golem;
      }
      if (str == "Crystal") {
        return Kind::Crystal;
      }
      if (str == "Bird") {
        return Kind::Bird;
      }

      static_assert(int(Role::Count) == 4);
      if (str == "Soldier") {
        return Role::Soldier;
      }
      if (str == "Scholar") {
        return Role::Scholar;
      }
      if (str == "Artisan") {
        return Role::Artisan;
      }
      if (str == "Citizen") {
        return Role::Citizen;
      }

      return Descriptor();
    }

    DescriptorType type = DescriptorType::Count;

    union {
      Kind kind;
      Role role;
    };
  };

  struct CardInfo {
    CardInfo(void) = default;

    CardInfo(CardType type) :
      type(type)
    {}

    CardInfo & AddDescriptor(Kind kind) {
      descriptors.emplace_back(kind);

      return *this;
    }

    CardInfo & AddDescriptor(Role role) {
      descriptors.emplace_back(role);

      return *this;
    }

    CardInfo & AddSigil(Attribute attribute) {
      sigils.emplace_back(attribute);

      return *this;
    }

    CardInfo & AddSigil(Action action) {
      sigils.emplace_back(action);

      return *this;
    }

    bool operator ==(CardInfo const & card) {
      return type == card.type && descriptors == card.descriptors && sigils == card.sigils;
    }

    static CardInfo FromString(std::string const & string) {
      CardInfo info;

      std::vector<std::string> lines = SplitString(string, '\n');

      for (std::string & line : lines) {
        line = line.substr(line.find_first_not_of(' '));
      }

      std::string const & nameLine = lines[0];

      size_t const endOfName = nameLine.find_first_of(":");
      info.name = nameLine.substr(0, endOfName);

      std::vector<std::string> descriptions = SplitString(lines[1], ' ');

      if (IsNumber(descriptions.back())) {
        descriptions.pop_back();
      }

      info.type = GetCardTypeFromString(descriptions.back());
      descriptions.pop_back();

      for (std::string const & descriptor : descriptions) {
        info.descriptors.emplace_back(Descriptor::FromString(descriptor));
      }

      for (int i = 2; i < lines.size(); ++i) {
        std::vector<std::string> const sigils = SplitString(lines[i], ' ');

        for (std::string const & sigil : sigils) {
          info.sigils.emplace_back(Sigil::FromString(sigil));
        }
      }

      return info;
    }

    std::string ToString(void) const {
      std::string result = name + ":\n ";

      for (Descriptor const & descriptor : descriptors) {
        result += " " + descriptor.ToString();
      }

      switch (type) {
        case CardType::Character: {
          int const level = sigils.size() - 2;
          result += " Character " + std::to_string(level) + "\n ";

        } break;
        case CardType::Structure: {
          result += " Structure\n ";
        } break;
        case CardType::Action: {
          result += " Action\n ";
        } break;
      }

      if (!sigils.empty()) {
        bool displayingAttributes = sigils.front().type == SigilType::Attribute;
        for (Sigil const & sigil : sigils) {
          if (displayingAttributes && sigil.type != SigilType::Attribute) {
            displayingAttributes = false;
            result += "\n ";
          }

          result += " " + sigil.ToString();
        }
      }

      return result;
    }

    CardType                type = CardType::Count;
    std::string             name;
    std::vector<Descriptor> descriptors;
    std::vector<Sigil>      sigils;
  };

  struct Faction {
    Faction(std::string const & name) :
      name(name)
    {}

    Faction & AddSubfaction(Faction const & faction) {
      subfactions.emplace_back(faction);

      return *this;
    }

    Faction & AddSigil(Attribute attribute) {
      sigils.emplace_back(attribute);

      return *this;
    }

    Faction & AddSigil(Action action) {
      sigils.emplace_back(action);

      return *this;
    }

    Faction & AddDescriptor(Kind kind) {
      descriptors.emplace_back(kind);

      return *this;
    }

    Faction & AddDescriptor(Role role) {
      descriptors.emplace_back(role);

      return *this;
    }

    std::string             name;
    std::vector<Faction>    subfactions;
    std::vector<Sigil>      sigils;
    std::vector<Descriptor> descriptors;
  };

  Faction c_factionRules = Faction("Aetherling")
    .AddSubfaction(Faction("Mossflame")
      .AddSigil(Attribute::Wonder)
      .AddSubfaction(Faction("Mossbone")
        .AddSigil(Attribute::Burst)
        .AddSigil(Action::Prepare)
        .AddDescriptor(Kind::Skeleton)
        .AddDescriptor(Role::Citizen)
        .AddDescriptor(Role::Scholar)
      )
      .AddSubfaction(Faction("Ember")
        .AddSigil(Attribute::Sparkle)
        .AddSigil(Action::Reorganize)
        .AddDescriptor(Kind::Wisp)
        .AddDescriptor(Role::Citizen)
        .AddDescriptor(Role::Soldier)
      )
    )
    .AddSubfaction(Faction("Wiffletail")
      .AddSigil(Attribute::Burst)
      .AddSigil(Action::Reveal)
      .AddDescriptor(Kind::Squirrel)
      .AddSubfaction(Faction("Wifflescout")
        .AddSigil(Attribute::Wonder)
        .AddDescriptor(Kind::Bird)
        .AddDescriptor(Role::Soldier)
      )
      .AddSubfaction(Faction("Crafttail")
        .AddSigil(Attribute::Sparkle)
        .AddDescriptor(Role::Citizen)
        .AddDescriptor(Role::Artisan)
      )
    )
    .AddSubfaction(Faction("Crystalid")
      .AddSigil(Attribute::Wonder)
      .AddSigil(Attribute::Sparkle)
      .AddSigil(Attribute::Burst)
      .AddDescriptor(Kind::Crystal)
      .AddSubfaction(Faction("Gemblade")
        .AddSigil(Action::Retire)
        .AddDescriptor(Role::Soldier)
      )
      .AddSubfaction(Faction("Stonemind")
        .AddSigil(Action::Rally)
        .AddSigil(Action::Repurpose)
        .AddDescriptor(Role::Artisan)
        .AddDescriptor(Role::Scholar)
      )
    )
  ;

  struct FactionAvailables {
    std::vector<Sigil>      sigils;
    std::vector<Attribute>  attributes;
    std::vector<Action>     actions;
    std::vector<Descriptor> descriptors;
    std::vector<Kind>       kinds;
    std::vector<Role>       roles;
  };

  FactionAvailables GetRandomFactionAvailables(Faction const & factionRules, std::string & o_factionName) {
    FactionAvailables availables;

    Faction const * currentFaction = &factionRules;
    while (currentFaction) {
      o_factionName = currentFaction->name;

      for (Sigil const & sigil : currentFaction->sigils) {
        availables.sigils.emplace_back(sigil);

        static_assert(int(SigilType::Count) == 2);
        switch (sigil.type) {
          case SigilType::Attribute: {
            availables.attributes.emplace_back(sigil.attribute);
          } break;
          case SigilType::Action: {
            availables.actions.emplace_back(sigil.action);
          } break;
        }
      }

      for (Descriptor const & descriptor : currentFaction->descriptors) {
        availables.descriptors.emplace_back(descriptor);

        static_assert(int(DescriptorType::Count) == 2);
        switch (descriptor.type) {
          case DescriptorType::Kind: {
            availables.kinds.emplace_back(descriptor.kind);
          } break;
          case DescriptorType::Role: {
            availables.roles.emplace_back(descriptor.role);
          } break;
        }
      }

      if (currentFaction->subfactions.empty()) {
        currentFaction = nullptr;
      }
      else {
        currentFaction = &currentFaction->subfactions[std::rand() % currentFaction->subfactions.size()];
      }
    }

    return availables;
  }

  std::vector<Descriptor> GetAllUsedDescriptors(Faction const & factionRules) {
    std::vector<Descriptor> results;

    for (Faction const & subfaction : factionRules.subfactions) {
      std::vector<Descriptor> const subfactionDescriptors = GetAllUsedDescriptors(subfaction);

      for (Descriptor const & descriptor : subfactionDescriptors) {
        bool contained = false;
        for (Descriptor const & containedDescriptor : results) {
          if (descriptor == containedDescriptor) {
            contained = true;
            break;
          }
        }

        if (!contained) {
          results.emplace_back(descriptor);
        }
      }
    }

    for (Descriptor const & descriptor : factionRules.descriptors) {
      bool contained = false;
      for (Descriptor const & containedDescriptor : results) {
        if (descriptor == containedDescriptor) {
          contained = true;
          break;
        }
      }

      if (!contained) {
        results.emplace_back(descriptor);
      }
    }

    return results;
  }

  CardInfo CreateCharacterCard(std::string const & subfaction, FactionAvailables const & availables) {
    CardInfo card;

    card.type = CardType::Character;

    int const characterLevel = std::rand() % 3 + std::rand() % 3;

    for (int i = 0; i < characterLevel + 2; ++i) {
      card.sigils.emplace_back(availables.attributes[std::rand() % availables.attributes.size()]);
    }

    std::sort(card.sigils.begin(), card.sigils.end());

    card.descriptors.emplace_back(availables.kinds[std::rand() % availables.kinds.size()]);
    card.descriptors.emplace_back(availables.roles[std::rand() % availables.roles.size()]);

    card.name = subfaction + " Character";

    return card;
  }

  CardInfo CreateStructureCard(std::vector<Descriptor> const & usedDescriptors) {
    CardInfo card;

    card.type = CardType::Structure;

    while (card.descriptors.size() < 2) {
      Descriptor newDescriptor = usedDescriptors[std::rand() % usedDescriptors.size()];

      bool present = false;
      for (Descriptor const & descriptor : card.descriptors) {
        if (descriptor == newDescriptor) {
          present = true;
          break;
        }
      }

      if (!present) {
        card.descriptors.emplace_back(newDescriptor);
      }
    }

    std::sort(card.descriptors.begin(), card.descriptors.end());

    card.sigils.emplace_back(Attribute(std::rand() % int(Attribute::Count)));

    card.name = "Structure";

    return card;
  }

  CardInfo CreateActionCard(std::string const & subfaction, FactionAvailables const & availables) {
    CardInfo card;

    card.type = CardType::Action;

    int const c_maxSigilCount                      = 3;
    int const c_sigilCountWeights[c_maxSigilCount] = { 8, 3, 1 };

    int sigilCount = 1;
    {
      int totalWeight = c_sigilCountWeights[0];
      for (int i = 1; i < c_maxSigilCount; ++i) {
        totalWeight += c_sigilCountWeights[i];
        if (std::rand() % totalWeight < c_sigilCountWeights[i]) {
          sigilCount = i + 1;
        }
      }
    }

    for (int i = 0; i < sigilCount; ++i) {
      card.sigils.emplace_back(availables.actions[std::rand() % availables.actions.size()]);
    }

    std::sort(card.sigils.begin(), card.sigils.end());

    card.descriptors.emplace_back(availables.descriptors[std::rand() % availables.descriptors.size()]);

    card.name = subfaction + " Action";

    return card;
  }

  bool DisplayAndNameCard(CardInfo & card) {
    std::cout << card.ToString() << std::endl;

    std::cout << "Name: ";

    std::string name;

    std::getline(std::cin, name);

    std::cout << std::endl;

    if (name == "") {
      return false;
    }
    else {
      card.name = name;
      return true;
    }
  }

  bool CreateCard(Faction const & factionRules, std::vector<CardInfo> & cards) {
    CardInfo newCard;

    CardType const type = CardType(std::rand() % int(CardType::Count));

    static_assert(int(CardType::Count) == 3);
    switch (type) {
      case CardType::Character: {
        std::string subfactionName;
        FactionAvailables const availables = GetRandomFactionAvailables(factionRules, subfactionName);

        newCard = CreateCharacterCard(subfactionName, availables);
      } break;
      case CardType::Structure: {
        std::vector<Descriptor> const usedDescriptors = GetAllUsedDescriptors(factionRules);

        newCard = CreateStructureCard(usedDescriptors);
      } break;
      case CardType::Action: {
        std::string subfactionName;
        FactionAvailables const availables = GetRandomFactionAvailables(factionRules, subfactionName);

        newCard = CreateActionCard(subfactionName, availables);
      } break;
    }

    for (CardInfo const & card : cards) {
      if (newCard == card) {
        return true;
      }
    }

    if (DisplayAndNameCard(newCard)) {
      cards.emplace_back(newCard);

      return true;
    }

    return false;
  }

  void LoadCards(std::vector<CardInfo> & cards) {
    std::ifstream cardFile("Cards.txt");

    std::string text;

    while (true) {
      std::string currentLine;
      std::getline(cardFile, currentLine);

      if (currentLine.empty()) {
        break;
      }

      text += currentLine + '\n';
    }

    size_t currentOffset = 0;

    while (currentOffset != std::string::npos && currentOffset < text.size()) {
      size_t nextLineStart = currentOffset;

      do {
        nextLineStart = text.find_first_of('\n', nextLineStart);

        if (nextLineStart != std::string::npos) {
          ++nextLineStart;
        }

      } while (nextLineStart != std::string::npos && nextLineStart < text.size() && text[nextLineStart] == ' ');

      size_t const nextCardStart = nextLineStart;

      std::string const nextCardText = text.substr(currentOffset, nextCardStart - currentOffset - 1);

      CardInfo const nextCard = CardInfo::FromString(nextCardText);

      //TODO: validate loaded card

      cards.emplace_back(nextCard);

      currentOffset = nextCardStart;
    }
  }

  void CreateCards(std::vector<CardInfo> & cards) {
    while (CreateCard(c_factionRules, cards)) {};
  }

  void SaveCards(std::vector<CardInfo> const & cards) {
    std::ofstream cardFile("Cards.txt");

    for (CardInfo const & card : cards) {
      cardFile << card.ToString() << "\n";
    }
  }

}

int main(void) {
  std::srand(std::time(nullptr));

  std::vector<CardInfo> cards;
  LoadCards(cards);
  CreateCards(cards);
  SaveCards(cards);

  return 0;
}

