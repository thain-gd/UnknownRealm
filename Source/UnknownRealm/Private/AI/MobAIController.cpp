#include "AI/MobAIController.h"
#include "GameTeam.h"

AMobAIController::AMobAIController()
{
	SetGenericTeamId(FGenericTeamId(EGameTeam::Monster));
}

ETeamAttitude::Type AMobAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (const APawn* OtherPawn = Cast<APawn>(&Other))
    {
        if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
        {
            //Create an alliance with Team with ID 10 and set all the other teams as Hostiles:
            const FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
            if (OtherTeamID == EGameTeam::Player)
            {
                return ETeamAttitude::Hostile;
            }
        }
    }

    return ETeamAttitude::Neutral;
}
