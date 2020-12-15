/****************************************************
Solver.cpp

Program:		Boggle
Author:			Ben Howenstein
Last Modified:	2/19/14

*****************************************************/

#include "Solver.hpp"
#include "Board.hpp"
#include "Trie.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace boggle;

namespace
{
	//	Quick and dirty utility function
	//
	inline std::string& toLower(std::string& str)
	{
		for_each(str.begin(), str.end(), [&str](char& c) { c = tolower(c); });
		return str;
	}
}

//	Performs a depth-first traversal of the game board
//
void Solver::solve()
{
	mSolution.clear();

	vector<SearchNode> searchNodes;
	searchNodes.reserve(mBoard->numSpaces());

	for(int row = 0; row < mBoard->size().numRows; ++row)
		for(int col = 0; col < mBoard->size().numCols; ++col)
		{
			VisitedList visited(mBoard->numSpaces(), false);
			traverseBoard(searchNodes, row, col);
		}
}

void Solver::traverseBoard(std::vector<SearchNode>& searchNodes, int row, int col)
{
	searchNodes.push_back(SearchNode(row, col, string(), VisitedList(mBoard->numSpaces(), false)));
	while(!searchNodes.empty())
	{
		SearchNode node = searchNodes.back();
		searchNodes.pop_back();
		
		node.mWord += mBoard->space(node.mRow, node.mCol);
		node.mVisited[mBoard->index(node.mRow, node.mCol)] = true;

		const Trie::Match wordMatch = mTrie->getMatch(node.mWord);
		if(wordMatch == Trie::WORD && node.mWord.size() >= 3)
			mSolution.insert(toLower(node.mWord));
		else if(wordMatch == Trie::NONE)
			continue;

		for(int rowDelta = -1; rowDelta <= 1; ++rowDelta)
		{
			for(int colDelta = -1; colDelta <= 1; ++colDelta)
			{
				if(rowDelta || colDelta)
				{
					const int newRow = node.mRow + rowDelta;
					const int newCol = node.mCol + colDelta;
					if(mBoard->inBounds(newRow, newCol) && !node.mVisited[mBoard->index(newRow, newCol)])
						searchNodes.push_back(SearchNode(newRow, newCol, node.mWord, node.mVisited));
				}
			}
		}
	}	
}

Solver::Solver(const char *boardPath, const shared_ptr<Trie> trie)
	:	mTrie(trie)
{
	//	NOTE: Ideally we'd use a custom implementation for more descriptive asserts
	//
	assert(boardPath);		
	ifstream ifs(boardPath);
	if(!ifs.good())
	{
		cerr << "Failed to open board file \'" << boardPath << "\'\n";
		return;
	}
	std::string boardStr((size_t)ifs.seekg(0, ios::end).tellg(), 0);
	ifs.seekg(ios::beg).read(&boardStr[0], boardStr.size());
	
	mBoard = unique_ptr<Board>(new Board(boardStr));
	
	//	NOTE: This is probably an atypical amount of work to be doing
	//	in a constructor; generating solutions on construction
	//	avoids the user having to construct and solve in two expressions.
	//	If performance is a problem we can delay generating the
	//	solution until after construction.
	//	 
	solve();
}

//	NOTE: trivial dtor is here to kill destruction of incomplete type
//	compiler warning for forward-declared Board class
//
Solver::~Solver() {}