#pragma once

#include <drogon/drogon.h>
#include <juce_core/juce_core.h>

class HttpServerThread : public juce::Thread
{
public:
    HttpServerThread()
        : juce::Thread("Drogon Http Server Thread")
    {
    }

    virtual ~HttpServerThread() override
    {
    }

    virtual void run() override
    {
        //Set HTTP listener address and port
        drogon::app().addListener("0.0.0.0", 11111);
        //Load config file
        //drogon::app().loadConfigFile("../config.json");
        //Run HTTP framework,the method will block in the internal event loop
        drogon::app().run();
    }

    void quit()
    {
        if (drogon::app().isRunning())
        {
            drogon::app().quit();
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HttpServerThread)
};

