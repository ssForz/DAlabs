#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<memory>
#include<algorithm>
#include<time.h>

using namespace std;
int total_id = 0;
string sample;
string finder;

vector<int> res;

class Node
{
public:
	Node() {
		id = total_id;
		total_id++;
	};

	void InsNd(int i) {
		auto it = nodes.find(sample[i]);
		if (it == nodes.end()) {
			CreateNode(i, -1);
		} else {
			it->second->Find(i);
		}
	};

	void CreateNode(int i, int e) {
		nodes[sample[i]] = make_unique<Node>(); //aab$ ab$  // aa -aab   aaa
		nodes[sample[i]]->start = i;			//0 2             -b
		if (e == -1) {
			nodes[sample[i]]->end = sample.size();
			return;
		}
		nodes[sample[i]]->end = e;
	};

	void Find(int i){
		int save = -1;
		int pos = 0;
		for (size_t k = 0; k < end - start + 1; ++k) {
			if ((i + k) >= sample.size()) {
				break;
			}

			if (sample[i + k] != sample[start + k]) {
				save = start + k;
				pos = i + k;
				break;
			}
		}
		
		if ((end - start + 1 < sample.size() - i) && save == -1) { //find next
			// cout<<"Change to "<<i + end - start + 1<<endl;
			auto it = nodes.find(sample[i + end - start + 1]);
			if (it == nodes.end()) {
				CreateNode(i + end - start + 1, -1);
				return;
			}
			it->second->Find(i + end - start + 1);
			return;
		}

		Split(pos, save);
	};

	void Split(int i, int sv){
		int save = end;
		end = sv - 1;
		map<char, unique_ptr<Node>> child;
		for (auto it = nodes.begin(); it != nodes.end(); ) {
			child[it->first] = move(it->second);
			nodes.erase(it++);
		}
		CreateNode(sv, save);
		for (auto it = child.begin(); it != child.end(); ++it) {
			nodes[sample[sv]]->nodes[it->first] = move(it->second);
		}
		CreateNode(i, -1);
	};


	void PrintNd(int depth) {
			if (nodes.empty()) {
				return;
			}
			for (auto pr = nodes.begin(); pr != nodes.end(); ++pr) {
				for (int i = 0; i < depth; ++i) {
					cout<<"\t";
				};

				int e = pr->second->end;

				for (int k = pr->second->start; k <= pr->second->end; ++k) {
					cout<<sample[k];
				}
				cout<<" ("<<pr->second->id<<")";
				cout<<endl;
				pr->second->PrintNd(depth + 1);
			}
	};



	void SearchStart() {
		auto it = nodes.find(finder[0]);
		if (it == nodes.end()) {
			return;
		}

		it->second->SearchIn(0);
	};

	void SearchIn(int i) {
		//cout<<id<<" "<<i<<endl;
		int save;
		int found = 0;
		for (int k = 0; k < end - start + 1; ++k) {
			if (k + i == finder.size() - 1) {
				found = 1;
				save = 0;
			}
			if (k + i >= finder.size()) {
				found = 1;
				save = end - start + 1 - k;
				break;
			}
			if (sample[k + start] != finder[k + i]) {
				found = 0;
				return;
			}
		}

		if (found) {
			if (nodes.empty()) {
				int sz = sample.size() - (finder.size() + save) + 1;
				res.push_back(sz);
			} else {
				for (auto it = nodes.begin(); it != nodes.end(); ++it) {
					//cout<<"in "<<it->second->id<<endl;
					//cout<<finder.size() + save<<endl;
					it->second->Dfs(finder.size() + save);
				}
			}
		} else {
			auto it = nodes.find(finder[i + end - start + 1]);
			if (it == nodes.end()) {
				return;
			}

			it->second->SearchIn(i + end - start + 1);
		}

	};

	void Dfs(int index) {
		index += end - start + 1;
		//cout<<"inn "<<id<<" "<<index<<endl;
		if (!nodes.empty()) {
			for (auto it = nodes.begin(); it != nodes.end(); ++it) {
				it->second->Dfs(index);
			}
		} else {
			res.push_back(sample.size() - index + 1);
		}

	};




private:
	int id;
	int start;
	int end;
	map<char, unique_ptr<Node>> nodes;
};

class SFTree
{
public:

	void InitString(string str) {
		sample = str;
	};

	void BuildTree() {
		for(size_t i = 0; i < sample.size(); ++i) {
			// cout<<"CUR: "<<i<<endl;
			InsertNd(i);
			// cout<<endl<<endl;
			// PrintT();
		}
	};

	void Search(string str) {
		finder = str;
		root.SearchStart();
	}

	void PrintT() {
		root.PrintNd(1);
	}

private:
	Node root;
	void InsertNd(size_t i) {
		root.InsNd(i);
	}
};


int main()
{
	string str;
	cin>>str;
	str += "$";
	SFTree tree;
	tree.InitString(str);
	unsigned int start_time = clock();
	tree.BuildTree();
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time; // искомое время
	cout<<(double)search_time / CLOCKS_PER_SEC<<"\n";
	return 0;
	//tree.PrintT();
	// cout<<"start search"<<endl;
	int number = 0;
	while(cin>>str) {
		++number;
		tree.Search(str);
		if (!res.empty()) {
			sort(res.begin(), res.end(), greater<>());
			cout<<number<<": ";
			while(!res.empty()) {
				int ind = res.back();
				res.pop_back();
				if (res.empty()) {
					cout<<ind + 1<<"\n";
				} else
					cout<<ind + 1<<", ";
			}
		}
	}

}