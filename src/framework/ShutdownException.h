#include <string>
#include <ostream>

/**
 * @brief Exception, which can be thrown ONLY at game loading to abort.
 * 
 * This is meant to NOT inherit from std::exception!
 */
class ShutdownException {
public:
	/**
	 * @brief Ctor
	 * @param reason The reason for the exception. You can also call it error message.
	 */
	ShutdownException(const std::string& reason) :
		mReason(reason)
	{
	}
	
	/**
	 * @brief Allows to read the reason for the exception. 
	 * @return Returns the reason passed to the Ctor as string.
	 */
	std::string getReason()
	{
		return mReason;
	}

private:
	std::string mReason;
};
