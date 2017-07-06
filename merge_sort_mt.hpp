#ifndef _MERGE_SORT_MT_HPP_
#define _MERGE_SORT_MT_HPP_

#include <cstdint>
#include <vector>
#include <algorithm>
#include <boost/thread.hpp>

static void merge_sort_mt_(std::vector<size_t>::iterator lo, std::vector<size_t>::iterator hi, const size_t depth);

/* Baseado no Sedgewick */
static void merge_(std::vector<size_t>::iterator lo, std::vector<size_t>::iterator mid, std::vector<size_t>::iterator hi)
{
	if (*mid <= *(mid + 1)) {
		return;
	}

	ssize_t aux_size = hi - lo + 1;
	size_t *aux = new size_t[aux_size];

	for (ssize_t k = 0; k < aux_size; ++k) {
		aux[k] = *(lo + k);
	}

	auto i = lo;
	auto j = mid + 1;
	for (ssize_t k = 0; k <= hi - lo; ++k) {
		if (i - lo > mid - lo) {
			*(lo + k) = aux[j++ - lo];
		} else if (j - lo > hi - lo) {
			*(lo + k) = aux[i++ - lo];
		} else if (aux[j - lo] < aux[i - lo]) {
			*(lo + k) = aux[j++ - lo];
		} else {
			*(lo + k) = aux[i++ - lo];
		}
	}

	delete[] aux;
}

static void merge_sort_thread_(std::vector<size_t>::iterator lo, std::vector<size_t>::iterator hi, const size_t depth)
{
	merge_sort_mt_(lo, hi, depth);
}

void merge_sort_mt_(std::vector<size_t>::iterator lo, std::vector<size_t>::iterator hi, const size_t depth)
{
	if (lo == hi) {
		return;
	}

	auto mid = lo + (hi - lo) / 2;

	if (depth <= 0) {
		merge_sort_mt_(lo, mid, 0);
		merge_sort_mt_(mid + 1, hi, 0);
	} else {
		boost::thread t(merge_sort_thread_, lo, hi, depth / 2);
		merge_sort_mt_(mid + 1, hi, depth / 2);

		t.join();
	}

	merge_(lo, mid, hi);
}

void merge_sort_mt(std::vector<size_t>::iterator begin, std::vector<size_t>::iterator end, const size_t n_threads)
{
	size_t max_threads = 1024;
	size_t n_threads_ = n_threads;

	if (n_threads > max_threads) {
		n_threads_ = max_threads;
	}

	size_t depth = (n_threads_ + 1) / 2;

	merge_sort_mt_(begin, end - 1, depth);
}

#endif
