#ifndef _SBBMLPROCESSOR_H
#define _SBBMLPROCESSOR_H

#include <sb/SBTypes.h>
#include <string>
#include <ostream>

namespace BML {
	class Processor;
}

namespace SmartBody {

class SBBmlProcessor
{
	public:
		SBAPI SBBmlProcessor();
		SBAPI ~SBBmlProcessor();

		SBAPI void vrSpeak(std::string agent, std::string recip, std::string msgId, std::string msg);
		SBAPI void vrAgentBML(std::string op, std::string agent, std::string msgId, std::string msg);
		
		SBAPI std::string execBML(std::string character, std::string bml);
		SBAPI std::string execBMLFile(std::string character, std::string filename);
		SBAPI std::string execXML(std::string character, std::string xml);
		SBAPI void execBMLAt(double time, std::string character, std::string bml);
		SBAPI void execBMLFileAt(double time, std::string character, std::string filename);
		SBAPI void execXMLAt(double time, std::string character, std::string xml);

		SBAPI void interruptCharacter(const std::string& character, double seconds);
		SBAPI void interruptBML(const std::string& character, const std::string& id, double seconds);

		SBAPI BML::Processor* getBMLProcessor();

	protected:
		std::string build_vrX(std::ostringstream& buffer, const std::string& cmd, const std::string& char_id, const std::string& recip_id, const std::string& content, bool for_seq );
		std::string send_vrX( const char* cmd, const std::string& char_id, const std::string& recip_id,
			const std::string& seq_id, bool echo, bool send, const std::string& bml );

		BML::Processor* _bmlProcessor;
};

}

#endif