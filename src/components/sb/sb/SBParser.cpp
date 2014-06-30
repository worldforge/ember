#include "SBParser.h"
#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <external/parser/GotIter.h>
#include <external/parser/Wrd.h>
#include <external/parser/InputTree.h>
#include <external/parser/Bchart.h>
#include <external/parser/ECArgs.h>
#include <external/parser/MeChart.h>
#include <external/parser/extraMain.h>
#include <external/parser/AnsHeap.h>
#include <external/parser/UnitRules.h>
#include <external/parser/Params.h>
#include <external/parser/TimeIt.h>
#include <external/parser/ewDciTokStrm.h>
#include <external/parser/Link.h>
#include <external/parser/InputTree.h>

#include <sb/SBParseNode.h>

#define log2(x) (log (x) / M_LN2)

MeChart* curChart;

namespace SmartBody {

SBParser::SBParser()
{
	_initialized = false;
}

SBParser::~SBParser()
{
}

void SBParser::initialize(const std::string& param1, const std::string& param2)
{
	int argc = 3;
	std::string arg0 = "";
	std::string arg1 = param1;
	std::string arg2 = param2;
	char** argv = new char*[3];
	argv[0] = (char*) arg0.c_str();
	argv[1] = (char*) arg1.c_str();
	argv[2] = (char*) arg2.c_str();

	LOG("starting Charniak parser..");

	ECArgs args( argc, argv );

	params.init( args );
	TimeIt timeIt;
	ECString  path( args.arg( 0 ) );
	generalInit(path);

	_initialized = true;
}


SmartBody::SBParseNode* SBParser::parse(const std::string& input)
{
	if (!_initialized)
	{
		LOG("Cannot parse until parser has been initialized.");
		return NULL;
	}

	std::string result;


	int totUnparsed = 0;
	double log600 = log2(600.0);



	LOG("parser ready");

	std::string string_to_parse = input;
	int      sentenceCount = 0;  //counts all sentences so we can use e.g,1/50;
	std::istringstream iss (istringstream::in);	
	istream* nontokStream = &iss;

	//SmartBody 
	std::ostringstream oss (ostringstream::out);

	ECString flnm = "dummy";
	ewDciTokStrm* tokStream = NULL;

	iss.str(string_to_parse);				

	SentRep* srp;
	//srp = new SentRep(*nontokStream, SentRep::SGML);			//ours is non-tokenized
	srp = new SentRep(iss, SentRep::SGML);			//ours is non-tokenized

	int len = srp->length();
	if(len > params.maxSentLen) 
		return NULL;
	if(len == 0) 
		return NULL;
	if( !params.field().in(sentenceCount) ) 
		return NULL;

	MeChart*	chart = new MeChart( *srp );
	curChart = chart;

	try {
		chart->parse( );
	} 
	catch(...)
	{ 
		LOG("Error: parsing error from natural language parser");
		return NULL;
	}	

	Item* topS = chart->topS();
	if(!topS)
	{
		totUnparsed++;
		if(!Bchart::silent)
		{
			cerr << "Parse failed" << endl;
			cerr << *srp << endl;
		}
		delete chart;
		return NULL;
	}

	// compute the outside probabilities on the items so that we can
	// skip doing detailed computations on the really bad ones 
	chart->set_Alphas();

	Bst& bst = chart->findMapParse();
	if( bst.empty())
	{
		totUnparsed++;
		if(!Bchart::silent)
		{
			cerr << "Parse failed" << endl;
			cerr << *srp << endl;
		}
		delete chart;
		return NULL;
	}

	int numVersions = 0;
	Link diffs(0);
	int numDiff = 0;
	std::vector<InputTree*> saveTrees;
	saveTrees.reserve(Bchart::Nth);
	vector<double> saveProbs;
	saveProbs.reserve(Bchart::Nth);
	for(numVersions = 0 ; ; numVersions++)
	{
		short pos = 0;
		Val* v = bst.next(numVersions);
		if(!v) break;
		InputTree* mapparse=inputTreeFromBsts(v,pos,*srp);
		bool isU;
		diffs.is_unique(mapparse, isU);
		//cerr << "V " << numVersions << " " << isU << " " << v->prob() << "\n" << *mapparse << endl;
		if(isU)
		{
			saveTrees.push_back(mapparse);
			saveProbs.push_back(v->prob());
			numDiff++;
		}
		else
		{
			delete mapparse;
		}
		if(numDiff >= Bchart::Nth) break;
	}
	ECString ind;
	if(srp->sentenceName.empty())
		ind = intToString(sentenceCount);
	else ind = srp->sentenceName;
	if(Bchart::Nth > 1)
		std::cout << numDiff << "\t" << ind <<"\n";

	SBParseNode* sbParseNode = NULL;
	for(int i = 0 ; i < numDiff ; i++)
	{
		short pos = 0;
		InputTree*  mapparse = saveTrees[i];
		

		double logP =log(saveProbs[i]);
		logP -= (srp->length()*log600);
		if(Bchart::Nth > 1)
			std::cout <<  logP << "\n";
		if(Bchart::prettyPrint) 
		{		
			oss << *mapparse << " oss\n";
		}
		else
		{
			mapparse->printproper(oss);				
		}					
		
		result = oss.str();	
	
		string_to_parse.clear();

		sbParseNode = new SBParseNode();
		createParseTree(mapparse, sbParseNode);

		delete mapparse;
		
	}
	std::cout << endl;
	oss.str("");

	delete chart;
	delete srp;
	iss.clear();

	return sbParseNode;
}

void SBParser::createParseTree(InputTree* inputTree, SBParseNode* node)
{
	if( inputTree->word().length() != 0 )
	{
		node->setWord(inputTree->word());
		node->setTerm(inputTree->term());
	}
	else
	{
		std::stringstream strstr;
		strstr << inputTree->term() << " " << inputTree->ntInfo();
		node->setTerm(strstr.str());

		ConstInputTreesIter subTreeIter = inputTree->subTrees().begin();
		InputTree* subTree = NULL;
		for( ; subTreeIter != inputTree->subTrees().end() ; subTreeIter++ )
		{
			subTree = *subTreeIter;
			SBParseNode* child = new SBParseNode();
			node->addChild(child);
			createParseTree(subTree, child);
		}
	}
}

void SBParser::cleanUp(SmartBody::SBParseNode* node)
{
	if (node)
		delete node;
}

bool SBParser::isInitialized()
{
	return _initialized;
}

}

