#include <iostream>
#include <systemc.h>
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>

void test_quill() {
    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);
    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>(
        "sink_id_1", quill::ConsoleColours::ColourMode::Automatic);
    // quill::Logger* logger = quill::Frontend::create_or_get_logger("root", std::move(console_sink));
    // quill::Logger* logger = quill::Frontend::create_or_get_logger(
    //     "root", std::move(console_sink),
    //     quill::PatternFormatterOptions{ "%(time) [%(thread_id)] %(short_source_location:<28) "
    //                                     "LOG_%(log_level:<9) %(logger:<12) %(message)",
    //                                     "%H:%M:%S.%Qns", quill::Timezone::GmtTime });
    quill::Logger* logger = quill::Frontend::create_or_get_logger(
        "root", std::move(console_sink),
        quill::PatternFormatterOptions{ "[%(file_name)][%(log_level)]      %(message)" });
    logger->set_log_level(quill::LogLevel::TraceL3);
    int a = 123;
    std::string l = "log";
    LOG_INFO(logger, "A {} message with number {}", l, a);
}

int sc_main(int argc, char* argv[]) {
    test_quill();
    sc_start();
    return 0;
}
