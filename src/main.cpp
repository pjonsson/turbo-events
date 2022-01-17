#include "turboevents.hpp"

#include <gflags/gflags.h>
#include <iostream>

/// Dummy event type
class SimpleEvent : public TurboEvents::Event {
public:
  /// Constructor
  SimpleEvent(int m, std::chrono::system_clock::time_point t)
      : Event(t), n(m) {}

  /// Destructor
  virtual ~SimpleEvent() override {}

  /// Trigger
  void trigger() const override { std::cout << "SimpleEvent " << n << "\n"; }

private:
  const int n; ///< Value to print
};

/// Dummy event stream
class SimpleEventStream : public TurboEvents::EventStream {
public:
  /// Constructor
  SimpleEventStream(int m, int i = 1000)
      : EventStream(nullptr), n(m), interval(i) {
    (void)generate();
  }

  /// Generator
  bool generate() override {
    if (next != nullptr) delete next;
    if (n <= 0) return false;
    next = new SimpleEvent(n, std::chrono::system_clock::now() +
                                  std::chrono::milliseconds(interval));
    time = next->time;
    n--;
    return true;
  }

private:
  int n;              ///< How many events to generate
  const int interval; ///< Interval in ms between events
};

int main(int argc, char **argv) {
  gflags::SetUsageMessage("fast event generator");
  gflags::SetVersionString("0.1");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  auto turbo = TurboEvents::TurboEvents::create();

  std::vector<TurboEvents::Input *> inputs = {};

  for (int i = 1; i < argc; i++)
    inputs.push_back(new TurboEvents::FileInput(argv[i]));

  SimpleEventStream *es = new SimpleEventStream(5);
  SimpleEventStream *fs = new SimpleEventStream(2, 1500);

  auto *esStream = new TurboEvents::StreamInput(es);
  auto *fsStream = new TurboEvents::StreamInput(fs);

  inputs.push_back(esStream);
  inputs.push_back(fsStream);

  turbo->run(inputs);

  for (auto *input : inputs) {
    delete input;
  }

  gflags::ShutDownCommandLineFlags();
  return 0;
}
