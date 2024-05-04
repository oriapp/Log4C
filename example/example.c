#include "../include/logger.h"

int main() {
    // Initialize logger with log level DEBUG for console, log level INFO for file,
    // log file path "log.txt", custom date format, and log to file enabled
    struct Logger logger;
    init_logger(&logger, DEBUG, INFO, "log.txt", "%Y-%m-%d %H:%M:%S", LOG_INTO_FILE);

    // Set custom log message prefix
    set_log_prefix(&logger, "[MY_APP] ");

    // Log some messages
    log_message(&logger, INFO, "This is an information message");
    log_message(&logger, DEBUG, "This is a debug message");
    log_message(&logger, ERROR, "This is an error message");

    // Rotate log file if it exceeds 1MB in size
    rotate_log(&logger, 1024 * 1024);

    // Change log file path
    set_log_file(&logger, "new_log.txt");

    // Log more messages
    log_message(&logger, SUCCESS, "This is a success message");
    log_message(&logger, WARNING, "This is a warning message");

    // Close the logger
    close_logger(&logger);

    return 0;
}
