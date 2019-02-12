#pragma once

#include <atomic>
#include <memory>
#include <vector>

#include <boost/lockfree/queue.hpp>

#include <engine/task/task_processor.hpp>
#include <engine/task/task_with_result.hpp>
#include <utils/swappingsmart.hpp>

#include <storages/postgres/detail/connection.hpp>
#include <storages/postgres/detail/connection_ptr.hpp>
#include <storages/postgres/options.hpp>
#include <storages/postgres/statistics.hpp>
#include <storages/postgres/transaction.hpp>

namespace storages {
namespace postgres {
namespace detail {

class ConnectionPoolImpl
    : public std::enable_shared_from_this<ConnectionPoolImpl> {
 public:
  static std::shared_ptr<ConnectionPoolImpl> Create(
      const std::string& dsn, engine::TaskProcessor& bg_task_processor,
      size_t initial_size, size_t max_size, CommandControl default_cmd_ctl);
  ~ConnectionPoolImpl();

  [[nodiscard]] ConnectionPtr Acquire();
  void Release(Connection* connection);

  const InstanceStatistics& GetStatistics() const;
  [[nodiscard]] Transaction Begin(const TransactionOptions& options,
                                  OptionalCommandControl trx_cmd_ctl = {});

  void SetDefaultCommandControl(CommandControl);

 protected:
  ConnectionPoolImpl(const std::string& dsn,
                     engine::TaskProcessor& bg_task_processor, size_t max_size,
                     CommandControl default_cmd_ctl);

 private:
  void Init(size_t initial_size);

  [[nodiscard]] engine::TaskWithResult<bool> Create();

  void Push(detail::Connection* connection);
  detail::Connection* Pop();

  void Clear();
  void DeleteConnection(detail::Connection* connection);

 private:
  mutable InstanceStatistics stats_;
  std::string dsn_;
  engine::TaskProcessor& bg_task_processor_;
  size_t max_size_;
  boost::lockfree::queue<detail::Connection*> queue_;
  std::atomic<size_t> size_;
  ::utils::SwappingSmart<const CommandControl> default_cmd_ctl_;
};

}  // namespace detail
}  // namespace postgres
}  // namespace storages
