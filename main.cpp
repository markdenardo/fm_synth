#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

// Constants
const double SAMPLE_RATE = 44100.0;
const double TWO_PI = 6.283185307179586476925286766559;
const int DURATION_SECONDS = 5;

// Function to generate a sine wave
double sineWave(double frequency, double time) {
    return sin(TWO_PI * frequency * time);
}

// Function to write WAV file
void writeWAV(const char* filename, const std::vector<int16_t>& audioData) {
    std::ofstream file(filename, std::ios::binary);

    // WAV file header
    file.write("RIFF", 4);
    uint32_t chunkSize = 36 + audioData.size() * 2;
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    uint32_t subchunk1Size = 16;
    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    uint16_t audioFormat = 1;
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    uint16_t numChannels = 1;
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    uint32_t sampleRate = static_cast<uint32_t>(SAMPLE_RATE);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    uint32_t byteRate = sampleRate * numChannels * 2;
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    uint16_t blockAlign = numChannels * 2;
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    uint16_t bitsPerSample = 16;
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    uint32_t subchunk2Size = audioData.size() * 2;
    file.write(reinterpret_cast<const char*>(&subchunk2Size), 4);

    // Audio data
    for (auto sample : audioData) {
        file.write(reinterpret_cast<const char*>(&sample), 2);
    }
}

int main() {
    double carrierFreq = 440.0; // Carrier frequency in Hz (A4 note)
    double modFreq = 220.0;     // Modulation frequency in Hz
    double modIndex = 100.0;    // Modulation index

    std::vector<int16_t> audioData;

    for (int i = 0; i < SAMPLE_RATE * DURATION_SECONDS; ++i) {
        double time = static_cast<double>(i) / SAMPLE_RATE;
        double modulator = sineWave(modFreq, time);
        double frequency = carrierFreq + modIndex * modulator;
        double sample = sineWave(frequency, time);
        audioData.push_back(static_cast<int16_t>(sample * 32767.0));
    }

    writeWAV("fm_synth.wav", audioData);

    std::cout << "WAV file generated: fm_synth.wav" << std::endl;

    return 0;
}
