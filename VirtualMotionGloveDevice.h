#include <osvr/PluginKit/PluginKit.h>
#include "VirtualMotionData.h"
#include "Analog.h"
#include "Tracker.h"
#include "Configure.h"

namespace OSVRVirtualMotion {

    class VirtualMotionGloveDevice {

    public:

        VirtualMotionGloveDevice(OSVR_PluginRegContext pContext, const std::vector<VML_GLOVE*>& gloves);
        ~VirtualMotionGloveDevice();
        OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);
        OSVR_ReturnCode update();

    private:

        std::vector<VML_GLOVE*> mGloves;
        VirtualMotionData* mVirtualMotionData;
        Analog* mAnalog;
        Tracker* mTracker;
        Configure* mConfigure;
        osvr::pluginkit::DeviceToken mDeviceToken;

        void initDevice(OSVR_PluginRegContext pContext);

    };

}
