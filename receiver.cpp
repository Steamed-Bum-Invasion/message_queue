#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <queue>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>

int main() {
  const char *fifo_path = "/tmp/my_fifo";

  // Create the FIFO if it doesn't exist
  mkfifo(fifo_path, 0666);

  int fd = open(fifo_path, O_RDONLY);
  if (fd < 0) {
    std::cerr << "Failed to open FIFO for reading\n";
    return 1;
  }

  std::queue<std::string> message_queue;

  char buffer[1024];
  while (true) {
    ssize_t bytes = read(fd, buffer, sizeof(buffer));
    if (bytes > 0) {
      buffer[bytes] = '\0';
      std::string msg(buffer);
      message_queue.push(msg);
    }

    // Process messages
    while (!message_queue.empty()) {
      std::string front = message_queue.front();
      message_queue.pop();
      std::cout << "Received: " << front << std::endl;
    }
  }

  close(fd);
  return 0;
}
