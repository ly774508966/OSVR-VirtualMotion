#include <osvr/PluginKit/PluginKit.h>
//#include <osvr/PluginKit/ConfigInterfaceC.h>
#include "VirtualMotionData.h"

namespace OSVRVirtualMotion {

    class Configure {

    public:

        Configure(const osvr::pluginkit::DeviceToken& pDeviceToken,
            OSVR_DeviceInitOptions pOptions, const VirtualMotionData& pVirtualMotionData);

    private:

        const VirtualMotionData& mVirtualMotionData;
        const osvr::pluginkit::DeviceToken& mDeviceToken;
        //OSVR_ConfigDeviceInterface mConfigInterface;
    };

    namespace ConfigureKey {

    }

}