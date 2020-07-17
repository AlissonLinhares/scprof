#ifndef REAL_H
#define REAL_H

#include <iostream>
#include "config.h"

class real {
public:
	real(double rdata = 0.0,
		    int line = __builtin_LINE(),
		    const char *file = __builtin_FILE()) :
		    rdata(rdata), line(line), file(file) {
		this->mask = Config::getInstance()->getMask(file, line);
		this->adata = approx(rdata);
	}

	real(double rdata, double adata, uint64_t mask) :
		    rdata(rdata), adata(adata), line(__LINE__), file(__FILE__),
		    mask(mask) {
	}

	friend bool operator<(const real& l, const real& r) {
		bool result = l.rdata < r.rdata;
		if ((l.adata < r.adata) != result)
			Config::getInstance()->fatalError();
		return result;
	}

	friend bool operator>(const real& l, const real& r) {
		bool result = l.rdata > r.rdata;
		if ((l.adata > r.adata) != result)
			Config::getInstance()->fatalError();
		return result;
	}

	friend bool operator==(const real& l, const real& r) {
		bool result = l.rdata == r.rdata;
		if ((l.adata == r.adata) != result)
			Config::getInstance()->fatalError();
		return result;
	}

	friend bool operator!=(const real& l, const real& r) {
		bool result = l.rdata != r.rdata;
		if ((l.adata != r.adata) != result)
			Config::getInstance()->fatalError();
		return result;
	}

	friend bool operator>=(const real& l, const real& r) {
		bool result = l.rdata >= r.rdata;
		if ((l.adata >= r.adata) != result)
			Config::getInstance()->fatalError();
		return result;
	}

	friend bool operator<=(const real& l, const real& r) {
		bool result = l.rdata <= r.rdata;
		if ((l.adata <= r.adata) != result)
			Config::getInstance()->fatalError();
		return result;
	}

	// ########################################################################
	real operator=(const double& r) {
		this->rdata = r;
		this->adata = this->approx(r);
		this->updateError();
		return *this;
	}

	real operator=(const real& r) {
		this->rdata = r.rdata;
		this->adata = approx(r.adata);
		updateError();
		return *this;
	}

	friend real operator+(real l, const real& r) {
		l.rdata += r.rdata;
		l.adata += l.approx(r.adata);
		l.updateError();
		return l;
	}

	friend real operator+=(real &l, const real& r) {
		l.rdata += r.rdata;
		l.adata += l.approx(r.adata);
		l.updateError();
		return l;
	}

	friend real operator-(real l, const real& r) {
		l.rdata -= r.rdata;
		l.adata -= l.approx(r.adata);
		l.updateError();
		return l;
	}

	friend real operator-=(real &l, const real& r) {
		l.rdata -= r.rdata;
		l.adata -= l.approx(r.adata);
		l.updateError();
		return l;
	}

	friend real operator*(real l, const real& r) {
		l.rdata *= r.rdata;
		l.adata *= l.approx(r.adata);
		l.updateError();
		return l;
	}

	friend real operator*=(real &l, const real& r) {
		l.rdata *= r.rdata;
		l.adata *= l.approx(r.adata);
		l.updateError();
		return l;
	}

	friend real operator/(real l, const real& r) {
		l.rdata /= r.rdata;
		l.adata /= l.approx(r.adata);
		l.updateError();
		return l;
	}

	friend real operator/=(real &l, const real& r) {
		l.rdata /= r.rdata;
		l.adata /= l.approx(r.adata);
		l.updateError();
		return l;
	}


	// ########################################################################
	friend ostream &operator<<(ostream &output, const real& r) {
		output << r.rdata << "(" << r.adata << ")";
		return output;
	}

	friend istream &operator>>(istream &input, real& r) {
		input >> r.rdata;
		r.adata = r.approx(r.rdata);
		return input;
	}

	virtual ~real() {

	}

protected:
	void updateError() {
		updateError(this->rdata, this->adata);
	}

	void updateError(double r, double a) {
		if (r != 0)
			Config::getInstance()->updateError((r-a)/r, mask);
	}

	double approx(double value) {
		uint64_t result = (*((uint64_t*) &value)) & mask;
		return *((double*)(&result));
	}

private:
	double rdata;
	double adata;
	int line;
	const char* file;
	uint64_t mask;
};

#endif
