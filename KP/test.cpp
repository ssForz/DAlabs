#include <iostream>
#include <vector>
#include <complex>
#include<algorithm>
#include<iomanip>
#include<time.h>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

using namespace std;

const int WIDTH = 4096;
const int STEP = 1024;

const double pi = acos(-1.0);

bool comp(complex<double> a, complex<double> b)
{
	return sqrt(a.real()*a.real() + a.imag() * a.imag()) < sqrt(b.real()*b.real() + b.imag() * b.imag()) ? true : false;
}

vector<short> decoder()
{
	mp3dec_t mp3d;
	mp3dec_file_info_t info;
	if (mp3dec_load(&mp3d, "./music/sample5.mp3", &info, NULL, NULL)) {
		throw runtime_error("Decode error");
	}
	vector<short> res(info.buffer, info.buffer + info.samples);
	free(info.buffer);
	return res;
}

int bitReversalPermutation(int sz, int a) 
{
	int reverse = 0;
	while (sz > 1) { //bit-reversal because fft: 2n and 2n+1 order
		reverse = reverse | (a & 1);
		reverse = reverse << 1;
		a = a >> 1;
		sz /= 2;
	}
	reverse = reverse >> 1;
	return reverse;
}
vector <complex <double>> save(WIDTH);

void fastFourier(vector <double>& window, vector <complex <double>>& res_window, complex <double> z, int left, int right) 
{
	if (right - left == 0) {
		int sz = window.size();
		int reverse = bitReversalPermutation(sz, left);
		res_window[right] = window[reverse];
		return;
	}
	int middle = (right + left) / 2;
	fastFourier(window, res_window, z * z, left, middle);
	fastFourier(window, res_window, z * z, middle + 1, right);
	int sz = right - left + 1;
    int divis = sz / 2;
    complex<double> zphase = 1.0;
    for (int i = 0; i < sz; i++)
    {
        save[left + i] = res_window[left + i % divis] + zphase * res_window[middle + 1 + i % divis];
        zphase *= z;
    }
    for (int i = 0; i < sz; i++)
        res_window[left + i] = save[left + i];
}

void calculateSpectrum(vector<short> sample) {
	// vector <vector <complex <double>>> spectrum;
	for (size_t i = 0; i < sample.size() - WIDTH; i += STEP) {
		vector <double> window(WIDTH);
		vector <complex <double>> res_window(WIDTH);
		for (int k = 0; k < WIDTH; k++) {
			window[k] = sample[i + k] * 0.5 * (1 - cos(2 * pi * k / (WIDTH - 1))); //Henning window
		}
		int sz = window.size();
		complex <double> z = polar((double)1.0, 2 * pi / sz); //returning complex number by r and phase
		fastFourier(window, res_window, z, 0, sz - 1);
		auto x = max_element(res_window.begin(), res_window.end(), comp);
		// cout<<fixed<<setprecision(10)<<sqrt(x->real()*x->real() + x->imag() * x->imag())<<"\n";
		// spectrum.push_back(res_window);
	}
	// return spectrum;
}

int main()
{
	vector<short> sample = decoder();
	cout<<"Кол-во отсчётов: "<<sample.size()<<"\n";
	// vector <vector <complex <double>>> spectrum = calculateSpectrum(sample);
	unsigned int start_time = clock();
	calculateSpectrum(sample);
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time; // искомое время
	cout<<(double)search_time / CLOCKS_PER_SEC<<"\n";
	// for (size_t i = 0; i < spectrum[0].size(); ++i) {
	// 	cout<<spectrum[0][i]<<" ";
	// }
	// for (size_t i = 0; i < spectrum.size(); ++i) {
	// 	auto x = max_element(spectrum[i].begin(), spectrum[i].end(), comp);
	// 	cout<<fixed<<setprecision(10)<<sqrt(x->real()*x->real() + x->imag() * x->imag())<<endl;
	// }
	
	
}