#include <cstdio>
#include <iostream>
#include "property.h"

#include <cassert>
#define EXPECT_EQ(EXCEPT, ACTUAL)                                             \
  if ((EXCEPT) != (ACTUAL)) {                                                 \
    std::cerr << "EXPECT_EQ(" << (EXCEPT) << ", " << (ACTUAL) << ") FAILED!"; \
    exit(-1);                                                                 \
  }

void runPropertyTest() {
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

void runPropertyCycleTest() {
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

int main() {
  runPropertyTest();
  runPropertyCycleTest();
}
