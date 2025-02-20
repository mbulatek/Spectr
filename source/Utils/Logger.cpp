//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include <Utils/Logger.h>
#include <conf.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <chrono>

namespace MaroVST {

void SetupLogger() {
  std::shared_ptr<spdlog::logger> logger;
  try {
    logger = spdlog::basic_logger_mt("global_logger", Config::LOG_FILENAME);
  } catch (spdlog::spdlog_ex e) {
    //! Logger already initialized
    return;
  }

  spdlog::set_default_logger(logger);
  spdlog::set_level(Config::LOG_LEVEL);
  spdlog::info("Logger initialized");
  spdlog::flush_every(std::chrono::milliseconds(200));
}

}  // namespace MaroVST
