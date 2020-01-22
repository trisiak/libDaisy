#include "newaudio.h"

#define DSY_AUDIO_DMA_MEM_SECTION __attribute__((section(".sram1_bss")))
#define DSY_AUDIO_CHANNELS_MAX 2
#define DSY_AUDIO_DMA_BUFFER_SIZE_MAX 96
#define DSY_AUDIO_DMA_BLOCK_SIZE_MAX (DSY_AUDIO_DMA_BUFFER_SIZE_MAX / 2 / DSY_AUDIO_CHANNELS_MAX)

// Externs for HAL stuff
extern DMA_HandleTypeDef hdma_sai1_a;
extern DMA_HandleTypeDef hdma_sai1_b;
extern DMA_HandleTypeDef hdma_sai2_a;
extern DMA_HandleTypeDef hdma_sai2_b;

// Static Functions for internal use
static void get_audio_from_sai(SAI_HandleTypeDef* hsai);
static void internal_callback(SAI_HandleTypeDef* hsai, size_t offset);
static void silence_callback(float *in, float *out, size_t);
static void passthru_callback(float *in, float *out, size_t);

static int32_t DSY_AUDIO_DMA_MEM_SECTION sai1_dma_buffer_rx[DSY_AUDIO_DMA_BUFFER_SIZE_MAX];
static int32_t DSY_AUDIO_DMA_MEM_SECTION sai1_dma_buffer_tx[DSY_AUDIO_DMA_BUFFER_SIZE_MAX];
static int32_t DSY_AUDIO_DMA_MEM_SECTION sai2_dma_buffer_rx[DSY_AUDIO_DMA_BUFFER_SIZE_MAX];
static int32_t DSY_AUDIO_DMA_MEM_SECTION sai2_dma_buffer_tx[DSY_AUDIO_DMA_BUFFER_SIZE_MAX];

namespace daisy {

struct AudioPeripheral
{
    void Init(dsy_sai_handle *sai, dsy_i2c_handle *dev, size_t block_size, size_t bitdepth) 
    {
        sai_ = sai;
        i2c_ = dev;
        block_size_ = block_size < DSY_AUDIO_BLOCK_SIZE_MAX ? block_size : DSY_AUDIO_BLOCK_SIZE_MAX;
        real_sample_rate_ = 48014.0f; // fixed for now.    
        offset_ = 0;
        dma_size_ = block_size_ * 2 * channels_;
    }
    dsy_sai_handle *sai_;
    dsy_i2c_handle *i2c_;
    float real_sample_rate_;
    size_t offset_, dma_size_, block_size_, bitdepth_;
    int32_t *dma_rx_, *dma_tx_;
    float in_[DSY_AUDIO_BLOCK_SIZE_MAX], out_[DSY_AUDIO_BLOCK_SIZE_MAX];
};

// Declare internal static structs
static AudioPeripheral audio_int, audio_ext;

void Init(Peripheral per, dsy_sai_handle* sai, dsy_i2c_handle* i2c_dev0, dsy_i2c_handle* i2c_dev1)
{

    switch(per)
    {
        case AUDIO_PERIPHERAL_INTERNAL:
            audio_int.Init(); 
            break;
        case AUDIO_PERIPHERAL_EXTERNAL:
            audio_ext.Init(); 
            break;
        case AUDIO_PERIPHERAL_BOTH:
            audio_int.Init(); 
            audio_ext.Init(); 
            break;
        default:
            break;
    }
}
void Start()
{
}
void Stop()
{
}
void SetCallback(AudioCallback cb)
{
}
void SetPassThru()
{
}
void SetSilence()
{
}
float SampleRate()
{
}

} // namespace daisy

static void silence_callback(float *in, float *out, size_t size)
{
    memset(out, 0, sizeof(float)*size)
}
static void passthru_callback(float *in, float *out, size_t size);
{
    memcpy(out, in, sizeof(float)*size)
}

// HAL Function Prototype Handling for IRQs
extern "C"
{
void DMA1_Stream0_IRQ_Handler(void)
{
    HAL_DMA_IRQHandler(&hdma_sai1_a);
}
void DMA1_Stream1_IRQ_Handler(void)
{
    HAL_DMA_IRQHandler(&hdma_sai1_b);
}
void DMA1_Stream3_IRQ_Handler(void)
{
    HAL_DMA_IRQHandler(&hdma_sai2_a);
}
void DMA1_Stream4_IRQ_Handler(void)
{
    HAL_DMA_IRQHandler(&hdma_sai2_b);
}
} // extern C
