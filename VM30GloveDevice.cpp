#include "VM30GloveDevice.h"
#include "org_osvr_VM30_json.h"

using namespace OSVRVM30;
using namespace osvr::pluginkit;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
VM30GloveDevice::VM30GloveDevice(OSVR_PluginRegContext pContext, const std::vector<VML_GLOVE*>& gloves) :
    mGloves(gloves),
    mVM30Data(NULL), mAnalog(NULL), mTracker(NULL), mConfigure(NULL)
{

	OSVR_DeviceInitOptions options = osvrDeviceCreateInitOptions(pContext);

	mVM30Data = new VM30Data(gloves);
	mAnalog = new Analog(mDeviceToken, options, *mVM30Data);
	mTracker = new Tracker(mDeviceToken, *mAnalog, options, *mVM30Data);
	mConfigure = new Configure(mDeviceToken, options, *mVM30Data);

	mDeviceToken.initAsync(pContext, "Controller", options);
	mDeviceToken.sendJsonDescriptor(org_osvr_VM30_json);
	mDeviceToken.registerUpdateCallback(this);
}

/*----------------------------------------------------------------------------------------------------*/
VM30GloveDevice::~VM30GloveDevice() {
	delete mVM30Data;
	delete mAnalog;
	delete mTracker;
	delete mConfigure;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode VM30GloveDevice::update() {
	mVM30Data->update();
	mAnalog->update();
	mTracker->update();
	return OSVR_RETURN_SUCCESS;
}
