#include<stdio.h>
#include<stdlib.h>


typedef struct _pair {
	int day;
	int month;
	int year;
	char *key;
	char *value;
} Pair;

int IsSymbol(char c)
{
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '.') {
		return 1;
	}
	return 0;
}

void MyStrcpy(char *dst, const char *src) 
{
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
}


size_t MyStrlen(const char *s) 
{
    size_t len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}

Pair* ParseDate(Pair *data, long long size)
{
	int number = 0;
	for (int i = 0; i < size; i++) {
		int pow_dig = 1;
		int count_num = 1;
		int j = 0;
		int cur_len = 0;
		while (IsSymbol(data[i].key[j])) {
			cur_len++;
			j++;
		}
		data[i].year = 0;
		data[i].month = 0;
		data[i].day = 0;
		for (int k = cur_len - 1; k >= 0; k--) {
			if (count_num == 1) {
				if (data[i].key[k] == '.') {
					pow_dig = 1;
					count_num++;
					continue;
				}
				data[i].year += pow_dig*(int)(data[i].key[k] - 48);
				pow_dig *= 10;
				continue;

			}
			if (count_num == 2) {
				if (data[i].key[k] == '.') {
					pow_dig = 1;
					count_num++;
					continue;
				}
				data[i].month += pow_dig*(int)(data[i].key[k] - 48);
				pow_dig *= 10;
				continue;
			}
			if (count_num == 3) {
				if (data[i].key[k] == '.') {
					pow_dig = 1;
					count_num++;
					continue;
				}
				data[i].day += pow_dig*(int)(data[i].key[k] - 48);
				pow_dig *= 10;
				continue;
			}

		}
	}
	return data;
}

Pair* RadixSort(Pair* data, long long datasize) {
	
	//day
	int pow_digit = 10;
	while (pow_digit != 1000) {
		Pair** index = (Pair**)malloc (10*sizeof(Pair*) + 1);
		for (int i = 0; i < 10; i++) {
			index[i] = (Pair*)malloc(sizeof(Pair) + 1);
		}
		int count[10];
		int size[10];
		int capacity = 1;
		for (int i = 0; i < 10; i++) {
			count[i] = 0;
			size[i] = 1;
		}
		for (int j = 0; j < datasize; j++) {
			int ind = (data[j].day % pow_digit)/(pow_digit/10);
			index[ind][count[ind]] = data[j];
			count[ind]++;
			if (count[ind] >= size[ind]) {
				capacity += 10;
				size[ind] = capacity;
				index[ind] = (Pair*)realloc(index[ind], capacity * sizeof(Pair));
			}
	
		}

		int cnt = 0;
		for(int i = 0; i < 10; i++) {
			for (int k = 0; k < count[i]; k++) {
				data[cnt] = index[i][k];
				cnt++;
			}
		}
		pow_digit *= 10;
		for (int i = 0; i < 10; i++) {
			free(index[i]);
		}
		free(index);
	}
	//month
	pow_digit = 10;
	while (pow_digit != 1000) {
		Pair** index = (Pair**)malloc (10*sizeof(Pair*) + 1);
		for (int i = 0; i < 10; i++) {
			index[i] = (Pair*)malloc(sizeof(Pair) + 1);
		}
		int count[10];
		int size[10];
		int capacity = 1;
		for (int i = 0; i < 10; i++) {
			count[i] = 0;
			size[i] = 1;
		}
		for (int j = 0; j < datasize; j++) {
			int ind = (data[j].month % pow_digit)/(pow_digit/10);
			index[ind][count[ind]] = data[j];
			count[ind]++;
			if (count[ind] >= size[ind]) {
				capacity += 10;
				size[ind] = capacity;
				index[ind] = (Pair*)realloc(index[ind], capacity * sizeof(Pair));
			}
			
		}

		int cnt = 0;
		for(int i = 0; i < 10; i++) {
			for (int k = 0; k < count[i]; k++) {
				data[cnt] = index[i][k];
				cnt++;
			}
		}
		pow_digit *= 10;
		for (int i = 0; i < 10; i++) {
			free(index[i]);
		}
		free(index);
	}

	//year
	pow_digit = 10;
	int status;

	while (pow_digit != 100000) {
		Pair** index = (Pair**)malloc (10*sizeof(Pair*) + 1);
		for (int i = 0; i < 10; i++) {
			index[i] = (Pair*)malloc(sizeof(Pair) + 1);
		}
		int count[10];
		int size[10];
		int capacity = 1;
		for (int i = 0; i < 10; i++) {
			count[i] = 0;
			size[i] = 1;
		}
		for (int j = 0; j < datasize; j++) {
			int ind = (data[j].year % pow_digit)/(pow_digit/10);
			index[ind][count[ind]] = data[j];
			count[ind]++;
			if (count[ind] >= size[ind]) {
				capacity += 10;
				size[ind] = capacity;
				index[ind] = (Pair*)realloc(index[ind], capacity * sizeof(Pair));
			}
			
		}

		int cnt = 0;
		for(int i = 0; i < 10; i++) {
			for (int k = 0; k < count[i]; k++) {
				data[cnt] = index[i][k];
				cnt++;
			}
		}
		pow_digit *= 10;
		for (int i = 0; i < 10; i++) {
			free(index[i]);
		}
		free(index);

	}
	return data;
}


int main(void)
{
    long long size = 0;
    char value[65];
    char key[11];
    int capacity = 10;
    Pair* data = (Pair*)malloc(capacity * sizeof(Pair));
    long long n = 0;
    while (scanf("%s\t%s\n", key, value) == 2) {
        size_t len = MyStrlen(value);
        size_t len2 = MyStrlen(key);
        data[n].key = (char*)calloc(sizeof(char),len2 + 1);
        MyStrcpy(data[n].key, key);
        data[n].value = (char*)calloc(sizeof(char),len + 1);
        MyStrcpy(data[n].value, value);

        n++;
        if (n >= capacity) {
            capacity *= 2;
            data = realloc(data, capacity * sizeof(Pair));
        }
    }
    size = n;


    data = ParseDate(data, size);
	data = RadixSort(data, size);
	for (int i = 0; i < size; i++) {
		printf("%s\t%s\n", data[i].key, data[i].value);
	}
	return 0;
}