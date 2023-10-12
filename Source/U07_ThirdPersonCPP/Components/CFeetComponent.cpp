
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


	OwnerCharacter = Cast<ACharacter>(GetOwner()); //오너캐릭터저장

	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();//캡슐크기의반저장
	
}




void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;//왼쪽
	FRotator leftRotation;
	Trace(LeftFootSocket, leftDistance, leftRotation);//왼쪽발소켓이름과위치전달한 트레이스실행


	float rightDistance;//오른쪽위치받을변수
	FRotator rightRotation; //회전받을애
	Trace(RightFootSocket, rightDistance, rightRotation);//소켓과위치전달 트레이스실행 ,발회전도

	float offset = FMath::Min(leftDistance, rightDistance); //차이값

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z,offset, DeltaTime, InterpSpeed);//펠비스위치옵셋만큼변경

	Data.LeftDistance.Y = UKismetMathLibrary::FInterpTo(Data.LeftDistance.Y,(leftDistance-offset),DeltaTime,InterpSpeed);//왼쪽 공중에뜬발은냅두고 지면에닿았던발만 올림 
	Data.RightDistance.Y = UKismetMathLibrary::FInterpTo(Data.RightDistance.Y, (rightDistance - offset), DeltaTime, InterpSpeed);//위와같음 오른쪽(현재 발위치, 내가 가야할 발위치, 시간의 흐름)
			
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation,leftRotation ,DeltaTime, InterpSpeed); //왼쪽발회전
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation,DeltaTime, InterpSpeed); //오른쪽발회전

}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance,FRotator& OutRotation)
{
	OutDistance = 0.f;//처음쓰레기값들어오는거초기화
	OutRotation = FRotator::ZeroRotator;

	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName); //소켓로케이션저장
	FVector start = FVector(socketLocation.X,socketLocation.Y,OwnerCharacter->GetActorLocation().Z);//스타트위치 x:소켓의x y:소켓의 y z:오너캐릭터의 z

	
	FVector end = FVector(start.X, start.Y, start.Z -CapsuleHalfHeight-AddLength);//엔드 xy는그대로 z만 캡슐끝에서  발로 연장할길이 빼기

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);
	FHitResult hitResult; //충돌결과저장할거
	//라인트레이스 싱글
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignoreActors,DrawDebugType, hitResult,false,FLinearColor::Green,FLinearColor::Red);//월드에서,스타트지점,엔드지점, 트레이스켜지는조건(보이는위치까지),복합적트레이스키기,제외할놈(자기자신),드로우보이기설정,히트결과(충돌정보),충돌안됬을때색그린,충돌됬을때색빨강,

	CheckFalse(hitResult.IsValidBlockingHit());//충돌있을시밑으로내려감

	//라인이 땅속으로 파고든 깊이
	float undergroundLength = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();

	//발이공중에 뜬 길이(떠있을시 음수값), 지면에닿아있으면 0나옴
	OutDistance = AdJustHeight + undergroundLength - AddLength;

	FVector impactNormal = hitResult.ImpactNormal;//충돌결과의 임팩트노멀저장

	float pitch = -UKismetMathLibrary::DegAtan2(impactNormal.X, impactNormal.Z); //피치회전만들기
	float roll = UKismetMathLibrary::DegAtan2(impactNormal.Y, impactNormal.Z); //롤회전만들기
	
	pitch = FMath::Clamp(pitch, -45.f, 45.f);//피치회전제한
	roll = FMath::Clamp(roll, -45.f, 45.f);//롤회전제한


	OutRotation=FRotator(pitch,0.f,roll);//회전넘기기
	
	DrawDebugDirectionalArrow(GetWorld(),hitResult.ImpactPoint,hitResult.ImpactPoint+impactNormal*100.f,3.f,FColor::Orange,false,-1.f,0,2.f);//줄그리기 월드,시작,끝,줄길이,줄색,끝업승,시간,깊이,두께

}

