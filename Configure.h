#include <osvr/PluginKit/PluginKit.h>
//#include <osvr/PluginKit/ConfigInterfaceC.h>
#include "VM30Data.h"

namespace OSVRVM30 {

    class Configure {

    public:

        Configure(const osvr::pluginkit::DeviceToken& pDeviceToken,
            OSVR_DeviceInitOptions pOptions, const VM30Data& pVM30Data);

    private:

        const VM30Data& mVM30Data;
        const osvr::pluginkit::DeviceToken& mDeviceToken;
        //OSVR_ConfigDeviceInterface mConfigInterface;
    };

    namespace ConfigureKey {

    }

}