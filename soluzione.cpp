#include <bits/stdc++.h>

using namespace std;

#ifdef EVAL
    ifstream in("input.txt");
    ofstream out("output.txt");
#else
    istream &in(cin);
    ostream &out(cout);
#endif

long long bfs(const vector<pair<int, int>> adj[], const int n, const int x, const int y, const vector<bool> &matel, vector<long long> &min_dist, vector<long long> &min_dist_safe, stack<int> &min_path) {
    vector<bool> visited(n, false), on_matel(n, false);
    queue<int> q;
    vector<long long> dist(n, LONG_LONG_MAX);
    vector<long long> arcs(n, LONG_LONG_MAX);
    vector<int> prec(n, -1);

    q.push(x);
    dist[x] = 0;
    arcs[x] = 0;

    while (!q.empty()) {
        auto act = q.front();
        q.pop();
        if (!visited[act]) {
            visited[act] = true;
            if (!on_matel[act]) {
                for (int i = 0; i < adj[act].size(); i++) {
                    if (dist[adj[act][i].first] > dist[act] + adj[act][i].second) {
                        dist[adj[act][i].first] = dist[act] + adj[act][i].second;
                        arcs[adj[act][i].first] = arcs[act] + 1;
                        prec[adj[act][i].first] = act;
                        q.push(adj[act][i].first);
                        on_matel[adj[act][i].first] = matel[adj[act][i].first];
                    }
                    if (adj[act][i].first == y) {
                        min_dist_safe[arcs[act] + 1] = min(min_dist_safe[arcs[act] + 1], dist[act] + adj[act][i].second);
                    }
                }
            } else {
                for (int i = 0; i < adj[act].size(); i++) {
                    if (dist[adj[act][i].first] > dist[act] + adj[act][i].second) {
                        dist[adj[act][i].first] = dist[act] + adj[act][i].second;
                        arcs[adj[act][i].first] = arcs[act] + 1;
                        prec[adj[act][i].first] = act;
                        q.push(adj[act][i].first);
                        on_matel[adj[act][i].first] = true;
                    } else if (!on_matel[adj[act][i].first] && dist[adj[act][i].first] == dist[act] + adj[act][i].second) {
                        prec[adj[act][i].first] = act;
                        on_matel[adj[act][i].first] = true;
                    }
                    if (adj[act][i].first == y) {
                        min_dist[arcs[act] + 1] = min(min_dist[arcs[act] + 1], dist[act] + adj[act][i].second);
                    }
                }
            }
        }
    }
    if (visited[y]) {
        int act = y;
        while (act != -1) {
            min_path.push(act);
            act = prec[act];
        }
        if (!on_matel[y]) {
            return -1;
        } else {
            return -2;
        }
    } else {
        cout << "y non raggiungibile da x" << endl;
        return -2;
    }
}

long long ComparaIntervalli(const vector<pair<int, long long>> &min_dist, const vector<pair<int, long long>> &min_dist_safe) {

    if (min_dist_safe.size() == 0) {
        return -2;
    } else if (min_dist.size() == 0) {
        return -1;
    }

    int a = min_dist_safe[0].first;
    int b = min_dist[0].first;

    if (a < b || (a == b && min_dist_safe[1].second < min_dist[1].second)) {
        return -1;
    }

    long long max_fee = -2;

    for (int i = 0; i < min_dist_safe.size(); i++) {

        long long max_fee_act = LONG_LONG_MAX;

        int j;
        for (j = 0; j < min_dist.size() && min_dist[j].first < min_dist_safe[i].first; j++) {
            if ((min_dist[j].second - min_dist_safe[i].second) % (min_dist_safe[i].first - min_dist[j].first) == 0) {
                max_fee_act = min((min_dist[j].second - min_dist_safe[i].second) /
                                  (min_dist_safe[i].first - min_dist[j].first) - 1, max_fee_act);
            } else {
                max_fee_act = min((min_dist[j].second - min_dist_safe[i].second) / (min_dist_safe[i].first - min_dist[j].first), max_fee_act);
            }


        }

        if (max_fee_act <= 0) {
            max_fee_act = -2;
            continue;
        }

        if (j < min_dist.size() && min_dist[j].first == min_dist_safe[i].first && min_dist[j].second <= min_dist_safe[i].second) {
            max_fee_act = -2;
            continue;
        }

        for (; j < min_dist.size(); j++) {
            if (min_dist[j].second <= min_dist_safe[i].second) {
                long long prova = ((min_dist_safe[i].second - min_dist[j].second) / (min_dist[j].first - min_dist_safe[i].first)) + 1;
                if (prova > max_fee_act) {
                    max_fee_act = -2;
                    break;
                }
            }
        }

        max_fee = max(max_fee, max_fee_act);

    }

    return (max_fee>0)?max_fee:-2;
}

int main() {
    int n, p;
    in >> n >> p;
    int x=0, y=n-1;

    vector<pair<int, int>> adj[n];

    int a, b, c;
    for (int i = 0; i < p; i++) {
        in >> a >> b >> c;
        adj[a].push_back({b, c});
        adj[b].push_back({a, c});
    }

    int m;
    in >> m;
    vector<bool> matel(n, false);
    for (int i = 0; i < m; i++) {
        in >> a;
        matel[a] = true;
    }

    vector<long long> min_dist(n, LONG_LONG_MAX), min_dist_safe(n, LONG_LONG_MAX);
    stack<int> min_path;

    bfs(adj, n, x, y, matel, min_dist, min_dist_safe, min_path);

    vector<pair<int, long long>> md, mds;
    for (int i = 0; i < n; i++) {
        if (min_dist[i] != LONG_LONG_MAX) md.push_back({i, min_dist[i]});
        if (min_dist_safe[i] != LONG_LONG_MAX) mds.push_back({i, min_dist_safe[i]});
    }

    long long max_fee = ComparaIntervalli(md, mds);

    out << max_fee << endl;
    out << min_path.size() << endl;

    while (!min_path.empty()) {
        out << min_path.top() << " ";
        min_path.pop();
    }
    out << endl;

    return 0;
}
