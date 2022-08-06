#include "MainComponent.h"
#include "drogon/drogon.h"

//==============================================================================
MainComponent::MainComponent()
{
    //restAPI = std::make_unique<cctn::api::v1::rest>(jsonSource);
    std::dynamic_pointer_cast<cctn::api::v1::rest>(DrClassMap::getSingleInstance("cctn::api::v1::rest"))->init(this->jsonSource);

    httpServerThread = std::make_unique<HttpServerThread>();
    httpServerThread->startThread();

    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    httpServerThread->quit();
    httpServerThread->stopThread(1000);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    this->jsonSource.setProperty("Width", this->getWidth());
    this->jsonSource.setProperty("Height", this->getHeight());
    std::dynamic_pointer_cast<cctn::api::v1::rest>(DrClassMap::getSingleInstance("cctn::api::v1::rest"))->init(this->jsonSource);
}
