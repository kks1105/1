
#include "CSelectActionWidget_Group.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "CSelectActionWidget_Icon.h"
#include "Components/Border.h"

void UCSelectActionWidget_Group::NativeConstruct()
{
	Super::NativeConstruct();


	//Add Item Widget to map �ʿ��ٰ� �������ִ°��߰�
	CheckNull(Grid);
	TArray<UWidget*> children = Grid->GetAllChildren();
	for (const auto& child : children)
	{
		UCSelectActionWidget_Icon* iconWidget = Cast<UCSelectActionWidget_Icon>(child); //����������Ÿ������ĳ��Ʈ
		if(!!iconWidget)
			IconWidgets.Add(child->GetName(), iconWidget);//�ʿ��ٰ��߰�


	}

	//��������ư���׸����� ������������
	//Images
	for (int32 i = 0; i < IconWidgets.Num(); i++)
	{
		FString key = "Icon"; //�׳ɾձ���
		key.Append(FString::FromInt(i + 1)); //�ձ��ڿ� ��ȣ���ϱ� ��) Icon0,Icon1 ...

		IconWidgets[key]->SetTextureToImageButton(Images[i]); //������������  �̹��� ��ȣ��μ��� icon�����̽����Լ�����

	}

}

void UCSelectActionWidget_Group::Pressed(FString InName)
{


	if (IconWidgets[InName]->OnImageButtonPressed.IsBound())
		IconWidgets[InName]->OnImageButtonPressed.Broadcast(); //icon���ִµ�������Ʈ �ٿ��

	//����������� �����ԵǸ� ��������
	SetVisibility(ESlateVisibility::Hidden);


	APlayerController* controller = Cast<APlayerController>(GetOwningPlayer());
	CheckNull(controller);

	controller->bShowMouseCursor = false;//���콺Ŀ������
	controller->SetInputMode(FInputModeGameOnly());//���Ӹ��κ���

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f); //�ð��ٽ�����ȭ



}

void UCSelectActionWidget_Group::Hover(FString InName)
{
	//IconWidgets.Find(InName);
	UBorder* border = Cast<UBorder>(IconWidgets[InName]->GetWidgetFromName("MyBorder"));//�������������� ���̺������̸��� ���𰡰�������
	CheckNull(border);
	border->SetBrushColor(FLinearColor::Red);//�����������Ǹ��̺�������

}

void UCSelectActionWidget_Group::Unhover(FString InName)
{
	//IconWidgets.Find(InName);
	UBorder* border = Cast<UBorder>(IconWidgets[InName]->GetWidgetFromName("MyBorder"));//�������������� ���̺������̸��� ���𰡰�������
	CheckNull(border);
	border->SetBrushColor(FLinearColor::White); //�����������Ǹ��̺��������

}


