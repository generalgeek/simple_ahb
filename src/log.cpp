#include "log.h"

quill::Logger* logger;

void Log_Init() {
    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);
    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>(
        "sink_id_1", quill::ConsoleColours::ColourMode::Automatic);
    // logger = quill::Frontend::create_or_get_logger("root", std::move(console_sink));
    logger = quill::Frontend::create_or_get_logger(
        "root", std::move(console_sink),
        quill::PatternFormatterOptions{ "%(file_name:<20) %(log_level:<10)  %(message)" });
    // :<20 表示字符串占位20个，不够就补空格
    logger->set_log_level(quill::LogLevel::TraceL3);
}
