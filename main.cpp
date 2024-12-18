#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>

struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
    stats& operator += (const stats& other) {
        comparison_count += other.comparison_count;
        copy_count += other.copy_count;
        return *this;
    }
};

// Пузырьковая сортировка
stats bubble_sort(std::vector<int>& arr) {
    stats s;
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            s.comparison_count++;
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                s.copy_count += 3; // 3 копирования при swap
            }
        }
    }
    return s;
}
//сортировка расческой
stats comb_sort(std::vector<int>& arr) {
    stats s;
    size_t n = arr.size();
    size_t gap = n;
    bool swapped = true;

    while (gap != 1 || swapped) {
        gap = gap * 10 / 13; 
        if (gap < 1) {
            gap = 1; // гарантируем, что gap не будет меньше 1
        }

        swapped = false;

        for (size_t i = 0; i + gap < n; ++i) {
            s.comparison_count++;
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                s.copy_count += 3; // 3 копирования при swap
                swapped = true;
            }
        }
    }
    return s;
}

// Пирамидальная сортировка
void heapify(std::vector<int>& arr, size_t n, size_t i, stats& s) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n) {
        s.comparison_count++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }

    if (right < n) {
        s.comparison_count++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        s.copy_count += 3; // 3 копирования при swap
        heapify(arr, n, largest, s);
    }
}

stats heap_sort(std::vector<int>& arr) {
    stats s;
    size_t n = arr.size();

    for (size_t i = n / 2; i > 0; --i) {
        heapify(arr, n, i - 1, s);
    }

    for (size_t i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        s.copy_count += 3; // 3 копирования при swap
        heapify(arr, i, 0, s);
    }
    return s;
}

// Генерация случайного массива
std::vector<int> generate_random_array(int size, unsigned seed) {
    std::vector<int> arr(size);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

//Уже отсортированный массив
std::vector<int> generate_sorted_array(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
    return arr;
}
//Отсортированный в обратном порядке
std::vector<int> generate_reverse_sorted_array(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
    return arr;
}
// Подсчет статистики для разных случаев
/*
void measure_statistics() {
    stats avg_bubble = { 0, 0 }, avg_comb = { 0, 0 }, avg_heap = { 0, 0 };
    std::srand(static_cast<unsigned int>(std::time(0))); // установка зерна

    for (auto size : sizes) {
        size_t total_comparisons_bubble = 0;
        size_t total_copies_bubble = 0;
        size_t total_comparisons_comb = 0;
        size_t total_copies_comb = 0;
        size_t total_comparisons_heap = 0;
        size_t total_copies_heap = 0;

        for (size_t i = 0; i < num_trials; ++i) {
            auto arr = generate_random_array(size);
            stats s = bubble_sort(arr);
            total_comparisons_bubble += s.comparison_count;
            total_copies_bubble += s.copy_count;

            //arr = generate_random_array(size);
            s = comb_sort(arr);
            total_comparisons_comb += s.comparison_count;
            total_copies_comb += s.copy_count;

            //arr = generate_random_array(size);
            s = heap_sort(arr);
            total_comparisons_heap += s.comparison_count;
            total_copies_heap += s.copy_count;
        }

        std::cout << "For size " << size << ":\n";
        std::cout << "bubble sort: "
            << total_comparisons_bubble / num_trials << " comparisions, "
            << total_copies_bubble / num_trials << " copies\n";
        std::cout << "comb sort: "
            << total_comparisons_comb / num_trials << " comparisions, "
            << total_copies_comb / num_trials << " copies\n";
        std::cout << "heap sort: "
            << total_comparisons_heap / num_trials << " comparisions, "
            << total_copies_heap / num_trials << " copies\n";
    }
}

int main() {
    const std::vector<size_t> sizes = { 1000, 2000, 3000, 10000, 25000, 50000, 100000 };

    measure_statistics();
    return 0;
}*/
    void analyze_for_size(int size, std::ofstream & outFile) {
        // Average case (100 random arrays)
        stats avg_bubble = { 0, 0 }, avg_comb = { 0, 0 }, avg_heap = { 0, 0 };

        for (int i = 0; i < 100; i++) {
            auto arr1 = generate_random_array(size, i);
            auto arr2 = arr1;
            auto arr3 = arr1;

            avg_bubble += bubble_sort(arr1);
            avg_comb += comb_sort(arr2);
            avg_heap += heap_sort(arr3);
        }

        avg_bubble.comparison_count /= 100;
        avg_bubble.copy_count /= 100;
        avg_comb.comparison_count /= 100;
        avg_comb.copy_count /= 100;
        avg_heap.comparison_count /= 100;
        avg_heap.copy_count /= 100;

        // Best case (sorted array)
        auto sorted_arr1 = generate_sorted_array(size);
        auto sorted_arr2 = sorted_arr1;
        auto sorted_arr3 = sorted_arr1;

        auto best_bubble = bubble_sort(sorted_arr1);
        auto best_comb = comb_sort(sorted_arr2);
        auto best_heap = heap_sort(sorted_arr3);

        // Worst case (reverse sorted array)
        auto reverse_arr1 = generate_reverse_sorted_array(size);
        auto reverse_arr2 = reverse_arr1;
        auto reverse_arr3 = reverse_arr1;

        auto worst_bubble = bubble_sort(reverse_arr1);
        auto worst_comb = comb_sort(reverse_arr2);
        auto worst_heap = heap_sort(reverse_arr3);

        // Write results to file
        outFile << size << ","
            << avg_bubble.comparison_count << "," << avg_bubble.copy_count << ","
            << best_bubble.comparison_count << "," << best_bubble.copy_count << ","
            << worst_bubble.comparison_count << "," << worst_bubble.copy_count << ","
            << avg_comb.comparison_count << "," << avg_comb.copy_count << ","
            << best_comb.comparison_count << "," << best_comb.copy_count << ","
            << worst_comb.comparison_count << "," << worst_comb.copy_count << ","
            << avg_heap.comparison_count << "," << avg_heap.copy_count << ","
            << best_heap.comparison_count << "," << best_heap.copy_count << ","
            << worst_heap.comparison_count << "," << worst_heap.copy_count << "\n";
    }

    int main() {
        std::ofstream outFile("sorting_analysis.csv");

        // Write CSV header
        outFile << "Size,"
            << "Bubble_Avg_Comp,Bubble_Avg_Copy,"
            << "Bubble_Best_Comp,Bubble_Best_Copy,"
            << "Bubble_Worst_Comp,Bubble_Worst_Copy,"
            << "Comb_Avg_Comp,Comb_Avg_Copy,"
            << "Comb_Best_Comp,Comb_Best_Copy,"
            << "Comb_Worst_Comp,Comb_Worst_Copy,"
            << "Heap_Avg_Comp,Heap_Avg_Copy,"
            << "Heap_Best_Comp,Heap_Best_Copy,"
            << "Heap_Worst_Comp,Heap_Worst_Copy\n";

        // Analyze for different sizes
        std::vector<int> sizes = { 1000, 2000, 3000, 5000, 10000}; //, 6000, 7000, 8000, 9000, 10000, 25000, 50000, 100000

        for (int size : sizes) {
            std::cout << "Analyzing for size: " << size << std::endl;
            analyze_for_size(size, outFile);
        }

        outFile.close();
        std::cout << "Analysis complete. Results written to sorting_analysis.csv" << std::endl;

        return 0;
    }
