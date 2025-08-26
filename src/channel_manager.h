#ifndef CHANNEL_MANAGER_H
#define CHANNEL_MANAGER_H

#include "fpga.h"

// Channel state management
enum ChannelState {
  CHANNEL_OFF = 0,
  CHANNEL_ON = 1
};

struct Channel {
  int selectedChannel;        // 1 or 2
  ChannelState ch1State;      // ON/OFF state for channel 1
  ChannelState ch2State;      // ON/OFF state for channel 2
  uint64_t ch1Frequency; // Frequency in millihertz for channel 1
  uint64_t ch2Frequency; // Frequency in millihertz for channel 2
  uint32_t ch1Amplitude;  // Amplitude in millivolts for channel 1
  uint32_t ch2Amplitude;  // Amplitude in millivolts for channel 2
  int32_t ch1Offset;      // Offset in millivolts for channel 1
  int32_t ch2Offset;      // Offset in millivolts for channel 2
  word ch1Phase;          // Phase in degrees for channel 1
  word ch2Phase;          // Phase in degrees for channel 2
  WaveformType ch1waveform;
  WaveformType ch2waveform;
};

// Global channel manager - extern declaration for use in other files
extern Channel channelManager;

// Accessor function declarations
int getSelectedChannel();
int getChannel1State();
int getChannel2State();
uint64_t getChannelFrequency(int channel);
uint32_t getChannelAmplitude(int channel);
int32_t getChannelOffset(int channel);
word getChannelPhase(int channel);
WaveformType getChannelWaveform(int channel);

// Channel control function declarations
void handleChannelKey(int channel);
void updateChannelLEDs();
void setFrequency(int channel, uint64_t frequencyMHz);
void setAmplitude(int channel, uint32_t amplitude);
void setOffset(int channel, int32_t offset);
void setPhase(int channel, word phase);
void setChannelWaveform(int channel, WaveformType waveform);
const char* waveformToString(WaveformType waveform);


// Accessor functions for channel state
int getSelectedChannel() { return channelManager.selectedChannel; }
int getChannel1State() { return channelManager.ch1State; }
int getChannel2State() { return channelManager.ch2State; }

uint64_t getChannelFrequency(int channel) {
  return (channel == 1) ? channelManager.ch1Frequency : channelManager.ch2Frequency;
}

uint32_t getChannelAmplitude(int channel) {
  return (channel == 1) ? channelManager.ch1Amplitude : channelManager.ch2Amplitude;
}

int32_t getChannelOffset(int channel) {
  return (channel == 1) ? channelManager.ch1Offset : channelManager.ch2Offset;
}

word getChannelPhase(int channel) {
  return (channel == 1) ? channelManager.ch1Phase : channelManager.ch2Phase;
}

WaveformType getChannelWaveform(int channel) {
  return (channel == 1) ? channelManager.ch1waveform : channelManager.ch2waveform;
}

// Set frequency for a specific channel (frequency in millihertz)
void setFrequency(int channel, uint64_t frequencyMHz) {
  if (channel == 1) {
    channelManager.ch1Frequency = frequencyMHz;
  } else if (channel == 2) {
    channelManager.ch2Frequency = frequencyMHz;
  }
  // TODO: Add actual hardware frequency setting here
 
}

void setAmplitude(int channel, uint32_t amplitude) {
  if (channel == 1) {
    channelManager.ch1Amplitude = amplitude;
  } else if (channel == 2) {
    channelManager.ch2Amplitude = amplitude;
  }
  // TODO: Add actual hardware amplitude setting here

}

void setOffset(int channel, int32_t offset) {
  if (channel == 1) {
    channelManager.ch1Offset = offset;
  } else if (channel == 2) {
    channelManager.ch2Offset = offset;
  }
}

void setPhase(int channel, word phase) {
  if (channel == 1) {
    channelManager.ch1Phase = phase;
  } else if (channel == 2) {
    channelManager.ch2Phase = phase;
  }
}

void setChannelWaveform(int channel, WaveformType waveform) {
  if (channel == 1) {
    channelManager.ch1waveform = waveform;
  } else if (channel == 2) {
    channelManager.ch2waveform = waveform;
  }
}

const char* waveformToString(WaveformType waveform) {
    switch (waveform) {
        case WAVE_SINE:     return "Sine";
        case WAVE_SQUARE:   return "Square";
        case WAVE_TRIANGLE: return "Triangle";
        case WAVE_SAW:      return "Sawtooth";
        case WAVE_CUSTOM1:  return "Custom 1";
        case WAVE_CUSTOM2:  return "Custom 2";
        default:            return "Unknown";
    }
}


#endif