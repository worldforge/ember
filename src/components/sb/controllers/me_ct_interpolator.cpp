/*
 *  me_ct_interpolator.h - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Yuyu Xu, USC
 */

#include <controllers/me_ct_interpolator.h>
#include <controllers/me_ct_param_animation.h>
#include <sb/SBAnimationStateManager.h>
#include <sb/SBAnimationState.h>
#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>

std::string MeCtInterpolator::Context::CONTEXT_TYPE = "MeCtInterpolator::Context";
std::string MeCtInterpolator::CONTROLLER_TYPE = "MeCtInterpolator";

void MeCtInterpolator::Context::child_channels_updated( MeController* child )
{
}

MeCtInterpolator::MeCtInterpolator(MeController* c1, MeController* c2, double time, double w, bool l, std::string controllerName) : MeCtContainer(new MeCtInterpolator::Context(this)), child1(c1), child2(c2)
{
	// controller initialization
	if (child1)
	{
		_sub_context->add_controller(child1);
		child1->ref();
	}
	if (child2)
	{
		_sub_context->add_controller(child2);
		child2->ref();
	}
	channels.init();
	channels.merge(child1->controller_channels());
	channels.merge(child2->controller_channels());

	// other data
	loop = l;
	newLoop = true;
	numLoops = 0;
	newCycle1 = false;
	newCycle2 = false;

	reverseWeight = false;
	
	startTime = time;
	weight = w;

	this->setName(controllerName.c_str());

	initKeys();
	initDuration();
}

MeCtInterpolator::~MeCtInterpolator()
{
	if (child1)
	{
		_sub_context->remove_controller(child1);
		child1->unref();
		child1 = NULL;
	}
	if (child2)
	{
		_sub_context->remove_controller(child2);
		child2->unref();
		child2 = NULL;
	}
}

MeController* MeCtInterpolator::child(size_t n)
{
	if (n == 0) return child1;
	else if (n == 1) return child2;
	else
	{
		LOG("MeCtInterpolator Error: No accessable Controller!");
		return NULL;
	}
}

int MeCtInterpolator::child(std::string name)
{
	MeCtMotion* motionCt1 = dynamic_cast<MeCtMotion*> (child1);
	MeCtMotion* motionCt2 = dynamic_cast<MeCtMotion*> (child2);
	if (motionCt1->motion()->getName() == name)
		return 0;
	if (motionCt2->motion()->getName() == name)
		return 1;
	return -1;

}

double MeCtInterpolator::getWeight()
{
	return weight;
}

void MeCtInterpolator::setWeight(float w)
{
	adjustStartTime(weight, w);
	weight = w;
	initDuration();
}

bool MeCtInterpolator::getLoop()
{
	return loop;
}

void MeCtInterpolator::setLoop(bool l)
{
	loop = l;
}

bool MeCtInterpolator::getNewLoop()
{
	return newLoop;
}

void MeCtInterpolator::setNewLoop(bool l)
{
	newLoop = l;
}

bool MeCtInterpolator::getNewCycle(int index)
{
	if (index == 1)	return newCycle1;
	if (index == 2) return newCycle2;
	return false;
}

std::vector<double>& MeCtInterpolator::getKey(int index)
{
	if (index == 1)	return key1;
	if (index == 2) return key2;
	return key1;
}

double MeCtInterpolator::phaseDuration()
{
	return duration;
}

void MeCtInterpolator::initKeys()
{
	
	MeCtMotion* motionCt1 = dynamic_cast<MeCtMotion*> (child1);
	MeCtMotion* motionCt2 = dynamic_cast<MeCtMotion*> (child2);
	SkMotion* motion1 = motionCt1->motion();
	SkMotion* motion2 = motionCt2->motion();
	
	key1.clear();
	key2.clear();

	SmartBody::SBAnimationBlend* state = SmartBody::SBScene::getScene()->getBlendManager()->getBlend(this->getName());
	if (state != NULL)
	{
		for (int i = 0; i < state->getNumMotions(); i++)
		{
			if (state->motions[i]->getName() == motion1->getName())
				key1 = state->keys[i];
			if (state->motions[i]->getName() == motion2->getName())
				key2 = state->keys[i];
		}
	}

	// check key map validation
	if (key1.size() != key2.size() || key1.size() == 1 || key2.size() == 1)
	{
		key1.clear();
		key2.clear();
		LOG(" MeCtInterpolator::initKeys() : Key map mismatching !!!");
	}

	if (key1.empty() && key2.empty())
	{
		key1.push_back(0.0);
		key2.push_back(0.0);
		key1.push_back(motion1->duration());
		key2.push_back(motion2->duration());
	}

	// if keys are not in order, should add on a cycle
	for (size_t i = 0; i < key1.size() - 1; i++)
	{
		if (key1[i] > key1[i + 1])
		{
			for (size_t j = i + 1; j < key1.size(); j++)
				key1[j] += motion1->duration();
		}
		if (key2[i] > key2[i + 1])
		{
			for (size_t j = i + 1; j < key1.size(); j++)
				key2[j] += motion2->duration();
		}
	}
}


double MeCtInterpolator::getDuration(int index)
{
	if (index == 1)
		return (key1[key1.size() - 1] - key1[0]);

	if (index == 2)
		return (key2[key2.size() - 1] - key2[0]);

	return 0.0;
}

void MeCtInterpolator::initDuration()
{
	if (key1.empty() || key2.empty())
		LOG(" MeCtInterpolator::initDuration() : Key map not initialized.");
	
	double dur1 = key1[key1.size() - 1] - key1[0];
	double dur2 = key2[key2.size() - 1] - key2[0];

	duration = dur1 * weight + dur2 * (1 - weight);
}

void MeCtInterpolator::updateChildren(int index, MeController* newController)
{
	MeController* anotherChild = this->child(1 - index);
	MeController* replacedChild = this->child(index);

	_sub_context->remove_controller(replacedChild);
	replacedChild->unref();
	_sub_context->add_controller(newController);
	newController->ref();

	channels.init();
	channels.merge(newController->controller_channels());
	channels.merge(anotherChild->controller_channels());
	
	if (index == 0)
		child1 = newController;
	else
		child2 = newController;

	controller_map_updated();

	initKeys();
	initDuration();
}


void MeCtInterpolator::getTiming(double t, double& t1, double& t2)
{
	std::vector<double> key;
	double offset = key1[0] * weight + key2[0] * (1 - weight);
	int numKeys = key1.size();
	for (int i = 0; i < numKeys; i++)
	{
		double k = key1[i] * weight + key2[i] * (1 - weight);
		k = k - offset;
		key.push_back(k);
	}

	bool err = true;
	int section;
	for (int i = 0; i < numKeys; i++)
	{
		if (t < key[i])
		{
			err = false;
			section = i;
			break;
		}
	}
	if (err)	
	{
		LOG("MeCtInterpolator::getTiming : Check the Timing.");
		return;
	}
	
	t1 =  key1[section - 1] + (key1[section] - key1[section - 1]) * (t - key[section - 1]) / (key[section] - key[section - 1]);
	t2 =  key2[section - 1] + (key2[section] - key2[section - 1]) * (t - key[section - 1]) / (key[section] - key[section - 1]);

	MeCtMotion* motionCt1 = dynamic_cast<MeCtMotion*> (child1);
	MeCtMotion* motionCt2 = dynamic_cast<MeCtMotion*> (child2);
	double dur1 = key1[key1.size() - 1] - key1[0];
	if (t1 > motionCt1->motion()->duration())
	{
		t1 -= dur1;
		newCycle1 = true;
	}
	else
		newCycle1 = false;
	double dur2 = key2[key2.size() - 1] - key2[0];
	if (t2 >= motionCt2->motion()->duration())
	{
		t2 -= dur2;
		newCycle2 = true;
	}
	else
		newCycle2 = false;
}

void MeCtInterpolator::adjustStartTime(double origW, double newW)
{
	
	std::vector<double> key;
	std::vector<double> keyP;
	double offset = key1[0] * origW + key2[0] * (1 - origW);
	double offsetP = key1[0] * newW + key2[0] * (1 - newW);
	int numKeys = key1.size();
	for (int i = 0; i < numKeys; i++)
	{
		double k = key1[i] * origW + key2[i] * (1 - origW);
		k = k - offset;
		key.push_back(k);
		k = key1[i] * newW + key2[i] * (1 - newW);
		k = k - offsetP;
		keyP.push_back(k);
	}

	double t = SmartBody::SBScene::getScene()->getSimulationManager()->getTime() - startTime;
	double dur = key[key.size() - 1] - key[0];
	if (t > dur) t -= dur; 
	bool err = true;
	int section;
	for (int i = 0; i < numKeys; i++)
	{
		if (t < key[i])
		{
			err = false;
			section = i;
			break;
		}
	}
	if (err)	
	{
		LOG("MeCtInterpolator::adjustStartTime : Check the Timing.");
		return;
	}

	double tP =  keyP[section - 1] + (keyP[section] - keyP[section - 1]) * (t - key[section - 1]) / (key[section] - key[section - 1]);
	startTime += t - tP;
}

void MeCtInterpolator::getFrame(int index, double t, SrBuffer<float>& buffer)
{
	MeCtMotion* motionCt = NULL;
	if (index == 1)
		motionCt = dynamic_cast<MeCtMotion*> (child1);
	else if (index == 2)
		motionCt = dynamic_cast<MeCtMotion*> (child2);
	else 
		return;
	SkMotion* motion = motionCt->motion();

	double deltaT = motion->duration() / double(motion->frames() - 1);
	int lastFrame = int (t/deltaT);
	motion->apply(float(t), &buffer[0], &motionCt->get_context_map(), SkMotion::Linear, &lastFrame);
}

void MeCtInterpolator::controller_map_updated()
{
	child1->remap();
	child2->remap();
}

bool MeCtInterpolator::controller_evaluate(double t, MeFrameData& frame)
{
	if (newLoop)	newLoop = false;
	t -= startTime;
	if (t <= 0.0)	
		t = 0.0;
	bool continuing = true;
	if (loop)
	{
		double x = t/duration;
		if (x > 1.0)	
		{
			numLoops++;
			startTime += int(x) * duration;
			t = duration * (x - int(x));
			newLoop = true;
		}
	}
	else
		continuing = t < duration;
	
	if (continuing)
	{
		MeCtMotion* motionCt1 = dynamic_cast<MeCtMotion*> (child1);
		MeCtMotion* motionCt2 = dynamic_cast<MeCtMotion*> (child2);
		SkMotion* motion1 = motionCt1->motion();
		SkMotion* motion2 = motionCt2->motion();

		double t1 = 0.0;
		double t2 = 0.0;
		getTiming(t, t1, t2);

		SrBuffer<float> buffer1;
		SrBuffer<float> buffer2;
		buffer1.size(frame.buffer().size());
		buffer2.size(frame.buffer().size());
		getFrame(1, t1, buffer1);
		getFrame(2, t2, buffer2);

		SkChannelArray& motion1Chan = motion1->channels();
		SkChannelArray& motion2Chan = motion2->channels();
		int chanSize = motion1Chan.size();
		for (int i = 0; i < chanSize; i++)
		{
			SkJoint* joint = motion1Chan[i].joint;
			if (!joint)
				continue;

			int idMotion2 = motion2Chan.search(joint->jointName().c_str(), motion1Chan[i].type);
			if (idMotion2 < 0)
				continue;

			int index1 = motionCt1->get_context_map().get(i);
			int index2 = motionCt2->get_context_map().get(idMotion2);
			if (index1 >= 0 && index2 >= 0)
				motion1->channels()[i].interp(&frame.buffer()[index1], &buffer1[index1], &buffer2[index2], (float)(1 - weight));
		}
	}
	return continuing;
}

SkChannelArray& MeCtInterpolator::controller_channels()
{
	return channels;
}

double MeCtInterpolator::controller_duration()
{
	return loop? -1.0: duration;
}
