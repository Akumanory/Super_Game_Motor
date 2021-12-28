#pragma once
#undef max
#include <entt/entt.hpp>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace rj = rapidjson;

class RapidJsonOutputArchive {
public:
    RapidJsonOutputArchive() {
        document_.SetArray();
        current_.SetArray();
    };
    ~RapidJsonOutputArchive() {
    }

    // new element for serialization. giving you the amount of elements that is going to be
    // pumped into operator()(entt::entity ent) or operator()(entt::entity ent, const T &t)
    void operator()(std::underlying_type_t<entt::entity> size) {
        int a = 0;
        if (!current_.Empty()) {
            document_.PushBack(current_, document_.GetAllocator());
        }
        current_ = rj::Value(rj::kArrayType);
        current_.PushBack(size, document_.GetAllocator()); // first element of each array keeps the amount of elements.
    }

    // persist entity ids
    void operator()(entt::entity entity) {
        // Here it is assumed that no custom entt-type is chosen
        current_.PushBack((uint32_t)entity, document_.GetAllocator());
    }

    template <typename T>
    void operator()(entt::entity ent, const T& t) {
        current_.PushBack((uint32_t)ent, document_.GetAllocator()); // persist the entity id of the following component

        // auto factory = entt::type_id<T>();
        // std::string component_name = std::string(factory.name());
        // current.push_back(component_name);

        rj::Value json(rj::kObjectType);
        t.to_json(json, document_.GetAllocator());
        current_.PushBack(json, document_.GetAllocator());
    }

    void Close() {
        if (!current_.Empty()) {
            document_.PushBack(current_, document_.GetAllocator());
        }
    }

    // create a json as string
    const std::string AsString() {
        rj::StringBuffer buffer;
        rj::PrettyWriter<rj::StringBuffer> writer(buffer);
        //rj::Writer<rj::StringBuffer> writer(buffer);
        document_.Accept(writer);
        return buffer.GetString();
    }

private:
    rj::Document document_;
    rj::Value current_;
};

class RapidJsonInputArchive {
public:
    RapidJsonInputArchive(std::string_view json_sv) {
        document_.Parse(json_sv.data(), json_sv.size());
    }
    RapidJsonInputArchive(std::istream& json_is) {
        rj::IStreamWrapper isw{ json_is };
        document_.ParseStream(isw);
    }
    ~RapidJsonInputArchive() {
    }

    void next_root() {
        root_idx_++;
        if (root_idx_ >= document_.Size()) {
            // ERROR
            return;
        }
        current_ = &document_[root_idx_];
        current_idx_ = 0;
    }

    void operator()(std::underlying_type_t<entt::entity>& s) {
        next_root();
        int size = current_->GetArray()[0].GetInt();
        current_idx_++;
        s = (std::underlying_type_t<entt::entity>)size; // pass amount to entt
    }

    void operator()(entt::entity& entity) {
        uint32_t ent = current_->GetArray()[current_idx_].GetUint();
        entity = entt::entity(ent);
        current_idx_++;
    }

    template <typename T>
    void operator()(entt::entity& ent, T& t) {
        auto& component_data = current_->GetArray()[current_idx_ * 2];
        t = T::from_json(component_data);

        uint32_t _ent = current_->GetArray()[current_idx_ * 2 - 1].GetUint();
        ent = entt::entity(_ent); // last element is the entity-id
        current_idx_++;
    }

private:
    rj::Document document_;
    rj::Value* current_ = nullptr;
    int root_idx_ = -1;
    int current_idx_ = 0;
};