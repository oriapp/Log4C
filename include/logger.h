#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_TAGS 10
#define MAX_TAG_LENGTH 20

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
    char *file_path;             // Log file path
    char *date_format;           // Date format for log entries
    char *prefix;                // Custom log message prefix
    int log_to_file;             // Flag indicating whether logging to file is enabled (1) or not (0)
    int include_thread_id;       // Flag indicating whether to include thread ID in log messages (1) or not (0)
    int include_process_id;      // Flag indicating whether to include process ID in log messages (1) or not (0)
    char tags[MAX_TAGS][MAX_TAG_LENGTH]; // Array to store tags
    int num_tags;                // Number of tags currently stored
};

/**
 * @brief Initializes the logger.
 * 
 * @param logger Pointer to the logger structure.
 * @param console_level Minimum log level for console output.
 * @param file_level Minimum log level for file output.
 * @param file_path Path to the log file.
 * @param date_format Custom date format for log entries (optional, set to NULL for default format).
 * @param log_to_file Flag indicating whether logging to file is enabled (1) or not (0).
 * @param include_thread_id Flag indicating whether to include thread ID in log messages (1) or not (0).
 * @param include_process_id Flag indicating whether to include process ID in log messages (1) or not (0).
 */
void init_logger(struct Logger *logger, enum LogLevel console_level, enum LogLevel file_level, const char *file_path, const char *date_format, int log_to_file, int include_thread_id, int include_process_id) {
    logger->console_level = console_level;
    logger->file_level = file_level;
    logger->file_path = strdup(file_path); // Dynamic memory allocation
    logger->date_format = date_format ? strdup(date_format) : strdup("%Y-%m-%d %H:%M:%S"); // Dynamic memory allocation
    logger->file = NULL;
    logger->prefix = strdup("");
    logger->log_to_file = log_to_file;
    logger->include_thread_id = include_thread_id;
    logger->include_process_id = include_process_id;
    logger->num_tags = 0;

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
    free(logger->prefix); // Free previous memory
    logger->prefix = strdup(prefix); // Dynamic memory allocation
}

/**
 * @brief Sets the log file path and opens the log file.
 * 
 * @param logger Pointer to the logger structure.
 * @param file_path Path to the new log file.
 */
void set_log_file(struct Logger *logger, const char *file_path) {
    free(logger->file_path); // Free previous memory
    logger->file_path = strdup(file_path); // Dynamic memory allocation
    if (logger->file)
        fclose(logger->file);
    logger->file = fopen(logger->file_path, "a");
    if (!logger->file) {
        fprintf(stderr, "Error opening log file %s\n", logger->file_path);
    }
}

/**
 * @brief Sets the log levels for console and file output.
 * 
 * @param logger Pointer to the logger structure.
 * @param console_level Minimum log level for console output.
 * @param file_level Minimum log level for file output.
 */
void set_log_levels(struct Logger *logger, enum LogLevel console_level, enum LogLevel file_level) {
    logger->console_level = console_level;
    logger->file_level = file_level;
}

/**
 * @brief Sets the date format for log entries.
 * 
 * @param logger Pointer to the logger structure.
 * @param date_format Custom date format for log entries.
 */
void set_date_format(struct Logger *logger, const char *date_format) {
    free(logger->date_format); // Free previous memory
    logger->date_format = strdup(date_format); // Dynamic memory allocation
}

/**
 * @brief Sets whether logging to file is enabled or not.
 * 
 * @param logger Pointer to the logger structure.
 * @param log_to_file Flag indicating whether logging to file is enabled (1) or not (0).
 */
void set_log_to_file(struct Logger *logger, int log_to_file) {
    logger->log_to_file = log_to_file;
}

/**
 * @brief Sets whether to include thread ID in log messages or not.
 * 
 * @param logger Pointer to the logger structure.
 * @param include_thread_id Flag indicating whether to include thread ID in log messages (1) or not (0).
 */
void set_include_thread_id(struct Logger *logger, int include_thread_id) {
    logger->include_thread_id = include_thread_id;
}

/**
 * @brief Sets whether to include process ID in log messages or not.
 * 
 * @param logger Pointer to the logger structure.
 * @param include_process_id Flag indicating whether to include process ID in log messages (1) or not (0).
 */
void set_include_process_id(struct Logger *logger, int include_process_id) {
    logger->include_process_id = include_process_id;
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

    time_t currentTime;
    struct tm *timeInfo;
    char timeBuffer[20]; // Sufficiently large buffer for date/time
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(timeBuffer, sizeof(timeBuffer), logger->date_format, timeInfo);

    const char *level_name;
    switch (level) {
        case DEBUG:
            level_name = "DEBUG";
            break;
        case INFO:
            level_name = "INFO";
            break;
        case SUCCESS:
            level_name = "SUCCESS";
            break;
        case WARNING:
            level_name = "WARNING";
            break;
        case ERROR:
            level_name = "ERROR";
            break;
        default:
            level_name = "UNKNOWN";
            break;
    }

    va_list args;
    va_start(args, format);

    printf("%s | %s %s", timeBuffer, level_name, logger->prefix);
    if (logger->include_thread_id) {
        printf(" | Thread ID: %lu", (unsigned long)pthread_self());
    }
    if (logger->include_process_id) {
        printf(" | Process ID: %d", getpid());
    }
    printf(" | ");
    vprintf(format, args);
    printf("\n");

    if (logger->log_to_file && logger->file && level >= logger->file_level) {
        fprintf(logger->file, "%s | %s %s", timeBuffer, level_name, logger->prefix);
        if (logger->include_thread_id) {
            fprintf(logger->file, " | Thread ID: %lu", (unsigned long)pthread_self());
        }
        if (logger->include_process_id) {
            fprintf(logger->file, " | Process ID: %d", getpid());
        }
        fprintf(logger->file, " | ");
        vfprintf(logger->file, format, args);
        fprintf(logger->file, "\n");
        fflush(logger->file);
    }

    va_end(args);
}

/**
 * @brief Adds a tag to the logger.
 * 
 * @param logger Pointer to the logger structure.
 * @param tag Tag to be added.
 */
void add_tag(struct Logger *logger, const char *tag) {
    // Check if the number of tags exceeds the maximum allowed
    if (logger->num_tags < MAX_TAGS) {
        strncpy(logger->tags[logger->num_tags], tag, MAX_TAG_LENGTH - 1);
        logger->tags[logger->num_tags][MAX_TAG_LENGTH - 1] = '\0';
        logger->num_tags++;
    }
}

/**
 * @brief Logs the current timestamp.
 * 
 * @param logger Pointer to the logger structure.
 * @param tag Tag associated with the timestamp.
 */
void log_timestamp(struct Logger *logger, const char *tag) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long milliseconds = (long long)(tv.tv_sec) * 1000 + (long long)(tv.tv_usec) / 1000;

    log_message(logger, DEBUG, "[%s] Timestamp: %lld ms", tag, milliseconds);
}

/**
 * @brief Rotates the log file if it exceeds a specified maximum size.
 * 
 * @param logger Pointer to the logger structure.
 * @param max_size Maximum size of the log file before rotation (in bytes).
 */
void rotate_log(struct Logger *logger, long max_size) {
    if (logger->file) {
        fseek(logger->file, 0, SEEK_END);
        long file_size = ftell(logger->file);
        if (file_size >= max_size) {
            fclose(logger->file);
            char new_file_path[strlen(logger->file_path) + 5]; // For ".old" suffix
            snprintf(new_file_path, sizeof(new_file_path), "%s.old", logger->file_path);
            rename(logger->file_path, new_file_path);
            logger->file = fopen(logger->file_path, "a");
            if (!logger->file) {
                fprintf(stderr, "Error opening log file %s\n", logger->file_path);
            }
        }
    }
}

/**
 * @brief Closes the logger and releases associated resources.
 * 
 * @param logger Pointer to the logger structure.
 */
void close_logger(struct Logger *logger) {
    if (logger->file)
        fclose(logger->file);
    free(logger->file_path);
    free(logger->date_format);
    free(logger->prefix);
}

#endif