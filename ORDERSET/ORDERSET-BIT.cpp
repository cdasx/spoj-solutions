#include <iostream>
#include <algorithm>

#define MAXUNIVSIZE 200000
#define UNIVMAX 1000000000
#define UNIVMIN -1000000000

char cmd[MAXUNIVSIZE];
long arg[MAXUNIVSIZE], sym[MAXUNIVSIZE];

unsigned long *arr, cnt = 0, mask_init;

long *alphabet; //pointer to a searchable array of alphabets
bool *is_present; //pointer to an array that tracks whether a number is present in the ordered set

using namespace std;

int cmp(const void *a, const void *b) {
	
	return	(int)(*(long *)a - *(long *)b);
}

void insert(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {

	long *res =  (long *)bsearch(&arg, alphabet, alphabetSize, sizeof(long), cmp);
	unsigned int idx = res - alphabet;
	
	if(is_present[idx]) {
		return;
	}
	else {
		cnt++;
		is_present[idx++] = true;
		while(idx <= alphabetSize) {
			arr[idx]++;
			idx += (idx & (-idx));
		}
	}
}

void remove(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {
	
	long *res =  (long *)bsearch(&arg, alphabet, alphabetSize, sizeof(long), cmp);
	
	if(res == NULL) {
		return;
	}
	
	unsigned int idx = res - alphabet;
	
	if(!is_present[idx]) {
		return;
	}
	else {
		cnt--;
		is_present[idx++] = false;
		while(idx <= alphabetSize) {
			arr[idx]--;
			idx += (idx & (-idx));
		}
	}
}

long kth(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {
	
	if(arg > (long)cnt || arg < 1) {
		return UNIVMAX + 1;
	}
	
	unsigned long idx = 0, mask = mask_init, mid;
	
	while(mask > 0 && idx <= alphabetSize) {
		
		mid = idx + mask;
		if(mid <= alphabetSize) {
			if(arg > (long)arr[mid]) {
				idx = mid;
				arg -= arr[mid];
			} else if (arg == (long)arr[mid]) {
				idx = mid;
				break;
			}
		}
		mask >>= 1;
	}
	while(idx > 1 && !is_present[idx - 1]) {
		--idx;
	}
	
	return alphabet[idx - 1];
}

int count(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {
	
	long *res =  (long *)bsearch(&arg, alphabet, alphabetSize, sizeof(long), cmp);
	
	if(res == NULL) {
		return -1;
	}
	
	unsigned int k = 0, idx = res - alphabet;
	k -= is_present[idx];
	idx++;
	while(idx > 0) {
		k += arr[idx];
		idx &= (idx - 1);
	}
	
	return k;
}

int main() {
	
	unsigned int cmdCount, alphabetSize, i = 0, j = 0, k = 1;
	int res = 0;
	
	//read and store all the commands
	//additionally, also store all numbers to be inserted in coords[]
	cin >> cmdCount;
	while(i < cmdCount) {
		
		cin >> cmd[i] >> arg[i];
		
		if(cmd[i] == 'I' || cmd[i] == 'C') 
			sym[j++] = arg[i];
		i++;
	}
	
	//in-place de-duplication and coordinate compression O(n*logn)
	sort(sym, sym + j);
	for(i = 1, k = 1; i < j; i++) {
		
		if(sym[i] != sym[i - 1]) 
			sym[k++] = sym[i];
	}
	alphabetSize = k;
	
	//allocate space for the BIT and the symbol alphabet
	arr = new unsigned long[alphabetSize + 1]; //Allocated memory is also zero-ed
	alphabet = new long[alphabetSize];
	is_present = new bool[alphabetSize];
	
	//compute initial mask value used in calls to kth
	unsigned long v = alphabetSize - 1;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	mask_init = v >> 1;
	
	for(i = 0; i < alphabetSize; i++) {
		
		alphabet[i] = sym[i];
	}
	
	//loop through all commands
	for(i = 0; i < cmdCount; ++i) {
		
		switch(cmd[i]) {
			
			case 'I':
				insert(arr, arg[i], alphabet, is_present, alphabetSize);
				break;
				
			case 'D':
				remove(arr, arg[i], alphabet, is_present, alphabetSize);
				break;
				
			case 'K':
				res = kth(arr, arg[i], alphabet, is_present, alphabetSize);
				if(res > UNIVMAX)
					cout << "invalid\n";
				else
					cout << res << "\n";
				break;
				
			case 'C':
				res = count(arr, arg[i], alphabet, is_present, alphabetSize);
				if(res < 0)
					cout << "invalid\n";
				else
					cout << res << "\n";
				break;
		}
	}
	
	return 0;
}