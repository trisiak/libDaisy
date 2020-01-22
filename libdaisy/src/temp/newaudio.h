#pragma once
#ifndef DSY_AUDIO_H
#define DSY_AUDIO_H

namespace daisy {

// Interleaved Audio
typedef void (*AudioCallback)(float*, float*, size_t);

class AudioHandle
{
  public:
    // Actually same as sai->init for right now...
    enum Peripheral
    {
        AUDIO_PERIPHERAL_INTERNAL,
        AUDIO_PERIPHERAL_EXTERNAL,
        AUDIO_PERIPHERAL_BOTH,
    };
    enum SampleRate
    {
        SR_32K,
        SR_48K,
        SR_96K,
        SR_192K,
    };
    enum  BitDepth
    {
        BITDEPTH_16,
        BITDEPTH_24,
        BITDEPTH_32,
    };
    AudioHandle() {};
    ~AudioHandle() {};
    void Init(Peripheral per, dsy_sai_handle* sai, dsy_i2c_handle* i2c_dev0, dsy_i2c_handle* i2c_dev1);
    void Start();
    void Stop();
    void SetCallback(AudioCallback cb);
    void SetPassThru();
    void SetSilence();
    float SampleRate();
  private:
    dsy_sai_handle *sai_;
    dsy_i2c_handle *i2c_dev0_, *i2c_dev1_;
    size_t block_size_, channels_, bitdepth_;
    SampleRate sr_;
};
} // namespace daisy

#endif
