#include "action_unit.hpp"
#include <sstream>

ActionUnit::ActionUnit(int id, SkMotion* unified ) :
		left( unified ),
		right( unified )
{
	_id = id;
	m_isLeft = false;
	m_isRight = false;
	m_isBilateral = true;
	_dirtyName = true;
}

ActionUnit::ActionUnit(int id,  SkMotion* left, SkMotion* right ) :
	left( left ),
	right( right )
{
	setId(id);
	reset_type();
	_dirtyName = true;
}

ActionUnit::ActionUnit(ActionUnit* source)
{
	reset_type();

	setId(source->getId());
	if (source->is_left())
		set_left();
	if (source->is_right())
		set_right();
	if (source->is_bilateral())
		set_bilateral();
	left = source->left;
	right = source->right;
	_dirtyName = true;
}

ActionUnit::~ActionUnit()
{
}

int ActionUnit::getId()
{
	return _id;
}

void ActionUnit::setId(int i)
{
	_id = i;
	_dirtyName = true;
}

bool ActionUnit::is_bilateral() const
{
	return m_isBilateral;
}

void ActionUnit::reset_type()
{
	m_isLeft = false;
	m_isRight = false;
	 m_isBilateral = false;
	_dirtyName = true;
}

void ActionUnit::set_left()
{
	m_isLeft = true;
	_dirtyName = true;
}

void ActionUnit::set_bilateral()
{
	m_isBilateral = true;
	_dirtyName = true;
}

void ActionUnit::set_right()
{
	m_isRight = true;
	_dirtyName = true;
}

bool ActionUnit::is_left() const
{
	return m_isLeft;
}

bool ActionUnit::is_right() const
{
	return m_isRight;
}

void ActionUnit::set( SkMotion* motion )
{
	set( motion, motion );
}

void ActionUnit::set( SkMotion* left, SkMotion* right )
{
	this->left	= left;
	this->right	= right;
}

const std::string& ActionUnit::getLeftName()
{
	if (_dirtyName)
	{
		std::stringstream strstr; 
		if( this->is_left() )
		{
			strstr << "au_" << _id << "_left";
		}
		else
		{
			_leftName = "";
		}

		_leftName = strstr.str();
	}
	
	return _leftName;
}

const std::string& ActionUnit::getRightName()
{
	if (_dirtyName)
	{
		if( this->is_right() )
		{
			std::stringstream strstr; 
			strstr << "au_" << _id << "_right";
			_rightName = strstr.str();
		}
		else
		{
			_rightName = "";
		}		
	}
	
	return _rightName;
}

const std::string& ActionUnit::getBilateralName()
{
	if (_dirtyName)
	{
		if( this->is_bilateral() )
		{
			std::stringstream strstr; 
			strstr << "au_" << _id;
			_bilateralName = strstr.str();
		}
		else
		{
			_bilateralName = "";
		}
	}
	
	return _bilateralName;
}

	

