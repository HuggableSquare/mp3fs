/*
 * Logging class header for mp3fs
 *
 * Copyright (C) 2017 K. Henriksson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef MP3FS_LOGGING_H_
#define MP3FS_LOGGING_H_

#include <cstdarg>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

class Logging {
 public:
    enum class level { INVALID = 0, ERROR = 1, INFO = 2, DEBUG = 3 };

    /*
     * Arguments:
     *   logfile: The name of a file to write logging output to. If empty, no
     *     output will be written.
     *   max_level: The maximum level of log output to write.
     *   to_stderr: Whether to write log output to stderr.
     *   to_syslog: Whether to write log output to syslog.
     */
    explicit Logging(std::string logfile, level max_level, bool to_stderr,
                     bool to_syslog);

    bool GetFail() const { return logfile_.fail(); }

 private:
    class Logger : public std::ostringstream {
     public:
        Logger(level loglevel, Logging* logging)
            : loglevel_(loglevel), logging_(logging) {}
        Logger() = default;
        ~Logger() override;

     private:
        const level loglevel_ = level::DEBUG;

        Logging* logging_ = nullptr;

        static const std::map<level, int> syslog_level_map_;
        static const std::map<level, std::string> level_name_map_;
    };

    friend Logger Log(level lev);
    friend Logger;

    std::ofstream logfile_;
    const level max_level_;
    const bool to_stderr_;
    const bool to_syslog_;
};

Logging::level StringToLevel(std::string level);

bool InitLogging(std::string logfile, Logging::level max_level, bool to_stderr,
                 bool to_syslog);

constexpr auto ERROR = Logging::level::ERROR;
constexpr auto INFO = Logging::level::INFO;
constexpr auto DEBUG = Logging::level::DEBUG;

void log_with_level(Logging::level level, const char* prefix,
                    const char* format, va_list ap);

#endif  // MP3FS_LOGGING_H_
