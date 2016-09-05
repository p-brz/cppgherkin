#include <iostream>
#include <set>
#include <functional>
#include <sstream>

#include <type_traits>
#include <tuple>

#include "cppgherkin/cppgherkin.h"

using namespace std;
using namespace cppgherkin;


STEP("{int} monkeys of name {text}", int a, string b){
	printf("%d monkeys of name '%s'\n", a, b.c_str());
}

void executeStep(const std::string & step){
	cout << "Execute step: \"" << step << "\": " << endl;
	StepsStorage::instance().executeStep(step);
	cout << endl;
}

int main(){
	StepsStorage::instance().registerStep<void, string, int>(string("greeting {text} {int} times"),
	[](string person, int num){
		for(int i=0; i < num; ++i){
			cout << "Hello, " << person << "!" << endl;
		}
	});

	cout << "Steps: " << endl;
	for(auto iter: StepsStorage::instance().steps){
		cout << "step: " << iter.first << endl;
	}
	cout << endl;

	executeStep("5 monkeys of name 'hermano'");
	executeStep("11 monkeys of name 'Fulano de tal'");
	executeStep("greeting 'world' 10 times");


    return 0;
}
