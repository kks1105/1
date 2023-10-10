
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSelectActionWidget_Group.generated.h"


UCLASS()
class U07_THIRDPERSONCPP_API UCSelectActionWidget_Group : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
		void Pressed(FString InName);

	UFUNCTION(BlueprintCallable)
		void Hover(FString InName);

	UFUNCTION(BlueprintCallable)
		void Unhover(FString InName);

	
public:
	FORCEINLINE class UCSelectActionWidget_Icon* GetChildWidget(FString InName) { return IconWidgets[InName]; }

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UTexture2D*> Images; //이미지관리할거블프에서처리

	UPROPERTY(BlueprintReadOnly)
		TMap<FString, class UCSelectActionWidget_Icon*> IconWidgets; //아이콘 맵으로관리 맵은키와밸류와 나열



private:
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* Grid; //위젯의그리드이름

	

};
