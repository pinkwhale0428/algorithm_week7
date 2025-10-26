#include <iostream>
#include <vector>
#include <algorithm> // sort, stable_sort, partial_sort, nth_element, lower/upper/equal_range, binary_search
#include <utility>   // pair

using namespace std;

template<typename T>
void printVec(const vector<T>& v, const string& tag) {
    cout << tag << " : ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << (i + 1 == v.size() ? '\n' : ' ');
    }
}

bool desc(int a, int b) { return a > b; }

int main() {
    // 기본 데이터
    vector<int> base = { 4, 2, 5, 3, 5, 8, 3, 1, 7, 4 };
    printVec(base, "[원본]");

    // 1) sort (오름차순)
    {
        vector<int> v = base;
        sort(v.begin(), v.end());
        printVec(v, "[sort 오름차순]");
    }

    // 2) sort + 사용자 비교 (내림차순)
    {
        vector<int> v = base;
        sort(v.begin(), v.end(), desc);
        printVec(v, "[sort 내림차순]");
    }

    // 3) stable_sort (안정 정렬 예시: 같은 key의 상대 순서 유지)
    //    key: first,  값: second
    {
        vector< pair<int, char> > a;
        a.push_back(make_pair(3, 'a'));
        a.push_back(make_pair(1, 'x'));
        a.push_back(make_pair(3, 'b'));
        a.push_back(make_pair(2, 'y'));
        a.push_back(make_pair(3, 'c'));

        stable_sort(a.begin(), a.end(), [](const pair<int, char>& L, const pair<int, char>& R) {
            return L.first < R.first;
            });

        cout << "[stable_sort] by first -> ";
        for (size_t i = 0; i < a.size(); ++i) {
            cout << "(" << a[i].first << "," << a[i].second << ")"
                << (i + 1 == a.size() ? '\n' : ' ');
        }
        // 기대: (1,x) (2,y) (3,a) (3,b) (3,c)  // 3의 상대 순서(a,b,c) 유지
    }

    // 4) partial_sort : 앞의 k개만 정렬 (나머지는 순서 보장 X)
    {
        vector<int> v = base;
        size_t k = 3;
        if (k > v.size()) k = v.size();
        partial_sort(v.begin(), v.begin() + k, v.end());
        printVec(v, "[partial_sort 상위3 최소값 앞에 정렬]");
    }

    // 5) nth_element : n번째 위치에 올 원소만 제자리, 좌/우는 '그보다 작음/큼'만 보장
    {
        vector<int> v = base;
        size_t n = 3; // 0-based: 4번째 자리
        if (n < v.size()) {
            nth_element(v.begin(), v.begin() + n, v.end());
            cout << "[nth_element n=3] ";
            printVec(v, "배열");
            cout << " -> v[3] = " << v[3] << " (이 위치의 원소가 제자리)\n";
        }
    }

    // 이진 탐색 계열은 "정렬된" 데이터에서 사용
    vector<int> sorted = base;
    sort(sorted.begin(), sorted.end()); // 1 2 3 3 4 4 5 5 7 8
    printVec(sorted, "[탐색용 정렬]");

    // 6) lower_bound / upper_bound
    {
        int key = 4;
        vector<int>::iterator lb = lower_bound(sorted.begin(), sorted.end(), key);
        vector<int>::iterator ub = upper_bound(sorted.begin(), sorted.end(), key);
        cout << "[lower/upper_bound] key=" << key
            << " -> lb idx=" << (lb - sorted.begin())
            << ", ub idx=" << (ub - sorted.begin()) << '\n';
    }

    // 7) equal_range : (lower, upper) 한 번에
    {
        int key = 4;
        pair<vector<int>::iterator, vector<int>::iterator> er =
            equal_range(sorted.begin(), sorted.end(), key);
        cout << "[equal_range] key=" << key
            << " -> (" << (er.first - sorted.begin())
            << ", " << (er.second - sorted.begin()) << ")\n";
    }

    // 8) binary_search : 존재 여부만 bool로
    {
        int key1 = 5, key2 = 10;
        bool f1 = binary_search(sorted.begin(), sorted.end(), key1);
        bool f2 = binary_search(sorted.begin(), sorted.end(), key2);
        cout << "[binary_search] " << key1 << " -> " << (f1 ? "Found" : "Not Found") << '\n';
        cout << "[binary_search] " << key2 << " -> " << (f2 ? "Found" : "Not Found") << '\n';
    }

    return 0;
}
