
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
		//애님인스턴스로넘길구조체
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
	FORCEINLINE const FFeetData& GetData() { return Data; } //데이터넣을인라인함수

private:
	void Trace(FName InSocketName, float& OutDistance); //라인트레이스 소켓이름을가진게 공중에떠있는정도를리턴받을거임

protected:
	UPROPERTY(EditAnywhere, Category = "IK")
		FName LeftFootSocket = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "IK")
		FName RightFootSocket = "Foot_R";

	UPROPERTY(EditAnywhere, Category = "IK")
		float AddLength = 55.f; // 라인트레이스연장선길이
	UPROPERTY(EditAnywhere, Category = "IK")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;//선보이기에디트에서설정할수있게 1바이트로설정
	UPROPERTY(EditAnywhere, Category = "IK")
		float AdJustHeight = 5.f; //발목에서트레이스재서 보정값주는거

private:
	FFeetData Data; 
		
	class ACharacter* OwnerCharacter; //오너캐릭터

	float CapsuleHalfHeight; //캡슐반크기저장받을거

};
