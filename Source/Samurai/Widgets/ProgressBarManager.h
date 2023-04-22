#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBarManager.generated.h"

UCLASS()
class SAMURAI_API UProgressBarManager : public UUserWidget {
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> Progressbar;

public:
	void SetBar(float Value);

};
