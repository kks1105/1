
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
	FRotator leftRotation;
	Trace(LeftFootSocket, leftDistance, leftRotation);//���ʹ߼����̸�����ġ������ Ʈ���̽�����


	float rightDistance;//��������ġ��������
	FRotator rightRotation; //ȸ��������
	Trace(RightFootSocket, rightDistance, rightRotation);//���ϰ���ġ���� Ʈ���̽����� ,��ȸ����

	float offset = FMath::Min(leftDistance, rightDistance); //���̰�

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z,offset, DeltaTime, InterpSpeed);//�����ġ�ɼ¸�ŭ����

	Data.LeftDistance.Y = UKismetMathLibrary::FInterpTo(Data.LeftDistance.Y,(leftDistance-offset),DeltaTime,InterpSpeed);//���� ���߿���������ΰ� ���鿡��Ҵ��߸� �ø� 
	Data.RightDistance.Y = UKismetMathLibrary::FInterpTo(Data.RightDistance.Y, (rightDistance - offset), DeltaTime, InterpSpeed);//���Ͱ��� ������(���� ����ġ, ���� ������ ����ġ, �ð��� �帧)
			
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation,leftRotation ,DeltaTime, InterpSpeed); //���ʹ�ȸ��
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation,DeltaTime, InterpSpeed); //�����ʹ�ȸ��

}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance,FRotator& OutRotation)
{
	OutDistance = 0.f;//ó�������Ⱚ�����°��ʱ�ȭ
	OutRotation = FRotator::ZeroRotator;

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

	FVector impactNormal = hitResult.ImpactNormal;//�浹����� ����Ʈ�������

	float pitch = -UKismetMathLibrary::DegAtan2(impactNormal.X, impactNormal.Z); //��ġȸ�������
	float roll = UKismetMathLibrary::DegAtan2(impactNormal.Y, impactNormal.Z); //��ȸ�������
	
	pitch = FMath::Clamp(pitch, -45.f, 45.f);//��ġȸ������
	roll = FMath::Clamp(roll, -45.f, 45.f);//��ȸ������


	OutRotation=FRotator(pitch,0.f,roll);//ȸ���ѱ��
	
	DrawDebugDirectionalArrow(GetWorld(),hitResult.ImpactPoint,hitResult.ImpactPoint+impactNormal*100.f,3.f,FColor::Orange,false,-1.f,0,2.f);//�ٱ׸��� ����,����,��,�ٱ���,�ٻ�,������,�ð�,����,�β�

}

