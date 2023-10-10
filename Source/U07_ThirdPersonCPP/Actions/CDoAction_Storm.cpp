
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

	//오너캐릭터를 오너로가진모든클래스가져오기
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
	
	CurrentYaw += AroundSpeed * DeltaTime; // 도는속도*델타타임

	//쿼렌트요가 360가까우면
	if (FMath::IsNearlyEqual(CurrentYaw, 360.f))
		CurrentYaw = 0.f;//0으로

	//위치
	FVector location = OwnerCharacter->GetActorLocation(); //오너캐릭터의 엑터로케이션
	//회전시작방향
	FVector awayFrom = FVector(Radius, 0, 0).RotateAngleAxis(CurrentYaw, OwnerCharacter->GetActorUpVector());//헤더의도는범위의 각도에서  회전각도는속도*델타타임 회전축은업벡터

	location += awayFrom;

	Box->SetWorldLocation(location);//박으의월드로케이션 설정

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
	
	//오너캐릭터의 전방 방향의요
	CurrentYaw = OwnerCharacter->GetActorForwardVector().Rotation().Yaw;
	

	//파티클스폰
	if (!!Datas[0].Effect)
	{
		Particle = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, Box, NAME_None); //스폰 컴포넌트, 어타치먼트포인트,위치
		Particle->SetRelativeLocation(Datas[0].EffectTransform.GetLocation());//위치세팅
		Particle->SetRelativeScale3D(Datas[0].EffectTransform.GetScale3D());//스케일세팅
	}

	//oncollission
	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner()); //박스의오너 어타치먼트로캐스트후저장
	if (!!attachment)
		attachment->OnCollisions(); //콜리젼키기 (어타치먼트에 콜리젼끄기가되있음)

	//DamagePerTime(0.1s) Timer 대미지주는주기
	UKismetSystemLibrary::K2_SetTimer(this, "TickDamage", 0.1f, true);//틱대미지함수 0.1초마다 취소없을시반복


}

void ACDoAction_Storm::End_DoAction()
{
	Super::End_DoAction();

	StatusComp->SetMove();//
	StateComp->SetIdleMode();//


	FTimerDynamicDelegate onEndActiveTime; //밑에넣을거
	onEndActiveTime.BindUFunction(this, "Finish");//바인드할이름
	UKismetSystemLibrary::K2_SetTimerDelegate(onEndActiveTime, ActiveTime, false);//딜리게이트, 시간,반복없음


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

		//밑에조건3개중 1개만트루여도 트루가되서 컨티뉴에걸려 대미지를주지않음
		bool bIgnore = false;
		bIgnore |= (stateComp->IsDeadMode()); //맞은자가데드모드
		bIgnore |= (HittedCharacters[i]->IsPendingKill());//삭제대기중일때
		bIgnore |= (HittedCharacters[i] == nullptr);//배열이비었을때
		if (bIgnore) continue; //넘기기
		
		//위에조건안걸리면실행
		HittedCharacters[i]->TakeDamage(Datas[0].Power,damageEvent,OwnerCharacter->GetController(),this);//대미지주기 파워, 대미지이벤트(비었),인스티게이터,코져


	}
	

}

void ACDoAction_Storm::Finish()
{
	//위에엑티브타임후에실행됨
	//파티클디스트로이
	if(!!Particle)
		Particle->DestroyComponent();

	//콜리젼끄기
	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	if (!!attachment)
		attachment->OffCollisions();//콜리젼끄기

	//타이머초기화
	UKismetSystemLibrary::K2_ClearTimer(this, "TickDamage"); //틱데미지함수타이머 초기화


	//엑티베이팅초기화
	bActivating = false;
}
