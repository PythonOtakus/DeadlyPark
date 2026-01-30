
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginUI.generated.h"

class UEditableTextBox;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class DEADLYPARK_API ULoginUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UButton* StartButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StartText;
	
	UPROPERTY(meta=(BindWidget))
	UButton* OptionsButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* OptionsText;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ExitButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ExitText;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* NameTextBox;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void OnStartButtonHovered();
	
	UFUNCTION()
	void OnStartButtonUnhovered();
};
