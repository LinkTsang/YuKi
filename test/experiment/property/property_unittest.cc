#include <gtest/gtest.h>
#include <cstdio>
#include <iostream>
#include "property.h"

namespace {
TEST(Property, Assign) {
  Property<int> a = 1;
  Property<int> b = 2;
  Property<int> c;
  c.bind([&] { return a + b; }, a, b);
  printf("a=%d\n", a.get());
  printf("b=%d\n", b.get());
  printf("c=%d\n", c.get());

  EXPECT_EQ(1, a.get());
  EXPECT_EQ(2, b.get());
  EXPECT_EQ(1 + 2, c.get());

  a = 40;
  EXPECT_EQ(42, c.get());
  printf("a=%d\n", a.get());
  printf("b=%d\n", b.get());
  printf("c=%d\n", c.get());
}

TEST(Property, Cycle) {
  Property<int> a;
  Property<int> b;

  a.bind([&] { return b; }, b);
  b.bind([&] { return a; }, a);

  a = 3;
  EXPECT_EQ(3, a.get());
  EXPECT_EQ(3, b.get());
  printf("a=%d\n", a.get());
  printf("b=%d\n", b.get());
  b = 8;
  EXPECT_EQ(8, a.get());
  EXPECT_EQ(8, b.get());
  printf("a=%d\n", a.get());
  printf("b=%d\n", b.get());
}

TEST(Property, Unbind) {
  Property<int> a;
  Property<int> b;

  a.bind([&] { return b; }, b);
  b = 3;
  a.unbind();
  b = 4;
  EXPECT_EQ(3, a.get());
  printf("a=%d\n", a.get());
}

TEST(Property, Rebind) {
  Property<int> a = 1;
  Property<int> b = 2;
  Property<int> c = 3;

  a.bind([&] { return b; }, b);
  b = 4;
  EXPECT_EQ(4, a.get());
  printf("a=%d\n", a.get());

  a.bind([&] { return c; }, c);
  b = 5;
  EXPECT_EQ(3, a.get());
  printf("a=%d\n", a.get());

  c = 9;
  EXPECT_EQ(9, a.get());
  printf("a=%d\n", a.get());
}

}  // namespace