#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

enum LogToFile {
    LOG_INTO_FILE = 0,
    DONT_LOG_INTO_FILE = 1,
};

enum LogLevel {
    DEBUG,
    INFO,
    SUCCESS,
    WARNING,
    ERROR
};

struct Logger {
    enum LogLevel console_level; // Minimum log level for console output
    enum LogLevel file_level;    // Minimum log level for file output
    FILE *file;                  // Log file pointer
    const char *file_path;       // Log file path
    const char *date_format;     // Date format for log entries
    const char *prefix;          // Custom log message prefix
    const char *color_codes[5];  // ANSI color codes for different log levels
    int log_to_file;             // Flag indicating whether logging to file is enabled (1) or not (0)
};

static const char* default_date_format = "%Y-%m-%d %H:%M:%S";
static const char* default_color_codes[5] = {"\033[39m", "\033[34m", "\033[32m", "\033[35m", "\033[31m"};

/**
 * @brief Initializes the logger.
 * 
 * @param logger Pointer to the logger structure.
 * @param console_level Minimum log level for console output.
 * @param file_level Minimum log level for file output.
 * @param file_path Path to the log file.
 * @param date_format Custom date format for log entries (optional, set to NULL for default format).
 * @param log_to_file Flag indicating whether logging to file is enabled (1) or not (0).
 */
void init_logger(struct Logger *logger, enum LogLevel console_level, enum LogLevel file_level, const char *file_path, const char *date_format, int log_to_file) {
    logger->console_level = console_level;
    logger->file_level = file_level;
    logger->file_path = file_path;
    logger->date_format = date_format ? date_format : default_date_format;
    logger->file = NULL;
    logger->prefix = "";
    logger->log_to_file = log_to_file;
    for (int i = 0; i < 5; ++i)
        logger->color_codes[i] = default_color_codes[i];

    if (logger->log_to_file) {
        logger->file = fopen(logger->file_path, "a");
        if (!logger->file) {
            fprintf(stderr, "Error opening log file %s\n", logger->file_path);
        }
    }
}

/**
 * @brief Sets the custom log message prefix.
 * 
 * @param logger Pointer to the logger structure.
 * @param prefix Custom log message prefix.
 */
void set_log_prefix(struct Logger *logger, const char *prefix) {
    logger->prefix = prefix;
}

/**
 * @brief Sets the log file path and opens the log file.
 * 
 * @param logger Pointer to the logger structure.
 * @param file_path Path to the new log file.
 */
void set_log_file(struct Logger *logger, const char *file_path) {
    logger->file_path = file_path;
    if (logger->file)
        fclose(logger->file);
    logger->file = fopen(logger->file_path, "a");
    if (!logger->file) {
        fprintf(stderr, "Error opening log file %s\n", logger->file_path);
    }
}

/**
 * @brief Rotates the log file if it exceeds a specified maximum size.
 * 
 * @param logger Pointer to the logger structure.
 * @param max_size Maximum size of the log file before rotation (in bytes).
 */
void rotate_log(struct Logger *logger, long max_size) {
    struct stat st;
    if (stat(logger->file_path, &st) == 0) {
        if (st.st_size >= max_size) {
            fclose(logger->file);
            char new_file_path[strlen(logger->file_path) + 5]; // For ".old" suffix
            sprintf(new_file_path, "%s.old", logger->file_path);
            rename(logger->file_path, new_file_path);
            logger->file = fopen(logger->file_path, "a");
            if (!logger->file) {
                fprintf(stderr, "Error opening log file %s\n", logger->file_path);
            }
        }
    }
}

/**
 * @brief Logs a message to the console and/or file, depending on log levels and settings.
 * 
 * @param logger Pointer to the logger structure.
 * @param level Log level of the message.
 * @param format Format string for the message.
 * @param ... Additional arguments for the format string.
 */
void log_message(struct Logger *logger, enum LogLevel level, const char *format, ...) {
    if (level < logger->console_level && level < logger->file_level)
        return;

    const char *typeColor = logger->color_codes[level];
    const char *typeStr = "";
    switch (level) {
        case DEBUG:
            typeStr = "DEBUG";
            break;
        case INFO:
            typeStr = "INFO";
            break;
        case SUCCESS:
            typeStr = "SUCCESS";
            break;
        case WARNING:
            typeStr = "WARNING";
            break;
        case ERROR:
            typeStr = "ERROR";
            break;
        default:
            break;
    }

    time_t currentTime;
    struct tm *timeInfo;
    char timeBuffer[20]; // Sufficiently large buffer for date/time
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(timeBuffer, sizeof(timeBuffer), logger->date_format, timeInfo);

    va_list args;
    va_start(args, format);

    printf("%s | \033[1m%s%s\033[21m \033[0m%s", timeBuffer, typeColor, typeStr, logger->prefix);
    vprintf(format, args);
    printf("\n");

    if (logger->log_to_file && logger->file && level >= logger->file_level) {
        fprintf(logger->file, "%s | %s %s", timeBuffer, typeStr, logger->prefix);
        vfprintf(logger->file, format, args);
        fprintf(logger->file, "\n");
        fflush(logger->file);
    }

    va_end(args);
}

/**
 * @brief Closes the logger and releases associated resources.
 * 
 * @param logger Pointer to the logger structure.
 */
void close_logger(struct Logger *logger) {
    if (logger->file)
        fclose(logger->file);
}

#endif
