#pragma once

#include <chrono>
#include <functional>

template <typename TTime = std::chrono::duration<double>>
class Timer {
 public:
  Timer(std::function<void(decltype(std::declval<TTime>().count()) elapsed)>
            destroy_callback = nullptr)
      : start_time_(std::chrono::steady_clock::now()),
        destroy_callback_(destroy_callback) {}

  template <typename TDuration = std::chrono::duration<double>>
  TDuration Elapsed() const {
    return std::chrono::duration_cast<TDuration>(
        std::chrono::steady_clock::now() - start_time_);
  }

  ~Timer() {
    if (destroy_callback_) {
      destroy_callback_(Elapsed<TTime>().count());
    }
  }

 private:
  std::chrono::steady_clock::time_point start_time_;
  std::function<void(decltype(std::declval<TTime>().count()))>
      destroy_callback_ = nullptr;
};

#define MG_RAII_TIMER(name, message) \
  memgraph::utils::Timer<> name(     \
      [&](auto elapsed) { spdlog::critical("{} {}s", message, elapsed); })
