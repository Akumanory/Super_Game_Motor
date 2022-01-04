#pragma once

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <functional>
#include <filesystem>
#include <concepts>
#include <fstream>

namespace motor {
namespace json {

namespace rj = rapidjson;
namespace fs = std::filesystem;

template <typename T>
concept Serializable = requires(T const& t, rapidjson::Value& obj, rapidjson::Document::AllocatorType& rjAllocator) {
    { T::from_json(obj) } -> std::same_as<T>;
    { t.to_json(obj, rjAllocator) } -> std::same_as<void>;
};

template <Serializable T>
auto readFile(std::filesystem::path path) -> T {
    std::ifstream file_in(path);
    std::stringstream json_input;
    json_input << file_in.rdbuf();
    auto json_sv = json_input.view();
    rj::Document doc;
    doc.Parse(json_sv.data(), json_sv.size());
    return T::from_json(doc);
}

template <Serializable T>
auto writeFile(fs::path path, T const& t) -> void {
    std::ofstream file_out(path);
    rj::StringBuffer buffer;
    rj::PrettyWriter<rj::StringBuffer> writer(buffer);
    rj::Document doc{ rj::kObjectType };
    t.to_json(doc, doc.GetAllocator());
    doc.Accept(writer);
    file_out.write(buffer.GetString(), buffer.GetSize());
}

} // namespace json
} // namespace motor