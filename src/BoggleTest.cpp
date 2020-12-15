/****************************************************
BoggleTest.cpp

Program:		Boggle
Author:			Ben Howenstein
Last Modified:	2/19/14

*****************************************************/

// Game Rules:
//  Boggle is a word game.  The goal is to make as many words as possible
//  out of the given set of letters laid out in a 4x4 grid.  Words are 
//  formed by starting with any letter and moving to an adjacent letter
//  (up, down, left, right, or diagonal) and so-forth on.  Once a letter
//  is used in a word, it can not be used again.  All words must be a 
//  minimum of 3 characters in length.  Finally, in this version, a "q"
//  will always represent "qu", since "u" nearly always follows "q" in
//  English.  If a word may be formed multiple ways on the same board, it
//  only counts once.
//
// Example:
//
// Board:
//    P W Y R
//    E N T H
//    G S I Q
//    O L S A
//
// A few possible words:
//   pen
//   peg
//   quit
//   hit
//   slit
//   slits
//
// Command line arguments:
//  BoggleTest.exe <dictionary_filename> <board_filename> <output_filename>
//
// Dictionary:
//  The dictionary file is an ASCII text file that lists acceptable words.  Each word is
//  new line separated.  Words are in alphabetical order and all lowercase, utilizing
//  only letters 'a' to 'z'.
//
// Board:
//  The board file is an ASCII text file that is 4 lines of 4 characters.  These
//  represent the game board, a 4x4 matrix of characters.  These may be mixed case.
//  Whitespace is optional and should be ignored.  Only letters 'a' to 'z' or 'A'
//  to 'Z' are used.
//
// Output:
//  The output should be an ASCII text file (in alphabetical order) of all legal words
//  possible to spell on the current board that are in the given dictionary.  These
//  should be all lowercase and newline separated (same format as the dictionary).
//

#include "Trie.hpp"
#include "Solver.hpp"
#include <fstream>
#include <iostream>
#include <crtdbg.h>

int main(const int argc, const char* const argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (argc != 4)
	{
		std::cerr << "Usage: BoggleTest <dictionary_filename> <board_filename> <output_filename>" << std::endl;
		return -1;
	}

	//	Fail early, fail often
	//
	{
		if(!std::ifstream(argv[1]).good())
		{
			std::cerr << "Error: failed to open dictionary file \'" << argv[1] << "\'n";
			return -1;
		}

		if(!std::ifstream(argv[2]).good())
		{
			std::cerr << "Error: failed to open board file \'" << argv[2] << "\'n";
			return -1;
		}
	}
	
	std::ofstream solution_ofs(argv[3]);
	if(!solution_ofs.good())
	{
		std::cerr << "Error: failed to open file \'" << argv[3] << "\' for writing\n";
		return -1;
	}
	
	//	Generate dictionary Trie structure
	//
	std::cout << "\n* Generating dictionary...\t(\'" << argv[1] << "\')";
	std::shared_ptr<boggle::Trie> trie = boggle::Trie::Create(argv[1]);
	if(!trie)
	{
		std::cerr << "Error: failed to generate dictionary\n";
		return -1;
	}

	//	Solve the Boggle board
	//
	std::cout << "\n* Solving Boggle board...\t(\'" << argv[2] << "\')";
	boggle::Solver solver(argv[2], trie);
	
	//	Write solution to filestream
	//
	solution_ofs << solver;

	//	Rejoice
	//
	std::cout << "\n\n* Huzzah! Solution written to file \'" << argv[3] << "\'\n\n";

	assert(_CrtCheckMemory());
	return 0;
}
