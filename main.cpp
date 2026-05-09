#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <random>
#include <fstream>
#include <tuple>
#include <string>
#include <list>
#include <future>
#include <thread>
#include <atomic>

using namespace std;

template <typename T>
void bubble_sort(vector<T> &a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) { 
                swap(a[j], a[j + 1]);
            }
        }
    }
}

template <typename T>
void selection_sort(vector<T>& a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(a[i], a[min_idx]);
        }
    }
}

template <typename T>
void insertion_sort(vector<T> &a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        T key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

template <typename T>
void merge_f(vector<T> &a, int mid, int low_p, int high_p) {
    int size_l = mid - low_p + 1;
    int size_r = high_p - mid;

    vector<T> al(size_l), ar(size_r);

    for (int i = 0; i < size_l; i++) al[i] = a[low_p + i];
    for (int i = 0; i < size_r; i++) ar[i] = a[mid + 1 + i];

    int i = 0, j = 0, size_n = low_p;

    while(i < size_l && j < size_r) {
        if (al[i] <= ar[j]) {
            a[size_n] = al[i];
            i++;
        } else {
            a[size_n] = ar[j];
            j++;
        }
        size_n++;
    }

    while(i < size_l) { a[size_n] = al[i]; i++; size_n++; }
    while(j < size_r) { a[size_n] = ar[j]; j++; size_n++; }
}

template <typename T>
void merge_sort(vector<T> &a, int low_p, int high_p) {
    if (low_p >= high_p) return;
    int mid = low_p + (high_p - low_p) / 2;
    merge_sort(a, low_p, mid);
    merge_sort(a, mid + 1, high_p);
    merge_f(a, mid, low_p, high_p);
}

template <typename T>
int quick_f(vector<T> &a, int low_p, int high_p) {
    T initial_pivot = a[high_p];
    int i = low_p - 1;

    for (int j = low_p; j <= high_p - 1; j++) {
        if (a[j] < initial_pivot) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[high_p]);
    return i + 1;
}

template <typename T>
void quick_sort(vector<T> &a, int low_p, int high_p) {
    if (low_p < high_p) {
        int pivot = quick_f(a, low_p, high_p);
        quick_sort(a, low_p, pivot - 1);
        quick_sort(a, pivot + 1, high_p);
    }
}

template <typename T>
void library_sort(vector<T>& a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int left = 0, right = i;
        while (left < right) {
            int mid = (left + right) >> 1;
            if (a[mid] <= a[i]) left = mid + 1;
            else right = mid;
        }
        for (int j = i; j > left; j--) {
            swap(a[j], a[j - 1]);
        }
    }
}

template <typename T>
void gnome_sort(vector<T>& a) {
    int pos = 0, n = a.size();
    while (pos < n) {
        if (pos == 0 || a[pos] >= a[pos - 1]) pos++;
        else {
            swap(a[pos], a[pos - 1]);
            pos--;
        }
    }
}

template <typename T>
void insert_sorted(vector<T>& a, T val) {
    size_t i = 0;
    while (i < a.size() && a[i] <= val) i++;
    a.insert(a.begin() + i, val);
}

template <typename T>
bool yeet_f(const vector<T>& a) {
    for (size_t i = 1; i < a.size(); i++) {
        if (a[i] < a[i - 1]) return false;
    }
    return true;
}

template <typename T>
void yeet_sort(vector<T>& a) {
    while (!yeet_f(a)) {
        for (size_t i = 1; i < a.size(); i++) {
            if (a[i] < a[i - 1]) {
                T val = a[i];
                a.erase(a.begin() + i);
                insert_sorted(a, val);
                break;
            }
        }
    }
}

template <typename T> void run_merge_sort(vector<T>& a) { merge_sort(a, 0, a.size() - 1); }
template <typename T> void run_quick_sort(vector<T>& a) { quick_sort(a, 0, a.size() - 1); }
template <typename T> void run_std_sort(vector<T>& a) { std::sort(a.begin(), a.end()); }

template <typename T> 
void run_parallel_sort(vector<T>& a) { 
    if (a.size() < 10000) {
        std::sort(a.begin(), a.end());
        return;
    }
    
    int mid = a.size() / 2;
    
    auto future_left = std::async(std::launch::async, [&a, mid]() {
        std::sort(a.begin(), a.begin() + mid);
    });
    
    std::sort(a.begin() + mid, a.end());
    
    future_left.get();
    
    std::inplace_merge(a.begin(), a.begin() + mid, a.end());
}

enum class dist_pattern { RANDOM, SORTED, REVERSE, ALMOST, FLAT };
string get_distname(dist_pattern dist) {
    switch(dist) {
        case dist_pattern::RANDOM: return "Random";
        case dist_pattern::SORTED: return "Sorted";
        case dist_pattern::REVERSE: return "Reversed";
        case dist_pattern::ALMOST: return "Almost_Sorted";
        case dist_pattern::FLAT: return "Flat";
        default: return "Unknown";
    }
}

vector<int> generate_data(int size, dist_pattern dist, mt19937& rng, int){
    vector<int> a(size);
    uniform_int_distribution<> dis(1, 100000);
    
    if (dist == dist_pattern::FLAT) {
        int flat_vals[] = {10, 20, 30, 40, 50};
        for (int i = 0; i < size; ++i) a[i] = flat_vals[dis(rng) % 5];
        return a;
    }

    for (int i = 0; i < size; ++i) a[i] = dis(rng);

    if (dist == dist_pattern::SORTED || dist == dist_pattern::ALMOST) std::sort(a.begin(), a.end());
    if (dist == dist_pattern::REVERSE) std::sort(a.rbegin(), a.rend());
    
    if (dist == dist_pattern::ALMOST) {
        int swaps = max(1, size / 50);
        uniform_int_distribution<> idx_dis(0, size - 1);
        for(int i = 0; i < swaps; i++) {
            swap(a[idx_dis(rng)], a[idx_dis(rng)]);
        }
    }
    return a;
}

vector<string> generate_data(int size, dist_pattern dist, mt19937& rng, string) {
    vector<string> a(size);
    uniform_int_distribution<> char_dis('a', 'z');
    for (int i = 0; i < size; ++i) {
        a[i] = string(5, (char)char_dis(rng));
    }
    if (dist == dist_pattern::SORTED) std::sort(a.begin(), a.end());
    return a;
}

vector<tuple<string, string, string, int, int, double>> all_results;

template<typename T>
void result(const string& name, void (*sort_func)(vector<T>&), const vector<vector<T>>& test_batches, const string& type_name, const string& pattern_name) {
    int batches = test_batches.size();
    int size = test_batches[0].size();

    std::atomic<bool> done(false);
    auto start = chrono::high_resolution_clock::now();

    vector<vector<T>> working_data = test_batches;

    std::thread worker_thread([working_data, sort_func, &done, batches]() mutable {
        for (int i = 0; i < batches; i++) {
            sort_func(working_data[i]); 
        }
        done = true;
    });

    double timeout_seconds = 3.0; 
    auto timeout_limit = start + std::chrono::milliseconds(static_cast<int>(timeout_seconds * 1000));

    while (!done) {
        if (chrono::high_resolution_clock::now() > timeout_limit) {
            worker_thread.detach(); 
            cout << "  [ " << left << setw(15) << name << " ]  =>  Timeout (> " << fixed << setprecision(1) << timeout_seconds << "s)\n";
            all_results.push_back(make_tuple(name, type_name, pattern_name, size, batches, -1.0)); 
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
    }

    worker_thread.join();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    all_results.push_back(make_tuple(name, type_name, pattern_name, size, batches, duration.count()));

    cout << "  [ " << left << setw(15) << name << " ]  =>  " 
         << fixed << setprecision(6) << duration.count() << " seconds\n";
}

template <typename T>
void run_experiment(string type_name, int size, dist_pattern dist, mt19937& rng) {
    cout << "---------------------------------------------------------\n";
    cout << " Type: " << type_name << " | Struct: " << get_distname(dist) << " | Size: " << size << "\n";
    
    int batches = 1;
    if (size <= 100) batches = 10000; 
    if (size > 100 && size <= 1000) batches = 100;
    cout << " Running " << batches << " batch(es) to gather relevant data...\n";
    cout << "---------------------------------------------------------\n";

    vector<vector<T>> data_batches(batches);
    for(int i=0; i<batches; ++i) {
        data_batches[i] = generate_data(size, dist, rng, T()); 
    }

    result("Bubble", bubble_sort<T>, data_batches, type_name, get_distname(dist));
    result("Selection", selection_sort<T>, data_batches, type_name, get_distname(dist));
    result("Insertion", insertion_sort<T>, data_batches, type_name, get_distname(dist));
    result("Library", library_sort<T>, data_batches, type_name, get_distname(dist));
    result("Gnome", gnome_sort<T>, data_batches, type_name, get_distname(dist));
    result("Yeet", yeet_sort<T>, data_batches, type_name, get_distname(dist));
    
    result("Merge", run_merge_sort<T>, data_batches, type_name, get_distname(dist));
    
    if (size >= 100000 && (dist == dist_pattern::SORTED || dist == dist_pattern::REVERSE || dist == dist_pattern::FLAT)) {
        cout << "  [ " << left << setw(15) << "Quick" << " ]  =>  Skipped (O(n^2) Stack Overflow risk)\n";
    } else {
        result("Quick", run_quick_sort<T>, data_batches, type_name, get_distname(dist));
    }
    
    result("std::sort", run_std_sort<T>, data_batches, type_name, get_distname(dist));
    
    if (size >= 10000) {
        result("std::sort(par)", run_parallel_sort<T>, data_batches, type_name, get_distname(dist)); 
    }
    cout << "\n";
}

int main() {
    mt19937 number_machine(1337); 

    vector<int> integer_sizes = {50, 5000, 1000000}; 
    vector<dist_pattern> patterns = {dist_pattern::RANDOM, dist_pattern::SORTED, dist_pattern::REVERSE, dist_pattern::ALMOST, dist_pattern::FLAT};

    cout << "=== PHASE 1: INTEGER ARRAYS (Length & Structure Testing) ===\n";
    for (int size : integer_sizes) {
        for (dist_pattern dist : patterns) {
            if (size == 1000000 && dist != dist_pattern::RANDOM && dist != dist_pattern::FLAT) continue; 
            
            run_experiment<int>("int", size, dist, number_machine);
        }
    }

    cout << "=== PHASE 2: STRING ARRAYS ===\n";
    run_experiment<string>("std::string", 5000, dist_pattern::RANDOM, number_machine);

    cout << "=== PHASE 3: LINKED LISTS VS ARRAYS ===\n";
    cout << "Testing std::list vs std::vector on 1,000,000 elements using native sorting...\n";
    int list_size = 1000000;

    vector<int> v_test = generate_data(list_size, dist_pattern::RANDOM, number_machine, 0); 
    list<int> l_test(v_test.begin(), v_test.end());

    auto s1 = chrono::high_resolution_clock::now();
    std::sort(v_test.begin(), v_test.end());
    chrono::duration<double> d1 = chrono::high_resolution_clock::now() - s1;
    cout << "  [ Vector std::sort  ] => " << fixed << setprecision(6) << d1.count() << " seconds\n";

    auto s2 = chrono::high_resolution_clock::now();
    l_test.sort(); 
    chrono::duration<double> d2 = chrono::high_resolution_clock::now() - s2;
    cout << "  [ List   l.sort()   ] => " << fixed << setprecision(6) << d2.count() << " seconds\n\n";

    ofstream csv("results.csv");
    csv << "Algorithm,DataType,Structure,Size,Batches,Time\n";
    for (const auto& r : all_results) {
        csv << get<0>(r) << "," << get<1>(r) << "," << get<2>(r) << "," 
            << get<3>(r) << "," << get<4>(r) << "," << get<5>(r) << "\n";
    }
    csv.close();

    cout << "All benchmarks completed successfully.\n";
    return 0;
}