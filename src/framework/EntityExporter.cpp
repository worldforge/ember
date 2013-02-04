//
// Copyright (C) 2009 Alistair Riddoch
// Copyright (C) 2012 Erik Ogenvik
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include "EntityExporter.h"

#include "LoggingInstance.h"
#include "Time.h"
#include "TinyXmlCodec.h"

#include "tinyxml/tinyxml.h"

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Eris/ServerInfo.h>
#include <Eris/TypeService.h>
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>

#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Encoder.h>
#include <Atlas/Objects/Operation.h>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Talk;
using Atlas::Objects::Operation::Get;

long integerId(const std::string & id)
{
	long intId = strtol(id.c_str(), 0, 10);
	if (intId == 0 && id != "0") {
		intId = -1L;
	}

	return intId;
}
namespace Ember
{

bool idSorter(const std::string& lhs, const std::string& rhs)
{
	return integerId(lhs) < integerId(rhs);
}

EntityExporter::EntityExporter(Eris::Account& account) :
		mAccount(account), mCount(0), mEntitiesCodec(0), mEntitiesEncoder(0), mEntitiesDecoder(0), mXmlDocument(0), mComplete(false), mCancelled(false), mOutstandingGetRequestCounter(0)
{
}

EntityExporter::~EntityExporter()
{
	delete mEntitiesEncoder;
	delete mEntitiesCodec;
	delete mEntitiesDecoder;
	delete mXmlDocument;
}

void EntityExporter::cancel()
{
	mCancelled = true;
}

void EntityExporter::dumpEntity(const RootEntity & ent)
{
	mEntitiesEncoder->streamObjectsMessage(ent);
}

void EntityExporter::dumpMind(const std::string& entityId, const Operation & op)
{
	Atlas::Message::MapType entityMap;
	entityMap["id"] = entityId;
	Atlas::Message::ListType thoughts;
	for (auto& thoughtOp : op->getArgs()) {
		if (*thoughtOp->getParents().begin() == "thought") {
			Atlas::Message::Element args = thoughtOp->getAttr("args");
			thoughts.push_back(args);
		}
	}
	entityMap["thoughts"] = thoughts;
	mMindsEncoder->streamMessageElement(entityMap);
}


void EntityExporter::thoughtInfoArrived(const Operation & op)
{
	S_LOG_VERBOSE("Got thought info when dumping.");

	std::map<int, std::string>::const_iterator I = mThoughtsOutstanding.find(op->getRefno());
	if (I == mThoughtsOutstanding.end()) {
		S_LOG_WARNING("Got unrecognized thought info.");
		return;
	}
	const std::string entityId = I->second;
	mOutstandingGetRequestCounter--;

	dumpMind(entityId, op);

	if (mQueue.empty() && mOutstandingGetRequestCounter == 0) {
		complete();
		return;
	}
}

void EntityExporter::infoArrived(const Operation & op)
{
	if (op->isDefaultRefno()) {
		S_LOG_WARNING("Got op not belonging to us when dumping.");
		return;
	}
	const std::vector<Root> & args = op->getArgs();
	if (args.empty()) {
		return;
	}
	RootEntity ent = smart_dynamic_cast<RootEntity>(args.front());
	if (!ent.isValid()) {
		S_LOG_WARNING("Malformed OURS when dumping.");
		return;
	}
	S_LOG_VERBOSE("Got info when dumping.");
	mOutstandingGetRequestCounter--;
	++mCount;
	EventProgress.emit(mCount);
	//Make a copy so that we can sort the contains list and update it in the
	//entity
	RootEntity entityCopy(ent->copy());
	std::list<std::string> contains = ent->getContains();
	//Sort the contains list so it's deterministic
	contains.sort(idSorter);
	entityCopy->setContains(contains);
	dumpEntity(entityCopy);
	std::list<std::string>::const_iterator I = contains.begin();
	std::list<std::string>::const_iterator Iend = contains.end();
	for (; I != Iend; ++I) {
		mQueue.push_back(*I);
	}

	Eris::TypeInfo* characterTypeInfo = mAccount.getConnection()->getTypeService()->getTypeByName("character");
	Eris::TypeInfo* entityType = mAccount.getConnection()->getTypeService()->getTypeByName(ent->getParentsAsList().begin()->asString());
	if (entityType->isA(characterTypeInfo)) {
		Get get;

		Anonymous get_arg;
		get_arg->setObjtype("thought");
		get_arg->setId(ent->getId());

		get->setArgs1(get_arg);
		get->setFrom(mAccount.getId());
		get->setSerialno(Eris::getNewSerialno());

		mAccount.getConnection()->getResponder()->await(get->getSerialno(), this, &EntityExporter::operationGetThoughtResult);
		mAccount.getConnection()->send(get);
		mThoughtsOutstanding.insert(std::make_pair(get->getSerialno(), ent->getId()));
		mOutstandingGetRequestCounter++;


	}

	if (mQueue.empty() && mOutstandingGetRequestCounter == 0) {
		complete();
		return;
	}

	//Make sure that no more than 5 outstanding get requests are currently sent to the server.
	//The main reason for us not wanting more is that we then run the risk of overflowing the server connection (which will then be dropped).
	while (mOutstandingGetRequestCounter < 5 && !mQueue.empty()) {
		Get get;

		Anonymous get_arg;
		get_arg->setObjtype("obj");
		get_arg->setId(mQueue.front());

		get->setArgs1(get_arg);
		get->setFrom(mAccount.getId());
		get->setSerialno(Eris::getNewSerialno());

		mAccount.getConnection()->getResponder()->await(get->getSerialno(), this, &EntityExporter::operationGetResult);
		mAccount.getConnection()->send(get);

		mOutstandingGetRequestCounter++;

		mQueue.pop_front();
	}
}

void EntityExporter::complete()
{
	mEntitiesEncoder->streamEnd();
	mMindsEncoder->streamEnd();

	mXmlDocument->SaveFile();
	mComplete = true;
	EventCompleted.emit();
	S_LOG_INFO("Completed dumping " << mCount << " entities.");
}

void EntityExporter::start(const std::string& filename, const std::string& entityId)
{
	S_LOG_INFO("Starting world dump to file '" << filename << "'.");

	mXmlDocument = new TiXmlDocument(filename);
	TiXmlNode* root = mXmlDocument->InsertEndChild(TiXmlElement("entityexport"));
	TiXmlElement name("name");
	root->InsertEndChild(name);
	TiXmlElement description("description");
	root->InsertEndChild(description);
	TiXmlElement timestamp("timestamp");
	std::stringstream ss;
	ss << Time::currentTimeMillis();
	timestamp.InsertEndChild(TiXmlText(ss.str()));
	root->InsertEndChild(timestamp);
	TiXmlElement server("server");
	{
		Eris::ServerInfo serverInfo;
		mAccount.getConnection()->getServerInfo(serverInfo);

		TiXmlElement host("host");
		host.InsertEndChild(TiXmlText(mAccount.getConnection()->getHost()));
		server.InsertEndChild(host);
		TiXmlElement serverName("name");
		serverName.InsertEndChild(TiXmlText(serverInfo.getServername()));
		server.InsertEndChild(serverName);
		TiXmlElement ruleset("ruleset");
		ruleset.InsertEndChild(TiXmlText(serverInfo.getRuleset()));
		server.InsertEndChild(ruleset);
	}
	root->InsertEndChild(server);

	TiXmlNode* entitiesNode = root->InsertEndChild(TiXmlElement("entities"));
	TiXmlNode* mindsNode = root->InsertEndChild(TiXmlElement("minds"));

	mEntitiesDecoder = new Atlas::Message::QueuedDecoder;
	mEntitiesCodec = new TinyXmlCodec(*entitiesNode, *mEntitiesDecoder);
	mEntitiesEncoder = new Atlas::Objects::ObjectsEncoder(*mEntitiesCodec);

	mEntitiesEncoder->streamBegin();


	mMindsDecoder = new Atlas::Message::QueuedDecoder;
	mMindsCodec = new TinyXmlCodec(*mindsNode, *mMindsDecoder);
	mMindsEncoder = new Atlas::Message::Encoder(*mMindsCodec);

	mMindsEncoder->streamBegin();

	// Send a get for the requested object
	mOutstandingGetRequestCounter++;
	Get get;

	Anonymous get_arg;
	get_arg->setObjtype("obj");
	get_arg->setId(entityId);
	get->setArgs1(get_arg);

	get->setFrom(mAccount.getId());
	get->setSerialno(Eris::getNewSerialno());
	mAccount.getConnection()->getResponder()->await(get->getSerialno(), this, &EntityExporter::operationGetResult);
	mAccount.getConnection()->send(get);

}

void EntityExporter::operationGetResult(const Operation & op)
{
	if (!mCancelled) {
		if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
			infoArrived(op);
		}
	}
}

void EntityExporter::operationGetThoughtResult(const Operation & op)
{
	if (!mCancelled) {
		if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
			thoughtInfoArrived(op);
		}
	}
}
}
