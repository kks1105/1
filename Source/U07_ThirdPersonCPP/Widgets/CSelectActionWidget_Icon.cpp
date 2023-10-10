

#include "CSelectActionWidget_Icon.h"
#include "Global.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "Characters/CPlayer.h"
#include "CSelectActionWidget_Group.h"


void UCSelectActionWidget_Icon::NativeConstruct()
{
	Super::NativeConstruct();


	CheckNull(ImageButton);
	ImageButton->OnPressed.AddDynamic(this, &UCSelectActionWidget_Icon::OnPressed);
	ImageButton->OnHovered.AddDynamic(this, &UCSelectActionWidget_Icon::OnHover);
	ImageButton->OnUnhovered.AddDynamic(this, &UCSelectActionWidget_Icon::OnUnhover);

}

void UCSelectActionWidget_Icon::OnPressed()
{
	CheckNull(GetParentWidget());
	GetParentWidget()->Pressed(GetName());
}

void UCSelectActionWidget_Icon::OnHover()
{
	CheckNull(GetParentWidget());
	GetParentWidget()->Hover(GetName());//그룹에있는호버실행

}

void UCSelectActionWidget_Icon::OnUnhover()
{
	CheckNull(GetParentWidget());
	GetParentWidget()->Unhover(GetName());//그룹에있는언호버실행
}

void UCSelectActionWidget_Icon::SetTextureToImageButton(UTexture2D* InImage)
{
	CheckNull(ImageButton);
	ImageButton->WidgetStyle.Normal.SetResourceObject(InImage);//기본이미지 에넣기
	ImageButton->WidgetStyle.Hovered.SetResourceObject(InImage);//호버이미지
	ImageButton->WidgetStyle.Pressed.SetResourceObject(InImage);//누르기이미지



}

UCSelectActionWidget_Group* UCSelectActionWidget_Icon::GetParentWidget()
{
	ACPlayer* player = Cast<ACPlayer>(GetOwningPlayer()->GetPawn());
	CheckNullResult(player, nullptr);

	return player->GetSelectActionWidget();
	
}
