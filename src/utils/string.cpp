#include "string.hpp"

namespace RedFox
{
	vector<str> SplitLines(const str& _text)
	{
		vector<str> result;

		stringstream ss(_text);
		str item;
		while (getline(ss, item))
		{
			if(!item.empty())
				result.push_back(item);
		}

		return result;
	}
}