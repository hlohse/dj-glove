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
        m_stream << value;
        return *this;
    }

    std::string str() const        { return m_stream.str(); }
    operator std::string() const   { return m_stream.str(); }

private:
    std::stringstream m_stream;
};

#endif // DJ_GLOVE_PC_FORMATTER_H_
