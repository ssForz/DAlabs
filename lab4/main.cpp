#include<iostream>
#include<vector>
#include<malloc.h>
#include<string>
#include <time.h> 

using namespace std;

char ConvertToLow(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 32;
	return c;
}

int* ZFunc(string pattern) //ababc
{
	int size = pattern.size();
	int* zfunc = (int*)malloc(size*sizeof(int));
	int right = 0, left = 0;
	zfunc[0] = size;
	for (int i = 0; i < size; ++i) {
		zfunc[i] = max(0,min(right-i, zfunc[i-left]));
		while(i + zfunc[i] < size && pattern[zfunc[i]] == pattern[i+zfunc[i]]) {
			zfunc[i]++;
		}
		if (i+zfunc[i]>right) {
			left = i;
			right = i + zfunc[i];
		}
	}
	return zfunc;
}

int Separator(char c)
{
	if (c == ' ' || c == '\t') {
		return 1;
	}
	return 0;
}

void SearchZ(string pattern, string text, vector<int> cntwords)
{
	string search = pattern + "*" + text;
	// cout<<search<<endl;
	for (int i = 0; i < search.size(); ++i) {
		search[i] = ConvertToLow(search[i]);
	}
	int* zfunc = (int*)malloc(search.size()*sizeof(int));
	zfunc = ZFunc(search);
	int count = 0;
	int word = 1;
	int line = 1;
	if (cntwords[line-1] == 0) {
		line++;
	}
	for (int i = pattern.size(); i < pattern.size() + text.size() + 1; ++i) {
		if (zfunc[i] == pattern.size() && (search[i-1] == '*'|| search[i-1] == ' ') && (search[i + zfunc[i]] == ' ')) {
			// cout<<line<<", "<<word<<endl;	
		}
		if (search[i] == ' ' && search[i - 1] != ' ') {
			if (word < cntwords[line - 1]) {
				word++;
			} else {
				line++;
				word = 1;
				if (cntwords[line - 1] == 0) {
					for (int k = line - 1; k < cntwords.size(); ++k) {
						if (cntwords[k] != 0)
							break;
						if (cntwords[k] == 0)
							line++;
					}
				}
			}
		}
	}

}

int main()
{
	ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
	string pattern;
	string str;
	vector<string> wholething;
	vector<int> cntwords;
	string text = "";
	int check = 0;
	int cntpat = 0;
	int first_void = 1;
	int cnt = 0;
	while(getline(cin, str, '\n')) {
		if (cnt == 0) {
			string line = "";
			int first = 1;
			int first_space = 1;
			for (int k = 0; k < str.size(); ++k) {
				if (first == 1 && !Separator(str[k])) {
					first = 0;
				}
				if (first == 1 && Separator(str[k])) {
					continue;
				}
				if (Separator(str[k]) && first_space == 0) {
					continue;
				}
				if (Separator(str[k]) && first_space == 1 && k != str.size() - 1) {
					first_space = 0;
					line += " ";
				}
				if (!Separator(str[k])) {
					first_space = 1;
					line += str[k];
				}
			}
			if (line[line.size() - 1] == ' ') {
				line.pop_back();
			}
			pattern = line;
		}
		if (cnt != 0) {
			cntwords.push_back(0);
			for (int k = 0; k < str.size(); ++k) {
				if (k == 0) {
					continue;
				}
				if (Separator(str[k]) && !Separator(str[k-1])) {
					cntwords[cnt-1]++;
				}
			}
			if (str[str.size() - 1] != ' ') {
				cntwords[cnt-1]++;
			}
			string line = "";
			int first = 1;
			int first_space = 1;
			for (int k = 0; k < str.size(); ++k) {
				if (first == 1 && !Separator(str[k])) {
					first = 0;
				}
				if (first == 1 && Separator(str[k])) {
					continue;
				}
				if (Separator(str[k]) && first_space == 0) {
					continue;
				}
				if (Separator(str[k]) && first_space == 1 && k != str.size() - 1) {
					first_space = 0;
					line += " ";
				}
				if (!Separator(str[k])) {
					first_space = 1;
					line += str[k];
				}

			}
			if (line[line.size() - 1] == ' ') {
				line.pop_back();
			}
			if (line == "") {
				cntwords[cnt-1] = 0;
			}
			if (line != "") {
				text += (line + " ") ;
			}

		}
		cnt++;
	}

	unsigned int start_time =  clock(); 
	SearchZ(pattern, text, cntwords);
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time; // искомое время
	printf("%lf sec \n", ((double)search_time / CLOCKS_PER_SEC));
	// for (int i = 0; i < cntwords.size(); ++i) {
	// 	cout<<cntwords[i]<<endl;
	// }

}