
#include "Global.h"
#include "CPlayerHealthUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Characters/CPlayer.h"
#include "Components/CStatusComponent.h"


void UCPlayerHealthUserWidget::NativeConstruct()
{
	//��Ƽ��������������
	CheckNull(CircleGuage);
	Material = CircleGuage->GetDynamicMaterial();
	CheckNull(Material);
	//�÷��̾�����
	ACPlayer* player = Cast<ACPlayer>(GetOwningPlayer()->GetPawn());
	CheckNull(player);

	//�������ͽ�������Ʈ
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(player);
	CheckNull(StatusComp);

	//��Ƽ������ �������̸��Ķ���� ���� �������ͽ�������Ʈ�� ����ü��/�ƚ�ü��
	Material->SetScalarParameterValue("Ratio", StatusComp->GetCurrentHealth() / StatusComp->GetMaxHealth());

	//Ŀ��Ʈ�ｺ�� ��Ʈ��(�Ҽ����ȳ�����)ĳ��Ʈ�ϰ� ������Ŀ��Ʈ��Ʈ�ؽ�Ʈ��  �װɷμ���
	CheckNull(CurrentHealthText);
	FString currentHealthStr = FString::FromInt( (int32)StatusComp->GetCurrentHealth());
	CurrentHealthText->SetText(FText::FromString(currentHealthStr));


	Super::NativeConstruct();

	

}

void UCPlayerHealthUserWidget::UpdateHealth_Implementation()
{
	//1.�������������̰�
	CheckNull(Material);
	CheckNull(StatusComp);

	//��Ƽ������ �������̸��Ķ���� ���� �������ͽ�������Ʈ�� ����ü��/�ƚ�ü��
	Material->SetScalarParameterValue("Ratio", StatusComp->GetCurrentHealth() / StatusComp->GetMaxHealth());
	//2.�ؽ�Ʈ�ǽð�����
	CheckNull(CurrentHealthText);
	FString currentHealthStr = FString::FromInt((int32)StatusComp->GetCurrentHealth());
	CurrentHealthText->SetText(FText::FromString(currentHealthStr));

	PlayAnimationForward(DecreaseImpact);

	

}
