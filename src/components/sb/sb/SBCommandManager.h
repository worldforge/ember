#ifndef _SBCOMMANDMANAGER_H_
#define _SBCOMMANDMANAGER_H_

#include <vhcl.h>
#include <sb/SBObject.h>
#include <sbm/sr_cmd_map.h>
#include <sbm/sr_cmd_seq.h>


class SequenceManager
{
	public:
		SBAPI SequenceManager();
		SBAPI ~SequenceManager();

		SBAPI bool addSequence(const std::string& seqName, srCmdSeq* seq);
		SBAPI bool removeSequence(const std::string& seqName, bool deleteSequence);
		SBAPI srCmdSeq* getSequence(const std::string& name);
		SBAPI srCmdSeq* getSequence(int num, std::string& name);
		SBAPI int getNumSequences();
		SBAPI void cleanupMarkedSequences();

		SBAPI void clear();

	protected:
		std::set<std::string> _sequenceSet;
		std::vector<std::pair<std::string, srCmdSeq*> > _sequences;
};


namespace SmartBody {

class SBCommandManager : public SBObject
{
	public:
		SBAPI SBCommandManager();
		SBAPI ~SBCommandManager();

		SBAPI void registerCallbacks();

		bool hasCommand(const std::string& command);

		SBAPI int execute( const char *key, srArgBuffer& args );
		SBAPI int execute( const char *key, char* strArgs );
		SBAPI int execute( char *cmd );
		SBAPI int execute_seq( srCmdSeq *seq );
		SBAPI int execute_seq( srCmdSeq *seq, const char* seq_name );
		SBAPI int execute_seq_chain( const std::vector<std::string>& seq_names, const char* error_prefix = NULL );
		SBAPI int execute_later( const char* command, float seconds );
		SBAPI int execute_later( const char* command );
		SBAPI srCmdSeq* lookup_seq( const char* );
		SBAPI int abortSequence( const char* name );
		SBAPI int deleteSequence( const char* name );

		SBAPI int insert_set_cmd( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp );
		SBAPI int insert_print_cmd( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp );
		SBAPI int insert_test_cmd( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp );
		SBAPI int insert( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp, char* description = NULL );

		SBAPI SequenceManager* getPendingSequences();
		SBAPI SequenceManager* getActiveSequences();

		/*! Executes a variable setting sub-command.   See mcuCBHandle::insert_set_cmd(..). */
		static int mcu_set_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
		/*! Executes a variable printing/data debugging sub-command.   See mcuCBHandle::insert_print_cmd(..). */
		static int mcu_print_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
		/*! Executes a test sub-command.   See mcuCBHandle::insert_test_cmd(..). */
		static int mcu_test_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );


	protected:

		SequenceManager	pendingSequences;
		SequenceManager activeSequences;

		srCmdMap<SBCommandManager>		cmd_map;
		srCmdMap<SBCommandManager>		set_cmd_map;
		srCmdMap<SBCommandManager>		print_cmd_map;
		srCmdMap<SBCommandManager>		test_cmd_map;
};

}

#endif