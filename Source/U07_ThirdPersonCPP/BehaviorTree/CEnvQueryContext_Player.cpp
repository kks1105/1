#include "CEnvQueryContext_Player.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"


void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(QueryInstance.Owner.Get()); //적가져오면->컨트롤러->behaviorcomp get player //쿼리의오너
	CheckNull(enemy);

	ACAIController* controller = Cast<ACAIController>( enemy->GetController()); //에너미의컨트롤러
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);//컨트롤러에 비헤이비어컴프
	CheckNull(behaviorComp);


	UEnvQueryItemType_Actor::SetContextHelper(ContextData,behaviorComp->GetPlayerKey());//비헤이비어컴프에서 플레이어키가져옴

}