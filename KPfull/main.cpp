#include <iostream>
#include <vector>
#include <complex>
#include<algorithm>
#include<iomanip>
#include<time.h>
#include<fstream>
#include<time.h>
#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

using namespace std;

const int WIDTH = 4096;
const int STEP = 2048;

const double pi = acos(-1.0);

struct vec7 {
	short vec[7];
	
};



bool comp(complex<double> a, complex<double> b)
{
	return sqrt(a.real()*a.real() + a.imag() * a.imag()) < sqrt(b.real()*b.real() + b.imag() * b.imag()) ? true : false;
}

const vector <int> octaves = {0, 32, 64, 128, 256, 512, 1024, 2048};

vec7 splitFrequency(vector <complex <double>>& cur_spectrum) {
	vec7 result;
	for (int i = 0; i < octaves.size() - 1; ++i) {
		double max = 0;
		short freq;
		for (int k = octaves[i]; k < octaves[i+1]; ++k) {
			if (abs(cur_spectrum[k]) > max) {
				max = abs(cur_spectrum[k]);
				freq = k;									//find maximum intense frequency
			}
		}
		result.vec[i] = freq;
	}

	return result;
}

vector<short> decoder(string filename)
{
	mp3dec_t mp3d;
	mp3dec_file_info_t info;
	if (mp3dec_load(&mp3d, filename.c_str(), &info, NULL, NULL)) {
		throw runtime_error("Decode error");
	}
	vector <short> file(info.buffer, info.buffer + info.samples);
    free(info.buffer);
    vector <short> res;
    for (int i = 0; i < file.size(); i += info.channels)
    {
        int sum = 0;
        for (int j = 0; j < info.channels; j++)
            sum += file[i + j];
        res.push_back(sum / info.channels);
    }
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

vector <vec7> calculateSpectrum(vector<short> sample) {
	vector <vec7> result;
	for (size_t i = 0; i < sample.size() - WIDTH; i += STEP) {
		vector <double> window(WIDTH);
		vector <complex <double>> res_window(WIDTH);
		for (int k = 0; k < WIDTH; k++) {
			window[k] = sample[i + k] * 0.5 * (1 - cos(2 * pi * k / (WIDTH - 1))); //Henning window
		}
		int sz = window.size();
		complex <double> z = polar((double)1.0, 2 * pi / sz); //returning complex number by r and phase
		fastFourier(window, res_window, z, 0, sz - 1);
		vec7 vec = splitFrequency(res_window);
		result.push_back(vec);
	}

	return result;
}


class AuPrint
{
public:
	AuPrint(vector<short> samples) {
		data = calculateSpectrum(samples);
	}

	AuPrint(ifstream& in) {
		in>>name;
		int size;
		in>>size;
		for (int i = 0; i < size; ++i) {
			vec7 v;
			for (int k = 0; k < 7; ++k) {
				in>>v.vec[k];
			}
			data.push_back(v);
		}
	}

	void SetName(string _name) {
		name = _name;
	}

	void Save(ofstream& out) {
		out << name <<"\n";
		out << data.size() << "\n";
		for (size_t i = 0; i < data.size(); ++i) {
			for (size_t k = 0; k < 7; ++k) {
				out<<data[i].vec[k]<<" ";
			}
			out<<"\n";
		}
	}

	string GetName() {
		return name;
	}

	size_t size()
	{
		return data.size();
	}

	vector <vec7> data;
private:
	string name;
	
};

int matchingVecs(vec7 v1, vec7 v2)
{
	int match = 0;
	for (int i = 0; i < 7; ++i) {
		if (abs(v1.vec[i] - v2.vec[i]) < 5) {
			++match;
		}
	}

	if (match >= 3) {
		return 0;
	} else return 2;
}

double compare(int pos, AuPrint& track, AuPrint& sample)
{
	int n = min(track.size(), sample.size());
	vector<vector<int>> LevDp(n + 1, vector<int>(n + 1));
	LevDp[0][0] = 0;
	for (int i = 0; i < n + 1; ++i) {
		LevDp[i][0] = i;
		LevDp[0][i] = i;
	}

	for (int i = 1; i < n + 1; ++i) {
		for (int j = 1; j < n + 1; ++j) {
			LevDp[i][j] = min(min(LevDp[i - 1][j] + 1, 
								LevDp[i][j - 1] + 1),
								LevDp[i - 1][j - 1] + matchingVecs(track.data[pos + i - 1], sample.data[j - 1])); //Wagner-Fisher
		}
	}

	return 1 - LevDp[n][n]/(2.0 * n);
}

// double startSearch(AuPrint& track, AuPrint& sample)
// {
// 	double maxSimilarCoef = 0;
// 	int ts = track.size();
// 	int ss = sample.size();
// 	for (int i = 0; i < ts - ss; ++i) {
// 		double cur = compare(i, track, sample);
// 		if (cur > maxSimilarCoef)
// 		{
// 			maxSimilarCoef = cur;
// 		}

// 		if (maxSimilarCoef > 0.7) {
// 			break;
// 		}
// 	}
// 	return maxSimilarCoef;
// }

double startSearch(AuPrint& track, AuPrint& sample)
{
	int tracksize = track.size();
	int samplesize = sample.size();
	double prev = compare(0, track, sample);
    double maxSimilarCoef = 0;
	for (int i = samplesize; i < tracksize - samplesize; i += samplesize) {
		double current = compare(i, track, sample);
        double leftC = prev;
        double rightC = current;
        int left = i - samplesize;
        int right = i + 1;
        if (prev + current < 0.4)
        {
            prev = current;
            continue;
        }
        int k = 12;
        while(k > 0) {
        	k--;
        	int middle = (left + right) / 2;
        	double midC = compare(middle, track, sample);
        	maxSimilarCoef = max(maxSimilarCoef, midC);
        	if (leftC > rightC) {
        		right = middle;
        		rightC = midC;
        	}
        	if (rightC >= leftC) {
        		left = middle;
        		leftC = midC;
        	}
        }
        prev = current;
	}
	return maxSimilarCoef;
}


int main(int argc, char* argv[])
{
	string command = argv[1];
	ifstream infile;
	ofstream outfile;
	ifstream indfile;
	vector<string> inputfiles;
	for (int i = 2; i < argc; ++i) {
		if (string(argv[i]) == "--input") {
			infile = ifstream(argv[i+1]);
			while(!infile.eof()) {
				string cur;
				getline(infile, cur);
				inputfiles.push_back(cur);
			}
		}
		if (string(argv[i]) == "--output") {
			outfile = ofstream(argv[i+1]);
		}
		if (string(argv[i]) == "--index") {
			indfile = ifstream(argv[i+1]);
		}
	}
	if (command == "index") {
		unsigned int start_time = clock();
		int size = inputfiles.size();
		outfile<<size<<"\n";
		for (size_t i = 0; i < inputfiles.size(); ++i) {
			vector<short> samples = decoder(inputfiles[i]);
			AuPrint auprint(samples);
			auprint.SetName(inputfiles[i]);
			auprint.Save(outfile);
		}
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time; // искомое время
		cout<<(double)search_time / CLOCKS_PER_SEC<<"\n";
	} else if (command == "search") {
		int size;
		indfile>>size;
		vector<AuPrint> prints;
		for (int i = 0; i < size; ++i) {
			AuPrint auprint(indfile);
			prints.push_back(auprint);
		}
		unsigned int start_time = clock();
		int sz = inputfiles.size();
		for (int i = 0; i < sz; ++i) {
			int match = -1;
			vector<short> samples = decoder(inputfiles[i]);
			AuPrint auprint(samples);
			// ofstream outfile2 = ofstream("testmobile.txt");
			// auprint.Save(outfile2);
			double maximum = 0.35;
			for (int k = 0; k < size; ++k) {
				double ans = startSearch(prints[k], auprint);
				cout<<"Curans: "<<ans<<endl;
				if (ans > maximum) {
					maximum = ans;
					match = k;
				} 
			}
			if (match == -1) {
				outfile<<"! NOT FOUND\n";
			} else {
				outfile<<prints[match].GetName()<<" chance:"<<maximum;
			}
		}
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time; // искомое время
		cout<<(double)search_time / CLOCKS_PER_SEC<<"\n";

	}
	
}