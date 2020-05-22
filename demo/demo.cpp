#include "systemc.h"
using namespace sc_dt;

void sc_int_test() {
	sc_fixed<8,8> a;
	sc_ufixed<8,8> b;
	sc_fixed_fast<8,8> c;
	sc_ufixed_fast<8,8> d;
	
	for (int i = 0; i < 255; i++) {
		a = i;
		b = i;
		c = i;
		d = i;
	}
}

void sc_fraq_test() {
	sc_fixed<8,0> a;
	sc_ufixed<8,0> b;
	sc_fixed_fast<8,0> c;
	sc_ufixed_fast<8,0> d;
	
	for (int i = 2; i <= 256; i *= 2) {
		float x = 1.0 / i;
		cout << x << endl;
		a = x;
		b = x;
		c = x;
		d = x;
	}
}

void sc_fraq2_test() {
	sc_fixed<8,-1> a;
	sc_ufixed<8,-1> b;
	sc_fixed_fast<8,-1> c;
	sc_ufixed_fast<8,-1> d;
	
	for (int i = 2; i <= 512; i *= 2) {
		float x = 1.0 / i;
		cout << x << endl;
		a = x;
		b = x;
		c = x;
		d = x;
	}
}

void regression_test() {
	sc_fixed<8,8> a = 0.0;
	a = -1;
	a = -2;
	a = -4;
	a = -8;
	a = 8;

	sc_fixed<8,8> b = 0.0;
	b = -1;
	b = -2;
	b = -4;
	b = 4;
	b = 8;

	sc_fixed<8,8> c = 0.0;
	c = -1;
	c = -2;
	c = -4;

	sc_fixed_fast<8,8> d = 0.0;
	d = -1;
	d = -2;
	d = -4;
	d = 1;
	d = 2;
	d = 4;

	sc_fixed_fast<8,8> e = 0.0;
	e = 1;
	e = 2;
	e = 4;
}

void array_test() {
	sc_fixed<8,2> a[4]; sc_fixed<4,2> b = 0.5;

	a[0] = 0.0;
	a[1] = 0.5;
	a[2] = 1.0;
	a[3] = 1.5;

	a[0] = a[0] + b;
	a[1] = a[1] + b;
	a[2] = a[2] + b;
	a[3] = a[3] + b;
}

void op_test() {
	sc_fixed<8,2> a = 0.0;
	double d = 0.125;
	cout << a.to_bin() << endl;
	a = a + d;
	cout << a.to_bin() << endl;
	a = a + d;
	cout << a.to_bin() << endl;
	a = a + d;
	cout << a.to_bin() << endl;
	a = a + d;
	cout << a.to_bin() << endl;
	a *= 2;
	cout << a.to_bin() << endl;
}

void op_aprox() {
	sc_fixed<8,2> a = 0.0;
	for ( int i = 0; i < 100; i++ )
		a = (double) i / 3.0;
}

void line_prof_test() {
	unsigned int x = 0.0;
	SCProf* prof = SCProf::getInstance();
	prof->profile(x = x + 4);
	prof->profile(++x);
	cout << x << endl;
}

int sc_main(int, char* []) {
	sc_int_test();
	sc_fraq_test();
	sc_fraq2_test();
	array_test();
	op_test();
	regression_test();
	line_prof_test();
	op_aprox();
	return 0;
}
