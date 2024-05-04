# Logger Library

## Overview

This is a simple C library for logging messages to both the console and a log file. It provides functionality to customize log levels, log file rotation, and log message formatting.

## Features

- **Customizable Log Levels**: Define different log levels such as DEBUG, INFO, SUCCESS, WARNING, and ERROR.
- **Console and File Logging**: Log messages can be output to both the console and a specified log file.
- **Log File Rotation**: Automatically rotate log files when they exceed a specified maximum size.
- **Customizable Log Message Prefix**: Add custom prefixes to log messages for better categorization.
- **ANSI Color Support**: Different log levels can be displayed in different colors on the console for improved readability.

## Getting Started

To use the logger library in your project, follow these steps:

1. Clone or download the repository.
2. Include the `logger.h` header file in your project.
3. Initialize the logger using the `init_logger()` function, specifying the desired log levels, log file path, and other settings.
4. Use the `log_message()` function to log messages at different log levels.
5. Optionally, customize log message prefixes or set a custom log file path using the provided helper functions.
6. Close the logger using the `close_logger()` function when done.

## Usage Example

```c
#include "logger.h"

int main() {
    struct Logger logger;
    init_logger(&logger, DEBUG, ERROR, "app.log", NULL, LOG_INTO_FILE);
    
    log_message(&logger, INFO, "Initializing application...");
    log_message(&logger, WARNING, "This is a warning message.");
    log_message(&logger, ERROR, "An error occurred!");

    close_logger(&logger);
    
    return 0;
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contribution

Contributions are welcome! If you find any issues or have suggestions for improvement, please open an issue or submit a pull request.
