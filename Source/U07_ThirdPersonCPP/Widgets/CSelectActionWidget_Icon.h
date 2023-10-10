
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSelectActionWidget_Icon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FImageButtonPressedSignature);

UCLASS()
class U07_THIRDPERSONCPP_API UCSelectActionWidget_Icon : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
		void OnPressed();

	UFUNCTION(BlueprintCallable)
		void OnHover();

	UFUNCTION(BlueprintCallable)
		void OnUnhover();

public:
	void SetTextureToImageButton(class UTexture2D* InImage); //버튼에이미지추가

private:
	class UCSelectActionWidget_Group* GetParentWidget();

public:
	UPROPERTY(BlueprintAssignable)
		FImageButtonPressedSignature OnImageButtonPressed;//딜리게이트

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ImageButton;

	
};
