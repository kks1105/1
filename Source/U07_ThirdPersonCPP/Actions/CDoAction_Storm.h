
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
	virtual void DoAction() override; //�θ�������̵� ��ư������
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
	virtual void Abort() override; //���������Ǵ°�

public:
	virtual void OnBeginOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override; //�θ𲨿������̵�


	virtual void OnEndOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override;

private:
	UFUNCTION()
		void TickDamage();//��Ÿ�̸��̺�Ʈ ��������Ʈ ����cpp
	UFUNCTION()
		void Finish();//��Ÿ�̸ӵ�������Ʈ�̺�Ʈ


private:
	UPROPERTY(EditDefaultsOnly)
		float DamagePerTime= 0.01f;//������ִ½ð�

	UPROPERTY(EditDefaultsOnly)
		float ActiveTime = 5.f; //���ӽð�

	UPROPERTY(EditDefaultsOnly)
		float AroundSpeed = 200.f; //���¼ӵ�

	UPROPERTY(EditDefaultsOnly)
		float Radius = 150.f; //���¹���

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle; //��ƼŬ



private:
	class UBoxComponent* Box; //��Ÿġ��Ʈ�ڽ������Ұ�

	TArray<class ACharacter*> HittedCharacters; //�ٴ���Ʈüũ��迭

	float CurrentYaw;//����

	bool bActivating;//Ȱ��ȭ���ο�


};
