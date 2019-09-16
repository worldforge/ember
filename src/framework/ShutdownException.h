#include <string>
#include <ostream>
#include <utility>

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
	explicit ShutdownException(std::string reason) :
			mReason(std::move(reason)) {
	}

	/**
	 * @brief Allows to read the reason for the exception. 
	 * @return Returns the reason passed to the Ctor as string.
	 */
	const std::string& getReason() const {
		return mReason;
	}

private:
	const std::string mReason;
};
