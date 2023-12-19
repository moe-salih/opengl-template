#pragma once
#ifndef OGL_CORE_LOG_HPP
#define OGL_CORE_LOG_HPP

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace ogl {
    struct log {
        public:
            static void init();
            inline static std::shared_ptr<spdlog::logger>& logger() { return s_logger; }
        private:
            static std::shared_ptr<spdlog::logger> s_logger;
    };

    std::shared_ptr<spdlog::logger> log::s_logger;

    void log::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_logger = spdlog::stdout_color_mt("OGL");
        s_logger->set_level(spdlog::level::trace);
    }

}

#define LOG_TRACE(...)      ::ogl::log::logger()->trace(__VA_ARGS__)
#define LOG_INFO(...)       ::ogl::log::logger()->info(__VA_ARGS__)
#define LOG_WARN(...)       ::ogl::log::logger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      ::ogl::log::logger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)   ::ogl::log::logger()->critical(__VA_ARGS__)

#define OGL_ASSERT(x, ...) { \
    if(!(x)) { \
        LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
        assert(x); \
    } \
}

#endif//OGL_CORE_LOG_HPP