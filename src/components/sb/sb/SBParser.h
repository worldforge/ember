#ifndef _SBPARSER_H_
#define _SBPARSER_H_

#include "SBTypes.h"
#include "components/sb/vhcl/vhcl.h"
#include "components/sb/external/parser/Params.h"

class InputTree;

namespace SmartBody {

class SBParseNode;

class SBParser
{
	public:
		SBAPI SBParser();
		SBAPI ~SBParser();

		SBAPI void initialize(const std::string& arg1, const std::string& arg2);
		SBAPI SmartBody::SBParseNode* parse(const std::string& input);
		SBAPI void cleanUp(SmartBody::SBParseNode* node);
		SBAPI bool isInitialized();

	protected:
		void createParseTree(InputTree* inputTree, SBParseNode* node);

		bool _initialized;
		Params params;
		bool release;
		string agent_id;

};

}

#endif