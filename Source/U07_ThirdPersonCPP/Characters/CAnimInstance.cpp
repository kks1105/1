#include "CAnimInstance.h"
#include "Global.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	APawn* ownerPawn = TryGetPawnOwner();
	CheckNull(ownerPawn);

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(ownerPawn);
	CheckNull(actionComp);

	actionComp->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* ownerPawn = TryGetPawnOwner();
	CheckNull(ownerPawn);

	Speed = ownerPawn->GetVelocity().Size2D();
	Direction = CalculateDirection(ownerPawn->GetVelocity(), ownerPawn->GetControlRotation());

	UCFeetComponent* feetComp = CHelpers::GetComponent<UCFeetComponent>(ownerPawn);//오너의 핏컴포넌트가져오기
	CheckNull(feetComp); 

	FeetData = feetComp->GetData(); //애님인스턴스 데이터에 가져온데이더넘겨주기

}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}

