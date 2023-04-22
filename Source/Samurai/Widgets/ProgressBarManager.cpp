#include "ProgressBarManager.h"

#include "Components/ProgressBar.h"

void UProgressBarManager::SetBar(float Value) {
	if(!Progressbar) return;

	Progressbar->SetPercent(Value);
}
