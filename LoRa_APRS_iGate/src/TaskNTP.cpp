#include <logger.h>

#include "TimeLib/TimeLib.h"

#include "Task.h"
#include "TaskNTP.h"
#include "project_configuration.h"

NTPTask::NTPTask() : Task(TASK_NTP, TaskNtp), _beginCalled(false) {
}

NTPTask::~NTPTask() {
}

bool NTPTask::setup(System &system) {
  _ntpClient.setPoolServerName(system.getUserConfig()->ntpServer.c_str());
  return true;
}

bool NTPTask::loop(System &system) {
  if (!system.isWifiOrEthConnected()) {
    return false;
  }
  if (!_beginCalled) {
    _ntpClient.begin();
    _beginCalled = true;
  }
  if (_ntpClient.update()) {
    setTime(_ntpClient.getEpochTime());
    system.getLogger().log(logging::LoggerLevel::LOGGER_LEVEL_INFO, getName(), "Current time: %s", _ntpClient.getFormattedTime().c_str());
  }
  _stateInfo = _ntpClient.getFormattedTime();
  _state     = Okay;
  return true;
}
