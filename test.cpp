#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <random>
#include <fstream>
#include <tuple>

using namespace std;

void bubble_sort(vector<int> &a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) { 
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void selection_sort(vector<int>& a) {
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

void insertion_sort(vector<int> &a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void merge_f(vector<int> &a, int mid, int low_p, int high_p) {
    int size_l = mid - low_p + 1;
    int size_r = high_p - mid;

    vector<int> al(size_l), ar(size_r);

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

void merge_sort(vector<int> &a, int low_p, int high_p) {
    if (low_p >= high_p) return;
    int mid = low_p + (high_p - low_p) / 2;
    merge_sort(a, low_p, mid);
    merge_sort(a, mid + 1, high_p);
    merge_f(a, mid, low_p, high_p);
}

int quick_f(vector<int> &a, int low_p, int high_p) {
    int initial_pivot = a[high_p];
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

void quick_sort(vector<int> &a, int low_p, int high_p) {
    if (low_p < high_p) {
        int pivot = quick_f(a, low_p, high_p);
        quick_sort(a, low_p, pivot - 1);
        quick_sort(a, pivot + 1, high_p);
    }
}

void library_sort(vector<int>& a) {
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

void gnome_sort(vector<int>& a) {
    int pos = 0, n = a.size();
    while (pos < n) {
        if (pos == 0 || a[pos] >= a[pos - 1]) pos++;
        else {
            swap(a[pos], a[pos - 1]);
            pos--;
        }
    }
}

void insert_sorted(vector<int>& a, int val) {
    size_t i = 0; // Changed 'int' to 'size_t'
    while (i < a.size() && a[i] <= val) i++;
    a.insert(a.begin() + i, val);
}

bool yeet_f(const vector<int>& a) {
    for (size_t i = 1; i < a.size(); i++) {
        if (a[i] < a[i - 1]) return false;
    }
    return true;
}

void yeet_sort(vector<int>& a) {
    while (!yeet_f(a)) {
        for (size_t i = 1; i < a.size(); i++) {
            if (a[i] < a[i - 1]) {
                int val = a[i];
                a.erase(a.begin() + i);
                insert_sorted(a, val);
                break;
            }
        }
    }
}


void run_merge_sort(vector<int>& a) {
    merge_sort(a, 0, a.size() - 1);
}

void run_quick_sort(vector<int>& a) {
    quick_sort(a, 0, a.size() - 1);
}

void run_std_sort(vector<int>& a) {
    std::sort(a.begin(), a.end());
}


void result(const string& name, void (*sortFunc)(vector<int>&), vector<int> a, int size, vector<tuple<string, int, double>>& results) {
    auto start = chrono::high_resolution_clock::now();
    
    sortFunc(a);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    results.push_back(make_tuple(name, size, duration.count()));

    cout << "  [ " << left << setw(10) << name << " ]  =>  " 
         << fixed << setprecision(25) << duration.count() << " seconds\n";
}

int main() {
    vector<int> sizes = {10, 500, 50000, 1000000, 10000000};
    mt19937 number_machine(1337); 
    vector<tuple<string, int, double>> results;

    for (int i = 0; i < 5; ++ i) {
        int size = sizes[i];
        cout << "----------------------------------\n";
        cout << " Testing array: " << size << "\n";
        cout << "----------------------------------\n";

        vector<int> original(size);
        uniform_int_distribution<> dis(1, 100000);
        for (int i = 0; i < size; ++i) {
            original[i] = dis(number_machine);
        }

        bool skip_simple = (size >= 50000);

        if (!skip_simple) {
            result("Bubble", bubble_sort, original, size, results);
            result("Selection", selection_sort, original, size, results);
            result("Insertion", insertion_sort, original, size, results);
            result("Library", library_sort, original, size, results);
            result("Gnome", gnome_sort, original, size, results);

            if (size <= 1000) {
                result("Yeet", yeet_sort, original, size, results);
            } else {
                cout << "  [ " << left << setw(10) << "Yeet" << " ]  =>  Skipped (too slow)\n";
            }
        } else {
            cout << " Simple O(n^2) sorts are skipped \n";
        }

        result("Merge", run_merge_sort, original, size, results);
        result("Quick", run_quick_sort, original, size, results);
        result("std::sort", run_std_sort, original, size, results);

        cout << "\n";
    }

    ofstream csv("results.csv");
    csv << "Algorithm,Size,Time\n";
    for (const auto& r : results) {
        csv << get<0>(r) << "," << get<1>(r) << "," << get<2>(r) << "\n";
    }
    csv.close();

    return 0;
}