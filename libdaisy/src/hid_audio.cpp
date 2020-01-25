#include <string.h>
#include "stm32h7xx_hal.h"
#include "hid_audio.h"
#include "daisy.h"
#include "dev_codec_wm8731.h"


namespace daisy
{
static void get_audio_from_sai(SAI_HandleTypeDef *hsai);
// Sai Callback
static void
internal_callback(int32_t *in, int32_t *out, size_t size, size_t sai_chn);
// Audio Callbacks
static void silence_callback(float *in, float *out, size_t size);
static void passthru_callback(float *in, float *out, size_t size);


struct AudioPeripheral
{
    void Init(dsy_i2c_handle *        dev,
              size_t                  block_size,
              size_t                  channels,
              AudioHandle::BitDepth   bd,
              AudioHandle::SampleRate sr)
    {
        i2c_        = dev;
        block_size_ = block_size < DSY_AUDIO_BLOCK_SIZE_MAX
                          ? block_size
                          : DSY_AUDIO_BLOCK_SIZE_MAX;
        channels_         = channels;
        real_sample_rate_ = 48014.0f; // fixed for now.
        callback_         = silence_callback;
        dsy_i2c_init(i2c_);
    }
    dsy_i2c_handle *i2c_;
    float           real_sample_rate_;
    size_t          block_size_, bitdepth_, channels_;
    AudioCallback   callback_;
};

// Declare internal static structs
static AudioPeripheral audio_int, audio_ext;

void AudioHandle::Init(Peripheral      per,
                       dsy_sai_handle *sai,
                       dsy_i2c_handle *i2c_dev0,
                       dsy_i2c_handle *i2c_dev1)
{
    block_size_ = 48;
    bd_         = AudioHandle::BITDEPTH_16;
    sr_         = AudioHandle::SR_48K;
    channels_   = 2;
    per_        = per;
    dsy_sai_init_from_handle(sai);
    // Set sai callbacks
    dsy_sai_set_callback(sai, DSY_SAI_1, internal_callback);
    dsy_sai_set_callback(sai, DSY_SAI_2, internal_callback);
    switch(per)
    {
        case AUDIO_PERIPHERAL_INTERNAL:
            audio_int.Init(i2c_dev0, block_size_, channels_, bd_, sr_);
            break;
        case AUDIO_PERIPHERAL_EXTERNAL:
            audio_ext.Init(i2c_dev1, block_size_, channels_, bd_, sr_);
            break;
        case AUDIO_PERIPHERAL_BOTH:
            audio_int.Init(i2c_dev0, block_size_, channels_, bd_, sr_);
            audio_ext.Init(i2c_dev1, block_size_, channels_, bd_, sr_);
            break;
        default: break;
    }
    // Fixed for right now...
    codec_wm8731_init(dsy_i2c_hal_handle(audio_int.i2c_), 1, 48000, 16);
}
void AudioHandle::Start() {}
void AudioHandle::Stop() {}
void AudioHandle::SetCallback(AudioCallback cb)
{
    switch(per_)
    {
        case AUDIO_PERIPHERAL_INTERNAL: audio_int.callback_ = cb; break;
        case AUDIO_PERIPHERAL_EXTERNAL: audio_ext.callback_ = cb; break;
        case AUDIO_PERIPHERAL_BOTH:
            audio_int.callback_ = cb;
            audio_ext.callback_ = cb;
            break;
    }
}
void AudioHandle::SetPassThru()
{
    switch(per_)
    {
        case AUDIO_PERIPHERAL_INTERNAL:
            audio_int.callback_ = passthru_callback;
            break;
        case AUDIO_PERIPHERAL_EXTERNAL:
            audio_ext.callback_ = passthru_callback;
            break;
        case AUDIO_PERIPHERAL_BOTH:
            audio_int.callback_ = passthru_callback;
            audio_ext.callback_ = passthru_callback;
            break;
    }
}
void AudioHandle::SetSilence()
{
    switch(per_)
    {
        case AUDIO_PERIPHERAL_INTERNAL:
            audio_int.callback_ = silence_callback;
            break;
        case AUDIO_PERIPHERAL_EXTERNAL:
            audio_ext.callback_ = silence_callback;
            break;
        case AUDIO_PERIPHERAL_BOTH:
            audio_int.callback_ = silence_callback;
            audio_ext.callback_ = silence_callback;
            break;
    }
}
float SampleRate()
{
    return 48014.0f; // fix hard coding.
}

static void
internal_callback(int32_t *in, int32_t *out, size_t size, size_t sai_chn)
{
#ifdef DSY_PROFILE_AUDIO_CALLBACK
    HAL_GPIO_WritePin(PROFILE_GPIO_PORT, PROFILE_GPIO_PIN, 1);
#endif
    float        in_flt[size];
    float        out_flt[size];
    float *      inf  = in_flt;
    const float *endi = (inf + size);
    // TODO add 24-bit support back in.
    while(inf != endi)
    {
        *inf++ = s162f(*in++);
    }
    if(sai_chn == DSY_SAI_1)
    {
        audio_int.callback_(in_flt, out_flt, size);
    }
    else
    {
        audio_ext.callback_(in_flt, out_flt, size);
    }
    const float *outf = out_flt;
    const float *endo = (out_flt + size);
    while(outf != endo)
    {
        *out++ = f2s16(*outf++);
    }
#ifdef DSY_PROFILE_AUDIO_CALLBACK
    HAL_GPIO_WritePin(PROFILE_GPIO_PORT, PROFILE_GPIO_PIN, 0);
#endif
}

static void silence_callback(float *in, float *out, size_t size)
{
    memset(out, 0, sizeof(float) * size);
}
static void passthru_callback(float *in, float *out, size_t size)
{
    memcpy(out, in, sizeof(float) * size);
}

} // namespace daisy

