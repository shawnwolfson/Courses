#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_
#include <exception>

class TreeExceptions : public std::exception {};
class NodeAlreadyExists: public TreeExceptions {};
class NodeNotExists: public TreeExceptions {};
class HasEmployees: public TreeExceptions {};
class NotValuableEnough: public TreeExceptions {};
class NoMatchingEmployee: public TreeExceptions {};
class NotEnoughCompanies: public TreeExceptions {};
class NotEnoughEmployees: public TreeExceptions {};





#endif //_EXCEPTIONS_H_