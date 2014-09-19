/*
 * Description	: An accepted solution to the SPOJ problem ORDERSET (http://www.spoj.com/problems/ORDERSET/)
 *		  This version uses Binary Indexed Trees (BITs); to know more, read FENWICK - http://pdf.aminer.org/001/073/976/a_new_data_structure_for_cumulative_frequency_tables.pdf
 *		  This version runs in O(n * log n) where n is the upper bound on the count of numbers that can be passed
 *		  as arguments to INSERT(S,x) or COUNT(S,x) -- the "keys". This is essentially the maximum possible number of 
 *		  queries. The O(n * log n) bound is mainly due to the step in which keys are sorted.
 *
 * Author	: Chitharanjan Das
 * Date		: Sept 19, 2014
 *
 */

#include <iostream>	//For I/O
#include <algorithm>	//For sort()

#define MAXUNIVSIZE 200000	//The maximum number of distinct numbers that may be present in the set

#define UNIVMAX 1000000000	//Upper and lower bounds on the range of numbers
#define UNIVMIN -1000000000

char cmd[MAXUNIVSIZE];	//cmd[] stores the list of commands (I, D, K, C)
long arg[MAXUNIVSIZE]; 	//arg[] stores the list of command arguments

long sym[MAXUNIVSIZE];	//sym[] stores a de-duplicated list of all arguments passed to I and C. Sub-frequencies for these numbers will be stored in our BIT, so that they may be looked up later.

unsigned long *arr;	//The BIT.

unsigned long cnt = 0;	//cnt tracks the count of numbers in the set after every operation.
unsigned long mask_init;	//mask_init stores an initial value for the bitmask used in kth()

long *alphabet; //pointer to a searchable array of alphabets
bool *is_present; //pointer to an array that tracks whether a number is present in the ordered set

using namespace std;

/*
 * cmp()	: Defines the sorting order used by calls to bsearch()
 *
 */
int cmp(const void *a, const void *b) {
	
	return	(int)(*(long *)a - *(long *)b);
}

/*
 * insert()	: Implements the INSERT(S,x) operation. Runs in O(log n) where n = UNIVMAX.
 *
 *		  Note that the original BIT insertion algorithm runs in O(log M) where M is the number of bits in UNIVMAX.
 *		  However, for this particular problem, an additional lookup step is required since we're using coordinate 
 *		  compression for fitting elements in the range [-10^9, 10^9] into a 2*10^5-sized array. This lookup is 
 *		  implemented using bsearch(), which runs in O(log n).
 *
 */
void insert(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {

	//lookup original value of the compressed coordinate
	long *res =  (long *)bsearch(&arg, alphabet, alphabetSize, sizeof(long), cmp);
	unsigned int idx = res - alphabet;
	
	if(is_present[idx]) {
		//arg is already present in the set, so it cannot be inserted
		return;
	}
	else {
		//add arg to the set, and update the BIT's sub-frequencies
		cnt++;
		is_present[idx++] = true;
		while(idx <= alphabetSize) {
			arr[idx]++;
			idx += (idx & (-idx));
		}
	}
}

/*
 * remove()	: Implements the DELETE(S,x) operation. Runs in O(log n) where n = UNIVMAX.
 *		  
 *		  Like with insert(), the original BIT deletion algorithm runs in O(log M), but for this particular problem,
 *		  we add an extra lookup step that runs in O(log n).
 *
 */
void remove(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {
	
	//lookup original value of the compressed coordinate
	long *res =  (long *)bsearch(&arg, alphabet, alphabetSize, sizeof(long), cmp);
	
	if(res == NULL) {
		//arg is not one of the arguments used by any of the I or C commands, so it cannot be deleted
		return;
	}
	
	unsigned int idx = res - alphabet;
	
	if(!is_present[idx]) {
		//arg is not present in the set, so it cannot be deleted
		return;
	}
	else {
		//remove arg from the set and update the BIT's sub-frequencies
		cnt--;
		is_present[idx++] = false;
		while(idx <= alphabetSize) {
			arr[idx]--;
			idx += (idx & (-idx));
		}
	}
}

/*
 * kth()	: Implements the K-TH(S,x) operation. Runs in O(n) where n = UNIVMAX. The average case performance is
 *		  much better, and this case is also fairly common, at least in the test inputs used by SPOJ.
 *		  
 *		  This is a pretty straightforward implementation of the algorithm for looking up an element
 *		  given a cumulative frequency, given in FENWICK. That algorithm runs in O(log M) where M is the 
 *		  number of bits in UNIVMAX. However, it returns any element it can find, even if its actual frequency
 *		  is zero. K-TH(S,x) requires the least such element.
 *
 *		  To work around this problem, I've used a linear lookup back from the found element, that runs in O(UNIVMAX).
 *
 *		  TODO: Implement a better way to solve the problem of finding the least element
 *
 */
long kth(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {
	
	if(arg > (long)cnt || arg < 1) {
		//arg is too small or too large, so return an invalid value
		return UNIVMAX + 1;
	}
	
	//search for any element at which the cumulative frequency is equal to arg
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
	
	//now search for the least such element
	while(idx > 1 && !is_present[idx - 1]) {
		--idx;
	}
	
	return alphabet[idx - 1];
}

/*
 * count()	: Implements the COUNT(S,x) operation. Runs in O(log n) where n = UNIVMAX. 
 *		  
 *		  This is a pretty straightforward implementation of the algorithm for looking up the cumulative frequency
 *		  at an element, given in FENWICK. That algorithm runs in O(log M) where M is the 
 *		  number of bits in UNIVMAX.
 *
 *		  Like with insert(), the original BIT frequency-calculation algorithm runs in O(log M), but for this 
 *		  particular problem, we add an extra lookup step that runs in O(log n)
 *
 */
int count(unsigned long *arr, int arg, long *alphabet, bool *is_present, unsigned long alphabetSize) {
	
	//lookup original value of the compressed coordinate
	long *res =  (long *)bsearch(&arg, alphabet, alphabetSize, sizeof(long), cmp);
	
	if(res == NULL) {
		//arg is not one of the arguments used by any of the I or C commands, so it cannot be used
		return -1;
	}
	
	//compute the cumulative frequency at arg's index
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
	//additionally, also store all numbers to be inserted in sym[]
	cin >> cmdCount;
	while(i < cmdCount) {
		
		cin >> cmd[i] >> arg[i];
		
		if(cmd[i] == 'I' || cmd[i] == 'C') 
			sym[j++] = arg[i];
		i++;
	}
	
	//in-place de-duplication and coordinate compression O(n * log n)
	sort(sym, sym + j);
	for(i = 1, k = 1; i < j; i++) {
		
		if(sym[i] != sym[i - 1]) 
			sym[k++] = sym[i];
	}
	alphabetSize = k;
	
	//allocate space for the BIT and the symbol alphabet
	arr = new unsigned long[alphabetSize + 1]; //Allocated memory is also zero-ed
	is_present = new bool[alphabetSize];
	
	//compute initial mask value used in calls to kth(). This is the highest power of 2 less than or equal to alphabetSize.
	unsigned long v = alphabetSize - 1;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	mask_init = v >> 1;
	
	//loop through all commands in order, and process
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
