#include "GL_Log.h"
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#define GL_LOG_FILE "gl.log"

namespace logger {
    bool restartGLLog() {
        FILE* file = fopen(GL_LOG_FILE, "w");
        if (!file) {
            fprintf(
                stderr,
                "ERROR: Could not open GL_LOG_FILE log file %s for writing\n",
                GL_LOG_FILE
            );
            return false;
        }
        time_t now = time(NULL);
        char* date = ctime(&now);
        fprintf(file, "GL_LOG_FILE log. Local time %s\n", date);
        fclose(file);
        return true;
    }

    bool glLog(const char* message, ...) {
        va_list argptr;
        FILE* file = fopen(GL_LOG_FILE, "a");
        if (!file) {
            fprintf(
                stderr,
                "ERROR: Could not open GL_LOG_FILE log file %s for appending\n",
                GL_LOG_FILE
            );
            return false;
        }
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);
        fclose(file);
        return true;
    }

    bool glLogOut(const char* message, ...) {
        va_list argptr;
        FILE* file = fopen(GL_LOG_FILE, "a");
        if (!file) {
            fprintf(
                stderr,
                "ERROR: Could not open GL_LOG_FILE log file %s for appending\n",
                GL_LOG_FILE
            );
            return false;
        }
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);
        va_start(argptr, message);
        vfprintf(stdout, message, argptr);
        va_end(argptr);
        fclose(file);
        return true;
    }

    bool glLogErr(const char* message, ...) {
        va_list argptr;
        FILE* file = fopen(GL_LOG_FILE, "a");
        if (!file) {
            fprintf(
                stderr,
                "ERROR: Could not open GL_LOG_FILE log file %s for appending\n",
                GL_LOG_FILE
            );
            return false;
        }
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);
        va_start(argptr, message);
        vfprintf(stderr, message, argptr);
        va_end(argptr);
        fclose(file);
        return true;
    }
}