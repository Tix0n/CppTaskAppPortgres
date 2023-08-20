#include "taskmanagerui.h"

#include <unistd.h>

#include <iomanip>
#include <iostream>

TaskManagerUI::TaskManagerUI(const std::string& db_name,
                             const std::string& db_user,
                             const std::string& db_password)
    : taskManager(db_name, db_user, db_password), consolePainter() {}

void TaskManagerUI::Run() {
  consolePainter.DrawBoxWithText("Task Manager", ConsolePainter::GREEN);

  while (true) {
    consolePainter.DrawBoxWithText(
        "Enter a command (add/list/set status/update/delete/exit):",
        ConsolePainter::YELLOW);

    std::string command;
    std::getline(std::cin, command);

    if (taskManager.IsValidCommand(command)) {
      if (command == "add") {
        AddTaskUi();
      } else if (command == "list") {
        ListTasksUi();
      } else if (command == "set status") {
        SetStatusUi();
      } else if (command == "delete") {
        DeleteTaskUi();
      } else if (command == "update") {
        UpdateTaskUi();
      } else if (command == "exit") {
        break;
      }
    } else {
      consolePainter.DrawBoxWithText("Unknown command.", ConsolePainter::RED);
    }
  }
}

void TaskManagerUI::AddTaskUi() {
  std::string description, deadline;

  consolePainter.DrawBoxWithText("Enter description:", ConsolePainter::YELLOW);
  std::getline(std::cin, description);

  do {
    consolePainter.DrawBoxWithText("Enter deadline (YYYY-MM-DD):",
                                   ConsolePainter::YELLOW);
    std::cin >> deadline;
    std::cin.ignore();
  } while (!taskManager.IsValidDate(deadline));

  taskManager.AddTask(description, deadline);

  consolePainter.DrawBoxWithText("Task added successfully.",
                                 ConsolePainter::GREEN);
}

void TaskManagerUI::ListTasksUi() {
  std::vector<std::tuple<int, std::string, std::string, int>> tasks =
      taskManager.ListTasks();

  consolePainter.DrawBoxWithText("Task List:", ConsolePainter::YELLOW);

  for (const auto& task : tasks) {
    int user_task_id, status;
    std::string description, deadline;
    std::tie(user_task_id, description, deadline, status) = task;

    std::string status_text;
    ConsolePainter::Color border_color = ConsolePainter::GRAY;

    if (status == 1) {
      status_text = "Uncompleted";
    } else if (status == 2) {
      status_text = "In Progress";
      border_color = ConsolePainter::BLUE;
    } else if (status == 3) {
      status_text = "Completed";
      border_color = ConsolePainter::GREEN;
    }

    std::string taskInfo = "ID: " + std::to_string(user_task_id) + " |" +
                           " Description: " + description + " |" +
                           " Deadline: " + deadline + " |" +
                           " Status: " + status_text;

    if (status == 1) {
      std::time_t current_time = std::time(nullptr);
      std::tm deadline_time = {};
      std::istringstream deadline_stream(deadline);
      deadline_stream >> std::get_time(&deadline_time, "%Y-%m-%d");

      if (std::mktime(&deadline_time) < current_time) {
        border_color = ConsolePainter::RED;
      }
    }

    consolePainter.DrawBoxWithText(taskInfo, border_color);
  }
}

void TaskManagerUI::SetStatusUi() {
  int task_id;
  int status = 1;
  std::string str_status;
  consolePainter.DrawBoxWithText("Enter task ID to change status:",
                                 ConsolePainter::YELLOW);
  std::cin >> task_id;
  std::cin.ignore();
  do {
    consolePainter.DrawBoxWithText(
        "Enter status for this task: completed/in progress/uncompleted",
        ConsolePainter::YELLOW);
    std::getline(std::cin, str_status);
  } while (!taskManager.IsValidStatus(str_status));

  if (str_status == "completed") {
    status = 3;
  } else if (str_status == "in progress") {
    status = 2;
  } else if (str_status == "uncompleted") {
    status = 1;
  }

  if (status == 3) {
    if (taskManager.UpdateStatus(task_id, status)) {
      consolePainter.DrawBoxWithText("Task completed successfully.",
                                     ConsolePainter::GREEN);
    } else {
      consolePainter.DrawBoxWithText("Task not found. No changes were made.",
                                     ConsolePainter::RED);
    }
  } else if (status == 2) {
    if (taskManager.UpdateStatus(task_id, status)) {
      consolePainter.DrawBoxWithText("Task in progress.",
                                     ConsolePainter::GREEN);
    } else {
      consolePainter.DrawBoxWithText("Task not found. No changes were made.",
                                     ConsolePainter::BLUE);
    }
  } else if (status == 1) {
    if (taskManager.UpdateStatus(task_id, status)) {
      consolePainter.DrawBoxWithText("Task uncompleted successfully.",
                                     ConsolePainter::GREEN);
    } else {
      consolePainter.DrawBoxWithText("Task not found. No changes were made.",
                                     ConsolePainter::RED);
    }
  }
}

void TaskManagerUI::DeleteTaskUi() {
  int task_id;
  consolePainter.DrawBoxWithText("Enter task ID to delete:",
                                 ConsolePainter::YELLOW);
  std::cin >> task_id;
  std::cin.ignore();

  if (taskManager.DeleteTask(task_id)) {
    consolePainter.DrawBoxWithText("Task deleted successfully.",
                                   ConsolePainter::GREEN);
  } else {
    consolePainter.DrawBoxWithText("Task not found. No changes were made.",
                                   ConsolePainter::RED);
  }
}

void TaskManagerUI::UpdateTaskUi() {
  int task_id;
  std::string new_description, new_deadline;

  consolePainter.DrawBoxWithText("Enter task ID to update:",
                                 ConsolePainter::YELLOW);
  std::cin >> task_id;

  std::cin.ignore();

  consolePainter.DrawBoxWithText("Enter new description:",
                                 ConsolePainter::YELLOW);
  std::getline(std::cin, new_description);

  do {
    consolePainter.DrawBoxWithText("Enter new deadline (YYYY-MM-DD):",
                                   ConsolePainter::YELLOW);
    std::cin >> new_deadline;
    std::cin.ignore();
  } while (!taskManager.IsValidDate(new_deadline));

  if (taskManager.UpdateTask(task_id, new_description, new_deadline)) {
    consolePainter.DrawBoxWithText("Task updated successfully.",
                                   ConsolePainter::GREEN);
  } else {
    consolePainter.DrawBoxWithText("Task not found. No changes were made.",
                                   ConsolePainter::RED);
  }
}
