#include <iostream>

#include "cppgherkin/cppgherkin.h"

using namespace std;
using namespace cppgherkin;

int main(int argc, char *argv[]){
	if(argc < 2){
		cout << "Usage: " << argv[0] << " <feature file>" << endl;
		return 1;
	}

	GherkinParser parser;
	Feature f = parser.parse(argv[1]);

	cout << f;

	return 0;
}
