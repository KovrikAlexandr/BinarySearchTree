#include <gtest/gtest.h>
#include <Set/Set.hpp>
#include <vector>

TEST(BaseTestSuite, InsertTest) {
    Set<int> s;
    s.insert(5);
    auto p1 = s.insert(5);
    auto p2 = s.insert(6);

    ASSERT_TRUE(s.contains(5));
    ASSERT_FALSE(s.contains(9));
    ASSERT_FALSE(p1.second);
    ASSERT_TRUE(p2.second);
}

TEST(BaseTestSuite, EraseTest) {
    Set<int> s;
    s.insert(5);
    s.insert(8);

    s.erase(5);
    bool erase_result = s.erase(7);

    ASSERT_TRUE(s.contains(8));
    ASSERT_FALSE(s.contains(5));
    ASSERT_FALSE(erase_result);
}

TEST(BaseTestSuite, EqualityTest) {
    Set<int> s;
    Set<int> r;

    s.insert(5);
    s.insert(3);
    s.insert(6);

    r.insert(5);
    r.insert(6);

    ASSERT_FALSE(s == r);

    s.erase(3);

    ASSERT_TRUE(s == r);
}

TEST(BaseTestSuite, AssigmentTest) {
    Set<int> s;

    s.insert(5);
    s.insert(3);
    s.insert(6);

    Set r = s;

    ASSERT_TRUE(r.contains(5));
    ASSERT_TRUE(r.contains(3));
    ASSERT_TRUE(r.contains(6));
    ASSERT_FALSE(r.contains(1));
}

TEST(InorderTestSuite, IteratorTest) {
    Set<int> s;
    s.insert(5);
    s.insert(3);
    s.insert(6);

    std::vector<int> values;
    auto it = s.begin();
    while(it != s.end()) {
        values.push_back(*it);
        ++it;
    }
    ASSERT_EQ(values[0], 3);
    ASSERT_EQ(values[1], 5);
    ASSERT_EQ(values[2], 6);
}

TEST(InorderTestSuite, ReverseIteratorTest) {
    Set<int> s;
    s.insert(5);
    s.insert(3);
    s.insert(6);

    std::vector<int> values;
    auto it = s.rbegin();
    while(it != s.rend()) {
        values.push_back(*it);
        ++it;
    }
    ASSERT_EQ(values[0], 6);
    ASSERT_EQ(values[1], 5);
    ASSERT_EQ(values[2], 3);
}

TEST(PreorderTestSuite, IteratorTest) {
    Set<int, iterator_order_traits::preorder_iterator_tag> s;
    s.insert(5);
    s.insert(3);
    s.insert(6);

    std::vector<int> values;
    auto it = s.begin();
    while(it != s.end()) {
        values.push_back(*it);
        ++it;
    }
    ASSERT_EQ(values[0], 5);
    ASSERT_EQ(values[1], 3);
    ASSERT_EQ(values[2], 6);
}

TEST(PreorderTestSuite, ReverseIteratorTest) {
    Set<int, iterator_order_traits::preorder_iterator_tag> s;
    s.insert(5);
    s.insert(3);
    s.insert(6);

    std::vector<int> values;
    auto it = s.rbegin();
    while(it != s.rend()) {
        values.push_back(*it);
        ++it;
    }
    ASSERT_EQ(values[0], 6);
    ASSERT_EQ(values[1], 3);
    ASSERT_EQ(values[2], 5);
}

TEST(PostorderTestSuite, IteratorTest) {
    Set<int, iterator_order_traits::postorder_iterator_tag> s;
    s.insert(5);
    s.insert(3);
    s.insert(6);

    std::vector<int> values;
    auto it = s.begin();
    while(it != s.end()) {
        values.push_back(*it);
        ++it;
    }
    std::cout << '\n';
    ASSERT_EQ(values[0], 3);
    ASSERT_EQ(values[1], 6);
    ASSERT_EQ(values[2], 5);
}

TEST(PostorderTestSuite, ReverseIteratorTest) {
    Set<int, iterator_order_traits::postorder_iterator_tag> s;
    s.insert(5);
    s.insert(3);
    s.insert(6);

    std::vector<int> values;
    auto it = s.rbegin();
    while(it != s.rend()) {
        values.push_back(*it);
        ++it;
    }
    ASSERT_EQ(values[0], 5);
    ASSERT_EQ(values[1], 6);
    ASSERT_EQ(values[2], 3);
}