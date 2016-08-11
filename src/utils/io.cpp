#include "io.hpp"

namespace RedFox
{
	//Legge il testo di un file
	string ReadFileText(const string& _filename)
	{
		string text;

		FILE* stream = fopen(_filename.c_str(), "rt");
		if (stream != nullptr)
		{
			fseek(stream, 0, SEEK_END);
			text.resize(ftell(stream));
			fseek(stream, 0, SEEK_SET);

			fread((void*)text.data(), 1, text.size(), stream);
			fclose(stream);
		}

		return text;
	}
}