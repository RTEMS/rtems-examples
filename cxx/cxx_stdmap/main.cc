#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
 
int main() {
 
  // Creating & Initializing a map of String & Ints
  std::map<std::string, int> mapOfWordCount = { { "aaa", 10 }, { "ddd", 41 },
  		{ "bbb", 62 }, { "ccc", 13 } };
 
  // Declaring the type of Predicate that accepts 2 pairs and return a bool
  typedef std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)> Comparator;
 
  // Defining a lambda function to compare two pairs. It will compare two pairs using second field
  Comparator compFunctor =
  		[](std::pair<std::string, int> elem1 ,std::pair<std::string, int> elem2)
  		{
  			return elem1.second < elem2.second;
  		};
 
  // Declaring a set that will store the pairs using above comparision logic
  std::set<std::pair<std::string, int>, Comparator> setOfWords(
  		mapOfWordCount.begin(), mapOfWordCount.end(), compFunctor);
 
  // Iterate over a set using range base for loop
  // It will display the items in sorted order of values
  for (std::pair<std::string, int> element : setOfWords)
  	std::cout << element.first << " :: " << element.second << std::endl;
 
  return 0;
}
