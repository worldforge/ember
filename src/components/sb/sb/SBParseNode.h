#ifndef _SBPARSETREE_H_
#define _SBPARSETREE_H_

#include <sb/SBTypes.h>
#include <string>
#include <vector>

namespace SmartBody {

class SBParseNode
{
	public:
		SBAPI SBParseNode();
		SBAPI SBParseNode(const std::string& word, const std::string& term);
		SBAPI ~SBParseNode();

		SBAPI bool isTerminal();
		SBAPI void setWord(const std::string& word);
		SBAPI void setTerm(const std::string& term);

		SBAPI const std::string& getWord();
		SBAPI const std::string& getTerm();

		SBAPI void addChild(SBParseNode* node);
		SBAPI int getNumChildren();
		SBAPI SBParseNode* getChild(int num);

	protected:
		std::string _word;
		std::string _term;
		std::vector<SBParseNode*> _children;

};

}


#endif