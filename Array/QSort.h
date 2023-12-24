#include <cmath>
#include <iterator>
#include <algorithm>

#define SORT_THRESHOLD 16

template<typename Iter, typename Compare>
void introsort(Iter begin, Iter end, Compare comp, int maxdepth);

template<typename Iter, typename Compare>
Iter partition(Iter begin, Iter end, Compare comp);

template<typename Iter, typename Compare>
void heapify(Iter begin, Iter end, Compare comp);

template<typename Iter, typename Compare>
void heapsort(Iter begin, Iter end, Compare comp);

template<typename Iter, typename Compare>
void insertionsort(Iter begin, Iter end, Compare comp);


template<typename Iter, typename Compare>
void ssort(Iter begin, Iter end, Compare comp) {
    int maxdepth = std::floor(std::log(std::distance(begin, end)) / std::log(2)) * 2;
    introsort(begin, end, comp, maxdepth);
}

template<typename Iter, typename Compare>
void introsort(Iter begin, Iter end, Compare comp, int maxdepth) {
    if (std::distance(begin, end) < SORT_THRESHOLD) {
        insertionsort(begin, end, comp);
    }
    else if (maxdepth == 0) {
        heapsort(begin, end, comp);
    }
    else {
        Iter pivot = ::partition(begin, end, comp);
        introsort(begin, pivot, comp, maxdepth - 1);
        introsort(pivot, end, comp, maxdepth - 1);
    }
}

template<typename Iter, typename Compare>
Iter partition(Iter begin, Iter end, Compare comp) {
    const auto& pivot = end[-1];
    Iter i = begin;
    for (Iter j = begin; j < end - 1; ++j) {
        if (!comp(pivot, *j)) {
            if (j != i) std::iter_swap(i, j);
            i++;
        }
    }
    std::iter_swap(i, end - 1);
    return i + 1;
}

template<typename Iter, typename Compare>
void heapify(Iter begin, Iter end, Compare comp) {
    auto n = std::distance(begin, end);

    for (typename Iter::difference_type i = 1; i < n; ++i) {
        if (comp(begin[(i - 1) / 2], begin[i])) {
            auto j = i;

            while (comp(begin[(j - 1) / 2], begin[j])) {
                std::iter_swap(begin + j, begin + (j - 1) / 2);
                j = (j - 1) / 2;
            }
        }
    }
}

template<typename Iter, typename Compare>
void heapsort(Iter begin, Iter end, Compare comp) {
    heapify(begin, end, comp);

    for (auto i = std::distance(begin, end - 1); i > 0; --i) {
        std::iter_swap(begin, begin + i);
        typename Iter::difference_type j = 0, index;

        do {
            index = (2 * j + 1);

            if (index < (i - 1) && comp(begin[index], begin[index + 1]))
                index++;

            if (index < i && comp(begin[j], begin[index]))
                std::iter_swap(begin + j, begin + index);

            j = index;
        } while (index < i);
    }
}

template<typename Iter, typename Compare>
void insertionsort(Iter begin, Iter end, Compare comp) {
    for (auto i = begin; i != end; ++i) {
        for (auto j = i; j != begin && comp(*j, *(j - 1)); --j) {
            std::iter_swap(j, j - 1);
        }
    }
}
