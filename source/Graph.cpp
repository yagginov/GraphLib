#include "Graph.h"

graph::Graph::Graph(std::set<UINT> v, const UINT flag) : v(v)
{
	flags = 0b00;
	flags |= flag;
}

graph::Graph::Graph(const UINT flag)
{
	flags = 0b00;
	flags |= flag;
}

void graph::Graph::compile_adj()
{
	adj.clear();
	if (flags & UNORDERED_LINK)
	{
		for (const auto& it : e) {
			adj[it.from].insert({ it.to, it.length });
			adj[it.to].insert({ it.from, it.length });
		}
	}
	else
	{
		for (const auto& it : e) {
			adj[it.from].insert({ it.to, it.length });
		}
	}

	flags |= ADJ_COMPILED;
}

void graph::Graph::compile_table()
{
	if (!(flags & ADJ_COMPILED)) { compile_adj(); }

	table.clear();

	for (const auto& it : v)
	{
		for (const auto& jt : v)
		{
			table[it][jt] = 0;
		}
	}

	for (const auto& it : adj)
	{
		for (const auto& jt : it.second)
		{
			++table[it.first][jt.first];
		}
	}

	flags |= TABLE_COMPILED;
}

void graph::Graph::add_node(UINT node)
{
	v.insert(node);
	flags &= ~ADJ_COMPILED;
	flags &= ~TABLE_COMPILED;
}

void graph::Graph::add_link(Link link)
{
	e.insert(link);
	flags &= ~ADJ_COMPILED;
	flags &= ~TABLE_COMPILED;
}

UINT graph::Graph::size()
{
	return v.size();
}

void graph::Graph::algorithm_Dijkstra(const UINT from, std::map<UINT, float>& res)
{
	if (!(flags & ADJ_COMPILED)) { compile_adj(); }

	for (const auto& it : v)
	{
		res[it] = INFINITY;
	}
	res[from] = 0.0f;

	std::set <std::pair<float, UINT>> s;
	s.insert({ 0.0f, from });

	while (!s.empty())
	{
		auto i = *s.begin();
		float wt = i.first;
		UINT node = i.second;
		s.erase(i);

		for (const auto& it : adj[node])
		{
			float length = wt + it.second;
			if (length < res[it.first])
			{
				res[it.first] = length;
				s.insert({ length, it.first });
			}
		}
	}
}

void graph::Graph::search_all_lengths()
{
	for (const auto& it : v)
	{
		algorithm_Dijkstra(it, lengts[it]);
	}
}


void graph::Graph::print_lengths()
{
	std::cout << "          ";

	for (const auto& it : v)
	{
		std::cout << " " << std::setw(8) << std::right << it << " ";
	}

	std::cout << std::endl;

	for (const auto& it : lengts)
	{
		std::cout << " " << std::setw(8) << std::right << it.first << " ";
		for (const auto jt : it.second)
		{
			std::cout << " " << std::setw(8) << std::right << std::setprecision(8) <<
				((jt.second != 0) ? std::to_string(jt.second) : "-") << " ";
		}
		std::cout << std::endl;
	}
}

void graph::Graph::print_table()
{
	if (!(flags & TABLE_COMPILED)) { compile_table(); }

	std::cout << "     ";

	for (const auto& it : v)
	{
		std::cout << " " << std::setw(3) << std::right << it << " ";
	}

	std::cout << std::endl;

	for (const auto& it : table)
	{
		std::cout << " " << std::setw(3) << std::right << it.first << " ";
		for (const auto jt : it.second)
		{
			std::cout << " " << std::setw(3) << std::right <<
				((jt.second != 0) ? std::to_string(jt.second) : "-") << " ";
		}
		std::cout << std::endl;
	}

}


void graph::Graph::save_adjacency_matrix(std::string filename)
{
	if (!(flags & TABLE_COMPILED)) { compile_table(); }

	std::ofstream myfile(filename);

	if (myfile.is_open())
	{
		myfile << "     ";

		for (const auto& it : v)
		{
			myfile << " " << std::setw(3) << std::right << it << " ";
		}

		myfile << std::endl;

		for (const auto& it : table)
		{
			myfile << " " << std::setw(3) << std::right << it.first << " ";
			for (const auto jt : it.second)
			{
				myfile << " " << std::setw(3) << std::right <<
					((jt.second != 0) ? std::to_string(jt.second) : "-") << " ";
			}
			myfile << std::endl;
		}
	}
}
void graph::Graph::save_lengths_table(std::string filename)
{
	std::ofstream myfile(filename);

	if (myfile.is_open())
	{
		myfile << "          ";

		for (const auto& it : v)
		{
			myfile << " " << std::setw(8) << std::right << it << " ";
		}

		myfile << std::endl;

		for (const auto& it : lengts)
		{
			myfile << " " << std::setw(8) << std::right << it.first << " ";
			for (const auto jt : it.second)
			{
				myfile << " " << std::setw(8) << std::right << std::setprecision(5) <<
					((jt.second != 0) ? std::to_string(jt.second) : "-") << " ";
			}
			myfile << std::endl;
		}
	}
}