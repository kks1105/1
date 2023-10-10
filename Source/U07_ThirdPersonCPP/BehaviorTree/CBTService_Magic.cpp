#include "CBTService_Magic.h"
#include "Global.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"

UCBTService_Magic::UCBTService_Magic()
{
	NodeName = "Magic";
}

void UCBTService_Magic::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behaviorComp);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	

	//Hitted
	if (stateComp->IsHittedMode())
	{
		behaviorComp->SetHittedMode();
		return;
	}

	//Get Player from BB
	ACPlayer* player = behaviorComp->GetPlayerKey();

	//No Perceived Player
	if (player == nullptr)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);//포커스해제 무브는안풀림 라스트는 본곳보고잇음

		behaviorComp->SetWaitMode();
		return;
	}

	UCStateComponent* playerStateComp = CHelpers::GetComponent<UCStateComponent>(player);
	if (!!playerStateComp)
	{
		if (playerStateComp->IsDeadMode())
		{
			behaviorComp->SetWaitMode();//죽은거확인되면 웨잇모드로
			return;
		}

	}

	//Perceived Player
	float distance = enemy->GetDistanceTo(player); 
	controller->SetFocus(player);//플레이어바라보기

	

	//-> Is in 워프 Range
	if (distance < controller->GetBehaviorRange())
	{
		behaviorComp->SetAvoidMode();
		return;
	}

	//-> Is in 공격 Range
	if (distance < controller->GetSightRadius())
	{
		behaviorComp->SetActionMode();
		return;
	}


}
