/* Compile and run: g++ -std=c++11 -W -Wall -O3 -o main main.cpp -lboost_system -lboost_thread && ./main
 */

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <utility>
#include <chrono>

#include "merge_sort_mt.hpp"

std::vector<size_t> generate_unsorted_array(size_t n)
{
	std::vector<size_t> ret;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 10 * n);

	for (size_t i = 0; i < n; ++i) {
		ret.push_back(dis(gen));
	}

	return ret;
}

double run_time(size_t m, 
		void (*sort_function)(std::vector<size_t>::iterator, std::vector<size_t>::iterator, size_t n_threads), 
		std::vector<size_t>::iterator begin, 
		std::vector<size_t>::iterator end, 
		size_t n_threads)
{
	double mean_time = 0.0;

	std::vector<size_t> input(begin, end);

	for (size_t i = 0; i < m; ++i) {
		std::vector<size_t> input_copy(input);

		/*std::cout << "\n";
		for (auto it = input_copy.begin(); it != input_copy.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << "\n";*/

		std::chrono::time_point<std::chrono::steady_clock> t_start = std::chrono::steady_clock::now();
		sort_function(input_copy.begin(), input_copy.end(), n_threads);
		std::chrono::time_point<std::chrono::steady_clock> t_end = std::chrono::steady_clock::now();

		/*std::cout << "\n";
		for (auto it = input_copy.begin(); it != input_copy.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << "\n";*/

		std::chrono::duration<double> elapsed_seconds = t_end - t_start;
		mean_time += elapsed_seconds.count();

	}

	mean_time /= m;
	return mean_time;
}

int main()
{
	const size_t m = 10;
	const size_t n = 10000;
	const size_t n_threads[] = {0, 1, 3, 5, 7, 9};

	std::vector<std::pair<std::string, void (*)(std::vector<size_t>::iterator, std::vector<size_t>::iterator, size_t n_threads)>> sort_function;
	sort_function.push_back(std::make_pair<std::string, void (*)(std::vector<size_t>::iterator, std::vector<size_t>::iterator, size_t n_threads)>("merge_sort_mt", merge_sort_mt));

	for (size_t n_ = 100; n_ <= n; n_ = n_ * 10) {
		std::cout << "N: " << n_ << "\n\n";
		for (size_t n_threads_ : n_threads) {
			//std::cout << "THREADS: " << n_threads_ << "\n";
			std::vector<size_t> input = generate_unsorted_array(n_);
			for (size_t i = 0; i < sort_function.size(); ++i) {
				double mean_time = run_time(m, sort_function[i].second, input.begin(), input.end(), n_threads_);
				std::cout << mean_time << " ";
			}
		}
		std::cout << "\n\n";
	}

	return 0;
}
