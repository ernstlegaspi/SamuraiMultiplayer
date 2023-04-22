#include "SamuraiCameraShake.h"

// please help me on how to implement this camerashake. thank you

/*

// #include "Camera/CameraShake.h"
// what should be the include of UCameraShake and UMatineeCameraShake?
// I have check the unreal engine documentation but it gives me error when I include the Camera/CameraShake.h
#include "Camera/CameraShakeBase.h"
#include "Camera/PlayerCameraManager.h"

void ASamuraiCameraShake::PlayCameraShake(TSubclassOf<UCameraShake> Shake, float Scale, ECameraShakePlaySpace::Type PlaySpace, FRotator UserPlaySpaceRot) {
    TObjectPtr<class UMatineeCameraShake> CameraShake = NewObject<UMatineeCameraShake>(this);
    CameraShake->PlayScale = Scale;
    CameraShake->OscillationDuration = 0.5f;
    CameraShake->OscillationBlendInTime = 0.1f;
    CameraShake->OscillationBlendOutTime = 0.1f;
    CameraShake->RotOscillation.Pitch.Amplitude = FMath::RandRange(1.0f, 2.0f);
    CameraShake->RotOscillation.Pitch.Frequency = FMath::RandRange(10.0f, 20.0f);
    CameraShake->RotOscillation.Yaw.Amplitude = FMath::RandRange(1.0f, 2.0f);
    CameraShake->RotOscillation.Yaw.Frequency = FMath::RandRange(10.0f, 20.0f);

    APlayerCameraManager::PlayCameraShake(CameraShake, Scale, PlaySpace, UserPlaySpaceRot);
}
*/