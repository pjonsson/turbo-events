#ifndef PRINTOUTPUT_HPP
#define PRINTOUTPUT_HPP

#include "turboevents-internal.hpp"

#include <iostream>

namespace TurboEvents {

/// Event that prints its data to standard output.
template <typename T> class PrintEvent : public Event {
public:
  /// Constructor
  PrintEvent(std::chrono::system_clock::time_point t, T data)
      : Event(t), d(data) {}

  /// Destructor
  virtual ~PrintEvent() override {}

  /// Prints the data to standard output.
  void trigger() const override { std::cout << d << "\n"; }

private:
  const T d; ///< Value to print
};

/// Output object that creates PrintObject
class PrintOutput : public Output {
public:
  /// Constructor
  PrintOutput(bool timeshift) : Output(timeshift) {}
  /// Destructor
  virtual ~PrintOutput() override {}

  /// Make an event that prints a string
  std::unique_ptr<Event> makeEvent(std::chrono::system_clock::time_point t,
                                   std::string data) override {
    return std::make_unique<PrintEvent<std::string>>(t, data);
  }

  /// Make an event that prints an int
  std::unique_ptr<Event> makeEvent(std::chrono::system_clock::time_point t,
                                   int data) override {
    return std::make_unique<PrintEvent<int>>(t, data);
  }
};

} // namespace TurboEvents
#endif
