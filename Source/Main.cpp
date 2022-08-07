#include <juce_gui_basics/juce_gui_basics.h>

#include "WebApp/HttpServerThread.h"
#include "WebApp/Controllers/cctn_api_v1_rest.h"

//==============================================================================
class ConsoleApplication : public juce::JUCEApplication
{
public:
    //==============================================================================
    ConsoleApplication() {}

    // We inject these as compile definitions from the CMakeLists.txt
    // If you've enabled the juce header with `juce_generate_juce_header(<thisTarget>)`
    // you could `#include <JuceHeader.h>` and use `ProjectInfo::projectName` etc. instead.
    const juce::String getApplicationName() override       { return JUCE_APPLICATION_NAME_STRING; }
    const juce::String getApplicationVersion() override    { return JUCE_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        juce::ignoreUnused (commandLine);

        std::dynamic_pointer_cast<cctn::api::v1::rest>(DrClassMap::getSingleInstance("cctn::api::v1::rest"))->init(this->jsonSource);

        httpServerThread = std::make_unique<HttpServerThread>();
        httpServerThread->startThread();
    }

    void shutdown() override
    {
        httpServerThread->quit();
        httpServerThread->stopThread(1000);
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
        juce::ignoreUnused (commandLine);
    }

private:
    // Your private member variables go here...
    std::unique_ptr<HttpServerThread> httpServerThread;
    std::unique_ptr<cctn::api::v1::rest> restAPI;

    juce::DynamicObject jsonSource;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (ConsoleApplication)
