#include "real.h"
#include <iostream>

using namespace std;

real fe(int n) {
	real e = 0.0;
	real factorial = 0;
	int j, k = 0;

	for (j = 0; j <= n; j++) {
		factorial = 1;
		for(k = 1; k <= j; k++)
			factorial *= k;
		e += 1 / factorial;
	}

	return e;
}

void test(real p) {
	real array[4];
	real *a = new real[4];

	for (int i = 0; i < 4; i++)
		array[i] = p;

	for (int i = 0; i < 4; i++ ) {
		a[i] = array[i] + p;
		cout << a[i] << endl;
	}

	delete[] a;
}

real pi(int n) {
	real p = 0;
	real s = 1.0;
	real div = 0;

	for (real odd = 1.0; odd < n; odd += 2.0) {
		div = s / odd;
		p = p + div;
		s = s * -1;
	}
	
	return p * 4.0;
}

int main() {
	Config* conf = Config::getInstance();
	conf->setMinError(0.0001);
	conf->setNumberOfSeeds(25);

	for (int i = 0; i < 1000; i++ ) {
		// test(i);
		cout << pi(1000000) << endl;
		// // cout << fe(100) << endl;
		conf->nextIteration();
	}

	return 0;
}
