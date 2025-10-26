#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

struct Station { int id; string name; string lines; };
struct Edge { int to; int w; };

const int INF = numeric_limits<int>::max() / 4;

static vector<Station> stations;           // 0..n-1
static vector<vector<Edge> > adj;          


static int findIndexById(int id) {
    for (int i = 0; i < (int)stations.size(); ++i)
        if (stations[i].id == id) return i;
    return -1;
}
static int findIndexByName(const string& name) {
    for (int i = 0; i < (int)stations.size(); ++i)
        if (stations[i].name == name) return i;
    return -1;
}

static bool loadStations(const string& path) {
    ifstream fin(path.c_str());
    if (!fin) { cerr << "[stations] open fail: " << path << "\n"; return false; }

    stations.clear();
    adj.clear();

    string line;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        Station s;
        if (!(ss >> s.id >> s.name >> s.lines)) continue; 
        stations.push_back(s);
    }
    if (stations.empty()) { cerr << "[stations] empty\n"; return false; }
    adj.assign(stations.size(), vector<Edge>());
    return true;
}

static bool loadEdges(const string& path) {
    ifstream fin(path.c_str());
    if (!fin) { cerr << "[edges] open fail: " << path << "\n"; return false; }

    string line;
    int ok = 0;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        int uid, vid, w;
        if (!(ss >> uid >> vid >> w)) continue;

        int u = findIndexById(uid);
        int v = findIndexById(vid);
        if (u == -1 || v == -1) {
            cerr << "[edges] unknown station id: " << uid << " or " << vid << "\n";
            continue;
        }
        adj[u].push_back(Edge { v, w });
        adj[v].push_back(Edge { u, w });
        ++ok;
    }
    if (!ok) { cerr << "[edges] no edges\n"; return false; }
    return true;
}

//dijkstra 
static pair<vector<int>, vector<int> > dijkstra(int src) {
    int n = (int)stations.size();
    vector<int> dist(n, INF), parent(n, -1);

    struct Node { int d, i; bool operator<(const Node& o) const { return d > o.d; } };
    priority_queue<Node> pq;

    dist[src] = 0;
    pq.push(Node{ 0, src });

    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        if (cur.d != dist[cur.i]) continue;

        const vector<Edge>& es = adj[cur.i];
        for (int k = 0; k < (int)es.size(); ++k) {
            int v = es[k].to;
            int nd = cur.d + es[k].w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = cur.i;
                pq.push(Node{ nd, v });
            }
        }
    }
    return make_pair(dist, parent);
}

static vector<int> buildPath(int s, int t, const vector<int>& p) {
    vector<int> path;
    if (s == t) { path.push_back(s); return path; }
    if (p[t] == -1) return path;
    for (int v = t; v != -1; v = p[v]) {
        path.push_back(v);
        if (v == s) break;
    }
    for (int i = 0, j = (int)path.size() - 1; i < j; ++i, --j) {
        int tmp = path[i]; path[i] = path[j]; path[j] = tmp;
    }
    if (path.empty() || path[0] != s) path.clear();
    return path;
}

static void printPath(const vector<int>& path, int totalMin) {
    if (path.empty()) { cout << "경로 없음\n"; return; }
    for (int i = 0; i < (int)path.size(); ++i) {
        cout << stations[path[i]].name;
        if (i + 1 < (int)path.size()) cout << " -> ";
    }
    cout << "\n시간: " << totalMin << "분, 정거장: " << (int)path.size() - 1 << "개\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    if (!loadStations("stations.txt") || !loadEdges("edges.txt")) {
        cerr << "데이터 로드 실패\n";
        return 1;
    }

    string sName, tName;
    cout << "출발역: ";
    getline(cin, sName);
    if (sName.empty()) getline(cin, sName); // 개행 잔여
    cout << "도착역: ";
    getline(cin, tName);

    int s = findIndexByName(sName);
    int t = findIndexByName(tName);
    if (s == -1 || t == -1) {
        cerr << "알 수 없는 역 이름\n";
        return 1;
    }

    pair<vector<int>, vector<int> > r = dijkstra(s);
    vector<int>& dist = r.first;
    vector<int>& parent = r.second;

    if (dist[t] >= INF / 2) {
        cout << "연결되지 않음\n";
        return 0;
    }

    vector<int> path = buildPath(s, t, parent);
    printPath(path, dist[t]);
    return 0;
}
