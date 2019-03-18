#ifndef GL_LOG_H_
#define GL_LOG_H_

namespace logger {
    bool restartGLLog();
    bool glLog(const char* message, ...);
    bool glLogOut(const char* message, ...);
    bool glLogErr(const char* message, ...);
}

#endif  // GL_LOG_H_