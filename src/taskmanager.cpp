#include "taskmanager.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <tuple>

TaskManager::TaskManager(const std::string& db_name, const std::string& db_user,
                         const std::string& db_password)
    : conn_("dbname=" + db_name + " user=" + db_user +
            " password=" + db_password) {}

bool TaskManager::IsValidCommand(const std::string& input) {
  std::vector<std::string> valid_commands = {"add",  "list",   "delete",
                                             "exit", "update", "set status"};
  return std::find(valid_commands.begin(), valid_commands.end(), input) !=
         valid_commands.end();
}

bool TaskManager::IsValidStatus(const std::string& input) {
  std::vector<std::string> valid_commands = {"completed", "uncompleted",
                                             "in progress"};
  return std::find(valid_commands.begin(), valid_commands.end(), input) !=
         valid_commands.end();
}

bool TaskManager::IsValidDate(const std::string& date) {
  std::regex date_pattern(R"(\d{4}-\d{2}-\d{2})");
  if (!std::regex_match(date, date_pattern)) {
    return false;
  }

  std::istringstream iss(date);
  std::tm t = {};
  iss >> std::get_time(&t, "%Y-%m-%d");
  if (iss.fail()) {
    return false;
  }

  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);

  if (std::mktime(&t) <= now_time) {
    return false;
  }

  int year = t.tm_year + 1900;
  int month = t.tm_mon + 1;
  int day = t.tm_mday;

  if (month < 1 || month > 12) {
    return false;
  }

  int days_in_month = 31;
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    days_in_month = 30;
  } else if (month == 2) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      days_in_month = 29;
    } else {
      days_in_month = 28;
    }
  }

  if (day < 1 || day > days_in_month) {
    return false;
  }

  return true;
}

void TaskManager::AddTask(const std::string& description,
                          const std::string& deadline) {
  pqxx::work txn(conn_);

  txn.exec_params(
      "INSERT INTO tasks (description, deadline, status) VALUES ($1, $2, 1)",
      description, deadline);

  pqxx::result result = txn.exec("SELECT lastval()");
  int real_task_id = result[0][0].as<int>();

  txn.commit();

  UpdateTaskMapping(GetNextUserTaskId(), real_task_id);
}

std::vector<std::tuple<int, std::string, std::string, int>>
TaskManager::ListTasks() {
  pqxx::nontransaction txn(conn_);

  pqxx::result result = txn.exec(
      "SELECT tm.user_id, t.description, t.deadline, t.status FROM "
      "task_mapping tm "
      "JOIN tasks t ON tm.real_id = t.id ORDER BY t.deadline");

  std::vector<std::tuple<int, std::string, std::string, int>> tasks;
  for (const auto& row : result) {
    tasks.emplace_back(row[0].as<int>(), row[1].as<std::string>(),
                       row[2].as<std::string>(), row[3].as<int>());
  }
  return tasks;
}

bool TaskManager::UpdateStatus(int user_task_id, int status) {
  int real_task_id = GetRealTaskId(user_task_id);

  if (real_task_id == -1) {
    return false;
  }

  pqxx::work txn(conn_);

  txn.exec_params("UPDATE tasks SET status = $2  WHERE id = $1", real_task_id,
                  status);

  txn.commit();
  return true;
}

bool TaskManager::DeleteTask(int user_task_id) {
  int real_task_id = GetRealTaskId(user_task_id);

  if (real_task_id == -1) {
    return false;
  }

  pqxx::work txn(conn_);

  txn.exec_params("DELETE FROM task_mapping WHERE user_id = $1", user_task_id);

  pqxx::result result = txn.exec("SELECT MAX(user_id) FROM task_mapping");
  if (!result[0][0].is_null()) {
    int maxUserId = result[0][0].as<int>();

    for (int i = user_task_id + 1; i <= maxUserId; ++i) {
      txn.exec_params("UPDATE task_mapping SET user_id = $1 WHERE user_id = $2",
                      i - 1, i);
    }
  }

  txn.exec_params("DELETE FROM tasks WHERE id = $1", real_task_id);

  txn.commit();
  return true;
}

void TaskManager::UpdateTaskMapping(int user_task_id, int real_task_id) {
  pqxx::work txn(conn_);

  txn.exec_params("DELETE FROM task_mapping WHERE user_id = $1", user_task_id);

  txn.exec_params("INSERT INTO task_mapping (user_id, real_id) VALUES ($1, $2)",
                  user_task_id, real_task_id);

  txn.commit();
}

int TaskManager::GetNextUserTaskId() {
  pqxx::nontransaction txn(conn_);
  pqxx::result result = txn.exec("SELECT COUNT(*) FROM task_mapping");
  return result[0][0].as<int>() + 1;
}

int TaskManager::GetRealTaskId(int user_task_id) {
  pqxx::nontransaction txn(conn_);
  pqxx::result result = txn.exec_params(
      "SELECT real_id FROM task_mapping WHERE user_id = $1", user_task_id);

  if (!result.empty()) {
    return result[0][0].as<int>();
  } else {
    return -1;
  }
}

bool TaskManager::UpdateTask(int user_task_id,
                             const std::string& new_description,
                             const std::string& new_deadline) {
  int real_task_id = GetRealTaskId(user_task_id);

  if (real_task_id == -1) {
    return false;
  }

  pqxx::work txn(conn_);

  txn.exec_params(
      "UPDATE tasks SET description = $2, deadline = $3 WHERE id = $1",
      real_task_id, new_description, new_deadline);

  txn.commit();
  return true;
}
