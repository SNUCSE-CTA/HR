#include <bits/stdc++.h>

using namespace std;

const int INF = 0x7fffffff;

class Graph {
public:
	int num_vertex;
	int num_edge;

	vector<vector<int>> adj_out;
	vector<vector<int>> adj_in;

	void init(size_t _num_vertex) {
		num_vertex = _num_vertex;
		adj_out.resize(num_vertex);
		adj_in.resize(num_vertex);
	}
};

struct item3 {
	int x, d, y;
};

item3 forwardBFS(const Graph& G, int r, vector<int>& dist) {
	queue<int> q;
	vector<bool> visited(G.num_vertex, false);
	vector<int> prev(G.num_vertex, -1);

	dist[r] = 0;
	prev[r] = r;

	q.push(r);
	visited[r] = true;

	while (!q.empty()) {
		int u = q.front(); q.pop();

		for (const int& v: G.adj_out[u]) {
			if (visited[v]) continue;

			dist[v] = dist[u] + 1;
			prev[v] = u;

			q.push(v);
			visited[v] = true;
		}
	}

	int max_dist = 0;
	int max_dist_vertex = -1;
	for (int i = 0; i < G.num_vertex; ++i) {
		if (visited[i] && max_dist < dist[i]) {
			max_dist = dist[i];
			max_dist_vertex = i;
		}
	}

	if (max_dist == 0) {
		return { r, 0, r };
	}

	int d = max_dist;
	int y = max_dist_vertex;
	while (d > 0) {
		y = prev[y];
		d -= 2;
	}

	return { max_dist_vertex, max_dist, y };
}

item3 backwardBFS(const Graph& G, int r, vector<int>& dist) {
	queue<int> q;
	vector<bool> visited(G.num_vertex, false);
	vector<int> prev(G.num_vertex, -1);

	dist[r] = 0;
	prev[r] = r;

	q.push(r);
	visited[r] = true;

	while (!q.empty()) {
		int u = q.front(); q.pop();

		for (const int& v: G.adj_in[u]) {
			if (visited[v]) continue;

			dist[v] = dist[u] + 1;
			prev[v] = u;

			q.push(v);
			visited[v] = true;
		}
	}

	int max_dist = 0;
	int max_dist_vertex = -1;
	for (int i = 0; i < G.num_vertex; ++i) {
		if (visited[i] && max_dist < dist[i]) {
			max_dist = dist[i];
			max_dist_vertex = i;
		}
	}

	if (max_dist == 0) {
		return { r, 0, r };
	}

	int d = max_dist;
	int y = max_dist_vertex;
	while (d > 0) {
		y = prev[y];
		d -= 2;
	}

	return { max_dist_vertex, max_dist, y };
}

struct item2 {
	int x, d;
};

item2 dSweep2(const Graph& G) {
	int r = rand() % G.num_vertex;

	vector<int> dist(G.num_vertex, INF);

	int a1 = forwardBFS(G, r, dist).x;
	item3 one = backwardBFS(G, a1, dist);
	int u1 = one.y;
	int d1 = one.d;

	int a2 = backwardBFS(G, r, dist).x;
	item3 two = forwardBFS(G, a2, dist);
	int u2 = two.y;
	int d2 = two.d;

	if (d1 > d2) {
		return { u1, d1 };
	} else {
		return { u2, d2 };
	}
}

int HR(const Graph& G) {
	int r = dSweep2(G).x;

	vector<int> distF(G.num_vertex, INF);
	vector<int> distB(G.num_vertex, INF);

	forwardBFS(G, r, distF);
	backwardBFS(G, r, distB);

	vector<pair<int, int>> dist(G.num_vertex);
	for (int i = 0; i < G.num_vertex; ++i) {
		dist[i] = make_pair(max(distF[i], distB[i]), i);
	}

	sort(dist.rbegin(), dist.rend());

	int R = INF;
	for (int i = 0; i < G.num_vertex; ++i) {
		int d = forwardBFS(G, dist[i].second, distF).d;
		if (R > d) {
			R = d;
		} else {
			return R;
		}
	}

	return R;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "Usage: ./program <input path>" << endl;
		return EXIT_FAILURE;
	}

	string input_path(argv[1]);
	ifstream input(input_path);

	if (!input.is_open()) {
		cerr << "Failed to open file." << endl;
		return EXIT_FAILURE;
	}

	Graph G;

	string line;
	while (getline(input, line)) {
		if (line[0] == 't') {
			istringstream iss(line);

			char t;
			size_t id, num_vertex;
			iss >> t >> id >> num_vertex;

			G.init(num_vertex);

		} else if (line[0] == 'v') {
		} else if (line[0] == 'e') {
			istringstream iss(line);

			char t;
			int u, v, l;
			iss >> t >> u >> v >> l;

			G.adj_out[u].push_back(v);
			G.adj_in[v].push_back(u);

		} else {
			cerr << "Invalid tag: " << line[0] << endl;
			return EXIT_FAILURE;
		}
	}

	cout << "radius: " << HR(G) << endl;

	return EXIT_SUCCESS;
}
