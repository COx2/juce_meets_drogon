#include "cctn_api_v1_rest.h"
#include <juce_core/juce_core.h>

using namespace cctn::api::v1;

// Add definition of your processing function here

cctn::api::v1::rest::rest()
{
}

void cctn::api::v1::rest::getInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string text) const
{
    LOG_DEBUG << "Receive Parameter: " << text;

    Json::Value ret;
    ret["result"] = "ok";
    ret["text"] = text;
    ret["current_time"] = juce::Time::getCurrentTime().toISO8601(true).toStdString();

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
