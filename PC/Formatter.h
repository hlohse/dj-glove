#ifndef DJ_GLOVE_PC_FORMATTER_H_
#define DJ_GLOVE_PC_FORMATTER_H_

#include <string>
#include <sstream>

class Formatter
{
public:
	Formatter() {}
	~Formatter() {}

	template <typename Type>
	Formatter & operator << (const Type & value)
	{
		stream_ << value;
		return *this;
	}

	std::string str() const        { return stream_.str(); }
	operator std::string() const   { return stream_.str(); }

private:
	std::stringstream stream_;
};

#endif // DJ_GLOVE_PC_FORMATTER_H_
