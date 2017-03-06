#include <osvr/PluginKit/PluginKit.h>
#include "HardwareDetection.h"

OSVR_PLUGIN(org_osvr_VM30) {
	osvr::pluginkit::PluginContext context(ctx);
	context.registerHardwareDetectCallback(new OSVRVM30::HardwareDetection());
	return OSVR_RETURN_SUCCESS;
}
