#include <iostream>
#include <vector>
#include <future>
#include <algorithm>
#include <cassert>

using namespace std;

bool make_thread = false;

void merge(vector<int>& array, size_t begin, size_t middle, size_t end)
{
	size_t nl = middle - begin + 1;
	size_t nr = end - middle;

	vector<int> left(nl);
	vector<int> right(nr);

	for (int i = 0; i < nl; i++)
		left[i] = array[begin + i];
	for (int j = 0; j < nr; j++)
		right[j] = array[middle + 1 + j];

	size_t i = 0, j = 0;
	size_t k = begin;

	while (i < nl && j < nr) {
		if (left[i] <= right[j]) {
			array[k] = left[i];
			i++;
		}
		else {
			array[k] = right[j];
			j++;
		}
		k++;
	}
	while (i < nl) {
		array[k] = left[i];
		i++;
		k++;
	}
	while (j < nr) {
		array[k] = right[j];
		j++;
		k++;
	}
}



void mergeSort(vector<int>& array, size_t begin, size_t end)
{
	if (begin >= end)
		return;

	size_t middle = begin + (end - begin) / 2;

	if (make_thread && (middle - begin) > 10000) {
		auto futureLeft = async(launch::async, [&]() {
			mergeSort(array, begin, middle);
			});
		mergeSort(array, middle + 1, end);
	}
	else {
		mergeSort(array, begin, middle);
		mergeSort(array, middle + 1, end);
	}
	merge(array, begin, middle, end);
}

void print(vector<int>& vec)
{
	for (auto& v : vec)
		cout << v << " ";
}

int main(int argc, const char* argv[])
{
	vector<int> test(2000000);
	for (auto& v : test)
		v = rand() % 10000;

	vector<int> printv = test;

	cout << "\nSINGLE THREAD\n";
	auto start = chrono::high_resolution_clock::now();
	mergeSort(printv, 0, printv.size() - 1);
	auto stop = chrono::high_resolution_clock::now();
	cout << "Elapsed time: " <<
		chrono::duration_cast<chrono::seconds> (stop - start).count() <<
		"sec\n";

	cout << "\nMULTIPLE THREAD\n";
	make_thread = true;
	start = chrono::high_resolution_clock::now();
	mergeSort(test, 0, test.size() - 1);
	stop = chrono::high_resolution_clock::now();
	cout << "Elapsed time: " <<
		chrono::duration_cast<chrono::seconds> (stop - start).count() <<
		"sec\n";

	assert(printv == test);
	cout << "\nFINE" << endl;

	return 0;
}