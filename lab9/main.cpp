#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_set>
#include<time.h>

using namespace std;

bool comp(const pair<int,int>& a, const pair<int,int>& b)
{ 
    return min(a.first, a.second) < min(b.first, b.second); 
}

int Kuhn(int v, vector<vector<int>>& data, vector<int>& pr, vector<int>& visited)
{
	if (visited[v]) {
		return 0;
	}
	visited[v] = 1;
	for (size_t i = 0; i < data[v].size(); ++i) {
		if (pr[data[v][i]] == -1 || Kuhn(pr[data[v][i]], data, pr, visited)) {
			pr[data[v][i]] = v;
			return 1;
		}
	}
	return 0;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	int n, m;
	cin >> n >> m;
	vector<vector<int>> data(n);
	vector<int>right;
	vector<int> pr(n);
	unordered_set<int> l;
	unordered_set<int> r;
	for (int i = 0; i < m; ++i) {
		int v, u;
		cin>>v>>u;
		data[v - 1].push_back(u - 1);
		data[u - 1].push_back(v - 1);
	}
	for (int i = 0; i < n; ++i) {
		if (r.find(i) == r.end() && l.find(i) == l.end()) {
			r.insert(i);
			for (size_t k =0; k < data[i].size(); ++k) {
				l.insert(data[i][k]);
			}
		}
		if (l.find(i) != l.end()) {
			for (size_t k =0; k < data[i].size(); ++k) {
				r.insert(data[i][k]);
			}
		}
		if (r.find(i) != r.end()) {
			for (size_t k =0; k < data[i].size(); ++k) {
				l.insert(data[i][k]);
			}
		}
	}
	for (auto it: r) {
		right.push_back(it);
	}
	sort(right.begin(), right.end());
	for (int i = 0; i < right.size(); ++i) {
		sort(data[right[i]].begin(), data[right[i]].end());
	}
	vector<int> visited(n);
	pr.assign(n, -1);
	unsigned int start_time = clock();
	for (int i = 0; i < right.size(); ++i) {
		visited.assign(n, 0);
		Kuhn(right[i], data, pr, visited);
	}
	unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time; // искомое время
    cout<<(double)search_time / CLOCKS_PER_SEC<<"\n";
	// int cnt = 0;
	// for (size_t i = 0; i < pr.size(); ++i) {
	// 	if (pr[i] != -1) {
	// 		cnt++;
	// 	}
	// }
	// cout<<cnt<<"\n";
	// vector<pair<int,int>> res;
	// for (int i = 0; i < pr.size(); ++i) {
	// 	if (pr[i] != -1) {
	// 		res.push_back({i, pr[i]});
	// 	}
	// }
	// sort(res.begin(), res.end(), comp);
	// for (int i = 0; i < res.size(); ++i) {
	// 	cout<<min(res[i].first, res[i].second) + 1 <<" "<<max(res[i].first, res[i].second) + 1<<"\n";
	// }
}