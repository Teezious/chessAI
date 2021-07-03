#include <gtest/gtest.h>

#include "MoveSorter.h"
#include <numeric>

TEST(SortTest, InsertionSortTest)
{
    std::vector< int > moveScores { 3, 4, 5, 6, 7, 8 };
    std::vector< thc::Move > moves { moveScores.size() };

    const std::vector< unsigned int > indices = MoveSorter::insertionSort(moves, moveScores);
    const std::vector< unsigned int > indices_correct { 5, 4, 3, 2, 1, 0 };

    EXPECT_EQ( indices, indices_correct );
}

TEST(SortTest, InsertionSortTestAdvanced)
{
    std::vector< int > moveScores { -2, 10, 5, -1, 15, 3 };
    std::vector< thc::Move > moves { moveScores.size() };

    const std::vector< unsigned int > indices = MoveSorter::insertionSort(moves, moveScores);
    const std::vector< unsigned int > indices_correct { 4, 1, 2, 5, 3, 0 };

    EXPECT_EQ( indices, indices_correct );
}

TEST(SortTest, QuickSortTest)
{
    std::vector< int > moveScores { 3, 4, 5, 6, 7, 8 };
    std::vector<unsigned int> indices ( moveScores.size() );
            std::iota(indices.begin(), indices.end(), 0);

    MoveSorter::quickSort(indices, moveScores,0, moveScores.size() - 1);
    const std::vector< unsigned int > indices_correct { 5, 4, 3, 2, 1, 0 };

    EXPECT_EQ( indices, indices_correct );
}

TEST(SortTest, QuickSortTestAdvanced)
{
    std::vector< int > moveScores { -2, 10, 5, -1, 15, 3 };

   std::vector<unsigned int> indices ( moveScores.size() );
            std::iota(indices.begin(), indices.end(), 0);

    MoveSorter::quickSort(indices, moveScores,0, moveScores.size() - 1);
    const std::vector< unsigned int > indices_correct { 4, 1, 2, 5, 3, 0 };

    EXPECT_EQ( indices, indices_correct );
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}