#include "Analog.h"

using namespace OSVRVirtualMotion;
using namespace OSVRVirtualMotion::AnalogChannels;

inline double getNormalizedSensorValue(uint16 bendValue) {
    if (bendValue > 1000) {
        return 1.0;
    }
    return static_cast<double>(bendValue) / 1000.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Analog::Analog(const osvr::pluginkit::DeviceToken& pDeviceToken,
    OSVR_DeviceInitOptions pOptions, const VirtualMotionData& pVirtualMotionData) :
    mDeviceToken(pDeviceToken), mVirtualMotionData(pVirtualMotionData), mAnalogInterface(NULL) {
    osvrDeviceAnalogConfigure(pOptions, &mAnalogInterface, AnalogChannelCount);
}

/*----------------------------------------------------------------------------------------------------*/
void Analog::update() {

    //mValues[IsDeviceConnected] = (controller.isConnected() ? 1 : 0);
    //mValues[IsServiceConnected] = (controller.isServiceConnected() ? 1 : 0);
    for (int i = 0; i < AnalogChannelCount; i++) {
        mValues[0] = 0.0;
    }

    mValues[IsLeftHandAvailable] = mVirtualMotionData.hasLeftGlove() ? 1.0 : 0.0;
    mValues[IsRightHandAvailable] = mVirtualMotionData.hasRightGlove() ? 1.0 : 0.0;

    {
        auto leftGloveSample = mVirtualMotionData.getLeftGloveSample();
        if (leftGloveSample) {
            mValues[RightLittleProximalBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[0][0]);
            mValues[RightLittleIntermediateBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[0][1]);
            mValues[RightRingProximalBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[1][0]);
            mValues[RightRingIntermediateBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[1][1]);
            mValues[RightMiddleProximalBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[2][0]);
            mValues[RightMiddleIntermediateBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[2][1]);
            mValues[RightIndexProximalBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[3][0]);
            mValues[RightIndexIntermediateBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[3][1]);
            mValues[RightThumbProximalBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[4][0]);
            mValues[RightThumbDistalBend] = getNormalizedSensorValue(leftGloveSample->FingerBend[4][1]);

            mValues[LeftLittleRingAbduction] = getNormalizedSensorValue(leftGloveSample->FingerAbduction[0]);
            mValues[LeftRingMiddleAbduction] = getNormalizedSensorValue(leftGloveSample->FingerAbduction[1]);
            mValues[LeftMiddleIndexAbduction] = getNormalizedSensorValue(leftGloveSample->FingerAbduction[2]);
            mValues[LeftIndexThumbAbduction] = getNormalizedSensorValue(leftGloveSample->FingerAbduction[3]);

            mValues[LeftLittleFingerPressure] = getNormalizedSensorValue(leftGloveSample->FingerPressure[0]);
            mValues[LeftRingFingerPressure] = getNormalizedSensorValue(leftGloveSample->FingerPressure[1]);
            mValues[LeftMiddleFingerPressure] = getNormalizedSensorValue(leftGloveSample->FingerPressure[2]);
            mValues[LeftIndexFingerPressure] = getNormalizedSensorValue(leftGloveSample->FingerPressure[3]);
            mValues[LeftThumbFingerPressure] = getNormalizedSensorValue(leftGloveSample->FingerPressure[4]);
        }
    }

    {
        auto rightGloveSample = mVirtualMotionData.getRightGloveSample();
        if (rightGloveSample) {
            mValues[RightLittleProximalBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[4][0]);
            mValues[RightLittleIntermediateBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[4][1]);
            mValues[RightRingProximalBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[3][0]);
            mValues[RightRingIntermediateBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[3][1]);
            mValues[RightMiddleProximalBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[2][0]);
            mValues[RightMiddleIntermediateBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[2][1]);
            mValues[RightIndexProximalBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[1][0]);
            mValues[RightIndexIntermediateBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[1][1]);
            mValues[RightThumbProximalBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[0][0]);
            mValues[RightThumbDistalBend] = getNormalizedSensorValue(rightGloveSample->FingerBend[0][1]);

            mValues[RightLittleRingAbduction] = getNormalizedSensorValue(rightGloveSample->FingerAbduction[3]);
            mValues[RightRingMiddleAbduction] = getNormalizedSensorValue(rightGloveSample->FingerAbduction[2]);
            mValues[RightMiddleIndexAbduction] = getNormalizedSensorValue(rightGloveSample->FingerAbduction[1]);
            mValues[RightIndexThumbAbduction] = getNormalizedSensorValue(rightGloveSample->FingerAbduction[0]);

            mValues[RightLittleFingerPressure] = getNormalizedSensorValue(rightGloveSample->FingerPressure[4]);
            mValues[RightRingFingerPressure] = getNormalizedSensorValue(rightGloveSample->FingerPressure[3]);
            mValues[RightMiddleFingerPressure] = getNormalizedSensorValue(rightGloveSample->FingerPressure[2]);
            mValues[RightIndexFingerPressure] = getNormalizedSensorValue(rightGloveSample->FingerPressure[1]);
            mValues[RightThumbFingerPressure] = getNormalizedSensorValue(rightGloveSample->FingerPressure[0]);
        }
    }

    osvrDeviceAnalogSetValues(mDeviceToken, mAnalogInterface, mValues, AnalogChannelCount);
}

double Analog::getValue(AnalogChannels::AnalogChannel channel) const {
    if (channel >= 0 && channel < AnalogChannelCount) {
        return mValues[channel];
    }
    // @todo: throw exception?
    return 0.0;
}

