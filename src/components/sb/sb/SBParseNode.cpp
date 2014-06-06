#include "SBParseNode.h"

namespace SmartBody {

SBParseNode::SBParseNode()
{
	_word = "";
	_term = "";
}

SBParseNode::SBParseNode(const std::string& word, const std::string& term)
{
	_word = word;
	_term = term;
}

SBParseNode::~SBParseNode()
{
	for (size_t c = 0; c < _children.size(); c++)
	{
		delete _children[c];
	}	
}

bool SBParseNode::isTerminal()
{
	return _word == "";
}

void SBParseNode::setWord(const std::string& word)
{
	_word = word;
}

void SBParseNode::setTerm(const std::string& term)
{
	_term = term;
}

const std::string& SBParseNode::getWord()
{
	return _word;
}

const std::string& SBParseNode::getTerm()
{
	return _term;
}

void SBParseNode::addChild(SBParseNode* node)
{
	_children.push_back(node);
}

int SBParseNode::getNumChildren()
{
	return _children.size();
}

SBParseNode* SBParseNode::getChild(int num)
{
	if (num >= 0 && _children.size() > (size_t) num)
		return _children[num];
	else
		return NULL;
}

}
