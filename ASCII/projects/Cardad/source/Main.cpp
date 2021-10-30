/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>

#include "Containers/DynamicArray.h"

namespace {
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
    DynamicArray<Element, Count> m_data;
  };

  template <auto T>
  using ExpectedCountType = std::conditional_t<std::is_same_v<decltype(T), std::nullptr_t>, int, decltype(T)>;

  template <typename Base, auto ExpectedCount = nullptr, typename Element, int Count>
  DataTable<Base, Element, Count, ExpectedCountType<ExpectedCount>> MakeDataTable(
    Element const (& array)[Count]
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
    std::pair<ExpectedCountType<ExpectedCount>, Element> const (& array)[Count]
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
      Set<IndexForType<U>>(value);
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
    Indexer m_type                           = Count;
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
  };

  enum class Faction {
  };

  enum class CardRequirementType {
    Level,
    Emblem,
    Faction,
    Owner,
    Controller,
    Lane,
    And,
    Or,
    Not,

    Count
  };

  static const int RequirementCount = 8;

  struct CardRequirementLevel;
  struct CardRequirementEmblem;
  struct CardRequirementFaction;
  struct CardRequirementOwner;
  struct CardRequirementController;
  struct CardRequirementLane;
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
    CardRequirementAnd,
    CardRequirementOr,
    CardRequirementNot
  >;

  struct CardRequirementLevel {
    int level;
  };

  struct CardRequirementEmblem {
    Emblem emblem;
  };

  struct CardRequirementFaction {
    Faction faction;
  };

  struct CardRequirementOwner {
  };

  struct CardRequirementController {
  };

  struct CardRequirementLane {
  };

  struct CardRequirementAnd {
    DynamicArray<std::shared_ptr<CardRequirement>, RequirementCount> requirements;
  };

  struct CardRequirementOr {
    DynamicArray<std::shared_ptr<CardRequirement>, RequirementCount> requirements;
  };

  struct CardRequirementNot {
    std::shared_ptr<CardRequirement> requirement;
  };

  struct EmblemData {
    EmblemData(
      char const *            name,
      int                     generationMod,
      CardRequirement const & cardRequirement
    ) :
      name(name),
      generationMod(generationMod),
      cardRequirement(cardRequirement)
    {}

    char const *    name;
    int             generationMod;
    CardRequirement cardRequirement;
  };

  static const int FactionPrimaryEmblemCount   = 5;
  static const int FactionSecondaryEmblemCount = 12;

  struct FactionData {
    char const *                                      name;
    DynamicArray<Emblem, FactionPrimaryEmblemCount>   primaryEmblems;
    DynamicArray<Emblem, FactionSecondaryEmblemCount> secondaryEmblems;
  };
}

int main(void) {
  std::srand(std::time(nullptr));

  return 0;
};
