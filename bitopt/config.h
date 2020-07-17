#include <iomanip>
#include <bitset>
#include <limits>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <algorithm>
#include <vector>

#ifndef CONFIG_H
#define CONFIG_H

using namespace std;

class Config {
private:
	Config(int seeds = 20, double error = 0.01) : it(0), seeds(seeds), genetic(vector<tGenetic>(seeds)), minError(error) {
		srand(time(NULL));
	}

private:
	struct tGenetic {
		double bits;
		double error;
		map<pair<string,int>,uint64_t> dna;

		tGenetic() : bits(0.0), error(0.0) {

		}

		void clear() {
			bits = 0.0;
			error = 0.0;
		}
	};

	static bool tGeneticComparator(tGenetic& a, tGenetic& b) {
		return (a.error < b.error) && (a.bits < b.bits);
	}

public:
	static Config* getInstance() {
		static Config* instance = 0;

		if (instance == 0)
			instance = new Config();

		return instance;
	}

	uint64_t getMask(const char* file, int line) {
		pair<string,int> key = make_pair(file,line);

		if (genetic[it].dna.count(key) == 0) {
			uint64_t mask = -1;
			mask = mask << (63 - (rand() % 64));
			genetic[it].dna[key] = mask;
			return mask;
		}

		return genetic[it].dna[key];
	}

	void setMinError(double error) {
		this->minError = error;
	}

	void setNumberOfSeeds(int seeds) {
		this->seeds = seeds;
		this->genetic.resize(seeds);
	}

	void updateError(double error, uint64_t mask) {
		genetic[it].error = max(genetic[it].error, error);

		if (genetic[it].bits == 0.0)
			genetic[it].bits = __builtin_popcountll(mask);
		else
			genetic[it].bits = (genetic[it].bits + __builtin_popcountll(mask)) / 2.0;
	}

	void fatalError() {
		updateError(numeric_limits<double>::max(), 100000000);
	}

	void nextIteration() {
		if (it < seeds - 1) {
			it++;
		} else {
			it = 0;
			selection();
			printBestConfig();
			crossover();
		}
	}

	void printBestConfig() {
		cout << "################ Best result ##############" << endl;
		for (pair<pair<string,int>, uint64_t> e : genetic[0].dna)
			cout << e.first.first << ":" << e.first.second << " -> " << bitset<64>(e.second).count() << endl;
		cout << "Bits used:" << genetic[0].bits << endl;
		cout << "Max Relative Error:" << genetic[0].error << endl;
		cout << "###########################################" << endl;
	}

	~Config() {

	}

protected:
	void selection() {
		auto tGeneticComparator = [&](tGenetic& a,tGenetic& b)-> bool {
			if ((a.error > minError) || (b.error > minError))
				return a.error < b.error;
			return a.bits < b.bits;
		};

		sort(genetic.begin(), genetic.end(), tGeneticComparator);
	}

	void crossover() {
		int survivors = seeds/4;
		
		for (int c = survivors; c < seeds; c++) {
			int i = rand() % (survivors);
			int j = rand() % (survivors);

			map<pair<string,int>,uint64_t>& dna = genetic[i].dna;

			for (pair<pair<string,int>, uint64_t> e : dna) {
				int noise = rand() % 100;

				if (noise > 75) {
					genetic[c].dna[e.first] = genetic[j].dna[e.first] & dna[e.first];
				} else {
					uint64_t m = -1;
					genetic[c].dna[e.first] = m << (63 - (rand() % 64));
				}

				genetic[c].clear();
			}
		}
	}

private:
	int it, seeds;
	vector<tGenetic> genetic;
	double minError;
};

#endif
