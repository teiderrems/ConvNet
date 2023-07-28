#ifndef ___IMAGE___
#define ___IMAGE___

#include <stdio.h>
#include <iostream>
#include<stdlib.h>
#include <vector>

#include <fstream>


using namespace std;
namespace cpr {

	typedef struct ImageGray {
		int height;
		int width;
		vector<vector< double >> p;
		int label=0;
	}ImageGray;

	typedef struct Pixel {
        /*double r;
        double g;
        double b;*/
		vector< double > pixel;
    }Pixel;

	/*typedef struct Weight {
		double r;
		double g;
		double b;
	}Weight;

	typedef struct Weights {
		vector<double> pixel;
	}Weights;*/

	typedef struct Image {
		int height;
		int width;
		vector<vector<Pixel>> p;
		int label=0;
		int nc = 3;
	}Image;
	/*typedef struct Image2 {
		int height;
		int width;
		vector<vector< Weight >> p;
	}Image2;*/

	/*typedef struct Filter {
		int dim;
		vector<vector< Weight >> w;
	}Filter;

	typedef struct Filters {
		int dim;
		int c;
		vector<vector< Weights >> w;
	}Filters;*/
	typedef struct Input {
		vector<double> x;
		int y;
	}Input;
}


#endif // !___IMAGE___
