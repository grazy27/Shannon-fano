#include< stdio.h>
#include< conio.h>

#include <map>
#include <vector>
#include <tuple>
#include <functional>
#include <iomanip>
#include <iostream>
#include <algorithm>
using namespace std;


void CalculateWords(const vector<pair<char, float>>&, size_t from, size_t to);
void CalcSymbols(); 
void CodeString();
void PrintResult();
void CalcBenefit();

map <char, tuple< int, float, vector<bool>*>> symbols; //symbol as a key, int - num of symbols in the text, float - propability, vector : bit array
string text = "The Red Room "
"We did not take a walk that day because of the rain and the cold winter wind.I was glad of that.I never liked long walks on chilly afternoons. "
"After dinner the three Reed children – Eliza, John and Georgiana – sat with their mother in the drawing - room.I slipped away into the room next door and found myself a book, a big, heavy one, all about British birds.I curled up in the window "
"– seat and drew the curtains close behind me. "
"I read happily for some time.And then I heard the door open. "
" \"Boo!\" cried the voice of John Reed.";

void main()
{
	CalcSymbols();
	CodeString();
	PrintResult();

	for (auto i : symbols)
	{
		delete get<2>(i.second);
	}
	_getch();
}
void CalcBenefit()
{
	unsigned long compressedSize = 0;
	for (auto i : symbols)
		compressedSize += get<0>(i.second) * get<2>(i.second)->size();

	cout << "Uncompressed size: " << text.length() * 8 << endl << " Compressed size: " << compressedSize;

}
void PrintResult()
{
	cout << "char    numOfchars    propability    coded sequence" << endl;
	for (auto i : symbols)
	{
		cout << setw(4) << i.first << "    " << get<0>(i.second) << "              " << get<1>(i.second) << "     ";
		for_each(get<2>(i.second)->begin(), get<2>(i.second)->end(), [](auto bit) {cout << bit; });
		cout << endl;
	}
	CalcBenefit();
}
void CalculateWords(const vector<pair<char, float>>& charPossibilities, size_t from, size_t to)
{
	if (!to) to = charPossibilities.size();

	float summPropability = 0.0, curSum = 0.0;

	for (size_t i = from; i < to; i++)
		summPropability += charPossibilities[i].second;

	for (size_t i = from; i < to; i++)
	{
		curSum += get<1>(charPossibilities[i]);
		if (((curSum < summPropability / 2.0) && (i < (to - 2))) || i == from)
			get<2>(symbols[charPossibilities[i].first])->push_back(1);
		else
		{
			
			for (size_t k = i; k < to; k++)
				get<2>(symbols[charPossibilities[k].first])->push_back(0);

			if (from < i - 1)
				CalculateWords(charPossibilities, from, i);
			if (i < to-1)
				CalculateWords(charPossibilities, i, to);

			return;
		}

	}

}
void CodeString()
{
	vector<pair<char, float>> charPossibilities;
	uint32_t charsSum = 0;

	for (auto i : symbols)
		charsSum += get<0>(i.second);

	for (auto i : symbols)
	{
		float probability = (float)get<0>(i.second) / (float)charsSum;
		charPossibilities.push_back(make_pair(i.first, probability));
		get<1>(symbols[i.first]) = probability;

	}

	sort(charPossibilities.begin(), charPossibilities.end(), [](auto& a, auto& b) {return a.second > b.second; });

	CalculateWords(charPossibilities, 0, 0);
}
void CalcSymbols()
{
	for (char& c : text) {
		if (symbols.find(c) != symbols.end())
		{
			get<0>(symbols[c]) += 1;
		}
		else
		{
			symbols.insert(make_pair(c, make_tuple(1, 0, new vector<bool>)));
		}
	}



}