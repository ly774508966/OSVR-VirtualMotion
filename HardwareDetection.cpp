#include "HardwareDetection.h"
#include "VirtualMotionGloveDevice.h"
#include <vector>

using namespace OSVRVirtualMotion;

#define MAX_NUM_GLOVES 4

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
HardwareDetection::HardwareDetection() : mFound(false) {
    //do nothing...
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode HardwareDetection::operator()(OSVR_PluginRegContext pContext) {

    VML_GLOVE* gloves[MAX_NUM_GLOVES];
    uint32 numGlovesFound = 0;
    if (!vml_glove_scan_usb(&numGlovesFound, gloves, MAX_NUM_GLOVES) || numGlovesFound == 0) {
        mFound = false;
        return OSVR_RETURN_FAILURE;
    }

    if (!mFound) {
        mFound = true;
        std::vector<VML_GLOVE*> vGloves;
        for (uint32 i = 0; i < numGlovesFound; i++) {
            vGloves.push_back(gloves[i]);
        }
        osvr::pluginkit::registerObjectForDeletion(pContext, new VirtualMotionGloveDevice(pContext, vGloves));
    }

    return OSVR_RETURN_SUCCESS;
}
