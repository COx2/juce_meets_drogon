#pragma once

#include <drogon/HttpController.h>
#include <JuceHeader.h>

using namespace drogon;

namespace cctn
{
namespace api
{
namespace v1
{
class rest : public drogon::HttpController<rest>
{
 public:
    rest();
    void init(juce::DynamicObject& dynamicObject_);

    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(rest::get, "/{2}/{1}", Get); // path is /cctn/api/v1/rest/{arg2}/{arg1}
    // METHOD_ADD(rest::your_method_name, "/{1}/{2}/list", Get); // path is /cctn/api/v1/rest/{arg1}/{arg2}/list
    // ADD_METHOD_TO(rest::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(rest::getInfo, "/info/{1}", Get);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void getInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string text);

private:
    juce::DynamicObject dynamicObject_;
};
}
}
}
