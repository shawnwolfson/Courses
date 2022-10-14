#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_
#include <exception>

class TreeExceptions : public std::exception {};
class Failure: public TreeExceptions {};






#endif //_EXCEPTIONS_H_