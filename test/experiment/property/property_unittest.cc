#include <gtest/gtest.h>
#include <cstdio>
#include <iostream>
#include "property.h"

namespace {

TEST(Property, BindingProperty) {
  Property<int> a = 1;
  Property<int> b = 2;

  EXPECT_EQ(1, a.get());
  a.bind(b);
  EXPECT_EQ(2, a.get());
  b = 3;
  EXPECT_EQ(3, a.get());

  Property<int> c = 4;
  a.bind(c);
  EXPECT_EQ(4, a.get());
  b = 5;
  EXPECT_EQ(4, a.get());
}

TEST(Property, Lambda) {
  Property<int> a = 1;
  Property<int> b = 2;
  Property<int> c;

  EXPECT_EQ(1, a.get());
  EXPECT_EQ(2, b.get());
  c.bind([&] { return a + b; }, a, b);
  EXPECT_EQ(1 + 2, c.get());

  a = 40;
  EXPECT_EQ(42, c.get());
  b = -2;
  EXPECT_EQ(38, c.get());
}

TEST(Property, Cycle) {
  Property<int> a;
  Property<int> b;

  a.bind([&] { return b; }, b);
  b.bind([&] { return a; }, a);

  a = 3;
  EXPECT_EQ(3, a.get());
  EXPECT_EQ(3, b.get());

  b = 8;
  EXPECT_EQ(8, a.get());
  EXPECT_EQ(8, b.get());
}

TEST(Property, Unbind) {
  Property<int> a;
  Property<int> b;

  a.bind(b);
  b = 3;
  EXPECT_EQ(3, a.get());
  a.unbind();
  b = 4;
  EXPECT_EQ(3, a.get());
}

TEST(Property, Rebind) {
  Property<int> a = 1;
  Property<int> b = 2;
  Property<int> c = 3;

  a.bind([&] { return b; }, b);
  b = 4;
  EXPECT_EQ(4, a.get());

  a.bind([&] { return c; }, c);
  b = 5;
  EXPECT_EQ(3, a.get());

  c = 9;
  EXPECT_EQ(9, a.get());
}

}  // namespace