#include "VirtualMotionGloveDevice.h"
#include "org_osvr_VirtualMotion_json.h"

using namespace OSVRVirtualMotion;
using namespace osvr::pluginkit;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
VirtualMotionGloveDevice::VirtualMotionGloveDevice(OSVR_PluginRegContext pContext, const std::vector<VML_GLOVE*>& gloves) :
    mGloves(gloves),
    mVirtualMotionData(NULL), mAnalog(NULL), mTracker(NULL), mConfigure(NULL)
{

	OSVR_DeviceInitOptions options = osvrDeviceCreateInitOptions(pContext);

	mVirtualMotionData = new VirtualMotionData(gloves);
	mAnalog = new Analog(mDeviceToken, options, *mVirtualMotionData);
	mTracker = new Tracker(mDeviceToken, *mAnalog, options, *mVirtualMotionData);
	mConfigure = new Configure(mDeviceToken, options, *mVirtualMotionData);

	mDeviceToken.initAsync(pContext, "Controller", options);
	mDeviceToken.sendJsonDescriptor(org_osvr_VirtualMotion_json);
	mDeviceToken.registerUpdateCallback(this);
}

/*----------------------------------------------------------------------------------------------------*/
VirtualMotionGloveDevice::~VirtualMotionGloveDevice() {
	delete mVirtualMotionData;
	delete mAnalog;
	delete mTracker;
	delete mConfigure;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode VirtualMotionGloveDevice::update() {
	mVirtualMotionData->update();
	mAnalog->update();
	mTracker->update();
	return OSVR_RETURN_SUCCESS;
}
