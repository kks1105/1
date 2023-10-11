
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
		//�ִ��ν��Ͻ��γѱ汸��ü
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U07_THIRDPERSONCPP_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCFeetComponent();

protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE const FFeetData& GetData() { return Data; } //�����ͳ����ζ����Լ�

private:
	void Trace(FName InSocketName, float& OutDistance); //����Ʈ���̽� �����̸��������� ���߿����ִ����������Ϲ�������

protected:
	UPROPERTY(EditAnywhere, Category = "IK")
		FName LeftFootSocket = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "IK")
		FName RightFootSocket = "Foot_R";

	UPROPERTY(EditAnywhere, Category = "IK")
		float AddLength = 55.f; // ����Ʈ���̽����弱����
	UPROPERTY(EditAnywhere, Category = "IK")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;//�����̱⿡��Ʈ���������Ҽ��ְ� 1����Ʈ�μ���
	UPROPERTY(EditAnywhere, Category = "IK")
		float AdJustHeight = 5.f; //�߸񿡼�Ʈ���̽��缭 �������ִ°�

private:
	FFeetData Data; 
		
	class ACharacter* OwnerCharacter; //����ĳ����

	float CapsuleHalfHeight; //ĸ����ũ�����������

};
