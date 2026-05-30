#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "libs/miniaudio/miniaudio.h"
namespace Boo
{

	class AudioAsset : public Asset
	{
	private:
        std::vector<uint8_t> _audioBuffer;
		float _totalSeconds;

	public:
		AudioAsset();
		AudioAsset(std::string uuid);
		AudioAsset(std::string uuid, std::string path, std::string name);
        void create(std::vector<uint8_t> &audioBuffer);
		const float& getTotalSeconds();
		std::vector<uint8_t>& getAudioBuffer();
		void destroy() override;
		~AudioAsset() {};
	};

} // namespace Boo