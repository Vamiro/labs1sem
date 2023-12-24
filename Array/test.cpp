#include "pch.h"

// Test case for Array class reserve method
TEST(Array, ReserveTest) {
    Array<int> array;
    std::vector<int> std_vector;
    array.reserve(5);
    std_vector.reserve(5);
    ASSERT_EQ(array.capacity(), std_vector.capacity());
}

// Test case for Array class
TEST(Array, PushBackTest) {
    Array<int> array;
    std::vector<int> std_vector;
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);

    std_vector.push_back(1);
    std_vector.push_back(2);
    std_vector.push_back(3);

    // Assert the sizes are equal
    ASSERT_EQ(array.size(), std_vector.size());

    // Assert each element is equal
    for (size_t i = 0; i < array.size(); ++i) {
        ASSERT_EQ(array[i], std_vector[i]);
    }
}

// Test case for Array class insert method
TEST(Array, InsertTest) {
    Array<int> array{1, 2};
    std::vector<int> std_vector{ 1, 3, 2 };
    array.insert(1, 3);

    ASSERT_EQ(array.size(), std_vector.size());

    for (size_t i = 0; i < array.size(); ++i) {
        ASSERT_EQ(array[i], std_vector[i]);
    }
}

// Test case for Array class remove method
TEST(Array, RemoveTest) {
    Array<int> array{1, 2, 3};
    std::vector<int> std_vector{1, 3};

    array.remove(1);


    ASSERT_EQ(array.size(), std_vector.size());

    for (size_t i = 0; i < array.size(); ++i) {
        ASSERT_EQ(array[i], std_vector[i]);
    }
}

// Test case for Array class iterator methods
TEST(Array, IteratorTest) {
    Array<int> array{1, 2, 3};
    std::vector<int> std_vector{1, 2, 3};

    int sum_array = 0;
    for (auto it = array.begin(); it != array.end(); ++it) {
        sum_array += *it;
    }

    int sum_std_vector = 0;
    for (auto it = std_vector.begin(); it != std_vector.end(); ++it) {
        sum_std_vector += *it;
    }

    ASSERT_EQ(sum_array, sum_std_vector);
}

// Test case for Array class copy assignment
TEST(Array, CopyAssignmentTest) {
    Array<int> array{1, 2, 3};
    std::vector<int> std_vector{1, 2, 3};
    Array<int> array2;
    array2 = array;

    std::vector<int> std_vector2;
    std_vector2 = std_vector;

    ASSERT_EQ(array2.size(), std_vector2.size());

    for (size_t i = 0; i < array2.size(); ++i) {
        ASSERT_EQ(array2[i], std_vector2[i]);
    }
}

// Test case for Array class move assignment
TEST(Array, MoveAssignmentTest) {
    Array<int> array{1, 2, 3};
    std::vector<int> std_vector{1, 2, 3};
    Array<int> array2;
    array2 = std::move(array);

    std::vector<int> std_vector2;
    std_vector2 = std::move(std_vector);

    ASSERT_EQ(array2.size(), std_vector2.size());

    for (size_t i = 0; i < array2.size(); ++i) {
        ASSERT_EQ(array2[i], std_vector2[i]);
    }
}

// Test case for Array class subscript operator
TEST(Array, SubscriptOperatorTest) {
    Array<int> array{1, 2, 3};

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
}

TEST(Array, KnownSort) {
    Array<int> array{ 16, 17, 19, 9, 14, 11, 10, 3, 1, 7, 2, 15, 8, 6, 4, 5, 13, 20, 18, 12 };
    std::vector<int> std_vector{ 16, 17, 19, 9, 14, 11, 10, 3, 1, 7, 2, 15, 8, 6, 4, 5, 13, 20, 18, 12 };
    
    ssort(std::begin(array), std::end(array), [](int a, int b) { return a < b; });
    std::sort(std_vector.begin(), std_vector.end());

    bool res = true;

    for (int i = 0; i < 20; i++) {
        res &= array[i] == std_vector[i];
    }

    ASSERT_TRUE(res);
}

TEST(Array, UnknownSort) {
    Array<int> array;
    std::vector<int> std_vector;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1000);

    for (int i = 0; i < 100; i++) {
        int n = dist(gen);
        array.push_back(n);
        std_vector.push_back(n);
    }

    ssort(std::begin(array), std::end(array), [](int a, int b) { return a < b; });
    std::sort(std_vector.begin(), std_vector.end());

    bool res = true;

    for (int i = 0; i < 100; i++) {
        res &= array[i] == std_vector[i];
    }

    ASSERT_TRUE(res);
}

TEST(Array, ArrSortTime) {
    Array<int> array;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1000);

    for (int i = 0; i < 1000000; i++) {
        int n = dist(gen);
        array.push_back(n);
    }

    ssort(std::begin(array), std::end(array), [](int a, int b) { return a < b; });
}

TEST(Array, VecSortTime) {
    std::vector<int> std_vector;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1000);

    for (int i = 0; i < 1000000; i++) {
        int n = dist(gen);
        std_vector.push_back(n);
    }

    ssort(std::begin(std_vector), std::end(std_vector), [](int a, int b) { return a < b; });
    //std::sort(std_vector.begin(), std_vector.end());
}

TEST(Array, Arrtime) {
    Array<int> array;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1000);

    for (int i = 0; i < 1000000; i++) {
        int n = dist(gen);
        array.push_back(n);
    }
}

TEST(Array, Vectime) {
    std::vector<int> std_vector;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1000);

    for (int i = 0; i < 1000000; i++) {
        int n = dist(gen);
        std_vector.push_back(n);
    }
}

TEST(Array, StringSort) {
    Array<std::string> array{ "sort", "array" };
    std::vector<std::string> std_vector{ "sort", "string", "array" };
    array.insert(1, "string");
    array.push_back("string");
    array.remove(array.size() - 1);
    Array<std::string> array2(array);

    Array<std::string> array3(std::move(array2));

    bool res = true;

    for (int i = 0; i < 3; i++) {
        res &= array3[i] == std_vector[i];
    }

    ASSERT_TRUE(res);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}