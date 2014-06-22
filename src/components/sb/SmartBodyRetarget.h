#ifndef RETARGET_H
#define RETARGET_H

	#include <string>
	#include <SmartBody.h>

namespace Ember
{

class SmartBodyManager;

/**
 @brief Create a retarget instance.

 This class has to be instantiate by any SmartBodyBehaviors to retarget a behavior set on another character.

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodyRetarget
{
public:

	/**
	 * @brief Ctor.
	 */
	Retarget(const std::string& srcSkName, const std::string& tgtSkName, SmartBody::RetargetManager *manager);

	/**
	 * @brief Dtor.
	 */
	~Retarget(void);

private:

	/**
	 * @brief Create an instance of SBRetarget.
	 */
	void createInstance(const std::string& srcSk, const std::string& tgtSk, SmartBody::RetargetManager *manager);
};

}

#endif