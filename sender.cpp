#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>

int main() {
  const char *fifo_path = "/tmp/my_fifo";

  // Create the FIFO if it doesn't exist
  mkfifo(fifo_path, 0666);

  int fd = open(fifo_path, O_WRONLY);
  if (fd < 0) {
    std::cerr << "Failed to open FIFO for writing\n";
    return 1;
  }

  int counter = 0;
  while (true) {
    std::string message = "Message " + std::to_string(counter++);
    write(fd, message.c_str(), message.size() + 1); // +1 for null terminator
    std::cout << "Sent: " << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  close(fd);
  return 0;
}
