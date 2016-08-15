#include "io.hpp"

namespace RedFox
{
	//Legge il testo di un file
	str ReadFileText(const str& _filename)
	{
		str text;

		FILE* stringingeam = fopen(_filename.c_str(), "rt");
		if (stringingeam != nullptr)
		{
			fseek(stringingeam, 0, SEEK_END);
			text.resize(ftell(stringingeam));
			fseek(stringingeam, 0, SEEK_SET);

			fread((void*)text.data(), 1, text.size(), stringingeam);
			fclose(stringingeam);
		}

		return text;
	}
}