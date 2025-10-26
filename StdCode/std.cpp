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
    // �⺻ ������
    vector<int> base = { 4, 2, 5, 3, 5, 8, 3, 1, 7, 4 };
    printVec(base, "[����]");

    // 1) sort (��������)
    {
        vector<int> v = base;
        sort(v.begin(), v.end());
        printVec(v, "[sort ��������]");
    }

    // 2) sort + ����� �� (��������)
    {
        vector<int> v = base;
        sort(v.begin(), v.end(), desc);
        printVec(v, "[sort ��������]");
    }

    // 3) stable_sort (���� ���� ����: ���� key�� ��� ���� ����)
    //    key: first,  ��: second
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
        // ���: (1,x) (2,y) (3,a) (3,b) (3,c)  // 3�� ��� ����(a,b,c) ����
    }

    // 4) partial_sort : ���� k���� ���� (�������� ���� ���� X)
    {
        vector<int> v = base;
        size_t k = 3;
        if (k > v.size()) k = v.size();
        partial_sort(v.begin(), v.begin() + k, v.end());
        printVec(v, "[partial_sort ����3 �ּҰ� �տ� ����]");
    }

    // 5) nth_element : n��° ��ġ�� �� ���Ҹ� ���ڸ�, ��/��� '�׺��� ����/ŭ'�� ����
    {
        vector<int> v = base;
        size_t n = 3; // 0-based: 4��° �ڸ�
        if (n < v.size()) {
            nth_element(v.begin(), v.begin() + n, v.end());
            cout << "[nth_element n=3] ";
            printVec(v, "�迭");
            cout << " -> v[3] = " << v[3] << " (�� ��ġ�� ���Ұ� ���ڸ�)\n";
        }
    }

    // ���� Ž�� �迭�� "���ĵ�" �����Ϳ��� ���
    vector<int> sorted = base;
    sort(sorted.begin(), sorted.end()); // 1 2 3 3 4 4 5 5 7 8
    printVec(sorted, "[Ž���� ����]");

    // 6) lower_bound / upper_bound
    {
        int key = 4;
        vector<int>::iterator lb = lower_bound(sorted.begin(), sorted.end(), key);
        vector<int>::iterator ub = upper_bound(sorted.begin(), sorted.end(), key);
        cout << "[lower/upper_bound] key=" << key
            << " -> lb idx=" << (lb - sorted.begin())
            << ", ub idx=" << (ub - sorted.begin()) << '\n';
    }

    // 7) equal_range : (lower, upper) �� ����
    {
        int key = 4;
        pair<vector<int>::iterator, vector<int>::iterator> er =
            equal_range(sorted.begin(), sorted.end(), key);
        cout << "[equal_range] key=" << key
            << " -> (" << (er.first - sorted.begin())
            << ", " << (er.second - sorted.begin()) << ")\n";
    }

    // 8) binary_search : ���� ���θ� bool��
    {
        int key1 = 5, key2 = 10;
        bool f1 = binary_search(sorted.begin(), sorted.end(), key1);
        bool f2 = binary_search(sorted.begin(), sorted.end(), key2);
        cout << "[binary_search] " << key1 << " -> " << (f1 ? "Found" : "Not Found") << '\n';
        cout << "[binary_search] " << key2 << " -> " << (f2 ? "Found" : "Not Found") << '\n';
    }

    return 0;
}
