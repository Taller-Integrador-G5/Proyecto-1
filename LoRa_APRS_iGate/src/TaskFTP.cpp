#include <FTPFilesystem.h>
#include <SPIFFS.h>
#include <logger.h>

#include "Task.h"
#include "TaskFTP.h"
#include "project_configuration.h"

FTPTask::FTPTask() : Task(TASK_FTP, TaskFtp), _beginCalled(false) {
}

FTPTask::~FTPTask() {
}

bool FTPTask::setup(System &system) {
  for (Configuration::Ftp::User user : system.getUserConfig()->ftp.users) {
    system.getLogger().log(logging::LoggerLevel::LOGGER_LEVEL_DEBUG, getName(), "Adding user to FTP Server: %s", user.name.c_str());
    _ftpServer.addUser(user.name, user.password);
  }
  _ftpServer.addFilesystem("SPIFFS", &SPIFFS);
  _stateInfo = "waiting";
  return true;
}

bool FTPTask::loop(System &system) {
  if (!_beginCalled) {
    _ftpServer.begin();
    _beginCalled = true;
  }
  _ftpServer.handle();
  static bool configWasOpen = false;
  if (configWasOpen && _ftpServer.countConnections() == 0) {
    system.getLogger().log(logging::LoggerLevel::LOGGER_LEVEL_WARN, getName(), "Maybe the config has been changed via FTP, lets restart now to get the new config...");
    ESP.restart();
  }
  if (_ftpServer.countConnections() > 0) {
    configWasOpen = true;
    _stateInfo    = "has connection";
  }
  return true;
}
