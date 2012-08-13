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

#include "WorldDumper.h"

#include "MultiLineListFormatter.h"
#include "LoggingInstance.h"

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>

#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Encoder.h>
#include <Atlas/Objects/Operation.h>

#include <iostream>
#include <functional>
#include <cstdlib>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
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

WorldDumper::WorldDumper(Eris::Account& account) :
		mAccount(account), mCount(0), mCodec(0), mEncoder(0), mFormatter(0), mComplete(false), mCancelled(false)
{
}

WorldDumper::~WorldDumper()
{
	delete mEncoder;
	delete mFormatter;
	delete mCodec;
}

void WorldDumper::cancel()
{
	mCancelled = true;
}

void WorldDumper::dumpEntity(const RootEntity & ent)
{
	mEncoder->streamObjectsMessage(ent);
}

void WorldDumper::infoArrived(const Operation & op)
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

	if (mQueue.empty()) {
		mFormatter->streamEnd();
		mComplete = true;
		EventCompleted.emit();
		S_LOG_INFO("Completed dumping " << mCount << " entities.");
		return;
	}

	Get get;

	Anonymous get_arg;
	get_arg->setObjtype("obj");
	get_arg->setId(mQueue.front());
	get->setArgs1(get_arg);

	get->setFrom(mAccount.getId());
	get->setSerialno(Eris::getNewSerialno());

	mAccount.getConnection()->getResponder()->await(get->getSerialno(), this, &WorldDumper::operation);
	mAccount.getConnection()->send(get);

	mQueue.pop_front();
}

void WorldDumper::start(const std::string& filename)
{
	S_LOG_INFO("Starting world dump to file '" << filename << "'.");
	mFile.open(filename.c_str(), std::ios::out);

	Atlas::Message::QueuedDecoder * decoder = new Atlas::Message::QueuedDecoder;
	mCodec = new Atlas::Codecs::XML(mFile, *decoder);
	mFormatter = new MultiLineListFormatter(mFile, *mCodec);
	mEncoder = new Atlas::Objects::ObjectsEncoder(*mFormatter);

	mFormatter->streamBegin();

	// Send a get for the root object
	Get get;

	Anonymous get_arg;
	get_arg->setObjtype("obj");
	get_arg->setId("0");
	get->setArgs1(get_arg);

	get->setFrom(mAccount.getId());
	get->setSerialno(Eris::getNewSerialno());
	mAccount.getConnection()->getResponder()->await(get->getSerialno(), this, &WorldDumper::operation);
	mAccount.getConnection()->send(get);

}

void WorldDumper::operation(const Operation & op)
{
	if (!mCancelled) {
		if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
			infoArrived(op);
		}
	}
}
}
