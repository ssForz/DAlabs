#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>

using namespace std;


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin>>m>>n;
    vector<vector<double>> data(m, vector<double>(n + 2, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n + 1; ++j) {
            cin>>data[i][j];
        }
        data[i][n + 1] = i + 1;
    }
    vector<int> ans;
    unsigned int start_time =  clock(); 
    for (int j = 0; j < n; ++j) {
        int min = 51;
        int found = -1;
        for (int i = j; i < m; ++i) {
            if (data[i][n] < min && data[i][j] != 0) {
                found = i;                                 //search in each column
                min = data[i][n];
            }
        }
        if (found != -1) {
            vector<double> save;
            save = data[j];
            data[j] = data[found];                     //swap
            data[found] = save;
            ans.push_back(data[j][n + 1]);
            for (int i = j + 1; i < m; ++i) {          //matrix to step type
                double coefStep = data[i][j] / data[j][j];
                for (int k = j; k < n; ++k) {
                    data[i][k] -= (coefStep * data[j][k]);
                }
            }

        } else {
            cout<<"-1\n";
            return 0;
        }
    }
    sort(ans.begin(), ans.end());
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time; // искомое время
    cout<<(double)search_time / CLOCKS_PER_SEC<<"\n";
    for (int i = 0; i < ans.size(); ++i) {
        cout<<ans[i]<<" ";
    }
    cout<<"\n";
}