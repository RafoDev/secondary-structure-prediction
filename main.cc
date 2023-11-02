#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

typedef vector<vector<int>> matrix;

int score(char i, char j)
{
	if ((i == 'G' && j == 'C') || (i == 'C' && j == 'G'))
		return -1;
	if ((i == 'A' && j == 'U') || (i == 'U' && j == 'A'))
		return -1;
	return 0;
}

void printMatrix(matrix &e)
{
	for (auto i : e)
	{
		for (auto j : i)
			cout << j << " ";
		cout << '\n';
	}
}

int min4(int &a, int &b, int &c, int &d)
{
	return min(min(min(a, b), c), d);
}

void traceback(int i, int j, matrix &e, string &s, string &structure)
{
	if (i >= j)
		return;

	if (e[i][j] == e[i + 1][j])
	{
		traceback(i + 1, j, e, s, structure);
	}
	else if (e[i][j] == e[i][j - 1])
	{
		traceback(i, j - 1, e, s, structure);
	}
	else if (e[i][j] == e[i + 1][j - 1] + score(s[i], s[j]))
	{
		structure[i] = '(';
		structure[j] = ')';
		traceback(i + 1, j - 1, e, s, structure);
	}
	else
	{
		for (int k = i + 1; k < j - 1; ++k)
		{
			if (e[i][j] == e[i][k] + e[k + 1][j])
			{
				traceback(i, k, e, s, structure);
				traceback(k + 1, j, e, s, structure);
				return;
			}
		}
	}
}

void traceback_all(int i, int j, matrix &e, string &s, string &structure, set<string> &structures)
{
	if (i >= j)
	{
		if (i == j)
		{
			structures.insert(structure);
		}
		return;
	}

	if (e[i][j] == e[i + 1][j])
	{
		traceback_all(i + 1, j, e, s, structure, structures);
	}

	if (e[i][j] == e[i][j - 1])
	{
		traceback_all(i, j - 1, e, s, structure, structures);
	}

	if (j >= i + 1 && e[i][j] == (e[i + 1][j - 1] + score(s[i], s[j])))
	{
		structure[i] = '(';
		structure[j] = ')';
		traceback_all(i + 1, j - 1, e, s, structure, structures);
		structure[i] = '.';
		structure[j] = '.';
	}

	for (int k = i + 1; k <= j - 1; ++k)
	{
		if (e[i][j] == e[i][k] + e[k + 1][j])
		{
			traceback_all(i, k, e, s, structure, structures);
			traceback_all(k + 1, j, e, s, structure, structures);
		}
	}
}

void nussinov(string &s)
{
	int seqSize = s.size();

	matrix e;
	e.resize(seqSize, vector<int>(seqSize));

	cout << "[INFO] Initial Matrix\n";
	printMatrix(e);

	for (int d = 1; d <= seqSize - 1; d++)
	{
		for (int i = 0; i <= seqSize - 1 - d; i++)
		{
			int j = i + d;

			e[i][j] = min(e[i + 1][j], e[i][j - 1]);

			if (j >= i + 1)
			{
				int tmpScore = e[i + 1][j - 1] + score(s[i], s[j]);
				e[i][j] = min(e[i][j], tmpScore);
			}

			for (int k = i; k < j; k++)
			{
				int tmp = e[i][k] + e[k + 1][j];
				e[i][j] = min(e[i][j], tmp);
			}
		}
	}
	cout << '\n';
	cout << "[INFO] Filled Matrix\n";
	printMatrix(e);
	cout << '\n';


	string structure(seqSize, '.');
	set<string> structures;
	traceback(0, seqSize - 1, e, s, structure);
	structures.insert(structure);
	traceback_all(0, seqSize - 1, e, s, structure, structures);

	cout << "[INFO] Results("<<structures.size()<<")\n";

	for (const auto &str : structures)
	{
		for (auto i : s)
			cout << i << " ";
		cout << '\n';
		for (auto i : str)
			cout << i << " ";
		cout << '\n';
	}
}

int main()
{
	string s1,s2;
	s1 = "GGAAAUCC";
	s2 = "ACUCGAUUCCGAG";
	nussinov(s1);
	nussinov(s2);
}