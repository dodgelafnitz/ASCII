/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include <type_traits>
#include <iostream>

template <int Begin, int End, typename Func>
constexpr Func && template_range(Func && func) {
  if constexpr (Begin == End) {
    return std::forward<Func &&>(func);
  }
  else {
    func(Begin);

    constexpr int NextVal = Begin + (Begin < End ? 1 : -1);

    return std::forward<Func &&>(template_range<NextVal, End>(std::forward<Func &&>(func)));
  }
}

struct PrintIt {
  template <int Val>
  constexpr static void func(void) {
    std::cout << Val;
  }
};

int main(void) {

  template_range<4, 0>([](int index) { std::cout << index; });
  return 0;
}
