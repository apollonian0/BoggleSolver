/****************************************************
Trie.hpp

Program:		Boggle
Author:			Ben Howenstein
Last Modified:	2/19/14

*****************************************************/

#ifndef BOGGLE_TRIE
#define BOGGLE_TRIE

#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <cassert>

namespace boggle
{
	class Trie
	{
	public:

		typedef enum Match_e { NONE, PARTIAL, WORD, } Match;

		//	Functions
		//
		bool insert(const std::string& word);
		Match getMatch(const std::string& word) const;

		//	Factories
		//
		static std::shared_ptr<Trie> Create(const char *dictPath);
		
	private:
		
		enum { ILLEGAL_CHAR = 127 };	//	Some unprintable we don't care about
		
		//	Types
		//
		struct Node
		{
			enum { LEAF = -1 };

			Node() : mValue(0) { mChildren.fill(0); }
			
			~Node()
			{ 
				std::for_each(mChildren.begin(), mChildren.end(),
					[](Node *child) { delete child; });
			}
			
			char mValue;
			std::array<Node*, 26> mChildren;
		};

		//	Utilities
		//
		int scrubIndex(char c) const;

		//	Enforce factory Create interface
		//
		Trie() {}

		//	Prevent copy construct/assignment
		//
		Trie(const Trie&);
		Trie& operator=(const Trie&);

		//	Variables
		//
		Node mRoot;
	};

	inline int Trie::scrubIndex(char c) const
	{
		assert(std::isalpha(c));
		return std::isalpha(c) ? std::tolower(c) - 'a' : ILLEGAL_CHAR;
	}

	inline bool Trie::insert(const std::string& word)
	{
		Node *curNode = &mRoot;
		std::string::const_iterator wordIt = word.begin();
		for(; wordIt != word.end(); ++wordIt)
		{
			const char index = scrubIndex(*wordIt);
			if(index == ILLEGAL_CHAR)
			{
				std::cerr << "\nError: illegal character \'" << *wordIt << "\' encountered in dictionary word \'" << word << "\'\n";
				return false;
			}
			if(!curNode->mChildren[index])
				curNode->mChildren[index] = new Trie::Node;
			curNode = curNode->mChildren[index];
		}
		curNode->mValue = Trie::Node::LEAF;
		return true;
	}

	inline Trie::Match Trie::getMatch(const std::string& word) const
	{
		const Node *curNode = &mRoot;
		std::string::const_iterator wordIt = word.begin();
		for(; wordIt != word.end(); ++wordIt)
		{
			const char index = scrubIndex(*wordIt);
			if(index == ILLEGAL_CHAR)
			{
				std::cerr << "Error: Illegal character \'" << *wordIt << "\' encountered in dictionary word \'" << word << "\'\n";
				return NONE;
			}
			if(!curNode->mChildren[index])
				return NONE;
			curNode = curNode->mChildren[index];
		}
		return curNode->mValue == Trie::Node::LEAF ? WORD : PARTIAL;
	}

	inline std::shared_ptr<Trie> Trie::Create(const char *dictPath)
	{
		std::shared_ptr<Trie> trie = std::shared_ptr<Trie>(new Trie());
		assert(trie);

		if(!trie)
		{
			std::cerr << "Error: failed to allocate an object of type \'Trie\'\n";
			return std::shared_ptr<Trie>();
		}

		std::ifstream ifs(dictPath);
		if(!ifs.good())
		{
			std::cerr << "Error: failed to open dictionary file \'" << dictPath << "\'n";
			return std::shared_ptr<Trie>();
		}

		std::string word;
		while(std::getline(ifs, word))
		{
			if(!trie->insert(word.c_str()))
				return std::shared_ptr<Trie>();
		}

		return trie;
	}
};

#endif