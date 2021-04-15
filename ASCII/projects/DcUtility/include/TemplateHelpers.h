/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#ifndef DCUTILITY_TEMPLATEHELPERS_H
#define DCUTILITY_TEMPLATEHELPERS_H

#include <utility>
#include <type_traits>

template <int Begin, int End, typename Func>
constexpr Func && template_range(Func && func) {
  if constexpr (Begin == End) {
    return std::forward<Func &&>(func);
  }
  else {
    func.operator ()<std::integral_constant<int, Begin>>();

    constexpr int NextVal = Begin + (Begin < End ? 1 : -1);

    return std::forward<Func &&>(template_range<NextVal, End>(std::forward<Func &&>(func)));
  }
}

#endif // DCUTILITY_TEMPLATEHELPERS_H
