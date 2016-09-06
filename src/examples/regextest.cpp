#include <iostream>
#include "PCRE2Plus.h"

using namespace std;
using namespace PCRE2Plus;

/** Example from pcre2plus library*/

int main(){
	string text = "5 monkeys of name 'Ulisses Raymond'";

//	std::string c = R"(\w+)";
//	std::string c = R"(^(\d+)\.(\d+))";


	std::string c = R"(^([+|-]{0,1}[\d]+) monkeys of name '([^']*)')";

	auto Regex = re::compile(c);
	if (Regex){
		//We don't support match, use search instead
		if (auto m = Regex->search(text)){
			auto v = m->groups();
			for (auto i = v.begin(); i < v.end(); i++){
				cout  << *i << " ";
			}
		}
		cout << endl;
	}
	else{
		std::cout<<"NOK"<<std::endl;
		std::cout<<re::getlasterror()<<std::endl;
		std::cout<<re::getlasterrorstr()<<std::endl;
	}
	return 0;
}
