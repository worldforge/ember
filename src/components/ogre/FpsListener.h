namespace Ember {
namespace OgreView {


/**
 * @brief Struture containing information about an Fps event
 */
struct FpsEvent
{
	double Fps;
};

/**
 * @brief A interface class defining a listener which can be used to receive notifications of Fps updated events
 */ 
class FpsListener
{
public:
	/**
	 * @brief Called for listeners when Fps has been updated
	 */
	virtual FpsUpdated(const FpsEvent);
};

}
}

