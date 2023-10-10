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

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(QueryInstance.Owner.Get()); //����������->��Ʈ�ѷ�->behaviorcomp get player //�����ǿ���
	CheckNull(enemy);

	ACAIController* controller = Cast<ACAIController>( enemy->GetController()); //���ʹ�����Ʈ�ѷ�
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);//��Ʈ�ѷ��� �����̺������
	CheckNull(behaviorComp);


	UEnvQueryItemType_Actor::SetContextHelper(ContextData,behaviorComp->GetPlayerKey());//�����̺���������� �÷��̾�Ű������

}