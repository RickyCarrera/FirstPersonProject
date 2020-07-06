// Fill out your copyright notice in the Description page of Project Settings.


#include "SCrosshair.h"
#include "Engine/Canvas.h"

void ASCrosshair::DrawHUD()
{
	Super::DrawHUD();

	if (ensure(CrosshairTexture))
	{
		// find the center of our canvas
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half of the texture's dimensions so that the center of the texture aligns with the center of the canvas
		FVector2D CrosshairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

		// Draw the crosshair at the centerpoint
		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent; // Blend mode for the generated material
		Canvas->DrawItem(TileItem);
	}
}