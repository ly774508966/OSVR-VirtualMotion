#include "Configure.h"

using namespace OSVRVirtualMotion;
using namespace ConfigureKey;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Configure::Configure(const osvr::pluginkit::DeviceToken& pDeviceToken, 
					OSVR_DeviceInitOptions pOptions, const VirtualMotionData& pVirtualMotionData) : 
					mDeviceToken(pDeviceToken), mVirtualMotionData(pVirtualMotionData)/*,mConfigInterface(NULL)*/ {
	//osvrDeviceConfigConfig(pOptions, &mConfigInterface);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
inline bool isMatch(const std::string& pKeyA, const std::string& pKeyB) {
	return (pKeyA.compare(pKeyB) == 0);
}
