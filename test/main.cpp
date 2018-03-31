#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "jsring.h"

namespace {
const auto test_size = 15;

TEST(construct, iterator_empty) {
  std::vector<int> it {};
  jsring::ring<int, test_size> ring{it.begin(), it.end()};
  EXPECT_TRUE(ring.empty());
}

TEST(construct, iterator_shorter_than_size) {
  std::vector<int> it {};
  it.push_back(1);
  jsring::ring<int, 2> ring{it.begin(), it.end()};
  EXPECT_FALSE(ring.empty());
  EXPECT_EQ(ring.size(), 1);

  ring.push_back(2);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 1);
  EXPECT_EQ(ring[1], 2);

  ring.push_back(3);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 3);
}

TEST(construct, iterator_equal_size) {
  std::vector<int> it {};
  it.push_back(1);
  it.push_back(2);
  jsring::ring<int, 2> ring{it.begin(), it.end()};
  EXPECT_FALSE(ring.empty());
  EXPECT_EQ(ring.size(), 2);

  EXPECT_EQ(ring[0], 1);
  EXPECT_EQ(ring[1], 2);

  ring.push_back(3);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 3);

  ring.push_back(4);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 3);
  EXPECT_EQ(ring[1], 4);
}

TEST(construct, iterator_bigger_than_size) {
  std::vector<int> it {};
  it.push_back(0);
  it.push_back(1);
  it.push_back(2);
  jsring::ring<int, 2> ring{it.begin(), it.end()};
  EXPECT_FALSE(ring.empty());
  EXPECT_EQ(ring.size(), 2);

  EXPECT_EQ(ring[0], 1);
  EXPECT_EQ(ring[1], 2);

  ring.push_back(3);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 3);

  ring.push_back(4);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 3);
  EXPECT_EQ(ring[1], 4);
}

TEST(modify, pop_back) {
  jsring::ring<int, 2> ring{1, 2};

  ring.pop_back();
  EXPECT_EQ(ring.size(), 1);
  EXPECT_EQ(ring[0], 1);

  ring.push_back(3);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 1);
  EXPECT_EQ(ring[1], 3);

  ring.push_back(4);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 3);
  EXPECT_EQ(ring[1], 4);

  ring.pop_back();
  EXPECT_EQ(ring.size(), 1);
  EXPECT_EQ(ring[0], 3);
  
  ring.pop_back();
  EXPECT_EQ(ring.size(), 0);

  ring.push_back(5);
  ring.push_back(6);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 5);
  EXPECT_EQ(ring[1], 6);
}

TEST(modify, pop_front) {
  jsring::ring<int, 2> ring{1, 2};

  ring.pop_front();
  EXPECT_EQ(ring.size(), 1);
  EXPECT_EQ(ring[0], 2);

  ring.push_back(3);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 3);

  ring.push_back(4);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 3);
  EXPECT_EQ(ring[1], 4);

  ring.pop_front();
  EXPECT_EQ(ring.size(), 1);
  EXPECT_EQ(ring[0], 4);
  
  ring.pop_front();
  EXPECT_EQ(ring.size(), 0);

  ring.push_back(5);
  ring.push_back(6);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 5);
  EXPECT_EQ(ring[1], 6);
}

TEST(modify, push_front_empty) {
  jsring::ring<int, 2> ring{};

  ring.push_front(1);
  EXPECT_EQ(ring.size(), 1);
  EXPECT_EQ(ring[0], 1);
  
  ring.push_front(2);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 1);

  ring.push_front(3);
  EXPECT_EQ(ring.size(), 2);
  EXPECT_EQ(ring[0], 3);
  EXPECT_EQ(ring[1], 2);
}

TEST(modify, push_front) {
  jsring::ring<int, 2> ring{1};

  ring.push_front(2);
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 1);
  
  ring.push_front(3);
  EXPECT_EQ(ring[0], 3);
  EXPECT_EQ(ring[1], 2);

  ring.push_front(4);
  EXPECT_EQ(ring[0], 4);
  EXPECT_EQ(ring[1], 3);
}

TEST(capacity, empty) {
  jsring::ring<int, test_size> ring;
  EXPECT_TRUE(ring.empty());
  ring.push_back(1);
  EXPECT_FALSE(ring.empty());
}

TEST(capacity, size) {
  jsring::ring<int, test_size> ring;
  EXPECT_EQ(ring.size(), 0);
  EXPECT_EQ(ring.max_size(), test_size);

  for (int i = 0; i < ring.max_size(); ++i) {
    ring.push_back(1);
    EXPECT_EQ(ring.size(), i+1);
  }

  for (int i = 0; i < ring.max_size(); ++i) {
    ring.push_back(1);
    EXPECT_EQ(ring.size(), ring.max_size());
  }
}
 
TEST(fill, tiny_empty_ring) {
  jsring::ring<int, 1> ring;
  ring.fill(42);
  EXPECT_EQ(ring[0], 42);
  ring.fill(23);
  EXPECT_EQ(ring[0], 23);
}

TEST(fill, tiny_filled_ring) {
  jsring::ring<int, 1> ring;
  ring.push_back(99);
  ring.fill(42);
  EXPECT_EQ(ring[0], 42);
  ring.fill(23);
  EXPECT_EQ(ring[0], 23);
}

TEST(fill, from_empty) {
  jsring::ring<int, test_size> ring;
  ring.fill(42);

  for (int i = 0; i < test_size; ++i) {
    EXPECT_EQ(ring[i], 42);
  }
}

TEST(fill, override_at_offset) {
  jsring::ring<int, test_size> ring;

  ring.push_back(23);
  ring.fill(42);

  for (int i = 0; i < test_size; ++i) {
    EXPECT_EQ(ring[i], 42);
  }

  ring.push_back(32);
  EXPECT_EQ(ring[test_size-1], 32);
}

TEST(operations, swap) {
  jsring::ring<int, test_size> ring1, ring2;

  for (int i = 0; i < test_size; ++i) {
    ring1.push_back(i + test_size);
    ring2.push_back(i);
  }

  std::swap(ring1, ring2);

  for (int i = 0; i < test_size; ++i) {
    EXPECT_EQ(ring1[i], i);
    EXPECT_EQ(ring2[i], i + test_size);
  }

  ring1.swap(ring2);

  for (int i = 0; i < test_size; ++i) {
    EXPECT_EQ(ring1[i], i + test_size);
    EXPECT_EQ(ring2[i], i);
  }
}

TEST(operations, swap_from_different_offset) {
  jsring::ring<int, test_size> ring1, ring2;

  ring1.push_back(-1);
  ring1.push_back(-2);
  ring1.push_back(-3);

  for (int i = 0; i < test_size; ++i) {
    ring1.push_back(i + test_size);
    ring2.push_back(i);
  }

  std::swap(ring1, ring2);

  for (int i = 0; i < test_size; ++i) {
    EXPECT_EQ(ring1[i], i);
    EXPECT_EQ(ring2[i], i + test_size);
  }

  ring1.swap(ring2);

  for (int i = 0; i < test_size; ++i) {
    EXPECT_EQ(ring1[i], i + test_size);
    EXPECT_EQ(ring2[i], i);
  }
}

TEST(accessor, offset) {
  jsring::ring<int, test_size> ring;

  for (int i = 0; i < ring.max_size(); ++i) {
    ring.push_back(i);
  }
  for (int i = 0; i < ring.max_size(); ++i) {
    EXPECT_EQ(ring[i], i);
  }
  for (int i = 0; i < ring.max_size(); ++i) {
    ring.push_back(10 + i);
  }
  for (int i = 0; i < ring.max_size(); ++i) {
    EXPECT_EQ(ring[i], 10 + i);
  }
}

TEST(accessor, at) {
  jsring::ring<int, test_size> ring;

  for (int i = 0; i < ring.max_size(); ++i) {
    ring.push_back(i);
  }
  for (int i = 0; i < ring.max_size(); ++i) {
    EXPECT_EQ(ring.at(i), i);
  }
  for (int i = 0; i < ring.max_size(); ++i) {
    ring.push_back(10 + i);
  }
  for (int i = 0; i < ring.max_size(); ++i) {
    EXPECT_EQ(ring.at(i), 10 + i);
  }

  EXPECT_ANY_THROW({ ring.at(ring.max_size()); });
  EXPECT_ANY_THROW({ ring.at(-1); });
}

TEST(accessor, offset_write) {
  jsring::ring<int, 2> ring{1, 2};

  ring[0] = 42;
  EXPECT_EQ(ring[0], 42);
  EXPECT_EQ(ring[1], 2);

  ring.push_back(3);
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 3);

  ring[1] = 43;
  EXPECT_EQ(ring[0], 2);
  EXPECT_EQ(ring[1], 43);

  ring.push_back(4);
  EXPECT_EQ(ring[0], 43);
  EXPECT_EQ(ring[1], 4);
}
  

TEST(accessor, front) {
  jsring::ring<int, test_size> ring;

  // While the buffer isn't filled the front item is the first we inserted ...
  for (int i = 0; i < ring.max_size(); ++i) {
    ring.push_back(i);
    EXPECT_EQ(ring.front(), 0);
  }

  // .. once the ring is filled we're moving
  for (int i = ring.max_size(); i < 4 * ring.max_size(); ++i) {
    ring.push_back(i);
    EXPECT_EQ(ring.front(), i - ring.max_size() + 1);
  }
}

TEST(accessor, back) {
  jsring::ring<int, test_size> ring;
  for (int i = 0; i < 4 * ring.max_size(); ++i) {
    ring.push_back(i);
    EXPECT_EQ(ring.back(), i);
  }
}

TEST(iterator, equality) {
  jsring::ring<int, test_size> ring;

  EXPECT_EQ(ring.begin(), ring.end());

  ring.push_back(1);
  EXPECT_NE(ring.begin(), ring.end());
  EXPECT_EQ(++ring.begin(), ring.end());

  ring.push_back(2);
  EXPECT_NE(ring.begin(), ring.end());
  EXPECT_NE(++ring.begin(), ring.end());
}

TEST(iterator, decrement) {
  jsring::ring<int, 3> ring{1};

  EXPECT_NE(ring.begin(), ring.end());
  EXPECT_EQ(ring.begin(), --ring.end());

  ring.push_back(2);
  EXPECT_NE(ring.begin(), ring.end());
  EXPECT_NE(ring.begin(), --ring.end());
  EXPECT_EQ(ring.begin(), --(--ring.end()));

  ring.push_back(3);
  EXPECT_NE(ring.begin(), ring.end());
  EXPECT_NE(ring.begin(), --ring.end());
  EXPECT_EQ(ring.begin(), --(--(--ring.end())));

  ring.push_back(4);
  EXPECT_NE(ring.begin(), ring.end());
  EXPECT_NE(ring.begin(), --ring.end());
  EXPECT_EQ(ring.begin(), --(--(--ring.end())));
}

TEST(iterator, rangebased) {
  jsring::ring<int, test_size> ring;

  int count = 0;
  for (auto &&e : ring) {
    ++count;
  }
  EXPECT_EQ(count, 0);

  for (int i = 0; i < test_size; ++i) {
    ring.push_back(i);
  }

  for (auto &&e : ring) {
    EXPECT_EQ(e, count++);
  }
  EXPECT_EQ(count, test_size);

  count = 0;

  // the ring is full, by pushing one element more iteration start's later and
  // wraps around
  ring.push_back(test_size);

  for (auto &&e : ring) {
    // mind: here we do pre-increment since the first item was lost in the push
    EXPECT_EQ(e, ++count);
  }
  EXPECT_EQ(count, test_size);
}

TEST(iterator, const) {
  const jsring::ring<int, 4> ring{1, 2, 3, 4};
  int count = 0;
  for (auto &&e : ring) {
    ++count;
  }
  EXPECT_EQ(count, 4);
}

TEST(non_copyable, basic) {
  jsring::ring<std::unique_ptr<int>, 2> ring;
  ring.push_back(std::move(std::make_unique<int>(1)));
  ring.push_back(std::move(std::make_unique<int>(2)));
  ring.push_back(std::move(std::make_unique<int>(3)));

  EXPECT_EQ(*ring[0], 2);
  EXPECT_EQ(*ring[1], 3);
}

TEST(non_copyable, move_costruct) {
  jsring::ring<std::unique_ptr<int>, 2> ring1;
  ring1.push_back(std::move(std::make_unique<int>(1)));
  ring1.push_back(std::move(std::make_unique<int>(2)));
  ring1.push_back(std::move(std::make_unique<int>(3)));

  jsring::ring<std::unique_ptr<int>, 2> ring2{std::move(ring1)};

  EXPECT_EQ(*ring2[0], 2);
  EXPECT_EQ(*ring2[1], 3);
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}
