#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>

#define MAX_FILES 100
#define TEMP_LOG_DURATION 900 // 15 minutes in seconds
#define CONFIG_FILES_PATH "/etc/"
#define TEMP_LOG_PATH "/tmp/valkyrieshield.log"

// Structure to hold file integrity information
typedef struct {
    char *filename;
    char checksum[65]; // For SHA256
} FileIntegrity;

// Global variables
FileIntegrity monitoredFiles[MAX_FILES];
int monitoredFileCount = 0;

// Function prototypes
void logMessage(const char *message);
void monitorFileIntegrity(const char *filename);
void auditConfigurationFiles();
void detectAnomalies();
void monitorUserActivity();
void createTemporaryLog();
void alertUser(const char *message);
void cleanUpLogs();

// Helper function to calculate SHA256 checksum
void calculateChecksum(const char *filename, char *checksum) {
    char command[128];
    snprintf(command, sizeof(command), "sha256sum %s | awk '{print $1}'", filename);
    FILE *fp = popen(command, "r");
    if (fp != NULL) {
        fgets(checksum, 65, fp); // SHA256 produces a 64 char hash + '\0'
        pclose(fp);
    }
}

// Function to log messages to the temp log file
void logMessage(const char *message) {
    FILE *logFile = fopen(TEMP_LOG_PATH, "a");
    if (logFile) {
        fprintf(logFile, "%s\n", message);
        fclose(logFile);
    }
}

// Monitor file integrity by checking checksums
void monitorFileIntegrity(const char *filename) {
    char currentChecksum[65];
    calculateChecksum(filename, currentChecksum);

    for (int i = 0; i < monitoredFileCount; i++) {
        if (strcmp(monitoredFiles[i].filename, filename) == 0) {
            if (strcmp(monitoredFiles[i].checksum, currentChecksum) != 0) {
                char alertMessage[256];
                snprintf(alertMessage, sizeof(alertMessage), "File modified: %s", filename);
                alertUser(alertMessage);
                return;
            }
        }
    }
    
    // If the file is not monitored, add it
    monitoredFiles[monitoredFileCount].filename = strdup(filename);
    strcpy(monitoredFiles[monitoredFileCount].checksum, currentChecksum);
    monitoredFileCount++;
}

// Audit important configuration files
void auditConfigurationFiles() {
    const char *configFiles[] = {
        "/etc/passwd",
        "/etc/shadow",
        "/etc/ssh/sshd_config",
        NULL
    };

    for (int i = 0; configFiles[i] != NULL; i++) {
        monitorFileIntegrity(configFiles[i]);
    }
}

// Detect unusual processes
void detectAnomalies() {
    FILE *fp = popen("ps aux", "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Analyze the output here for anomalies
        // For simplicity, we just log all processes
        logMessage(buffer);
    }
    pclose(fp);
}

// Monitor user activity by tracking command history
void monitorUserActivity() {
    FILE *fp = fopen("/home/yourusername/.bash_history", "r"); // Update with actual user home directory
    if (fp == NULL) {
        perror("Failed to open history file");
        return;
    }

    char command[256];
    while (fgets(command, sizeof(command), fp) != NULL) {
        logMessage(command);
    }
    fclose(fp);
}

// Create temporary log file
void createTemporaryLog() {
    FILE *fp = fopen(TEMP_LOG_PATH, "w");
    if (fp) {
        fprintf(fp, "Temporary log created at: %s\n", ctime(&(time_t){time(NULL)}));
        fclose(fp);
    }
}

// Alert the user with a message
void alertUser(const char *message) {
    fprintf(stderr, "ALERT: %s\n", message);
    logMessage(message);
}

// Clean up logs older than 15 minutes
void cleanUpLogs() {
    struct stat statbuf;
    if (stat(TEMP_LOG_PATH, &statbuf) == 0) {
        if (difftime(time(NULL), statbuf.st_mtime) > TEMP_LOG_DURATION) {
            remove(TEMP_LOG_PATH);
        }
    }
}

int main() {
    // Create the temporary log file at the start
    createTemporaryLog();

    // Monitor integrity of critical files
    auditConfigurationFiles();

    // Continuous monitoring loop (for demonstration, run for a short duration)
    for (int i = 0; i < 5; i++) {
        detectAnomalies();
        monitorUserActivity();
        cleanUpLogs();
        sleep(60); // Run checks every minute
    }

    return 0;
}

