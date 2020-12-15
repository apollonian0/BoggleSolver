/****************************************************
Board.hpp

Program:		Boggle
Author:			Ben Howenstein
Last Modified:	2/19/14

*****************************************************/

#ifndef BOGGLE_BOARD
#define BOGGLE_BOARD

#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <cassert>

namespace boggle
{
	class Board
	{
	public:

		struct Size;

		//	NOTE: index bounds-checking policy is
		//	assert in debug, clamp in release
		//

		//	Accessors
		//
		Size size() const						{ return mSize; }
		int numSpaces() const					{ return mSize.numRows * mSize.numCols; }
		int index(int row, int col) const		{ return mSize.numCols * clampRow(row) + clampCol(col); }
		char space(int row, int col) const		{ return mData[index(row, col)]; }
		bool inBounds(int row, int col) const;

		//	Ctors
		//
		Board(const std::string& boardStr);

		//	Types
		//
		struct Size
		{ 
			int numRows, numCols;
			Size() : numRows(0), numCols(0) {}
		};

	private:

		//	Utilities
		//
		int clampRow(int row) const;
		int clampCol(int col) const;

		//	Variables
		//
		Size mSize;
		std::string mData;
	};

	inline int Board::clampRow(int row) const
	{
		assert(row >= 0 && row < mSize.numCols);
		return (row < 0 ? 0 : (row >= mSize.numCols ? mSize.numCols - 1 : row));
	}
	
	inline int Board::clampCol(int col) const
	{
		assert(col >= 0 && col < mSize.numCols);
		return (col < 0 ? 0 : (col >= mSize.numCols ? mSize.numCols - 1 : col));
	}

	inline bool Board::inBounds(int row, int col) const
	{ 
		return row >= 0 && row < size().numRows &&
			   col >= 0 && col < size().numCols;
	}

	inline Board::Board(const std::string& boardStr)
	{
		std::back_insert_iterator<std::string> dataIt(mData);
		std::stringstream board(boardStr);
		for(std::string line; std::getline(board, line);)
		{
			
			++mSize.numRows;
			std::stringstream row(line);
			for(char c = 0; (row >> std::skipws >> c);) { *dataIt++ = c; }
		}
		mSize.numCols = mSize.numRows > 0 ? mData.size() / mSize.numRows : 0;
	}
}

//	Unused debugging utility, no reason to remove
//
inline std::ostream& operator<<(std::ostream& os, const boggle::Board& board)
{
	for(int row = 0; row < board.size().numRows; ++row)
	{
		for(int col = 0; col < board.size().numCols; ++col)
			os << board.space(row, col) << " ";
		os << "\n";
	}
	return os;
}

#endif