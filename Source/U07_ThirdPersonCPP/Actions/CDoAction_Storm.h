
#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Storm.generated.h"

UCLASS()
class U07_THIRDPERSONCPP_API ACDoAction_Storm : public ACDoAction
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() override; //부모오버라이드 버튼누를시
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
	virtual void Abort() override; //맞으면실행되는거

public:
	virtual void OnBeginOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override; //부모꺼오버라이드


	virtual void OnEndOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override;

private:
	UFUNCTION()
		void TickDamage();//셋타이머이벤트 딜리게이트 스톰cpp
	UFUNCTION()
		void Finish();//셋타이머딜리게이트이벤트


private:
	UPROPERTY(EditDefaultsOnly)
		float DamagePerTime= 0.01f;//대미지주는시간

	UPROPERTY(EditDefaultsOnly)
		float ActiveTime = 5.f; //지속시간

	UPROPERTY(EditDefaultsOnly)
		float AroundSpeed = 200.f; //도는속도

	UPROPERTY(EditDefaultsOnly)
		float Radius = 150.f; //도는범위

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle; //파티클



private:
	class UBoxComponent* Box; //어타치먼트박스저장할거

	TArray<class ACharacter*> HittedCharacters; //다단히트체크용배열

	float CurrentYaw;//계산용

	bool bActivating;//활성화여부용


};
