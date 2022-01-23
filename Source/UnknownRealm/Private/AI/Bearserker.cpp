// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Bearserker.h"

ABearserker::ABearserker()
	: bIsCrawling(false)
{
}

void ABearserker::MC_StopCrawling_Implementation()
{
	bIsCrawling = true;
}

void ABearserker::MC_StartCrawling_Implementation()
{
	bIsCrawling = false;
}
