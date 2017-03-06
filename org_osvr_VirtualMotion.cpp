#include <osvr/PluginKit/PluginKit.h>
#include "HardwareDetection.h"

OSVR_PLUGIN(org_osvr_VirtualMotion) {
	osvr::pluginkit::PluginContext context(ctx);
	context.registerHardwareDetectCallback(new OSVRVirtualMotion::HardwareDetection());
	return OSVR_RETURN_SUCCESS;
}
