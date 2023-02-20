#include "Client.hpp"

Client::Client(const std::vector<ServerConfig>& serverConfigs) {
	_serverConfigs = serverConfigs;
}

Client::~Client() {}

ServerConfig*	Client::matchServerConfig(const std::string& host) {
	for (int i = 0; i < _serverConfigs.size(); i++) {
		if (_serverConfigs[i].hasName(host)) {
			return &_serverConfigs[i];
		}
	}
	return &_serverConfigs[0];
}
