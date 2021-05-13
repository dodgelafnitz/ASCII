/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Vector.h"
#include "gtest/gtest.h"

#include <tuple>

#include "Helpers/TemplateHelpers.h"
#include "Helpers/TestHelpers.h"

namespace {
  template <typename T>
  void ExpectAlmostEqual(T const & val0, T const & val1) {
    EXPECT_EQ(val0, val1);
  }

  template <>
  void ExpectAlmostEqual<float>(float const & val0, float const & val1) {
    EXPECT_FLOAT_EQ(val0, val1);
  }

  template <>
  void ExpectAlmostEqual<double>(double const & val0, double const & val1) {
    EXPECT_DOUBLE_EQ(val0, val1);
  }
}

TEST(VectorTest, DefaultConstruct) {
  ivec<5> const vec;

  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 0);
  EXPECT_EQ(vec[2], 0);
  EXPECT_EQ(vec[3], 0);
  EXPECT_EQ(vec[4], 0);
}

TEST(VectorTest, Assign) {
  ivec<5>       vec0;
  fvec<5> const vec1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f);
  ivec<5> const vec2(0, 1, 2, 3, 4);
  ivec<5> const vec3(0, 2, 4, 6, 8);
  ivec<5> const vec4(0, 0, 0, 0, 0);
  ivec<5> const vec5(0, 1, 4, 9, 16);
  fvec<5>       vec6;
  fvec<5> const vec7(0.0f, 0.5f, 1.0f, 1.5f, 2.0f);
  ivec<5> const vec8(4, 3, 2, 2, 3);
  ivec<5> const vec9(0, 1, 0, 1, 1);
  ivec<5> const vec10(1, 1, 1, 1, 1);
  ivec<5> const vec11(5, 4, 3, 2, 1);
  ivec<5> const vec12(1, 2, 3, 4, 5);
  ivec<5> const vec13(1, 2, 3, 2, 1);
  ivec<5> const vec14(5, 4, 3, 4, 5);

  vec0 = vec1;
  EXPECT_EQ(vec0, vec2);

  vec0 += vec1;
  EXPECT_EQ(vec0, vec3);

  vec0 -= vec3;
  EXPECT_EQ(vec0, vec4);

  vec0 = vec1;
  vec0 *= vec1;
  EXPECT_EQ(vec0, vec5);

  vec6 = vec0;
  vec6 /= vec3;
  EXPECT_NE(vec6[0], vec6[0]);

  vec6[0] = 0;
  EXPECT_EQ(vec6, vec7);

  vec0 %= vec8;
  EXPECT_EQ(vec0, vec9);

  int const dotTotal = vec0.Dot(vec8);
  EXPECT_EQ(dotTotal, 8);

  EXPECT_EQ(vec2 + vec2, vec3);
  EXPECT_EQ(vec3 - vec2, vec2);
  EXPECT_EQ(vec2 * vec2, vec5);
  EXPECT_EQ(vec8 / vec8, vec10);
  EXPECT_EQ(vec5 % vec8, vec9);

  EXPECT_EQ(vec11.Min(vec12), vec13);
  EXPECT_EQ(vec11.Max(vec12), vec14);
}

template <typename T, int Count, typename ... Params >
void TestVec(T const & differentValue, Params && ... values) {
  auto tupledValues = std::forward_as_tuple<Params && ...>(std::forward<Params &&>(values)...);
  Vector<T, Count> const vec(std::forward<Params &&>(values)...);

  template_range<0, Count>([&tupledValues, &vec]<typename T>() constexpr {
    EXPECT_EQ(vec[T::value], std::get<T::value>(tupledValues));
  });

  if constexpr (Count >= 1 && Count < 5) {
    EXPECT_EQ(vec.x, std::get<0>(tupledValues));
    EXPECT_EQ(&vec[0], &vec.x);
  }

  if constexpr (Count >= 2 && Count < 5) {
    EXPECT_EQ(vec.y, std::get<1>(tupledValues));
    EXPECT_EQ(&vec[1], &vec.y);
  }

  if constexpr (Count >= 3 && Count < 5) {
    EXPECT_EQ(vec.z, std::get<2>(tupledValues));
    EXPECT_EQ(&vec[2], &vec.z);
  }

  if constexpr (Count >= 4 && Count < 5) {
    EXPECT_EQ(vec.w, std::get<3>(tupledValues));
    EXPECT_EQ(&vec[3], &vec.w);
  }

  {
    auto otherVec = vec;
    EXPECT_TRUE(vec == otherVec);
    EXPECT_FALSE(vec != otherVec);
    EXPECT_EQ(vec, otherVec);
  }

  for (int i = 0; i < Count; ++i) {
    auto otherVec = vec;
    otherVec[i] = differentValue;
    EXPECT_FALSE(vec == otherVec);
    EXPECT_TRUE(vec != otherVec);
    EXPECT_NE(vec, otherVec);
  }

  T totalSum = T(0);
  template_range<0, Count>([&tupledValues, &totalSum]<typename T>() constexpr {
    totalSum += std::get<T::value>(tupledValues);
  });
  ExpectAlmostEqual(totalSum, vec.Sum());

  T totalProduct = T(1);
  template_range<0, Count>([&tupledValues, &totalProduct]<typename T>() constexpr {
    totalProduct *= std::get<T::value>(tupledValues);
  });
  ExpectAlmostEqual(totalProduct, vec.Product());

  T totalLengthSquared = T(0);
  template_range<0, Count>([&tupledValues, &totalLengthSquared]<typename T>() constexpr {
    totalLengthSquared += std::get<T::value>(tupledValues) * std::get<T::value>(tupledValues);
  });
  ExpectAlmostEqual(totalLengthSquared, vec.LengthSquared());
}

TEST(VectorTest, VectorInt1) {
  TestVec<int, 1>(0, 1);
}

TEST(VectorTest, VectorFloat1) {
  TestVec<float, 1>(0.0f, 1.0f);
}

TEST(VectorTest, VectorDouble1) {
  TestVec<double, 1>(0.0, 1.0);
}

TEST(VectorTest, VectorInt2) {
  TestVec<int, 2>(-4, 2, 56);
}

TEST(VectorTest, VectorFloat2) {
  TestVec<float, 2>(100.0f, -2.0f, 30.0f);
}

TEST(VectorTest, VectorDouble2) {
  TestVec<double, 2>(10.0, 14.0, 0.444444444);
}

TEST(VectorTest, VectorInt3) {
  TestVec<int, 3>(3, 6, 12, 8);
}

TEST(VectorTest, VectorFloat3) {
  TestVec<float, 3>(1.0f, 0.5f, 0.2f, 5.1f);
}

TEST(VectorTest, VectorDouble3) {
  TestVec<double, 3>(0.01, 0.2, 0.0, 2.5);
}

TEST(VectorTest, VectorInt4) {
  TestVec<int, 4>(0, 1, 6, 19, -4);
}

TEST(VectorTest, VectorFloat4) {
  TestVec<float, 4>(4.9f, 21.5f, 426.0f, 13.41f, 9715.3f);
}

TEST(VectorTest, VectorDouble4) {
  TestVec<double, 4>(725.2, 74.6, 23.899, 2.4, 435843.1);
}

TEST(VectorTest, VectorInt5) {
  TestVec<int, 5>(175, -71, -517574, 44, 23, 73);
}

TEST(VectorTest, VectorFloat5) {
  TestVec<float, 5>(1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
}

TEST(VectorTest, VectorDouble5) {
  TestVec<double, 5>(15.0, 345735.0, 25.23, 246.2, -3246.33, -124.7);
}

TEST(VectorTest, CountIs1_DefaultConstructed_ElementIsDefaultConstructed) {
  Vector<DefaultConstructorCheck, 1> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
}

TEST(VectorTest, CountIs2_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 2> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
  EXPECT_TRUE(vec.y.WasDefaultConstructed());
}

TEST(VectorTest, CountIs3_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 3> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
  EXPECT_TRUE(vec.y.WasDefaultConstructed());
  EXPECT_TRUE(vec.z.WasDefaultConstructed());
}

TEST(VectorTest, CountIs4_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 4> const vec;
  EXPECT_TRUE(vec.x.WasDefaultConstructed());
  EXPECT_TRUE(vec.y.WasDefaultConstructed());
  EXPECT_TRUE(vec.z.WasDefaultConstructed());
  EXPECT_TRUE(vec.w.WasDefaultConstructed());
}

TEST(VectorTest, CountIs5_DefaultConstructed_ElementsAreDefaultConstructed) {
  Vector<DefaultConstructorCheck, 5> const vec;
  EXPECT_TRUE(vec[0].WasDefaultConstructed());
  EXPECT_TRUE(vec[1].WasDefaultConstructed());
  EXPECT_TRUE(vec[2].WasDefaultConstructed());
  EXPECT_TRUE(vec[3].WasDefaultConstructed());
  EXPECT_TRUE(vec[4].WasDefaultConstructed());
}

TEST(VectorTest, ConstAndCountIs1_CheckIndexAndNamedElement_ReferencesAreSame) {
  Vector<int, 1> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
}

TEST(VectorTest, ConstAndCountIs2_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 2> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
}

TEST(VectorTest, ConstAndCountIs3_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 3> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
}

TEST(VectorTest, ConstAndCountIs4_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 4> const vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
  EXPECT_EQ(&vec.w, &vec[3]);
}

TEST(VectorTest, NonConstAndCountIs1_CheckIndexAndNamedElement_ReferencesAreSame) {
  Vector<int, 1> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
}

TEST(VectorTest, NonConstAndCountIs2_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 2> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
}

TEST(VectorTest, NonConstAndCountIs3_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 3> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
}

TEST(VectorTest, NonConstAndCountIs4_CheckIndexAndNamedElements_ReferencesAreSame) {
  Vector<int, 4> vec;
  EXPECT_EQ(&vec.x, &vec[0]);
  EXPECT_EQ(&vec.y, &vec[1]);
  EXPECT_EQ(&vec.z, &vec[2]);
  EXPECT_EQ(&vec.w, &vec[3]);
}
