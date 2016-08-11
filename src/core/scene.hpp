#pragma once

#include "../common.hpp"

namespace RedFox
{
	class Node
	{
		public:
			Node(const string& _name);

			//Ricerca ricorsiva
			Node* findChild(const string& _name);

		private:
			vector<Node> m_children;
			string m_name;
			u32 m_id;
	};
}