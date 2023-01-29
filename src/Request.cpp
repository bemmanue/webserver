#include "../include/Request.hpp"

void Request::setRequestLine(const Request::RequestLine &requestLine) {

}

void Request::setHeaderField(const std::string &fieldName, const std::string& fieldValue) {

}

void Request::setBody(const std::string &body) {

}

std::string getNextLine(std::string& string) {
	std::string line;
	int			i;

	for (i = 0; i < string.size(); i++) {
		line.push_back(string[i]);
		if ()
	}
	return line;
}

void	parseRequestLine(const std::string& line) {

}

void	parseHeaderField(const std::string& line) {

}

void	parseBody(std::string& str) {

}

Request parseRequest(std::string& str) {
	Request		request;
	std::string	line;

	parseRequestLine(str);

//	line = getNextLine(str);
//	while (line != "\n" && !line.empty()) {
//		parseHeaderField(line);
//	}
//
//	parseBody(str);

	return request;
}
