#include <string>
#include <ostream>

//Exception, which can be thrown ONLY at game loading to abort.
//This is meant to NOT inherit from std::exception!
class ShutdownException {
public:
	ShutdownException(const std::string& reason) :
		mReason(reason)
	{
	}
	std::string getReason()
	{
		return mReason;
	}

private:
	std::string mReason;
};
