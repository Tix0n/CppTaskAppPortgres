#include "taskmanagerui.h"

int main() {
  TaskManagerUI taskManager("task_manager", "postgres", "postgres");
  taskManager.Run();

  return 0;
}
