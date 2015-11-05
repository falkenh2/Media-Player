#include <string>
#include "AudioData.h"

using namespace std;

class coder{

public:
    static AudioData * decode(string filename);
    static void encode(AudioData * data, string filename);
    static short ** seperateChannels(short * data, int channels, int numberOfSamples);
    static short * combineChannels(short ** channelData, int channels, int numberOfSamples);
};
