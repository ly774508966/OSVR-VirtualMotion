#include "Tracker.h"
#include <osvr/Util/Vec3C.h>
#include <Eigen/Geometry>

#include "org_osvr_VM30_json.h"

using namespace OSVRVM30;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Tracker::Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
    Analog& analog, OSVR_DeviceInitOptions pOptions, const VM30Data& pVM30Data) :
    mDeviceToken(pDeviceToken), mAnalog(analog), mVM30Data(pVM30Data),
    mTrackerInterface(NULL) {
    // note we have two skeleton sensors, one for each hand, each with their own spec
    // this is because the articulation specs for each hand are not connected by a parent joint
    osvrDeviceSkeletonConfigure(pOptions, &mSkeletonInterface, org_osvr_VM30_json, 2);

    osvrDeviceTrackerConfigure(pOptions, &mTrackerInterface);
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::update() {
    OSVR_TimeValue timestamp;
    osvrTimeValueGetNow(&timestamp);

    auto leftHandGloveIndex = mVM30Data.getLeftGloveIndex();
    auto rightHandGloveIndex = mVM30Data.getRightGloveIndex();

    sendGlove(leftHandGloveIndex, 0, timestamp);
    sendGlove(rightHandGloveIndex, 1, timestamp);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
bool Tracker::sendGlove(uint32 pGloveIndex, OSVR_ChannelCount pSkeletonChannel, OSVR_TimeValue pTimestamp) {
    auto glove = mVM30Data.getGlove(pGloveIndex);
    if (!glove) {
        // no error necessary, normal if glove is not plugged in
        return false;
    }

    const VML_GLOVE_SAMPLE* gloveSample = mVM30Data.getSample(pGloveIndex);
    if (!gloveSample) {
        // @todo log error?
        return false;
    }

        

    VML_GLOVE_INFO gloveInfo;
    if (!vml_glove_get_info(&gloveInfo, glove)) {
        // @todo log error?
        return false;
    }
    bool isLeft = gloveInfo.Hand == VML_GLOVE_HAND_LEFT;

    sendPose(Elbow, isLeft, gloveSample->WristQuat);
    sendPose(Wrist, isLeft, gloveSample->WristQuat);
    sendPose(Palm, isLeft, gloveSample->HandQuat);

    //for (int i = 0; i < fingerCount; i++) {
    //    sendFinger(fingers[i], isLeft);
    //}

    osvrDeviceSkeletonComplete(mSkeletonInterface, pSkeletonChannel, &pTimestamp);
    return true;
}

///*----------------------------------------------------------------------------------------------------*/
//void Tracker::sendFinger(const Finger& pFinger, bool pIsLeft) {
//    Finger::Type type = pFinger.type();
//
//    sendBone(pFinger.bone(Bone::TYPE_METACARPAL), type, pIsLeft);
//    sendBone(pFinger.bone(Bone::TYPE_PROXIMAL), type, pIsLeft);
//    sendBone(pFinger.bone(Bone::TYPE_INTERMEDIATE), type, pIsLeft);
//    sendBone(pFinger.bone(Bone::TYPE_DISTAL), type, pIsLeft);
//}

/*----------------------------------------------------------------------------------------------------*/
//void Tracker::sendBone(const Bone& pBone, Finger::Type pFingerType, bool pIsLeft) {
//    Channel channel = mChannelMap[pFingerType][pBone.type()];
//    sendPose(channel, pIsLeft, pBone.center(), pBone.basis());
//}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendPose(Channel pChannel, bool pIsLeft, const double pVM30Quat[4]) {
    OSVR_PoseState pose;
    osvrVec3Zero(&pose.translation);
    pose.rotation = getOsvrQuaternion(pVM30Quat, pIsLeft);

    OSVR_ChannelCount channel = pChannel + (pIsLeft ? 0 : ChannelsPerHand);

    osvrDeviceTrackerSendPose(mDeviceToken, mTrackerInterface, &pose, channel);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
//OSVR_Vec3 Tracker::getOsvrVector(const Vector& pVector) {
//    const float scale = 0.001f;
//
//    OSVR_Vec3 vec;
//    vec.data[0] = pVector.x*scale;
//    vec.data[1] = pVector.y*scale;
//    vec.data[2] = pVector.z*scale;
//    return vec;
//}

/*----------------------------------------------------------------------------------------------------*/
OSVR_Quaternion Tracker::getOsvrQuaternion(const double pVM30Quat[4], bool pIsLeft) {
    OSVR_Quaternion quat;
    osvrQuatSetW(&quat, pVM30Quat[0]);
    osvrQuatSetX(&quat, pVM30Quat[1]);
    osvrQuatSetY(&quat, pVM30Quat[2]);
    osvrQuatSetZ(&quat, pVM30Quat[3]);
    return quat;
}
