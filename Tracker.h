#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/TrackerInterfaceC.h>
#include <osvr/PluginKit/SkeletonInterfaceC.h>
#include <osvr/Util/Pose3C.h>
#include "VirtualMotionData.h"
#include "Analog.h"

namespace OSVRVirtualMotion {

    /** @brief Handles tracker and skeleton interfaces */
    class Tracker {

    public:

        Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
            Analog& analog, OSVR_DeviceInitOptions pOptions, const VirtualMotionData& pVirtualMotionData);
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

        const VirtualMotionData& mVirtualMotionData;
        const osvr::pluginkit::DeviceToken& mDeviceToken;
        OSVR_TrackerDeviceInterface mTrackerInterface;
        OSVR_SkeletonDeviceInterface mSkeletonInterface;
        Analog& mAnalog;

        bool sendGlove(uint32 pGloveIndex, OSVR_ChannelCount pSkeletonChannel, OSVR_TimeValue pTimestamp);
        void sendPose(Channel pChannel, bool pIsLeft, const double pVirtualMotionQuat[4]);
        OSVR_Quaternion getOsvrQuaternion(const double pVirtualMotionQuat[4], bool pIsLeft);

    };

}