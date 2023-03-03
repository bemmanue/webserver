#ifndef HTML_HPP
#define HTML_HPP

#include <iostream>

#include "Status.hpp"

#ifndef SP
    # define SP " "
#endif

#ifndef CRLF
    # define CRLF "\r\n"
#endif

static char http_error_301_page[] =
		"<html>" CRLF
		"<head><title>301 Moved Permanently</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>301 Moved Permanently</h1></center>" CRLF
		;

static char http_error_302_page[] =
		"<html>" CRLF
		"<head><title>302 Found</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>302 Found</h1></center>" CRLF
		;

static char http_error_303_page[] =
		"<html>" CRLF
		"<head><title>303 See Other</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>303 See Other</h1></center>" CRLF
		;

static char http_error_304_page[] =
		"<html>" CRLF
		"<head><title>304 Not Modified</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>304 Not Modified</h1></center>" CRLF
		;

static char http_error_305_page[] =
		"<html>" CRLF
		"<head><title>305 Use Proxy</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>305 Use Proxy</h1></center>" CRLF
		;

static char http_error_307_page[] =
		"<html>" CRLF
		"<head><title>307 Temporary Redirect</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>307 Temporary Redirect</h1></center>" CRLF
		;

static char http_error_308_page[] =
		"<html>" CRLF
		"<head><title>308 Permanent Redirect</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>308 Permanent Redirect</h1></center>" CRLF
		;

static char http_error_400_page[] =
		"<html>" CRLF
		"<head><title>400 Bad Request</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>400 Bad Request</h1></center>" CRLF
		;

static char http_error_401_page[] =
		"<html>" CRLF
		"<head><title>401 Authorization Required</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>401 Authorization Required</h1></center>" CRLF
		;

static char http_error_402_page[] =
		"<html>" CRLF
		"<head><title>402 Payment Required</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>402 Payment Required</h1></center>" CRLF
		;

static char http_error_403_page[] =
		"<html>" CRLF
		"<head><title>403 Forbidden</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>403 Forbidden</h1></center>" CRLF
		;

static char http_error_404_page[] =
		"<html>" CRLF
		"<head><title>404 Not Found</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>404 Not Found</h1></center>" CRLF
		;

static char http_error_405_page[] =
		"<html>" CRLF
		"<head><title>405 Not Allowed</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>405 Not Allowed</h1></center>" CRLF
		;

static char http_error_406_page[] =
		"<html>" CRLF
		"<head><title>406 Not Acceptable</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>406 Not Acceptable</h1></center>" CRLF
		;

static char http_error_407_page[] =
		"<html>" CRLF
		"<head><title>407 Proxy Authentication Required</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>406 Not Acceptable</h1></center>" CRLF
		;

static char http_error_408_page[] =
		"<html>" CRLF
		"<head><title>408 Request Time-out</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>408 Request Time-out</h1></center>" CRLF
		;

static char http_error_409_page[] =
		"<html>" CRLF
		"<head><title>409 Conflict</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>409 Conflict</h1></center>" CRLF
		;

static char http_error_410_page[] =
		"<html>" CRLF
		"<head><title>410 Gone</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>410 Gone</h1></center>" CRLF
		;

static char http_error_411_page[] =
		"<html>" CRLF
		"<head><title>411 Length Required</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>411 Length Required</h1></center>" CRLF
		;

static char http_error_412_page[] =
		"<html>" CRLF
		"<head><title>412 Precondition Failed</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>412 Precondition Failed</h1></center>" CRLF
		;

static char http_error_413_page[] =
		"<html>" CRLF
		"<head><title>413 Request Entity Too Large</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>413 Request Entity Too Large</h1></center>" CRLF
		;

static char http_error_414_page[] =
		"<html>" CRLF
		"<head><title>414 Request-URI Too Large</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>414 Request-URI Too Large</h1></center>" CRLF
		;

static char http_error_415_page[] =
		"<html>" CRLF
		"<head><title>415 Unsupported Media Type</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>415 Unsupported Media Type</h1></center>" CRLF
		;

static char http_error_416_page[] =
		"<html>" CRLF
		"<head><title>416 Requested Range Not Satisfiable</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>416 Requested Range Not Satisfiable</h1></center>" CRLF
		;

static char http_error_417_page[] =
		"<html>" CRLF
		"<head><title>417 Expectation Failed</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>417 Expectation Failed</h1></center>" CRLF
		;

static char http_error_418_page[] =
		"<html>" CRLF
		"<head><title>418 Im a teapot</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>418 Im a teapot</h1></center>" CRLF
		;

static char http_error_419_page[] =
		"<html>" CRLF
		"<head><title>419 Authentication Timeout</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>419 Authentication Timeout</h1></center>" CRLF
		;

static char http_error_421_page[] =
		"<html>" CRLF
		"<head><title>421 Misdirected Request</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>421 Misdirected Request</h1></center>" CRLF
		;

static char http_error_422_page[] =
		"<html>" CRLF
		"<head><title>422 Unprocessable Entity</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>422 Unprocessable Entity</h1></center>" CRLF
		;

static char http_error_423_page[] =
		"<html>" CRLF
		"<head><title>423 Locked</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>423 Locked</h1></center>" CRLF
		;

static char http_error_424_page[] =
		"<html>" CRLF
		"<head><title>424 Failed Dependency</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>424 Failed Dependency</h1></center>" CRLF
		;

static char http_error_425_page[] =
		"<html>" CRLF
		"<head><title>425 Too Early</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>425 Too Early</h1></center>" CRLF
		;

static char http_error_426_page[] =
		"<html>" CRLF
		"<head><title>426 Upgrade Required</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>426 Upgrade Required</h1></center>" CRLF
		;

static char http_error_428_page[] =
		"<html>" CRLF
		"<head><title>421 Misdirected Request</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>421 Misdirected Request</h1></center>" CRLF
		;

static char http_error_429_page[] =
		"<html>" CRLF
		"<head><title>429 Too Many Requests</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>429 Too Many Requests</h1></center>" CRLF
		;

static char http_error_431_page[] =
		"<html>" CRLF
		"<head><title>431 Request Header Fields Too Large</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>431 Request Header Fields Too Large</h1></center>" CRLF
		;

static char http_error_449_page[] =
		"<html>" CRLF
		"<head><title>449 Retry With</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>449 Retry With</h1></center>" CRLF
		;

static char http_error_451_page[] =
		"<html>" CRLF
		"<head><title>451 Unavailable For Legal Reasons</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>451 Unavailable For Legal Reasons</h1></center>" CRLF
		;

static char http_error_500_page[] =
		"<html>" CRLF
		"<head><title>500 Internal Server Error</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>500 Internal Server Error</h1></center>" CRLF
		;

static char http_error_501_page[] =
		"<html>" CRLF
		"<head><title>501 Not Implemented</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>501 Not Implemented</h1></center>" CRLF
		;

static char http_error_502_page[] =
		"<html>" CRLF
		"<head><title>502 Bad Gateway</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>502 Bad Gateway</h1></center>" CRLF
		;

static char http_error_503_page[] =
		"<html>" CRLF
		"<head><title>503 Service Temporarily Unavailable</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>503 Service Temporarily Unavailable</h1></center>" CRLF
		;

static char http_error_504_page[] =
		"<html>" CRLF
		"<head><title>504 Gateway Time-out</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>504 Gateway Time-out</h1></center>" CRLF
		;

static char http_error_505_page[] =
		"<html>" CRLF
		"<head><title>505 HTTP Version Not Supported</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>505 HTTP Version Not Supported</h1></center>" CRLF
		;

static char http_error_506_page[] =
		"<html>" CRLF
		"<head><title>506 Variant Also Negotiates</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>506 Variant Also Negotiates</h1></center>" CRLF
		;

static char http_error_507_page[] =
		"<html>" CRLF
		"<head><title>507 Insufficient Storage</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>507 Insufficient Storage</h1></center>" CRLF
		;

static char http_error_508_page[] =
		"<html>" CRLF
		"<head><title>508 Loop Detected</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>508 Loop Detected</h1></center>" CRLF
		;

static char http_error_509_page[] =
		"<html>" CRLF
		"<head><title>509 Bandwidth Limit Exceeded/title></head>" CRLF
		"<body>" CRLF
		"<center><h1>509 Bandwidth Limit Exceeded</h1></center>" CRLF
		;

static char http_error_510_page[] =
		"<html>" CRLF
		"<head><title>510 Not Extended</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>510 Not Extended</h1></center>" CRLF
		;

static char http_error_511_page[] =
		"<html>" CRLF
		"<head><title>511 Network Authentication Required</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>511 Network Authentication Required</h1></center>" CRLF
		;

static char http_error_520_page[] =
		"<html>" CRLF
		"<head><title>520 Unknown Error</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>520 Unknown Error</h1></center>" CRLF
		;

static char http_error_521_page[] =
		"<html>" CRLF
		"<head><title>521 Web Server Is Down</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>521 Web Server Is Down</h1></center>" CRLF
		;

static char http_error_522_page[] =
		"<html>" CRLF
		"<head><title>522 Connection Timed Out</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>522 Connection Timed Out</h1></center>" CRLF
		;

static char http_error_523_page[] =
		"<html>" CRLF
		"<head><title>523 Origin Is Unreachable</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>523 Origin Is Unreachable</h1></center>" CRLF
		;

static char http_error_524_page[] =
		"<html>" CRLF
		"<head><title>524 A Timeout Occurred</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>524 A Timeout Occurred</h1></center>" CRLF
		;

static char http_error_525_page[] =
		"<html>" CRLF
		"<head><title>525 SSL Handshake Failed</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>525 SSL Handshake Failed</h1></center>" CRLF
		;

static char http_error_526_page[] =
		"<html>" CRLF
		"<head><title>526 Invalid SSL Certificate</title></head>" CRLF
		"<body>" CRLF
		"<center><h1>526 Invalid SSL Certificate</h1></center>" CRLF
		;

static char http_error_tail[] =
		"<hr><center>webserv</center>" CRLF
		"</body>" CRLF
		"</html>" CRLF
		;


std::string getPageForStatus(Status status);


#endif //HTML_HPP
