#include<iostream>
#include<string>
#include<cmath>
#include <time.h> 

using namespace std;
unsigned long long CountInRange(unsigned long long upEdge, long long m, unsigned long long downEdge) {
	if (upEdge - downEdge < m) {
		if (upEdge % m == 0 || downEdge % m == 0) {
			return 1;
		}
		upEdge -= upEdge % m;
		if (upEdge > downEdge) {
			return 1;
		}
		return 0;
	}
	if (upEdge < m) {
		return 0;
	}
	upEdge -= upEdge % m;
	if (downEdge % m != 0) {
		downEdge += (m - downEdge % m);
	}
	return (upEdge - downEdge) / m + 1;
}

unsigned long long DP(unsigned long long n, long long m, long long len)
{
	if (n <= 0) {
		return 0;
	}
	if (n < m) {
		return 0;
	}
	return (DP(n/10, m, len - 1) + CountInRange(n, m, pow(10, len - 1)));
	
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	unsigned long long n;
	long long m;
	cin>>n>>m;
	long long len = to_string(n).size();
	unsigned int start_time =  clock(); 
	unsigned long long res = DP(n, m, len);
	if (res != 0) {
		res -= (n % m == 0);
	}
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time; // искомое время
	cout<<(double)search_time / CLOCKS_PER_SEC<<"\n";
	cout <<res<<"\n";
}