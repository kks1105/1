
#include "CDoAction_Storm.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "CAttachment.h"



void ACDoAction_Storm::BeginPlay()
{
	Super::BeginPlay();

	//����ĳ���͸� ���ʷΰ������Ŭ������������
	for (AActor* child : OwnerCharacter->Children)
	{
		if (child->IsA<ACAttachment>() && child->GetActorLabel().Contains("Storm"))
		{
			 Box = CHelpers::GetComponent<UBoxComponent>(child);
			break;
		}
	}

}


void ACDoAction_Storm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(bActivating);
	
	CurrentYaw += AroundSpeed * DeltaTime; // ���¼ӵ�*��ŸŸ��

	//����Ʈ�䰡 360������
	if (FMath::IsNearlyEqual(CurrentYaw, 360.f))
		CurrentYaw = 0.f;//0����

	//��ġ
	FVector location = OwnerCharacter->GetActorLocation(); //����ĳ������ ���ͷ����̼�
	//ȸ�����۹���
	FVector awayFrom = FVector(Radius, 0, 0).RotateAngleAxis(CurrentYaw, OwnerCharacter->GetActorUpVector());//����ǵ��¹����� ��������  ȸ�������¼ӵ�*��ŸŸ�� ȸ������������

	location += awayFrom;

	Box->SetWorldLocation(location);//�����ǿ�������̼� ����

}

void ACDoAction_Storm::DoAction()
{
	Super::DoAction();

	CheckFalse(StateComp->IsIdleMode());

	CheckTrue(bActivating);
	bActivating = true;

	StateComp->SetActionMode();
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	

}

void ACDoAction_Storm::Begin_DoAction()
{
	Super::Begin_DoAction();
	
	//����ĳ������ ���� �����ǿ�
	CurrentYaw = OwnerCharacter->GetActorForwardVector().Rotation().Yaw;
	

	//��ƼŬ����
	if (!!Datas[0].Effect)
	{
		Particle = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, Box, NAME_None); //���� ������Ʈ, ��Ÿġ��Ʈ����Ʈ,��ġ
		Particle->SetRelativeLocation(Datas[0].EffectTransform.GetLocation());//��ġ����
		Particle->SetRelativeScale3D(Datas[0].EffectTransform.GetScale3D());//�����ϼ���
	}

	//oncollission
	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner()); //�ڽ��ǿ��� ��Ÿġ��Ʈ��ĳ��Ʈ������
	if (!!attachment)
		attachment->OnCollisions(); //�ݸ���Ű�� (��Ÿġ��Ʈ�� �ݸ������Ⱑ������)

	//DamagePerTime(0.1s) Timer ������ִ��ֱ�
	UKismetSystemLibrary::K2_SetTimer(this, "TickDamage", 0.1f, true);//ƽ������Լ� 0.1�ʸ��� ��Ҿ����ùݺ�


}

void ACDoAction_Storm::End_DoAction()
{
	Super::End_DoAction();

	StatusComp->SetMove();//
	StateComp->SetIdleMode();//


	FTimerDynamicDelegate onEndActiveTime; //�ؿ�������
	onEndActiveTime.BindUFunction(this, "Finish");//���ε����̸�
	UKismetSystemLibrary::K2_SetTimerDelegate(onEndActiveTime, ActiveTime, false);//��������Ʈ, �ð�,�ݺ�����


}

void ACDoAction_Storm::Abort()
{
	Super::Abort();

	Finish();
}

void ACDoAction_Storm::OnBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	HittedCharacters.AddUnique(InOtherCharacter);

}

void ACDoAction_Storm::OnEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	HittedCharacters.Remove(InOtherCharacter);


}

void ACDoAction_Storm::TickDamage()
{
	FDamageEvent damageEvent;

	for (int32 i = 0; i < HittedCharacters.Num(); i++)
	{
		UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(HittedCharacters[i]);

		//�ؿ�����3���� 1����Ʈ�翩�� Ʈ�簡�Ǽ� ��Ƽ�����ɷ� ���������������
		bool bIgnore = false;
		bIgnore |= (stateComp->IsDeadMode()); //�����ڰ�������
		bIgnore |= (HittedCharacters[i]->IsPendingKill());//����������϶�
		bIgnore |= (HittedCharacters[i] == nullptr);//�迭�̺������
		if (bIgnore) continue; //�ѱ��
		
		//�������ǾȰɸ������
		HittedCharacters[i]->TakeDamage(Datas[0].Power,damageEvent,OwnerCharacter->GetController(),this);//������ֱ� �Ŀ�, ������̺�Ʈ(���),�ν�Ƽ������,����


	}
	

}

void ACDoAction_Storm::Finish()
{
	//������Ƽ��Ÿ���Ŀ������
	//��ƼŬ��Ʈ����
	if(!!Particle)
		Particle->DestroyComponent();

	//�ݸ�������
	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	if (!!attachment)
		attachment->OffCollisions();//�ݸ�������

	//Ÿ�̸��ʱ�ȭ
	UKismetSystemLibrary::K2_ClearTimer(this, "TickDamage"); //ƽ�������Լ�Ÿ�̸� �ʱ�ȭ


	//��Ƽ�������ʱ�ȭ
	bActivating = false;
}
