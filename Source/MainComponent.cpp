#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
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
}
