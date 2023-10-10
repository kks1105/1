
#include "Global.h"
#include "CPlayerHealthUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Characters/CPlayer.h"
#include "Components/CStatusComponent.h"


void UCPlayerHealthUserWidget::NativeConstruct()
{
	//마티리얼가져오고저장
	CheckNull(CircleGuage);
	Material = CircleGuage->GetDynamicMaterial();
	CheckNull(Material);
	//플레이어의폰
	ACPlayer* player = Cast<ACPlayer>(GetOwningPlayer()->GetPawn());
	CheckNull(player);

	//스테이터스컴포넌트
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(player);
	CheckNull(StatusComp);

	//마티리얼의 레쇼라는이름파라미터 세팅 스테이터스컴포넌트의 현재체력/맥슽체력
	Material->SetScalarParameterValue("Ratio", StatusComp->GetCurrentHealth() / StatusComp->GetMaxHealth());

	//커런트헬스를 인트로(소수점안나오게)캐스트하고 위젯의커런트헬트텍스트를  그걸로세팅
	CheckNull(CurrentHealthText);
	FString currentHealthStr = FString::FromInt( (int32)StatusComp->GetCurrentHealth());
	CurrentHealthText->SetText(FText::FromString(currentHealthStr));


	Super::NativeConstruct();

	

}

void UCPlayerHealthUserWidget::UpdateHealth_Implementation()
{
	//1.실제게이지줄이고
	CheckNull(Material);
	CheckNull(StatusComp);

	//마티리얼의 레쇼라는이름파라미터 세팅 스테이터스컴포넌트의 현재체력/맥슽체력
	Material->SetScalarParameterValue("Ratio", StatusComp->GetCurrentHealth() / StatusComp->GetMaxHealth());
	//2.텍스트실시간반응
	CheckNull(CurrentHealthText);
	FString currentHealthStr = FString::FromInt((int32)StatusComp->GetCurrentHealth());
	CurrentHealthText->SetText(FText::FromString(currentHealthStr));

	PlayAnimationForward(DecreaseImpact);

	

}
