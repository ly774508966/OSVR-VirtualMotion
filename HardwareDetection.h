#include <osvr/PluginKit/PluginKit.h>

namespace OSVRVirtualMotion {
	
	class HardwareDetection {

		public:

			HardwareDetection();
			OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);

		private:

			bool mFound;

	};

}
