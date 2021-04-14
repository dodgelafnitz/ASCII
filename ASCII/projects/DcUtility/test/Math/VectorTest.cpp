/*
 * Copywrite 2021 Dodge Lafnitzegger
 */

#include "Math/Vector.h"
#include "gtest/gtest.h"

TEST(VectorTest, DefaultConstruct) {
  ivec<5> const vec;

  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 0);
  EXPECT_EQ(vec[2], 0);
  EXPECT_EQ(vec[3], 0);
  EXPECT_EQ(vec[4], 0);
}

template <typename T, int Count>
void TestVec(void) {
  Vector<T, Count> const vec = [](void) {
    if constexpr (Count == 1) {
      return Vector<T, Count>(1);
    }
    else if constexpr (Count == 2) {
      return Vector<T, Count>(1.0, 2.0f);
    }
    else if constexpr (Count == 3) {
      return Vector<T, Count>(1, 2.0, 3.0f);
    }
    else if constexpr (Count == 4) {
      return Vector<T, Count>(1, 2.0, 3.0f, 4);
    }
    else if constexpr (Count == 5) {
      return Vector<T, Count>(1.0, 2, 3.0, 4, 5.0f);
    }
    else {
      return Vector<T, Count>();
    }
  }();

  if constexpr (Count >= 1) {
    EXPECT_EQ(vec[0], 1);

    if constexpr (Count < 5) {
      EXPECT_EQ(vec.x, 1.0);
      EXPECT_EQ(&vec[0], &vec.x);
    }
  }

  if constexpr (Count >= 2) {
    EXPECT_EQ(vec[1], 2.0f);

    if constexpr (Count < 5) {
      EXPECT_EQ(vec.y, 2);
      EXPECT_EQ(&vec[1], &vec.y);
    }
  }

  if constexpr (Count >= 3) {
    EXPECT_EQ(vec[2], 3);

    if constexpr (Count < 5) {
      EXPECT_EQ(vec.z, 3.0f);
      EXPECT_EQ(&vec[2], &vec.z);
    }
  }

  if constexpr (Count >= 4) {
    EXPECT_EQ(vec[3], 4.0f);

    if constexpr (Count < 5) {
      EXPECT_EQ(vec.w, 4);
      EXPECT_EQ(&vec[3], &vec.w);
    }
  }

  if constexpr (Count >= 5) {
    EXPECT_EQ(vec[4], 5.0);
  }

  {
    auto otherVec = vec;
    EXPECT_TRUE(vec == otherVec);
    EXPECT_FALSE(vec != otherVec);
    EXPECT_EQ(vec, otherVec);
  }

  for (int i = 0; i < Count; ++i) {
    auto otherVec = vec;
    otherVec[i] = 0;
    EXPECT_FALSE(vec == otherVec);
    EXPECT_TRUE(vec != otherVec);
    EXPECT_NE(vec, otherVec);
  }
}

TEST(VectorTest, VectorInt1) {
  TestVec<int, 1>();
}

TEST(VectorTest, VectorFloat1) {
  TestVec<float, 1>();
}

TEST(VectorTest, VectorDouble1) {
  TestVec<double, 1>();
}

TEST(VectorTest, VectorInt2) {
  TestVec<int, 2>();
}

TEST(VectorTest, VectorFloat2) {
  TestVec<float, 2>();
}

TEST(VectorTest, VectorDouble2) {
  TestVec<double, 2>();
}

TEST(VectorTest, VectorInt3) {
  TestVec<int, 3>();
}

TEST(VectorTest, VectorFloat3) {
  TestVec<float, 3>();
}

TEST(VectorTest, VectorDouble3) {
  TestVec<double, 3>();
}

TEST(VectorTest, VectorInt4) {
  TestVec<int, 4>();
}

TEST(VectorTest, VectorFloat4) {
  TestVec<float, 4>();
}

TEST(VectorTest, VectorDouble4) {
  TestVec<double, 4>();
}

TEST(VectorTest, VectorInt5) {
  TestVec<int, 5>();
}

TEST(VectorTest, VectorFloat5) {
  TestVec<float, 5>();
}

TEST(VectorTest, VectorDouble5) {
  TestVec<double, 5>();
}
