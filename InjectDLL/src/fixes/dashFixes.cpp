#include "pch.h"
#include "interception.h"
#include "interceptionMacros.h"
#include "dllmain.h"
#include "dashFixes.h"

bool hasRealDash = false;
BINDING(bool, SeinDashNew__ShouldDig, (SeinDashNew_o* this_ptr))
BINDING(bool, SeinDashNew__ShouldSwim, (SeinDashNew_o* this_ptr))
BINDING(bool, SeinCharacter__HasAbility, (SeinCharacter_o* this_ptr, uint8_t abilityType))
INTERCEPT(bool, SeinDashNew__get_CanDash, (SeinDashNew_o* this_ptr), {
	auto result = SeinDashNew__get_CanDash(this_ptr);
	if(!hasRealDash)
		result = result & (SeinDashNew__ShouldDig(this_ptr) || SeinDashNew__ShouldSwim(this_ptr));
	
	return result;
})

const uint8_t WaterDash = 104;
const uint8_t Digging = 101;
const uint8_t DashNew = 102;
BINDING(bool, PlayerAbilities__HasAbility, (PlayerAbilities_o* this_ptr, uint8_t ability))
BINDING(void, PlayerAbilities__SetAbility, (PlayerAbilities_o* this_ptr, uint8_t ability, bool value))

BINDING(bool, Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility, (Moon_uberSerializationWisp_PlayerUberStateAbilities_o* this_ptr, uint8_t ability))
INTERCEPT(void, Moon_uberSerializationWisp_PlayerUberStateAbilities__SetAbility, (Moon_uberSerializationWisp_PlayerUberStateAbilities_o* this_ptr, uint8_t ability, bool value), {
	if(ability == DashNew) {
		hasRealDash = value;
		debug("uberstate, setting hasRealDash: " + std::to_string(hasRealDash));
	}
	Moon_uberSerializationWisp_PlayerUberStateAbilities__SetAbility(this_ptr, ability, value);
	if(ability == WaterDash || ability == Digging || ability == DashNew) {
        debug("there be updateDashState here");
		updateDashState(this_ptr);
	}
})

void updateDashState(Moon_uberSerializationWisp_PlayerUberStateAbilities_o* this_ptr){
	if(!Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, DashNew) && (Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, Digging) || Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, WaterDash)))	{
        debug("Updating dash state to true");
		Moon_uberSerializationWisp_PlayerUberStateAbilities__SetAbility(this_ptr, DashNew, true);
	} else if(Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, DashNew) && !Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, Digging) && !Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, WaterDash) && !hasRealDash) {
        debug("Updating dash state to false");
		Moon_uberSerializationWisp_PlayerUberStateAbilities__SetAbility(this_ptr, DashNew, false);
	}
}

INTERCEPT(void, Moon_uberSerializationWisp_PlayerUberStateAbilities__Save, (Moon_uberSerializationWisp_PlayerUberStateAbilities_o* this_ptr, UberStateArchive_o* archive, Moon_uberSerializationWisp_PlayerUberStateAbilities_o* abilities), {

    if(Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, DashNew) && !hasRealDash)
    {
        debug("saving dash as: 0");
        Moon_uberSerializationWisp_PlayerUberStateAbilities__SetAbility(this_ptr, DashNew, hasRealDash);
        Moon_uberSerializationWisp_PlayerUberStateAbilities__SetAbility(abilities, DashNew, hasRealDash);
    }else
    {
        debug("saving dash as: 1");
    }
	Moon_uberSerializationWisp_PlayerUberStateAbilities__Save(this_ptr, archive, abilities);
	updateDashState(this_ptr);
})

INTERCEPT(void, Moon_uberSerializationWisp_PlayerUberStateAbilities__Load, (Moon_uberSerializationWisp_PlayerUberStateAbilities_o* this_ptr, UberStateArchive_o* archive, int32_t storeVersion), {
	Moon_uberSerializationWisp_PlayerUberStateAbilities__Load(this_ptr, archive, storeVersion);
	hasRealDash = Moon_uberSerializationWisp_PlayerUberStateAbilities__HasAbility(this_ptr, DashNew);
	debug("load, setting hasRealDash: " + std::to_string(hasRealDash));
	updateDashState(this_ptr);
})

INTERCEPT(void, GeneralDebugMenuPage__SetAbility, (GeneralDebugMenuPage_o* this_ptr, uint8_t ability, bool value), {
	GeneralDebugMenuPage__SetAbility(this_ptr, ability, value);
	if(ability == DashNew) {
		hasRealDash = value;
		debug("debug menu, setting hasRealDash: " + std::to_string(hasRealDash));
	}
})
INTERCEPT(bool, GeneralDebugMenuPage__GetAbility, (GeneralDebugMenuPage_o* this_ptr, uint8_t abilityType), {
	if(abilityType == DashNew)
		return hasRealDash;

	return GeneralDebugMenuPage__GetAbility(this_ptr, abilityType);
})