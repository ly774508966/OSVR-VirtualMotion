#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/TrackerInterfaceC.h>
#include <osvr/PluginKit/SkeletonInterfaceC.h>
#include <osvr/Util/Pose3C.h>
#include "VM30Data.h"
#include "Analog.h"

namespace OSVRVM30 {

    /** @brief Handles tracker and skeleton interfaces */
    class Tracker {

    public:

        Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
            Analog& analog, OSVR_DeviceInitOptions pOptions, const VM30Data& pVM30Data);
        void update();

    private:

        enum Channel {
            Elbow,
            Wrist,

            Palm,

            ThumbMeta,
            ThumbProx,
            ThumbInter,
            ThumbDist,

            IndexMeta,
            IndexProx,
            IndexInter,
            IndexDist,

            MiddleMeta,
            MiddleProx,
            MiddleInter,
            MiddleDist,

            RingMeta,
            RingProx,
            RingInter,
            RingDist,

            PinkyMeta,
            PinkyProx,
            PinkyInter,
            PinkyDist,

            ChannelsPerHand //must be the last element
        };

        const VM30Data& mVM30Data;
        const osvr::pluginkit::DeviceToken& mDeviceToken;
        OSVR_TrackerDeviceInterface mTrackerInterface;
        OSVR_SkeletonDeviceInterface mSkeletonInterface;
        Analog& mAnalog;

        bool sendGlove(uint32 pGloveIndex, OSVR_ChannelCount pSkeletonChannel, OSVR_TimeValue pTimestamp);
        void sendPose(Channel pChannel, bool pIsLeft, const double pVM30Quat[4]);
        OSVR_Quaternion getOsvrQuaternion(const double pVM30Quat[4], bool pIsLeft);

    };

}