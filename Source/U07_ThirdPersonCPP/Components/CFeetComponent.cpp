
#include "CFeetComponent.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UCFeetComponent::UCFeetComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();


	OwnerCharacter = Cast<ACharacter>(GetOwner()); //����ĳ��������

	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();//ĸ��ũ���ǹ�����
	
}




void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;//����
	Trace(LeftFootSocket, leftDistance);//���ʹ߼����̸�����ġ������ Ʈ���̽�����


	float rightDistance;//��������ġ��������
	Trace(RightFootSocket, rightDistance);//���ϰ���ġ���� Ʈ���̽�����

	float offset = FMath::Min(leftDistance, rightDistance); //���̰�


	Data.LeftDistance.Y = UKismetMathLibrary::FInterpTo();
	//Data.RightDistance; Lerp(���� ����ġ, ���� ������ ����ġ, �ð��� �帧)
}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance)
{
	OutDistance = 0.f;//ó�������Ⱚ�����°��ʱ�ȭ

	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName); //���Ϸ����̼�����
	FVector start = FVector(socketLocation.X,socketLocation.Y,OwnerCharacter->GetActorLocation().Z);//��ŸƮ��ġ x:������x y:������ y z:����ĳ������ z

	
	FVector end = FVector(start.X, start.Y, start.Z -CapsuleHalfHeight-AddLength);//���� xy�±״�� z�� ĸ��������  �߷� �����ұ��� ����

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);
	FHitResult hitResult; //�浹��������Ұ�
	//����Ʈ���̽� �̱�
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignoreActors,DrawDebugType, hitResult,false,FLinearColor::Green,FLinearColor::Red);//���忡��,��ŸƮ����,��������, Ʈ���̽�����������(���̴���ġ����),������Ʈ���̽�Ű��,�����ҳ�(�ڱ��ڽ�),��ο캸�̱⼳��,��Ʈ���(�浹����),�浹�ȉ��������׸�,�浹������������,

	CheckFalse(hitResult.IsValidBlockingHit());//�浹�����ù����γ�����

	//������ �������� �İ�� ����
	float undergroundLength = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();

	//���̰��߿� �� ����(�������� ������), ���鿡��������� 0����
	OutDistance = AdJustHeight + undergroundLength - AddLength;


}

