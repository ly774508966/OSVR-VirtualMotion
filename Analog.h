#if !defined(__Analog_h__)
#define __Analog_h__
#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/AnalogInterfaceC.h>
#include "VirtualMotionData.h"

namespace OSVRVirtualMotion {

    namespace AnalogChannels {
        enum AnalogChannel {
            IsLeftHandAvailable,
            IsRightHandAvailable,

            LeftLittleProximalBend,
            LeftLittleIntermediateBend,
            LeftRingProximalBend,
            LeftRingIntermediateBend,
            LeftMiddleProximalBend,
            LeftMiddleIntermediateBend,
            LeftIndexProximalBend,
            LeftIndexIntermediateBend,
            LeftThumbProximalBend,
            LeftThumbDistalBend,

            RightLittleProximalBend,
            RightLittleIntermediateBend,
            RightRingProximalBend,
            RightRingIntermediateBend,
            RightMiddleProximalBend,
            RightMiddleIntermediateBend,
            RightIndexProximalBend,
            RightIndexIntermediateBend,
            RightThumbProximalBend,
            RightThumbDistalBend,

            LeftLittleRingAbduction,
            LeftRingMiddleAbduction,
            LeftMiddleIndexAbduction,
            LeftIndexThumbAbduction,

            RightLittleRingAbduction,
            RightRingMiddleAbduction,
            RightMiddleIndexAbduction,
            RightIndexThumbAbduction,

            LeftLittleFingerPressure,
            LeftRingFingerPressure,
            LeftMiddleFingerPressure,
            LeftIndexFingerPressure,
            LeftThumbFingerPressure,

            RightLittleFingerPressure,
            RightRingFingerPressure,
            RightMiddleFingerPressure,
            RightIndexFingerPressure,
            RightThumbFingerPressure,

            AnalogChannelCount //must be the last element
        };
    }

    class Analog {

    public:

        Analog(const osvr::pluginkit::DeviceToken& pDeviceToken,
            OSVR_DeviceInitOptions pOptions, const VirtualMotionData& pVirtualMotionData);
        void update();

        double getValue(AnalogChannels::AnalogChannel channel) const;

    private:
        double getNormalizedValue(uint32 bendValue) const;
        const VirtualMotionData& mVirtualMotionData;
        const osvr::pluginkit::DeviceToken& mDeviceToken;
        OSVR_AnalogDeviceInterface mAnalogInterface;
        OSVR_AnalogState mValues[AnalogChannels::AnalogChannelCount];
    };
}
#endif
