#include <stdio.h>
#include "../include/logger.h"

int main() {
    // Initialize the logger
    struct Logger logger;
    init_logger(&logger, DEBUG, INFO, "example.log", NULL, 1, 1, 1);

    // Set a custom log message prefix
    set_log_prefix(&logger, "[Example] ");

    // Add tags to the logger
    add_tag(&logger, "Start");
    log_timestamp(&logger, "Start");
    
    // Log messages with different log levels
    log_message(&logger, DEBUG, "This is a debug message.");
    log_message(&logger, INFO, "This is an info message.");
    log_message(&logger, SUCCESS, "This is a success message.");
    log_message(&logger, WARNING, "This is a warning message.");
    log_message(&logger, ERROR, "This is an error message.");

    // Add another tag and log the elapsed time
    add_tag(&logger, "End");
    log_timestamp(&logger, "End");

    // Close the logger
    close_logger(&logger);

    return 0;
}