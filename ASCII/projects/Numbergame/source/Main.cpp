/*
 * Copywrite 2022 Dodge Lafnitzegger
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

namespace {
  int const c_handSize = 9;
  struct Hand {
    bool operator ==(Hand const &) const = default;
    bool operator !=(Hand const &) const = default;

    unsigned char values[c_handSize];
  };

  Hand const c_baseHand = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  Hand const c_emptyHand = { 0 };

  Hand GetRandomHand(void) {
    Hand hand = c_baseHand;

    for (int i = 0; i < c_handSize; ++i) {
      std::swap(hand.values[i], hand.values[std::rand() % c_handSize]);
    }

    return hand;
  }

  int GetCodeFromHand(Hand const & hand) {
    int code = 0;

    for (int i = 0; i < c_handSize; ++i) {
      code *= c_handSize;
      code += hand.values[i] - 1;
    }

    return code;
  }

  Hand GetHandFromCode(int code) {
    Hand hand = c_emptyHand;

    for (int i = c_handSize - 1; i >= 0; --i) {
      hand.values[i] = (code % c_handSize) + 1;
      code /= c_handSize;
    }

    return hand;
  }

  int EvaluateGameScore(Hand const & hand0, Hand const & hand1) {
    bool takenValues[c_handSize] = { 0 };

    int score0 = 0;
    int score1 = 0;

    for (int i = 0; i < c_handSize; ++i) {
      int const value0 = hand0.values[i];
      int const value1 = hand1.values[i];

      if (value0 == value1) {
        continue;
      }

      if (!takenValues[value0 - 1]) {
        takenValues[value0 - 1] = true;
        score0 += value0;
      }

      if (!takenValues[value1 - 1]) {
        takenValues[value1 - 1] = true;
        score1 += value1;
      }
    }

    return score1 - score0;
  }

  int const c_handsCount = 100000;
}

int main(void) {
  std::srand(std::time(nullptr));

  std::vector<int> hands;
  hands.reserve(c_handsCount);
  for (int i = 0; i < c_handsCount; ++i) {
    hands.emplace_back(GetCodeFromHand(GetRandomHand()));
  }

  std::vector<int> wins;
  wins.resize(c_handsCount, 0);

  for (int i = 0; i < c_handsCount - 1; ++i) {
    for (int j = i + 1; j < c_handsCount; ++j) {
      Hand const hand0 = GetHandFromCode(hands[i]);
      Hand const hand1 = GetHandFromCode(hands[j]);

      int winner = EvaluateGameScore(hand0, hand1);

      if (winner < 0) {
        ++wins[i];
      }
      else if (winner > 0) {
        ++wins[j];
      }
    }
  }

  int bestIndex = 0;
  int bestScore = wins[0];

  for (int i = 1; i < c_handsCount; ++i) {
    if (wins[i] > bestScore) {
      bestIndex = 1;
      bestScore = wins[i];
    }
  }

  std::cout << "Best Hand (" << bestScore << " wins, " << c_handsCount - bestScore - 1 << " losses):" << std::endl;
  std::cout << "  { ";

  Hand const bestHand = GetHandFromCode(hands[bestIndex]);

  for (int i = 0; i < c_handSize; ++i) {
    if (i != 0) {
      std::cout << ", ";
    }

    std::cout << int(bestHand.values[i]);
  }
  std::cout << " }" << std::endl;

  std::cin.get();

  return 0;
}
