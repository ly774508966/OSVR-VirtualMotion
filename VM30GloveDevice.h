#include <osvr/PluginKit/PluginKit.h>
#include "VM30Data.h"
#include "Analog.h"
#include "Tracker.h"
#include "Configure.h"

namespace OSVRVM30 {

    class VM30GloveDevice {

    public:

        VM30GloveDevice(OSVR_PluginRegContext pContext, const std::vector<VML_GLOVE*>& gloves);
        ~VM30GloveDevice();
        OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);
        OSVR_ReturnCode update();

    private:

        std::vector<VML_GLOVE*> mGloves;
        VM30Data* mVM30Data;
        Analog* mAnalog;
        Tracker* mTracker;
        Configure* mConfigure;
        osvr::pluginkit::DeviceToken mDeviceToken;

        void initDevice(OSVR_PluginRegContext pContext);

    };

}
