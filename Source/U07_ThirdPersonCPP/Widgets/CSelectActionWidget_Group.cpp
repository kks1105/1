
#include "CSelectActionWidget_Group.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "CSelectActionWidget_Icon.h"
#include "Components/Border.h"

void UCSelectActionWidget_Group::NativeConstruct()
{
	Super::NativeConstruct();


	//Add Item Widget to map 맵에다가 가지고있는거추가
	CheckNull(Grid);
	TArray<UWidget*> children = Grid->GetAllChildren();
	for (const auto& child : children)
	{
		UCSelectActionWidget_Icon* iconWidget = Cast<UCSelectActionWidget_Icon>(child); //아이콘위젯타입으로캐스트
		if(!!iconWidget)
			IconWidgets.Add(child->GetName(), iconWidget);//맵에다가추가


	}

	//각위젯버튼에그림세팅 블프에서넣음
	//Images
	for (int32 i = 0; i < IconWidgets.Num(); i++)
	{
		FString key = "Icon"; //그냥앞글자
		key.Append(FString::FromInt(i + 1)); //앞글자에 번호더하기 예) Icon0,Icon1 ...

		IconWidgets[key]->SetTextureToImageButton(Images[i]); //아이콘위젯에  이미지 번호대로세팅 icon위젯이실행함수있음

	}

}

void UCSelectActionWidget_Group::Pressed(FString InName)
{


	if (IconWidgets[InName]->OnImageButtonPressed.IsBound())
		IconWidgets[InName]->OnImageButtonPressed.Broadcast(); //icon에있는딜리게이트 바운드

	//위젯누르기시 위에게되면 위젯끄기
	SetVisibility(ESlateVisibility::Hidden);


	APlayerController* controller = Cast<APlayerController>(GetOwningPlayer());
	CheckNull(controller);

	controller->bShowMouseCursor = false;//마우스커서끄기
	controller->SetInputMode(FInputModeGameOnly());//게임모드로변경

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f); //시간다시정상화



}

void UCSelectActionWidget_Group::Hover(FString InName)
{
	//IconWidgets.Find(InName);
	UBorder* border = Cast<UBorder>(IconWidgets[InName]->GetWidgetFromName("MyBorder"));//아이콘위젯안의 마이보더란이름의 무언가가져오기
	CheckNull(border);
	border->SetBrushColor(FLinearColor::Red);//아이콘위젯의마이보더색빨

}

void UCSelectActionWidget_Group::Unhover(FString InName)
{
	//IconWidgets.Find(InName);
	UBorder* border = Cast<UBorder>(IconWidgets[InName]->GetWidgetFromName("MyBorder"));//아이콘위젯안의 마이보더란이름의 무언가가져오기
	CheckNull(border);
	border->SetBrushColor(FLinearColor::White); //아이콘위젯의마이보더색흰색

}


