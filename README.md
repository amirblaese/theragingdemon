# 👿 The Raging Demon
Written by Shayan Gheidi.

![Alt text](rd.PNG?raw=true "the raging demon.")

## About 

The raging demon is a distortion plugin designed to add flavor to audio.

## How does it work?

First, the data is sent to a state variable IIR filter to be either high or low passed. Next, the data can have gain added (input [dB]) and then distorted with four modes to choose from:

- 0: Applies f(x) = tanh(x/(1-s))
- 1: Applies f(x) = 2/pi arctan(pi/(2 * (1-s) * x))
- 2: Applies f(x) = x/(1+(s * x))
- 3: Applies f(x) = 0.1 * x / ((1 - s) * abs(x));

The "s" parameter adds an additional adjustable parameter for fine tuning. This controls the slope of distortion and is called "edge" in the plugin. While there is no dry/wet knob implemented, keep in mind that this plugin (or any effect) can have dry/wet associated with it by using it as a send and changing the amount sent to the send channel.

## Installation

Simply copy and paste the appropriate file (.dll for VST and .vst3 for VST3) into your DAW's plugin folder.
