// License: BSD 2 Clause
// Copyright (C) 2015+, The LabSound Authors. All rights reserved.

#include "internal/Assertions.h"

#include "LabSound/core/Macros.h"
#include "LabSound/core/AudioBus.h"
#include "LabSound/core/Mixing.h"

#include "LabSound/extended/AudioFileReader.h"

#include "libnyquist/AudioDecoder.h"

#include <mutex>
#include <cstring>

namespace detail
{
	//拿到的是解码后的音频pcm？
    std::shared_ptr<lab::AudioBus> LoadInternal(nqr::AudioData * audioData, bool mixToMono)
    {
		//采样点总数
        size_t numSamples = audioData->samples.size();
        if (!numSamples) return nullptr;

		//采样点总数除以声道数目就是 “length”
        size_t length = int(numSamples / audioData->channelCount);
		//业务是否需要合并为单声道
        const size_t busChannelCount = mixToMono ? 1 : (audioData->channelCount);

		//采样点个float数据
        std::vector<float> planarSamples(numSamples);

        // Create AudioBus where we'll put the PCM audio data
        std::shared_ptr<lab::AudioBus> audioBus(new lab::AudioBus(busChannelCount, length));
		//采样率
        audioBus->setSampleRate((float) audioData->sampleRate);

		//立体声变为webaudio格式
        // Deinterleave stereo into LabSound/WebAudio planar channel layout
        nqr::DeinterleaveChannels(audioData->samples.data(), planarSamples.data(), length, audioData->channelCount, length);

        // Mix to mono if stereo -- easier to do in place instead of using libnyquist helper functions 更容易做到而不是使用libnyquist帮助函数
        // because we've already deinterleaved
        if (audioData->channelCount == lab::Channels::Stereo && mixToMono)
        {
            float * destinationMono = audioBus->channel(0)->mutableData();
            float * leftSamples = planarSamples.data();
            float * rightSamples = planarSamples.data() + length;

            for (size_t i = 0; i < length; i++)
            {
                destinationMono[i] = 0.5f * (leftSamples[i] + rightSamples[i]);
            }
        }
        else
        {
            for (size_t i = 0; i < busChannelCount; ++i)
            {
                std::memcpy(audioBus->channel(i)->mutableData(), planarSamples.data() + (i * length), length * sizeof(float));
            }
        }

		//删除解码后的数据缓存，这个struct里头的vector也会被释放吧
        delete audioData;

        return audioBus;
    }
}

namespace lab
{

nqr::NyquistIO nyquistFileIO;
std::mutex g_fileIOMutex; //文件io是要加锁的

std::shared_ptr<AudioBus> MakeBusFromFile(const char * filePath, bool mixToMono)
{
    std::lock_guard<std::mutex> lock(g_fileIOMutex);
    nqr::AudioData * audioData = new nqr::AudioData();
	//用来加载音频文件的
    nyquistFileIO.Load(audioData, std::string(filePath));
    return detail::LoadInternal(audioData, mixToMono);
}

std::shared_ptr<AudioBus> MakeBusFromMemory(const std::vector<uint8_t> & buffer, std::string extension, bool mixToMono)
{
    std::lock_guard<std::mutex> lock(g_fileIOMutex);
    nqr::AudioData * audioData = new nqr::AudioData();
    nyquistFileIO.Load(audioData, extension, buffer);
    return detail::LoadInternal(audioData, mixToMono);
}

} // end namespace lab
