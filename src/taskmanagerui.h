#ifndef SRC_TASKMANAGERUI_H_
#define SRC_TASKMANAGERUI_H_

#include "consolepainter.h"
#include "taskmanager.h"

class TaskManagerUI {
 public:
  TaskManagerUI(const std::string& db_name, const std::string& db_user,
                const std::string& db_password);
  void Run();

 private:
  void AddTaskUi();
  void ListTasksUi();
  void SetStatusUi();
  void DeleteTaskUi();
  void UpdateTaskUi();

  TaskManager taskManager;
  ConsolePainter consolePainter;
};

#endif  // SRC_TASKMANAGERUI_H_
