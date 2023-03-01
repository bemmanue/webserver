#include "Global.hpp"

bool	isHTTPMethod(const std::string& method) {
	if (method == GET	||
	method == POST		||
	method == PUT		||
	method == HEAD		||
	method == DELETE	||
	method == CONNECT	||
	method == OPTIONS	||
	method == TRACE		||
	method == PATCH) {
		return true;
	}
	return false;
}
