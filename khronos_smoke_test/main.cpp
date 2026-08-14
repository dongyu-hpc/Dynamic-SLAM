#include <iostream>
#include <thread>
#include <config_utilities/config_utilities.h>
#include <khronos/active_window/active_window.h>

int main() {
  khronos::ActiveWindow::Config config;

  unsigned int n_threads = std::thread::hardware_concurrency();
  if (n_threads == 0) n_threads = 4;

  config.projective_integrator.num_threads = n_threads;
  config.tracking_integrator.num_threads = n_threads;
  config.mesh_integrator.integrator_threads = n_threads;

  try {
    config::checkValid(config);
    std::cout << "Khronos ActiveWindow::Config is valid!" << std::endl;
    std::cout << config::toString(config) << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Config validation failed: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
