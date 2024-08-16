#ifndef FLEET_LOGGER_H
#define FLEET_LOGGER_H

#define BOOST_LOG_DYN_LINK 1

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <sstream>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

inline void init_logging(std::string logfile, unsigned char log_level, long long log_size)
{
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

    logging::add_file_log(
            keywords::file_name = logfile,
            keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] [%ProcessID%] [%LineID%] %Message%",
            keywords::open_mode = std::ios_base::app,
            keywords::auto_flush = true,
            keywords::rotation_size = log_size,
            keywords::time_based_rotation = logging::sinks::file::rotation_at_time_point(0,0,0)
    );

    switch (log_level)
    {
        case 0:
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::trace);
            break;
        case 1:
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
            break;
        case 2:
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
            break;
        case 3:
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
            break;
        case 4:
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::error);
            break;
        case 5:
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::fatal);
            break;
        default:
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
    };

    logging::add_common_attributes();
}

constexpr inline const char* __filename(const char* path)
{
    const char* file = path;
    while (*path) {
        if (*path++ == '/') {
            file = path;
        }
    }
    return file;
}

inline std::string srcref(const char* filename, int line)
{
    std::stringstream ss;
    ss << "[" << filename << ":" << line << "] ";
    return ss.str();
}

#define __FILENAME__ __filename(__FILE__)
#define __SRCREF__ srcref(__FILENAME__, __LINE__)

#endif //FLEET_LOGGER_H
