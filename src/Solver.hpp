/****************************************************
Solver.hpp

Program:		Boggle
Author:			Ben Howenstein
Last Modified:	2/19/14


*****************************************************/

#ifndef BOGGLE_SOLVER
#define BOGGLE_SOLVER

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

namespace boggle
{
	class Trie;
	class Board;

	class Solver
	{
	public:

		//	Accessors
		//
		const std::set<std::string>& solution() const { return mSolution; }

		//	Ctors
		//
		Solver(const char *boardPath, const std::shared_ptr<Trie> trie);

		//	Dtor
		//
		~Solver();

	private:

		struct SearchNode;
		typedef std::vector<bool> VisitedList;

		//	Functions
		//
		void solve();
		void traverseBoard(std::vector<SearchNode>& searchNodes, int row, int col);

		//	Copy construct/assign is outside intended semantics for usage
		//	 of Solver objects
		//
		Solver(const Solver&);
		Solver& operator=(const Solver&);

		//	Variables
		//
		std::set<std::string> mSolution;
		const std::shared_ptr<Trie> mTrie;

		//	Using a pointer lets us avoid including Board.hpp;
		//  in this context there's not much benefit, but in larger
		//	projects it worth being conservative when including
		//	non-std lib/stl headers.
		//	
		std::unique_ptr<Board> mBoard;
		
		//	Types
		//
		struct SearchNode
		{
			SearchNode(int row, int col, const std::string& word, const VisitedList& visited)
				:	mRow(row)
				,	mCol(col)
				,	mWord(word)
				,	mVisited(visited)
			{}

			int mRow, mCol;
			std::string mWord;
			VisitedList mVisited;
		};
	};
};

inline std::ostream& operator<<(std::ostream& os, const boggle::Solver& solver)
{
	std::for_each(solver.solution().begin(), solver.solution().end(),
		[&os](const std::string& str)
	{
		os << str << "\n";
	});
	return os;
}

#endif