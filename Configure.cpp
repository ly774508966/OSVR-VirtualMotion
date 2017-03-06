#include "Configure.h"

using namespace OSVRVM30;
using namespace ConfigureKey;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Configure::Configure(const osvr::pluginkit::DeviceToken& pDeviceToken, 
					OSVR_DeviceInitOptions pOptions, const VM30Data& pVM30Data) : 
					mDeviceToken(pDeviceToken), mVM30Data(pVM30Data)/*,mConfigInterface(NULL)*/ {
	//osvrDeviceConfigConfig(pOptions, &mConfigInterface);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
inline bool isMatch(const std::string& pKeyA, const std::string& pKeyB) {
	return (pKeyA.compare(pKeyB) == 0);
}
