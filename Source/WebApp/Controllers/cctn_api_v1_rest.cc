#include "cctn_api_v1_rest.h"
#include <juce_core/juce_core.h>

using namespace cctn::api::v1;

// Add definition of your processing function here
cctn::api::v1::rest::rest()
{
}

void cctn::api::v1::rest::init(juce::DynamicObject& dynamicObject_)
{
    this->dynamicObject_ = dynamicObject_;
}

void cctn::api::v1::rest::getInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string text)
{
    LOG_DEBUG << "Receive Parameter: " << text;

    juce::MemoryOutputStream stream;
    dynamicObject_.writeAsJSON(stream, 4, false, 6);
    const auto json_string = juce::String::createStringFromData(stream.getMemoryBlock().getData(), stream.getMemoryBlock().getSize());

    Json::Value ret;
    ret["result"] = "ok";
    ret["current_time"] = juce::Time::getCurrentTime().toISO8601(true).toStdString();
    ret["values"] = "";

    Json::Reader reader;
    bool parsingSuccessful = reader.parse(json_string.toStdString(), ret["values"]);
    if (!parsingSuccessful)
    {
        LOG_DEBUG << "Error parsing the string";
    }

    ret["text"] = text;

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
