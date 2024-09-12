#pragma once

#include <chrono>

/**
 * @brief A timer class to track the elapsed time
 *
 * This class is based on std::chrono::high_resolution_clock and should be used
 * to time functions. The timer is set at creation and the time difference can
 * be obtained with the get() function. The reset() function can be used to
 * reset the timer.
 */
class timer {
public:
  /// constructor. Creates and starts the timer object
  timer() : start_(std::chrono::high_resolution_clock::now()) {}

  /// reset the timer
  void reset() { start_ = std::chrono::high_resolution_clock::now(); }

  /// return the elapsed time in seconds
  double get() {
    auto duration = std::chrono::high_resolution_clock::now() - start_;
    return std::chrono::duration_cast<std::chrono::duration<double>>(duration)
        .count();
  }

private:
  /// stores the time when this object is created
  std::chrono::high_resolution_clock::time_point start_;
};
