
#include "System.h"

System::System() : _boardConfig(0), _userConfig(0), _isEthConnected(false), _isWifiConnected(false) {
}

System::~System() {
}

void System::setBoardConfig(BoardConfig const *const boardConfig) {
  _boardConfig = boardConfig;
}

void System::setUserConfig(Configuration const *const userConfig) {
  _userConfig = userConfig;
}

BoardConfig const *const System::getBoardConfig() const {
  return _boardConfig;
}

Configuration const *const System::getUserConfig() const {
  return _userConfig;
}

TaskManager &System::getTaskManager() {
  return _taskManager;
}

Display &System::getDisplay() {
  return _display;
}

bool System::isWifiOrEthConnected() const {
  return _isEthConnected || _isWifiConnected;
}

void System::connectedViaEth(bool status) {
  _isEthConnected = status;
}

void System::connectedViaWifi(bool status) {
  _isWifiConnected = status;
}

logging::Logger &System::getLogger() {
  return _logger;
}
