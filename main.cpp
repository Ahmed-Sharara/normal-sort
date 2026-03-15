/*
first one and last first
*/

#include <iostream>
#include <vector>
#include <algorithm> 

using namespace std;

// Helper to merge two adjacent sorted sequences in O(1) placement time
void merge_runs(vector<int>& arr, int start, int mid, int end, vector<int>& buffer) {
    int i = start;
    int j = mid;
    int k = start;

    // Place the smallest elements into the buffer in O(1) time
    while (i < mid && j < end) {
        if (arr[i] <= arr[j]) {
            buffer[k++] = arr[i++];
        } else {
            buffer[k++] = arr[j++];
        }
    }

    // Append leftovers
    while (i < mid) buffer[k++] = arr[i++];
    while (j < end) buffer[k++] = arr[j++];

    // Copy back to the main array
    for (int idx = start; idx < end; idx++) {
        arr[idx] = buffer[idx];
    }
}

// The highly optimized sorting algorithm
void optimized_custom_sort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // METHOD 1 & 2: O(N) Run Extraction & Reversal
    // We store the {start, end} index of every naturally sorted sequence.
    vector<pair<int, int>> runs;
    int i = 0;

    while (i < n) {
        int start = i;
        i++;

        if (i == n) {
            runs.push_back({start, i});
            break;
        }

        // Detect if the sequence is going DOWN (Your old Worst-Case)
        if (arr[i] < arr[i - 1]) {
            while (i < n && arr[i] < arr[i - 1]) {
                i++;
            }
            // METHOD 2: Reverse the descending run to make it ascending in O(N) time!
            reverse(arr.begin() + start, arr.begin() + i);
        } 
        // Detect if the sequence is going UP (Your old Best-Case)
        else {
            while (i < n && arr[i] >= arr[i - 1]) {
                i++;
            }
        }
        // Save the boundaries of this cleanly sorted mini-array
        runs.push_back({start, i});
    }

    // METHOD 3: Bottom-Up Buffer Merging
    // We allocate EXACTLY ONE buffer for memory efficiency
    vector<int> buffer(n);

    // Merge the mini-arrays together in pairs until only 1 fully sorted array remains
    while (runs.size() > 1) {
        vector<pair<int, int>> next_runs;

        for (size_t k = 0; k < runs.size(); k += 2) {
            if (k + 1 < runs.size()) {
                // We have a pair of runs, merge them
                int start = runs[k].first;
                int mid   = runs[k + 1].first;
                int end   = runs[k + 1].second;

                merge_runs(arr, start, mid, end, buffer);
                next_runs.push_back({start, end});
            } else {
                // Odd run out, just carry it over to the next merging phase
                next_runs.push_back(runs[k]);
            }
        }
        runs = next_runs; // Update our list of runs
    }
}

// --- Example Usage ---
int main() {
    // A mix of Ascending, Descending, and Random data
    vector<int> arr = {1, 2, 3, 9, 8, 7, 4, 6, 5};
    
    optimized_custom_sort(arr);

    cout << "Sorted: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << "\n";

    return 0;
}
