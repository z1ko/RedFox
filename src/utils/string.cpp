#include "string.hpp"

namespace RedFox
{
	vector<str> split(const str& _text, char _delim)
	{
		vector<str> result;

		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}


		return result;
	}
}