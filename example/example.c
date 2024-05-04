#include "../include/logger.h"
#include <unistd.h>

int main() {
    // Initialize logger
    struct Logger logger;
    init_logger(&logger, DEBUG, INFO, "app.log", NULL, 1, 1, 1);

    // Set custom log prefix
    set_log_prefix(&logger, "[APP]");

    // Log some messages
    log_message(&logger, INFO, "Application started.");
    log_message(&logger, DEBUG, "Debug message.");
    log_message(&logger, WARNING, "Warning: Something unexpected happened.");
    log_message(&logger, ERROR, "Error: Something went wrong.");

    // Simulate some processing
    sleep(2);

    // Log performance metrics
    log_message(&logger, INFO, "Processing completed.");

    // Close logger
    close_logger(&logger);

    return 0;
}
