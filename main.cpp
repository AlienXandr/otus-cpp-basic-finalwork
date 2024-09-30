#include "src/device_emulator/device_emulator.hpp"
#include "src/device_emulator/helpers/getRandomData.h"
#include "src/research_tests/json_test.hpp"
#include "src/server/clientList.h"
#include "src/server/server.hpp"

std::vector<IClient> clientList;
DevEmulator emulator;

int main(int argc, char *argv[]) {
  //  json_test();

  std::thread thrEmulator{&DevEmulator::run, std::ref(emulator)};

  // Check command line arguments.
  if (argc != 5) {
    std::cerr
        << "Usage: emulator_server <address> <port> <doc_root> <threads>\n"
        << "Example:\n"
        << "    emulator_server 0.0.0.0 8080 ../client 1\n";
    return EXIT_FAILURE;
  }
  auto const address = net::ip::make_address(argv[1]);
  auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
  auto const doc_root = std::make_shared<std::string>(argv[3]);
  auto const threads = std::max<int>(1, std::atoi(argv[4]));

  // The io_context is required for all I/O
  net::io_context ioc{threads};

  // Create and launch a listening port
  std::make_shared<listener>(ioc, tcp::endpoint{address, port}, doc_root)
      ->run();

  // Capture SIGINT and SIGTERM to perform a clean shutdown
  net::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait([&](beast::error_code const &, int) {
    // Stop the `io_context`. This will cause `run()`
    // to return immediately, eventually destroying the
    // `io_context` and all of the sockets in it.
    ioc.stop();
  });

  // Run the I/O service on the requested number of threads
  std::vector<std::thread> v;
  v.reserve(threads - 1);
  for (auto i = threads - 1; i > 0; --i)
    v.emplace_back([&ioc] { ioc.run(); });
  ioc.run();

  // (If we get here, it means we got a SIGINT or SIGTERM)

  // Block until all the threads exit
  for (auto &t : v)
    t.join();

  thrEmulator.join();

  return EXIT_SUCCESS;
}