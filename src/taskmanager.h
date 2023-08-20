#ifndef SRC_TASKMANAGER_H_
#define SRC_TASKMANAGER_H_

#include <pqxx/pqxx>
#include <string>
#include <vector>

class TaskManager {
 public:
  TaskManager(const std::string& db_name, const std::string& db_user,
              const std::string& db_password);

  bool IsValidCommand(const std::string& input);
  bool IsValidDate(const std::string& date);
  bool IsValidStatus(const std::string& input);

  void AddTask(const std::string& description, const std::string& deadline);
  std::vector<std::tuple<int, std::string, std::string, int>> ListTasks();
  bool UpdateStatus(int user_task_id, int status);
  bool DeleteTask(int user_task_id);
  bool UpdateTask(int user_task_id, const std::string& new_description,
                  const std::string& new_deadline);

 private:
  pqxx::connection conn_;

  void UpdateTaskMapping(int user_task_id, int real_task_id);
  int GetNextUserTaskId();
  int GetRealTaskId(int user_task_id);
};

#endif  // SRC_TASKMANAGER_H_
