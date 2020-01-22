// # Audio
// ## Description
// Class for accessing the Audio Hardware.
//
// Both the built-in, and any external audio can be accessed here.
//
// The Built In Audio will use the SAI1 peripheral on the Daisy Seed,
// while external audio can be connected to the SAI2 Peripheral.
//
// Previously, INT/EXT needed to be specified for everything, but now
// an object can be created separately for each if desired.
//
// Several Samplerates are available, but not all devices support all samplerates.
// The onboard WM8731 only supports up to 96kHz (but the firmware currently only supports up to 48kHz)
//
// Static Buffers will be allocated in a cache-less section of RAM for DMA transfers. 
//
// ## Credit
// Written by Stephen Hensley 
//
#ifndef DSY_AUDIO_H
#define DSY_AUDIO_H
#include <stddef.h>
#include <stdint.h>
#include "per_sai.h"
#include "per_i2c.h"

#define DSY_AUDIO_DMA_BUFFER_SIZE_MAX 96
#define DSY_AUDIO_CHANNELS_MAX 2
#define DSY_AUDIO_BLOCK_SIZE_MAX (DSY_AUDIO_DMA_BUFFER_SIZE_MAX / 2)

// TODO Fix hard coding of these parameters
#ifndef DSY_AUDIO_SAMPLE_RATE
//#define SAMPLE_RATE 47916.0f
//#define SAMPLE_RATE 31941.0f
//#define SAMPLE_RATE 48014.0f
#define DSY_AUDIO_SAMPLE_RATE 48014.0f
//#define SAMPLE_RATE 95820.0f
#endif


namespace daisy
{
enum AudioPeripheral
{
    DSY_AUDIO_INTERNAL,
    DSY_AUDIO_EXTERNAL,
};

typedef void (*dsy_audio_callback)(float*, float*, size_t);

class Audio
{
  public:
    // ### SampleRate
    // Available Sample Rates, not all devices support all samplerates.
    enum SampleRate
    {
        SR_32K,
        SR_48K,
        SR_96K,
        SR_192K,
        SR_LAST,
    };
    Audio() {}
    ~Audio() {}
    void Init(dsy_sai_handle* sai,
              dsy_i2c_handle* i2c_dev0,
              dsy_i2c_handle* i2c_dev1);

    void SetCallback(AudioPeripheral intext, dsy_audio_callback cb);
    void SetBlockSize(AudioPeripheral intext, size_t blocksize);

    void Start(AudioPeripheral intext);

    // Stops transmitting/receiving audio.
    void Stop(AudioPeripheral intext);

    // Returns the real sample rate of the audio engine.
    float SampleRate();

    // Default Callbacks
    void audio_passthru(float* in, float* out, size_t size);
    void audio_silence(float* in, float* out, size_t size);

  private:
    float           real_samplerate_;
    size_t          block_size_, offset_, dma_size_, channels_;
    dsy_sai_handle* sai_;
    dsy_i2c_handle *dev0_i2c_, *dev1_i2c;
    // Stuff from the old, separate struct within .c
    dsy_audio_callback callback_;
    int32_t *          dma_rx_, *dma_tx_;
    float              in_[DSY_AUDIO_BLOCK_SIZE_MAX], out_[DSY_AUDIO_BLOCK_SIZE_MAX];
    uint32_t           bitdepth_, device_;
};
} // namespace daisy

#ifdef __cplusplus
extern "C"
{
#endif


    // Thinking about getting rid of this...

    // If initialized to a single channel, its just that.
    // If both initialized, then you get a quad callback.
    enum
    {
        DSY_AUDIO_INTERNAL,
        DSY_AUDIO_EXTERNAL,
        DSY_AUDIO_LAST,
    };

    typedef struct
    {
        size_t          block_size;
        dsy_sai_handle* sai;
        dsy_i2c_handle* dev0_i2c;
        dsy_i2c_handle* dev1_i2c;
    } dsy_audio_handle;

    typedef void (*dsy_audio_callback)(float*, float*, size_t);
    void dsy_audio_init(dsy_audio_handle* handle);
    void dsy_audio_set_callback(uint8_t intext, dsy_audio_callback cb);
    void dsy_audio_set_blocksize(uint8_t intext, size_t blocksize);

    void dsy_audio_start(uint8_t intext);

    // Stops transmitting/receiving audio.
    void dsy_audio_stop(uint8_t intext);

    // If the device supports hardware bypass, enter that mode.
    void dsy_audio_enter_bypass(uint8_t intext);

    // If the device supports hardware bypass, exit that mode.
    void dsy_audio_exit_bypass(uint8_t intext);

    // Default Callbacks
    void dsy_audio_passthru(float* in, float* out, size_t size);
    void dsy_audio_silence(float* in, float* out, size_t size);

#ifdef __cplusplus
}
#endif
#endif