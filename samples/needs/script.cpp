
#include "script.h"
#include <string>
#include <vector>
#include "Misc.h"
#include "sstream"
#include "DataFiles.h"

using namespace std;

Prompt Prompt_Drink;
Prompt Prompt_Wash;
Prompt Prompt_Fill;
Prompt Prompt_Boil;
Prompt Prompt_Stop; 

Prompt Prompt_Drink_Flask;
Prompt Prompt_Pee;
Prompt Prompt_Back;
Prompt Prompt_Sleep;

Prompt Prompt_Nap1;
Prompt Prompt_Nap2;
Prompt Prompt_Nap3;

Prompt Prompt_Leave;
Prompt Prompt_Holster;
Prompt Prompt_Walk;

Prompt Prompt_Drop;

bool alt_control = GetPrivateProfileInt("MISC", "ALTERNATIVE_HOLSTER_KEY", 0, ".\\needs.ini");

const char* stopPrompt;
const char* drinkPrompt;
const char* washPrompt;
const char* refillPrompt;
const char* drinkCanteenPrompt;
const char* urinatePrompt;
const char* backPrompt;
const char* sleepPrompt;
const char* hr2Prompt;
const char* hr4Prompt;
const char* hr8Prompt;
const char* upPrompt;

const char* walkPrompt;
const char* runPrompt;

const char* fullCanteenPrompt;
const char* notThirstyPrompt;
const char* canteenEmptyPrompt;


void initialize()
{

	Misc::createPrompt(Prompt_Boil, "INPUT_DUCK", "Boil Water", 0, 1754796591);

	Misc::createPrompt(Prompt_Stop, "INPUT_FRONTEND_CANCEL", stopPrompt, 0);

	Misc::createPrompt(Prompt_Drink, "INPUT_SHOP_BUY", drinkPrompt, 0);

	Misc::createPrompt(Prompt_Wash, "INPUT_JUMP", washPrompt, 0);

	Misc::createPrompt(Prompt_Fill, "INPUT_SPRINT", refillPrompt, 0);

	Misc::createPrompt(Prompt_Drink_Flask, "INPUT_FRONTEND_ACCEPT", drinkCanteenPrompt, 2);

	Misc::createPrompt(Prompt_Pee, "INPUT_INTERACT_LOCKON_POS", urinatePrompt, 2, 704572841);

	Misc::createPrompt(Prompt_Back, "INPUT_FRONTEND_CANCEL", backPrompt, 0);

	Misc::createPrompt(Prompt_Sleep, "INPUT_FRONTEND_ACCEPT", sleepPrompt, 2, 342152817);

	Misc::createPrompt(Prompt_Nap1, "INPUT_SHOP_BUY", hr2Prompt, 2, 1323335645);

	Misc::createPrompt(Prompt_Nap2, "INPUT_JUMP", hr4Prompt, 2, 1323335645);

	Misc::createPrompt(Prompt_Nap3, "INPUT_SPRINT", hr8Prompt, 2, 1323335645);

	Misc::createPrompt(Prompt_Leave, "INPUT_FRONTEND_CANCEL", upPrompt, 0);
	
	Misc::createPrompt(Prompt_Drop, "INPUT_FRONTEND_CANCEL", "Drop Weapon", 0);


	if (alt_control) {
		Misc::createPrompt(Prompt_Holster, "INPUT_INTERACT_OPTION2", "Holster Off-Hand", 0);
	}
	else {
		Misc::createPrompt(Prompt_Holster, "INPUT_DUCK", "Holster Off-Hand", 0);
	}

	Misc::createPrompt(Prompt_Walk, "INPUT_SHOP_BUY", "Sprint", 2);

	//704572841

}


float celciusToFarenheit(float temperature) // using exactly the same formula based on the decompiled scripts
{
	return ((temperature * 1.8f) + 32.0f);
}

float getSurroundingTemperature()
{
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
	float temperature = MISC::_GET_TEMPERATURE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z); // this function return in celcius

	return (MISC::_SHOULD_USE_METRIC_TEMPERATURE()) ? temperature : celciusToFarenheit(temperature);
}

float getTemperaturePointsNeeded()
{
	float temperaturePoints[] = { -20.0f, -16.0f, -12.0f, -8.0f, -4.0f, 0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f, 28.0f }; // based on a climate/temperature map by hopper on reddit https://i.redd.it/p6f6etiw7by11.jpg in celcius
	int size = sizeof(temperaturePoints) / sizeof(temperaturePoints[0]);

	if (!MISC::_SHOULD_USE_METRIC_TEMPERATURE())
	{
		for (int i = 0; i < size; i++)
		{
			temperaturePoints[i] = celciusToFarenheit(temperaturePoints[i]);
		}
	}

	if (getSurroundingTemperature() < temperaturePoints[0]) // coldest
		return 9.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[0] && getSurroundingTemperature() < temperaturePoints[1])
		return 8.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[1] && getSurroundingTemperature() < temperaturePoints[2])
		return 8.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[2] && getSurroundingTemperature() < temperaturePoints[3])
		return 7.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[3] && getSurroundingTemperature() < temperaturePoints[4])
		return 7.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[4] && getSurroundingTemperature() < temperaturePoints[5])
		return 6.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[5] && getSurroundingTemperature() < temperaturePoints[6])
		return 6.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[6] && getSurroundingTemperature() < temperaturePoints[7])
		return 5.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[7] && getSurroundingTemperature() < temperaturePoints[8])
		return 5.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[8] && getSurroundingTemperature() < temperaturePoints[9])
		return 4.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[9] && getSurroundingTemperature() < temperaturePoints[10])
		return 4.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[10] && getSurroundingTemperature() < temperaturePoints[11])
		return 3.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[11] && getSurroundingTemperature() < temperaturePoints[12])
		return 3.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[12]) // hottest
		return 2.5f;
	else return 0.0f;
}

bool isPlayerStartedSleepScenario()
{
	const char* sleepScenarios[] = {
		"WORLD_PLAYER_SLEEP_BEDROLL",
		"WORLD_PLAYER_SLEEP_BEDROLL_ARTHUR",
		"WORLD_PLAYER_SLEEP_GROUND",
		"PROP_PLAYER_SLEEP_BED",
		"PROP_PLAYER_SLEEP_BED_ARTHUR",
		"PROP_PLAYER_SLEEP_TENT_A_FRAME",
		"PROP_PLAYER_SLEEP_TENT_A_FRAME_ARTHUR",
		"PROP_PLAYER_SLEEP_TENT_MALE_A",
		"PROP_PLAYER_SLEEP_TENT_MALE_A_ARTHUR",
		"PROP_PLAYER_SLEEP_A_FRAME_TENT_PLAYER_CAMPS",
		"PROP_PLAYER_SLEEP_A_FRAME_TENT_PLAYER_CAMPS_ARTHUR"
	};

	for (const char* sleepScenario : sleepScenarios)
	{
		if (PED::_IS_PED_USING_SCENARIO_HASH(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY(sleepScenario)) && TASK::_GET_SCENARIO_POINT_PED_IS_USING(PLAYER::PLAYER_PED_ID(), 1) == -1 && !PLAYER::IS_PLAYER_CONTROL_ON(0))
		{
			return true;
			break;
		}
	}
	return false;
}

bool isPlayerBathing()
{
	return MISC::ARE_STRINGS_EQUAL(TASK::GET_TASK_MOVE_NETWORK_STATE(PLAYER::PLAYER_PED_ID()), "Bathing");
}

int tAnimationSet = 0;

bool refillThirstCoreScenario()
{
	const char* animations[] = {
		"face_human@gen_male@scenario@drinkbottle",
		"face_human@gen_male@scenario@drinkshot",
		"face_human@gen_male@scenario@drinkmug",
		"face_human@gen_male@scenario@drinkhand"
		"mech_dynamic@world_player_dynamic_kneel_ground@coffee@male_a@use_idles@idle_b"
		"mech_dynamic@world_player_dynamic_kneel_ground@coffee@male_a@use_idles@idle_a"
		"mech_dynamic@world_player_dynamic_kneel_ground@coffee@male_a@stand_exit_withprop"

	};
	for (const char* animation : animations)
	{
		if (STREAMING::HAS_ANIM_DICT_LOADED(animation))
		{
			return true;
			tAnimationSet = MISC::GET_GAME_TIMER() + 1000;
			break;
		}
	}
	return false;
}

bool refillHungerCoreScenario()
{
	const char* animations[] = {
		"face_human@gen_male@scenario@eating",

	};
	for (const char* animation : animations)
	{
		if (STREAMING::HAS_ANIM_DICT_LOADED(animation))
		{
			return true;
			tAnimationSet = MISC::GET_GAME_TIMER() + 2000;
			break;
		}
	}
	return false;
}


bool sitting = 0;

bool is_in_sequence = 0;

int sleep_core = 100;

void sleepSeq(int time) {


	is_in_sequence = 1;

	//WAIT(1000);

	CAM::DO_SCREEN_FADE_OUT(1500);
	CAM::DO_SCREEN_FADE_IN(0);

	HUD::_HIDE_HUD_COMPONENT(-1679307491);




	AUDIO::TRIGGER_MUSIC_EVENT("SLEEP_TRANSITION_START");





	if (CLOCK::GET_CLOCK_HOURS() >= 5 && CLOCK::GET_CLOCK_HOURS() < 9) {
		GRAPHICS::ANIMPOSTFX_PLAY("skytl_0600_01clear");
	}
	if (CLOCK::GET_CLOCK_HOURS() >= 9 && CLOCK::GET_CLOCK_HOURS() < 12) {
		GRAPHICS::ANIMPOSTFX_PLAY("skytl_0900_01clear");
	}
	if (CLOCK::GET_CLOCK_HOURS() >= 12 && CLOCK::GET_CLOCK_HOURS() < 15) {
		GRAPHICS::ANIMPOSTFX_PLAY("skytl_1200_01clear");
	}
	if (CLOCK::GET_CLOCK_HOURS() >= 15 && CLOCK::GET_CLOCK_HOURS() < 23) {
		GRAPHICS::ANIMPOSTFX_PLAY("skytl_1500_03clouds");
	}
	if (CLOCK::GET_CLOCK_HOURS() >= 23 || (CLOCK::GET_CLOCK_HOURS() >= 0 && CLOCK::GET_CLOCK_HOURS() < 5)) {
		GRAPHICS::ANIMPOSTFX_PLAY("skytl_0000_01clear");
	}

	WAIT(1000);

	TASK::_TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WORLD_PLAYER_SLEEP_BEDROLL"), -1, 1, 1, 1, 1);

	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1, 0);
	if (MISC::_GET_TEMPERATURE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z) <= 0) {
		PED::APPLY_DAMAGE_TO_PED(PLAYER::PLAYER_PED_ID(), 1000, 0, 0, 0);


		AUDIO::TRIGGER_MUSIC_EVENT("SLEEP_TRANSITION_STOP");

	}
	else {
		WAIT(2000);

		int tempTime = CLOCK::GET_CLOCK_HOURS() + time;
		if (tempTime >= 24) {
			tempTime -= 24;
		}

		CLOCK::ADVANCE_CLOCK_TIME_TO(tempTime, 0, 0);

		sleep_core += 10 * time;

		ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(PLAYER::PLAYER_PED_ID(), 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(PLAYER::PLAYER_PED_ID(), 1) + (20 * time));


		WAIT(7000);



		AUDIO::TRIGGER_MUSIC_EVENT("SLEEP_TRANSITION_STOP");

		sitting = 1;

	}





}


Weapon gatherWeapons() {
	const char* weapons[] = {
		"WEAPON_PISTOL_MAUSER",
		"WEAPON_PISTOL_MAUSER_DRUNK",
		"WEAPON_PISTOL_SEMIAUTO",
		"WEAPON_PISTOL_VOLCANIC",
		"WEAPON_PISTOL_M1899",

		"WEAPON_REVOLVER_CATTLEMAN",
		"WEAPON_REVOLVER_CATTLEMAN_JOHN",
		"WEAPON_REVOLVER_CATTLEMAN_MEXICAN",
		"WEAPON_REVOLVER_CATTLEMAN_PIG",


		"WEAPON_REVOLVER_DOUBLEACTION",
		"WEAPON_REVOLVER_DOUBLEACTION_EXOTIC",
		"WEAPON_REVOLVER_DOUBLEACTION_MICAH",

		"WEAPON_REVOLVER_SCHOFIELD",
		"WEAPON_REVOLVER_NAVY",
		"WEAPON_REVOLVER_LEMAT",
		"WEAPON_REVOLVER_SCHOFIELD_CALLOWAY",
		"WEAPON_REVOLVER_SCHOFIELD_GOLDEN",


		"WEAPON_SHOTGUN_SAWEDOFF",

	};



	//_DUALWIELD
	for (const char* weapon : weapons)
	{
		if (WEAPON::IS_PED_CARRYING_WEAPON(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY(weapon)))
		{
			return MISC::GET_HASH_KEY(weapon);
		}
	}
}



bool core_adjustment = GetPrivateProfileInt("MISC", "CORE_ADJUSTMENT", 1, ".\\needs.ini");

int food_timer;

int currentStaminaLevel = 7;
int currentDeadeyeLevel = 7;
int currentHealthLevel = 7;

float temperature_food_drop;

float temperature_food_drop1 = GetPrivateProfileInt("CORES", "FOOD_CORE_RATE_TEMPERATURE1", 15, ".\\needs.ini") / 10;
float temperature_food_drop2 = GetPrivateProfileInt("CORES", "FOOD_CORE_RATE_TEMPERATURE2", 20, ".\\needs.ini") / 10;
float temperature_food_drop3 = GetPrivateProfileInt("CORES", "FOOD_CORE_RATE_TEMPERATURE3", 30, ".\\needs.ini") / 10;


void foodCore(int cold_core, float menuX, float menuY, float sizeX, float sizeY)
{
	//drawText((char*)title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, true, 0.5f, 0.5f);
	if (HUD::IS_RADAR_HIDDEN() == true) {



	}
	else {

		stringstream core;
		core << "rpg_meter_" << cold_core;
		//showSubtitle(core.str().c_str());


		Misc::drawSprite("rpg_textures", "rpg_core_background", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 0, 0, 0, 220);


		



		if (getTemperaturePointsNeeded() < 6.0) {
			Misc::drawSprite("rpg_textures", "rpg_overfed", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 230);
			temperature_food_drop = temperature_food_drop1;
		}

		if (getTemperaturePointsNeeded() == 6.0) {
			Misc::drawSprite("rpg_textures", "rpg_overfed", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 0, 230);
			temperature_food_drop = temperature_food_drop1;
			//temperature_food_drop = temperature_drink_drop1;
		}

		if (getTemperaturePointsNeeded() == 6.5) {
			Misc::drawSprite("rpg_textures", "rpg_overfed", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 229, 148, 0, 230);
			temperature_food_drop = temperature_food_drop2;
			//temperature_food_drop = temperature_drink_drop1;
		}

		if (getTemperaturePointsNeeded() >= 7.0) {
			Misc::drawSprite("rpg_textures", "rpg_overfed", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 250, 0, 80, 230);
			temperature_food_drop = temperature_food_drop3;
			//temperature_food_drop = temperature_drink_drop1;
		}



		//Misc::drawSprite("rpg_textures", "rpg_tracked", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 45, 255, 255, 255, 230);

		//Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 255, 230);


		Misc::drawSprite("rpg_meter_track", "rpg_meter_track_9", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 110, 110, 110, 230);




		if (cold_core != 100) {
			Misc::drawSprite("rpg_meter", core.str().c_str(), menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}
		else {
			Misc::drawSprite("rpg_meter", "rpg_meter_99", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}


		/*
		if (food_timer < MISC::GET_GAME_TIMER()) {
			if (!PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_ON_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
				if (cold_core >= 0 && cold_core < 25) {
					if (currentHealthLevel - 3 >= 0) {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0, currentHealthLevel - 6);
					}
					else {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0, 0);
					}
				}
				if (cold_core >= 25 && cold_core < 40) {
					if (currentHealthLevel - 2 >= 0) {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0, currentHealthLevel - 4);
					}
					else {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0, 0);
					}
				}
				if (cold_core < 60) {
					if (currentHealthLevel - 1 >= 0) {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0, currentHealthLevel - 2);
					}
					else {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0, 0);
					}

				}
				if (cold_core >= 60) {
					ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0, currentHealthLevel);



				}
			}



			food_timer = MISC::GET_GAME_TIMER() + 500;



		}
		*/

	}
}

int number_of_drinks = 0;

float drinks_pee_drop;

float drinks_pee_drop1 = GetPrivateProfileInt("CORES", "PEE_CORE_RATE_DRINKS1", 15, ".\\needs.ini") / 10;
float drinks_pee_drop2 = GetPrivateProfileInt("CORES", "PEE_CORE_RATE_DRINKS2", 20, ".\\needs.ini") / 10;
float drinks_pee_drop3 = GetPrivateProfileInt("CORES", "PEE_CORE_RATE_DRINKS3", 30, ".\\needs.ini") / 10;



void peeCore(int cold_core, float menuX, float menuY, float sizeX, float sizeY, bool extend)
{
	//drawText((char*)title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, true, 0.5f, 0.5f);
	if (HUD::IS_RADAR_HIDDEN() == true) {



	}
	else {

		stringstream core;
		core << "rpg_meter_" << cold_core;
		//showSubtitle(core.str().c_str());


		Misc::drawSprite("rpg_textures", "rpg_core_background", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 0, 0, 0, 220);






		if (!extend || !core_adjustment) {
			if (number_of_drinks < 4) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX - 0.0015, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 255, 255, 255, 230);
				drinks_pee_drop = drinks_pee_drop1;
			}
			if (number_of_drinks >= 4 && number_of_drinks < 7) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX - 0.0015, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 255, 255, 0, 230);
				drinks_pee_drop = drinks_pee_drop1;
			}
			if (number_of_drinks >= 7 && number_of_drinks < 9) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX - 0.0015, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 229, 148, 0, 230);
				drinks_pee_drop = drinks_pee_drop2;
			}
			if (number_of_drinks >= 9) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX - 0.0015, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 250, 0, 80, 230);
				drinks_pee_drop = drinks_pee_drop3;
			}


		}
		else {
			if (number_of_drinks < 4) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 255, 255, 255, 230);
				drinks_pee_drop = drinks_pee_drop1;
			}
			if (number_of_drinks >= 4 && number_of_drinks < 7) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 255, 255, 0, 230);
				drinks_pee_drop = drinks_pee_drop1;
			}
			if (number_of_drinks >= 7 && number_of_drinks < 9) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 229, 148, 0, 230);
				drinks_pee_drop = drinks_pee_drop2;
			}
			if (number_of_drinks >= 9) {
				Misc::drawSprite("rpg_textures", "rpg_tracked", menuX, menuY, 0.03 + sizeX, 0.043f + sizeY, 0, 250, 0, 80, 230);
				drinks_pee_drop = drinks_pee_drop3;
			}

			

		}
		
		//Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 255, 230);


		Misc::drawSprite("rpg_meter_track", "rpg_meter_track_9", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 110, 110, 110, 230);




		if (cold_core != 100) {
			Misc::drawSprite("rpg_meter", core.str().c_str(), menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}
		else {
			Misc::drawSprite("rpg_meter", "rpg_meter_99", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}



	}
}


float temperature_drink_drop;

float temperature_drink_drop1 = GetPrivateProfileInt("CORES", "DRINK_CORE_RATE_TEMPERATURE1", 15, ".\\needs.ini") / 10;
float temperature_drink_drop2 = GetPrivateProfileInt("CORES", "DRINK_CORE_RATE_TEMPERATURE2", 20, ".\\needs.ini") / 10;
float temperature_drink_drop3 = GetPrivateProfileInt("CORES", "DRINK_CORE_RATE_TEMPERATURE3", 30, ".\\needs.ini") / 10;


bool no_slowdown = GetPrivateProfileInt("MISC", "SLOWDOWN", 1, ".\\needs.ini");



void drinkCore(int cold_core, float menuX, float menuY, float sizeX, float sizeY, bool extend)
{
	//drawText((char*)title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, true, 0.5f, 0.5f);
	if (HUD::IS_RADAR_HIDDEN() == true) {



	}
	else {
		stringstream core;
		core << "rpg_meter_" << cold_core;
		//showSubtitle(core.str().c_str());

		Ped player = PLAYER::PLAYER_PED_ID();
		Misc::drawSprite("rpg_textures", "rpg_core_background", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 0, 0, 0, 220);



		//menuX + 0.0037f, menuY + 0.006f, 0.019 + sizeX, 0.028f + sizeY
		if (!extend) {
			//Misc::drawSprite("blips", "blip_mg_drinking", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 255, 230);
			Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 255, 230);

			temperature_drink_drop = 1;
			if (getTemperaturePointsNeeded() == 3.5) {
				Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 0, 230);
				temperature_drink_drop = temperature_drink_drop1;
			}
			if (PED::_IS_PED_COMPONENT_EQUIPPED(player, 0x9925C067) == true) {
				if (getTemperaturePointsNeeded() == 3.0) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 0, 230);
					temperature_drink_drop = temperature_drink_drop1;
				}

				if (getTemperaturePointsNeeded() <= 2.5) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 0, 230);
					temperature_drink_drop = temperature_drink_drop1;
				}


			}
			else {

				if (getTemperaturePointsNeeded() == 3.0) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 229, 148, 0, 230);
					temperature_drink_drop = temperature_drink_drop2;
				}

				if (getTemperaturePointsNeeded() <= 2.5) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 250, 0, 80, 230);
					temperature_drink_drop = temperature_drink_drop3;
				}

			}



		}
		else {
			Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.005f, menuY + 0.008f, 0.0155 + sizeX, 0.024f + sizeY, 0, 255, 255, 255, 230);

			temperature_drink_drop = 1;
			if (getTemperaturePointsNeeded() == 3.5) {
				Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.005f, menuY + 0.008f, 0.0155 + sizeX, 0.024f + sizeY, 0, 255, 255, 0, 230);
				temperature_drink_drop = temperature_drink_drop1;
			}
			if (PED::_IS_PED_COMPONENT_EQUIPPED(player, 0x9925C067) == true) {
				if (getTemperaturePointsNeeded() == 3.0) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.005f, menuY + 0.008f, 0.0155 + sizeX, 0.024f + sizeY, 0, 255, 255, 0, 230);
					temperature_drink_drop = temperature_drink_drop1;
				}

				if (getTemperaturePointsNeeded() == 2.5) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.005f, menuY + 0.008f, 0.0155 + sizeX, 0.024f + sizeY, 0, 255, 255, 0, 230);
					temperature_drink_drop = temperature_drink_drop1;
				}


			}
			else {

				if (getTemperaturePointsNeeded() == 3.0) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.005f, menuY + 0.008f, 0.0155 + sizeX, 0.024f + sizeY, 0, 229, 148, 0, 230);
					temperature_drink_drop = temperature_drink_drop2;
				}

				if (getTemperaturePointsNeeded() == 2.5) {
					Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.005f, menuY + 0.008f, 0.0155 + sizeX, 0.024f + sizeY, 0, 250, 0, 80, 230);
					temperature_drink_drop = temperature_drink_drop3;
				}

			}

		}


		Misc::drawSprite("rpg_meter_track", "rpg_meter_track_9", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 110, 110, 110, 255);

		if (cold_core != 100) {
			Misc::drawSprite("rpg_meter", core.str().c_str(), menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}
		else {
			Misc::drawSprite("rpg_meter", "rpg_meter_99", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}



		



	}
}

int canteen_type = 0; //0 - nothing/normal      1 - swamp / poisonous     2 - cold 

Vector3 RGBvec = { 250, 250, 250 };

void canteenCore(int canteen, float menuX, float menuY, float sizeX, float sizeY, bool extend) {

	if (HUD::IS_RADAR_HIDDEN() == true) {



	}
	else {
		switch (canteen_type) {
		case 0:
			RGBvec = Misc::toVector3(250, 250, 250);
			break;
		case 1:
			RGBvec = Misc::toVector3(0, 128, 0);
			break;
		case 2:
			RGBvec = Misc::toVector3(0, 183, 235);
			break;

		};


		if (canteen == 4) {
			Misc::drawSprite("blips", "blip_horseshoe_4", menuX + 0.0051f, menuY + 0.009f, 0.020 + sizeX, 0.030f + sizeY, 0, RGBvec.x, RGBvec.y, RGBvec.z, 255);
		}
		if (canteen == 3) {
			Misc::drawSprite("blips", "blip_horseshoe_3", menuX + 0.0051f, menuY + 0.009f, 0.020 + sizeX, 0.030f + sizeY, 0, RGBvec.x, RGBvec.y, RGBvec.z, 255);
		}
		if (canteen == 2) {
			Misc::drawSprite("blips", "blip_horseshoe_2", menuX + 0.0051f, menuY + 0.009f, 0.020 + sizeX, 0.030f + sizeY, 0, RGBvec.x, RGBvec.y, RGBvec.z, 255);
		}
		if (canteen == 1) {
			Misc::drawSprite("blips", "blip_horseshoe_1", menuX + 0.0051f, menuY + 0.009f, 0.020 + sizeX, 0.030f + sizeY, 0, RGBvec.x, RGBvec.y, RGBvec.z, 255);
		}
		if (canteen == 0) {
			Misc::drawSprite("blips", "blip_horseshoe_0", menuX + 0.0051f, menuY + 0.009f, 0.020 + sizeX, 0.030f + sizeY, 0, 250, 250, 250, 255);
		}


		if (!extend || !core_adjustment) {
			Misc::drawSprite("inventory_items", "consumable_moonshine", menuX + 0.01f, menuY + 0.0165f, 0.010, 0.016f, 0, 0, 0, 0, 255);
		}
		else {
			Misc::drawSprite("inventory_items", "consumable_moonshine", menuX + 0.0125f, menuY + 0.021f, 0.010 + sizeX / 2, 0.016f + sizeY / 2, 0, 0, 0, 0, 255);
		}



	}
}

int is_night = 0;
int is_running = 0;

bool overpowerSleep = 0;
int overpowerSleepTimer = 0;


int sleep_timer;

void sleepCore(int cold_core, float menuX, float menuY, float sizeX, float sizeY, bool extend)
{
	//drawText((char*)title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, true, 0.5f, 0.5f);
	if (HUD::IS_RADAR_HIDDEN() == true) {



	}
	else {

		stringstream core;
		core << "rpg_meter_" << cold_core;
		//showSubtitle(core.str().c_str());


		Misc::drawSprite("rpg_textures", "rpg_core_background", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 0, 0, 0, 220);







		if (!is_running && !is_night) {
			Misc::drawSprite("rpg_textures", "rpg_disoriented", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 230);
		}

		if (!is_running != !is_night) {
			Misc::drawSprite("rpg_textures", "rpg_disoriented", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 0, 230);

		}
		if (is_running && is_night) {
			Misc::drawSprite("rpg_textures", "rpg_disoriented", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 250, 0, 80, 230);

		}



		Misc::drawSprite("rpg_meter_track", "rpg_meter_track_9", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 110, 110, 110, 230);




		if (cold_core != 100) {
			Misc::drawSprite("rpg_meter", core.str().c_str(), menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}
		else {
			Misc::drawSprite("rpg_meter", "rpg_meter_99", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
		}

		//|| PED::IS_PED_USING_ANY_SCENARIO(PLAYER::PLAYER_PED_ID())

		if (PED::IS_PED_IN_COMBAT(PLAYER::PLAYER_PED_ID(), 0) || overpowerSleep) {
			Misc::drawSprite("rpg_meter", "rpg_meter_99", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 245, 215, 110, 250);
			if (sleep_core < 10) {
				sleep_core = 10;
			}
		}

		/*
		if (sleep_timer < MISC::GET_GAME_TIMER()) {
			if (!PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_ON_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
				if (sleep_core >= 0 && sleep_core < 25) {
					if (currentDeadeyeLevel - 6 >= 0) {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, currentDeadeyeLevel - 6);
					}
					else {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, 0);
					}
				}
				if (sleep_core >= 25 && sleep_core < 40) {
					if (currentDeadeyeLevel - 4 >= 0) {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, currentDeadeyeLevel - 4);
					}
					else {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, 0);
					}
				}
				if (sleep_core < 60) {
					if (currentDeadeyeLevel - 2 >= 0) {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, currentDeadeyeLevel - 2);
					}
					else {
						ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, 0);
					}

				}
				if (sleep_core >= 60) {
					ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, currentDeadeyeLevel);



				}
			}



			sleep_timer = MISC::GET_GAME_TIMER() + 500;



		}

		*/
	}

}
int weight_timer;

int food_drop_timer = 0;
int pee_drop_timer = 0;
int drink_drop_timer = 0;
int sleep_drop_timer = 0;

int repeater_weight = GetPrivateProfileInt("CORES", "REPEATER_WEIGHT", 15, ".\\needs.ini");
int shotgun_weight = GetPrivateProfileInt("CORES", "SHOTGUN_WEIGHT", 20, ".\\needs.ini");
int shotgun_one_handed_weight = GetPrivateProfileInt("CORES", "SHOTGUN_ONE_HAND_WEIGHT", 15, ".\\needs.ini");
int revolver_weight = GetPrivateProfileInt("CORES", "REVOLVER_WEIGHT", 10, ".\\needs.ini");
int pistol_weight = GetPrivateProfileInt("CORES", "PISTOL_WEIGHT", 5, ".\\needs.ini");
int sniperRifle_weight = GetPrivateProfileInt("CORES", "SNIPERRIFLE_WEIGHT", 25, ".\\needs.ini");

int coat_weight = GetPrivateProfileInt("CORES", "COAT_WEIGHT", 10, ".\\needs.ini");

int global_weight;

bool simplify_weight = GetPrivateProfileInt("CORES", "SIMPLIFIED_WEIGHT", 1, ".\\needs.ini");

void weightCore(int cold_core, float menuX, float menuY, float sizeX, float sizeY)
{
	int weight = 0;

	weight += (ATTRIBUTE::GET_ATTRIBUTE_RANK(PLAYER::PLAYER_PED_ID(), 13) / 10) * 2;

	const char* weapons[] = {
		"WEAPON_PISTOL_MAUSER",
		"WEAPON_PISTOL_MAUSER_DRUNK",
		"WEAPON_PISTOL_SEMIAUTO",
		"WEAPON_PISTOL_VOLCANIC",
		"WEAPON_PISTOL_M1899",

		"WEAPON_REPEATER_CARBINE",
		"WEAPON_REPEATER_HENRY",
		"WEAPON_REPEATER_WINCHESTER",

		"WEAPON_REVOLVER_CATTLEMAN",
		"WEAPON_REVOLVER_CATTLEMAN_JOHN",
		"WEAPON_REVOLVER_CATTLEMAN_MEXICAN",
		"WEAPON_REVOLVER_CATTLEMAN_PIG",


		"WEAPON_REVOLVER_DOUBLEACTION",
		"WEAPON_REVOLVER_DOUBLEACTION_EXOTIC",
		"WEAPON_REVOLVER_DOUBLEACTION_MICAH",

		"WEAPON_REVOLVER_SCHOFIELD",
		"WEAPON_REVOLVER_NAVY",
		"WEAPON_REVOLVER_LEMAT",
		"WEAPON_REVOLVER_SCHOFIELD_CALLOWAY",
		"WEAPON_REVOLVER_SCHOFIELD_GOLDEN",

		"WEAPON_RIFLE_BOLTACTION",
		"WEAPON_RIFLE_SPRINGFIELD",
		"WEAPON_RIFLE_VARMINT",
		"WEAPON_RIFLE_ELEPHANT",

		"WEAPON_SHOTGUN_DOUBLEBARREL",
		"WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC",


		"WEAPON_SHOTGUN_PUMP",
		"WEAPON_SHOTGUN_REPEATING",
		"WEAPON_SHOTGUN_SAWEDOFF",

		"WEAPON_SHOTGUN_SEMIAUTO",

		"WEAPON_SNIPERRIFLE_CARCANO",
		"WEAPON_SNIPERRIFLE_ROLLINGBLOCK",
		"WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC",
	};

	const char* weapons2[] = {
		"WEAPON_PISTOL_MAUSER_DUALWIELD",
		"WEAPON_PISTOL_MAUSER_DRUNK_DUALWIELD",
		"WEAPON_PISTOL_SEMIAUTO_DUALWIELD",
		"WEAPON_PISTOL_VOLCANIC_DUALWIELD",
		"WEAPON_PISTOL_M1899_DUALWIELD",

		"WEAPON_REVOLVER_CATTLEMAN_DUALWIELD",
		"WEAPON_REVOLVER_CATTLEMAN_JOHN_DUALWIELD",
		"WEAPON_REVOLVER_CATTLEMAN_MEXICAN_DUALWIELD",
		"WEAPON_REVOLVER_CATTLEMAN_PIG_DUALWIELD",


		"WEAPON_REVOLVER_DOUBLEACTION_DUALWIELD",
		"WEAPON_REVOLVER_DOUBLEACTION_EXOTIC_DUALWIELD",
		"WEAPON_REVOLVER_DOUBLEACTION_MICAH_DUALWIELD",

		"WEAPON_REVOLVER_SCHOFIELD_DUALWIELD",
		"WEAPON_REVOLVER_NAVY_DUALWIELD",
		"WEAPON_REVOLVER_LEMAT_DUALWIELD",
		"WEAPON_REVOLVER_SCHOFIELD_CALLOWAY_DUALWIELD",
		"WEAPON_REVOLVER_SCHOFIELD_GOLDEN_DUALWIELD",


		"WEAPON_SHOTGUN_SAWEDOFF_DUALWIELD",

	};

	//_DUALWIELD
	for (const char* weapon : weapons)
	{
		if (!simplify_weight) {
			if (WEAPON::IS_PED_CARRYING_WEAPON(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY(weapon)))
			{
				//weight += 10;
				if (WEAPON::_IS_WEAPON_REVOLVER(MISC::GET_HASH_KEY(weapon))) {
					weight += revolver_weight;
				}
				if (WEAPON::_IS_WEAPON_PISTOL(MISC::GET_HASH_KEY(weapon))) {
					weight += pistol_weight;
				}

				if (WEAPON::_IS_WEAPON_SHOTGUN(MISC::GET_HASH_KEY(weapon))) {
					if (!WEAPON::_IS_WEAPON_ONE_HANDED(MISC::GET_HASH_KEY(weapon))) {
						weight += shotgun_weight;
					}
					else {
						weight += shotgun_one_handed_weight;
					}

				}
				if (WEAPON::_IS_WEAPON_SNIPER(MISC::GET_HASH_KEY(weapon))) {
					weight += sniperRifle_weight;
				}


				if (WEAPON::_IS_WEAPON_REPEATER(MISC::GET_HASH_KEY(weapon)) || WEAPON::_IS_WEAPON_RIFLE(MISC::GET_HASH_KEY(weapon))) {
					//Misc::drawSprite("rpg_textures", "rpg_core_background", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 43, 132, 12, 51, 220);

					weight += repeater_weight;
				}

			}
		}
		else {


			if (WEAPON::IS_PED_CARRYING_WEAPON(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY(weapon)))
			{

				if (WEAPON::_IS_WEAPON_SHOTGUN(MISC::GET_HASH_KEY(weapon))) {
					if (!WEAPON::_IS_WEAPON_ONE_HANDED(MISC::GET_HASH_KEY(weapon))) {
						weight += 20;
					}
					

				}
				if (WEAPON::_IS_WEAPON_SNIPER(MISC::GET_HASH_KEY(weapon))) {
					weight += 20;
				}


				if (WEAPON::_IS_WEAPON_REPEATER(MISC::GET_HASH_KEY(weapon)) || WEAPON::_IS_WEAPON_RIFLE(MISC::GET_HASH_KEY(weapon))) {
					//Misc::drawSprite("rpg_textures", "rpg_core_background", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 43, 132, 12, 51, 220);

					weight += 20;
				}

			}
		}
	}

	for (const char* weaponDual : weapons2)
	{
		if (WEAPON::IS_PED_CARRYING_WEAPON(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY(weaponDual)))
		{
			//weight += 10;
			if (WEAPON::_IS_WEAPON_REVOLVER(MISC::GET_HASH_KEY(weaponDual))) {
				//weight += 10;
			}
			if (WEAPON::_IS_WEAPON_PISTOL(MISC::GET_HASH_KEY(weaponDual))) {
				//weight += 5;
			}
			if (WEAPON::_IS_WEAPON_SHOTGUN(MISC::GET_HASH_KEY(weaponDual))) {
				//weight += 15;
			}
		}
	}
	
	if (!simplify_weight) {
		if (PED::_IS_PED_COMPONENT_EQUIPPED(PLAYER::PLAYER_PED_ID(), 0xE06D30CE)) {
			weight += coat_weight;
		}
	}
	else {
		if (PED::_IS_PED_COMPONENT_EQUIPPED(PLAYER::PLAYER_PED_ID(), 0xE06D30CE)) {
			weight += 20;
		}
	}
	cold_core = weight;

	global_weight = weight;



	if (weight_timer < MISC::GET_GAME_TIMER()) {
		if (!PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_ON_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
			weight += 30;
			if (weight >= 0 && weight < 40) {

				ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, currentStaminaLevel);


			}
			if (weight >= 40 && weight < 60) {

				ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, currentStaminaLevel - 1);

				//sleep_drop_timer++;

				//PLAYER::SET_PLAYER_STAMINA_RECHARGE_MULTIPLIER(PLAYER::_GET_PLAYER_PED_2(1), 0.5f);
				//PED::_CHARGE_PED_STAMINA(PLAYER::PLAYER_PED_ID(), -0.5f);
			}
			if (weight >= 60 && weight < 80) {
				if (currentStaminaLevel - 2 >= 0) {
					ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, currentStaminaLevel - 2);
				}
				else {
					ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, 0);
				}
				//drink_drop_timer++;
				//food_drop_timer++;
				//sleep_drop_timer++;

				//PLAYER::SET_PLAYER_STAMINA_RECHARGE_MULTIPLIER(PLAYER::_GET_PLAYER_PED_2(1), 0.2f);
				//PED::_CHARGE_PED_STAMINA(PLAYER::PLAYER_PED_ID(), -1.0f);
			}
			if (weight >= 80) {
				if (currentStaminaLevel - 3 >= 0) {
					ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, currentStaminaLevel - 3);
				}
				else {
					ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, 0);
				}
				//drink_drop_timer += 2;
				//food_drop_timer += 2;
				//sleep_drop_timer += 2;

				//PLAYER::SET_PLAYER_STAMINA_RECHARGE_MULTIPLIER(PLAYER::_GET_PLAYER_PED_2(1), 0.0f);
				//PED::_CHARGE_PED_STAMINA(PLAYER::PLAYER_PED_ID(), -1.3f);
			}
		}



		weight_timer = MISC::GET_GAME_TIMER() + 500;
	}

	if (!simplify_weight) {
		if (HUD::IS_RADAR_HIDDEN() == true) {



		}
		else {

			stringstream core;
			core << "rpg_meter_" << cold_core;
			//showSubtitle(core.str().c_str());


			Misc::drawSprite("rpg_textures", "rpg_core_background", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 0, 0, 0, 220);



			//GET_CURRENT_PED_WEAPON(Ped ped, Hash * weaponHash, BOOL p2, int p3, BOOL p4) {



			//Misc::drawSprite("rpg_textures", "rpg_underweight", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 45, 255, 255, 255, 230);

			//Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_10", menuX + 0.0035, menuY + 0.007, 0.0175 + sizeX, 0.025f + sizeY, 0, 255, 255, 255, 255);

			if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()) || PED::IS_PED_ON_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
				Misc::drawSprite("rpg_textures", "rpg_overweight", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 230);
			}
			else {
				if (weight >= 0 && weight < 40) {
					Misc::drawSprite("rpg_textures", "rpg_overweight", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 230);
				}
				if (weight >= 40 && weight < 60) {
					Misc::drawSprite("rpg_textures", "rpg_overweight", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 0, 230);
				}
				if (weight >= 60 && weight < 80) {
					Misc::drawSprite("rpg_textures", "rpg_overweight", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 229, 148, 0, 230);
				}
				if (weight >= 80) {
					Misc::drawSprite("rpg_textures", "rpg_overweight", menuX + 0.0001, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 250, 0, 80, 230);
				}
			}
			//Misc::drawSprite("rpg_textures", "rpg_confusion", menuX + 0.0047f, menuY + 0.007f, 0.0165 + sizeX, 0.027f + sizeY, 0, 255, 255, 255, 230);


			Misc::drawSprite("rpg_meter_track", "rpg_meter_track_9", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 110, 110, 110, 230);




			if (cold_core != 100) {
				Misc::drawSprite("rpg_meter", core.str().c_str(), menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
			}
			else {
				Misc::drawSprite("rpg_meter", "rpg_meter_99", menuX, menuY, 0.025 + sizeX, 0.04f + sizeY, 0, 255, 255, 255, 200);
			}



		}
	}
}


int food_core = 100;
int food_core_max = 100;


int piss_core = 100;
int piss_core_max = 100;


int drink_core = 100;
int drink_core_max = 100;

int canteen = 0;

const float menuX = 0.040f;
const float menuY = 0.68f;

struct SampleToastRight_arg0
{
	alignas(8) int f_0;
	alignas(8) const  char* f_1;
	alignas(8) const char* f_2;
	alignas(8) int f_3;
};

struct SampleToastRight_arg1
{
	alignas(8) int f_0;
	alignas(8) const char* f_1;
	alignas(8) const  char* f_2;
	alignas(8) int f_3;
	alignas(8) int f_4;
	alignas(8) int f_5;
	alignas(8) int f_6;
};


void removeItemFromPedInventory(Ped ped, int itemHash, const char* name, const char* directory) {
	if (TXD::HAS_STREAMED_TEXTURE_DICT_LOADED(directory)) {
		TXD::REQUEST_STREAMED_TEXTURE_DICT(directory, 0);
	}

	SampleToastRight_arg0 args1;

	args1.f_0 = 450;
	args1.f_1 = "Transaction_Feed_Sounds";
	args1.f_2 = "Transaction_Negative";
	args1.f_3 = 0;
	//SampleToastRight_arg0.f_0 = 450;

	SampleToastRight_arg1 args2;

	args2.f_0 = 0;
	args2.f_1 = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", name);
	args2.f_2 = directory;
	args2.f_3 = itemHash;
	args2.f_4 = 1;
	args2.f_5 = MISC::GET_HASH_KEY("COLOR_WHITE");;
	args2.f_6 = 0;
	UIFEED::_0xB249EBCB30DD88E0((Any*)&args1, (Any*)&args2, 1);

}

void main()
{		
	DataFiles::load();

	stopPrompt = DataFiles::Lang->get("prompts.stop");
	drinkPrompt = DataFiles::Lang->get("prompts.drink");
	washPrompt = DataFiles::Lang->get("prompts.wash");
	refillPrompt = DataFiles::Lang->get("prompts.refill");
	drinkCanteenPrompt = DataFiles::Lang->get("prompts.drinkCanteen");

	fullCanteenPrompt = DataFiles::Lang->get("prompts.fullCanteenPrompt");
	notThirstyPrompt = DataFiles::Lang->get("prompts.notThirstyPrompt");
	canteenEmptyPrompt = DataFiles::Lang->get("prompts.canteenEmptyPrompt");


	urinatePrompt = DataFiles::Lang->get("prompts.urinate");
	backPrompt = DataFiles::Lang->get("prompts.back");
	sleepPrompt = DataFiles::Lang->get("prompts.sleep");
	hr2Prompt = DataFiles::Lang->get("prompts.2hr");
	hr4Prompt = DataFiles::Lang->get("prompts.4hr");
	hr8Prompt = DataFiles::Lang->get("prompts.8hr");
	upPrompt = DataFiles::Lang->get("prompts.getUp");
	
	walkPrompt = DataFiles::Lang->get("prompts.walkPrompt");
	runPrompt = DataFiles::Lang->get("prompts.runPrompt");


	int food_drop = GetPrivateProfileInt("CORES", "FOOD_CORE_RATE", 1, ".\\needs.ini");
	int drink_drop = GetPrivateProfileInt("CORES", "DRINK_CORE_RATE", 1, ".\\needs.ini");
	int piss_drop = GetPrivateProfileInt("CORES", "PISS_CORE_RATE", 1, ".\\needs.ini");
	int sleep_drop = GetPrivateProfileInt("CORES", "SLEEP_CORE_RATE", 1, ".\\needs.ini");

	int core_drop = GetPrivateProfileInt("CORES", "CORE_DROP_TIME", 1, ".\\needs.ini");

	int food_drop_amount = GetPrivateProfileInt("CORES", "FOOD_CORE_RATE_DROP", 2, ".\\needs.ini");
	int drink_drop_amount = GetPrivateProfileInt("CORES", "DRINK_CORE_RATE_DROP", 2, ".\\needs.ini");
	int piss_drop_amount = GetPrivateProfileInt("CORES", "PISS_CORE_RATE_DROP", 1, ".\\needs.ini");

	int piss_replenish = GetPrivateProfileInt("CORES", "PISS_CORE_REPLENISH", 120, ".\\needs.ini");


	int food_damage_amount = GetPrivateProfileInt("CORES", "FOOD_CORE_DAMAGE", 2, ".\\needs.ini");
	int drink_damage_amount = GetPrivateProfileInt("CORES", "DRINK_CORE_DAMAGE", 2, ".\\needs.ini");
	int piss_damage_amount = GetPrivateProfileInt("CORES", "PISS_CORE_DAMAGE", 0, ".\\needs.ini");

	int food_on = GetPrivateProfileInt("CORES", "FOOD_ON", 1, ".\\needs.ini");
	int drink_on = GetPrivateProfileInt("CORES", "DRINK_ON", 1, ".\\needs.ini");
	int piss_on = GetPrivateProfileInt("CORES", "PISS_ON", 1, ".\\needs.ini");
	int sleep_on = GetPrivateProfileInt("CORES", "SLEEP_ON", 1, ".\\needs.ini");
	int weight_on = GetPrivateProfileInt("CORES", "WEIGHT_ON", 1, ".\\needs.ini");



	float negative_food_x = GetPrivateProfileInt("POSITION", "NEGATIVE_FOOD_CORE_X", 0, ".\\needs.ini");
	float food_x = GetPrivateProfileInt("POSITION", "FOOD_CORE_X", 0, ".\\needs.ini");

	float negative_food_y = GetPrivateProfileInt("POSITION", "NEGATIVE_FOOD_CORE_Y", 0, ".\\needs.ini");
	float food_y = GetPrivateProfileInt("POSITION", "FOOD_CORE_Y", 0, ".\\needs.ini");

	float negative_food_x_size = GetPrivateProfileInt("POSITION", "NEGATIVE_FOOD_CORE_X_SIZE", 0, ".\\needs.ini");
	float food_x_size = GetPrivateProfileInt("POSITION", "FOOD_CORE_X_SIZE", 0, ".\\needs.ini");

	float negative_food_y_size = GetPrivateProfileInt("POSITION", "NEGATIVE_FOOD_CORE_Y_SIZE", 0, ".\\needs.ini");
	float food_y_size = GetPrivateProfileInt("POSITION", "FOOD_CORE_Y_SIZE", 0, ".\\needs.ini");




	float negative_drink_x = GetPrivateProfileInt("POSITION", "NEGATIVE_DRINK_CORE_X", 0, ".\\needs.ini");
	float drink_x = GetPrivateProfileInt("POSITION", "DRINK_CORE_X", 0, ".\\needs.ini");

	float negative_drink_y = GetPrivateProfileInt("POSITION", "NEGATIVE_DRINK_CORE_Y", 0, ".\\needs.ini");
	float drink_y = GetPrivateProfileInt("POSITION", "DRINK_CORE_Y", 0, ".\\needs.ini");

	float negative_drink_x_size = GetPrivateProfileInt("POSITION", "NEGATIVE_DRINK_CORE_X_SIZE", 0, ".\\needs.ini");
	float drink_x_size = GetPrivateProfileInt("POSITION", "DRINK_CORE_X_SIZE", 0, ".\\needs.ini");

	float negative_drink_y_size = GetPrivateProfileInt("POSITION", "NEGATIVE_DRINK_CORE_Y_SIZE", 0, ".\\needs.ini");
	float drink_y_size = GetPrivateProfileInt("POSITION", "DRINK_CORE_Y_SIZE", 0, ".\\needs.ini");



	float negative_piss_x = GetPrivateProfileInt("POSITION", "NEGATIVE_PISS_CORE_X", 0, ".\\needs.ini");
	float piss_x = GetPrivateProfileInt("POSITION", "PISS_CORE_X", 0, ".\\needs.ini");

	float negative_piss_y = GetPrivateProfileInt("POSITION", "NEGATIVE_PISS_CORE_Y", 0, ".\\needs.ini");
	float piss_y = GetPrivateProfileInt("POSITION", "PISS_CORE_Y", 0, ".\\needs.ini");

	float negative_piss_x_size = GetPrivateProfileInt("POSITION", "NEGATIVE_PISS_CORE_X_SIZE", 0, ".\\needs.ini");
	float piss_x_size = GetPrivateProfileInt("POSITION", "PISS_CORE_X_SIZE", 0, ".\\needs.ini");

	float negative_piss_y_size = GetPrivateProfileInt("POSITION", "NEGATIVE_PISS_CORE_Y_SIZE", 0, ".\\needs.ini");
	float piss_y_size = GetPrivateProfileInt("POSITION", "PISS_CORE_Y_SIZE", 0, ".\\needs.ini");




	float negative_canteen_x = GetPrivateProfileInt("POSITION", "NEGATIVE_CANTEEN_CORE_X", 0, ".\\needs.ini");
	float canteen_x = GetPrivateProfileInt("POSITION", "CANTEEN_CORE_X", 0, ".\\needs.ini");

	float negative_canteen_y = GetPrivateProfileInt("POSITION", "NEGATIVE_CANTEEN_CORE_Y", 0, ".\\needs.ini");
	float canteen_y = GetPrivateProfileInt("POSITION", "CANTEEN_CORE_Y", 0, ".\\needs.ini");

	float negative_canteen_x_size = GetPrivateProfileInt("POSITION", "NEGATIVE_CANTEEN_CORE_X_SIZE", 0, ".\\needs.ini");
	float canteen_x_size = GetPrivateProfileInt("POSITION", "CANTEEN_CORE_X_SIZE", 0, ".\\needs.ini");

	float negative_canteen_y_size = GetPrivateProfileInt("POSITION", "NEGATIVE_CANTEEN_CORE_Y_SIZE", 0, ".\\needs.ini");
	float canteen_y_size = GetPrivateProfileInt("POSITION", "CANTEEN_CORE_Y_SIZE", 0, ".\\needs.ini");




	float negative_weight_x = GetPrivateProfileInt("POSITION", "NEGATIVE_WEIGHT_CORE_X", 0, ".\\needs.ini");
	float weight_x = GetPrivateProfileInt("POSITION", "WEIGHT_CORE_X", 0, ".\\needs.ini");

	float negative_weight_y = GetPrivateProfileInt("POSITION", "NEGATIVE_WEIGHT_CORE_Y", 0, ".\\needs.ini");
	float weight_y = GetPrivateProfileInt("POSITION", "WEIGHT_CORE_Y", 0, ".\\needs.ini");




	float negative_sleep_x = GetPrivateProfileInt("POSITION", "NEGATIVE_SLEEP_CORE_X", 0, ".\\needs.ini");
	float sleep_x = GetPrivateProfileInt("POSITION", "SLEEP_CORE_X", 0, ".\\needs.ini");

	float negative_sleep_y = GetPrivateProfileInt("POSITION", "NEGATIVE_SLEEP_CORE_Y", 0, ".\\needs.ini");
	float sleep_y = GetPrivateProfileInt("POSITION", "SLEEP_CORE_Y", 0, ".\\needs.ini");

	food_x /= 1000;
	food_y /= 1000;

	drink_x /= 1000;
	drink_y /= 1000;

	piss_x /= 1000;
	piss_y /= 1000;

	canteen_x /= 1000;
	canteen_y /= 1000;

	sleep_x /= 1000;
	sleep_y /= 1000;

	weight_x /= 1000;
	weight_y /= 1000;



	negative_food_x /= 1000;
	negative_food_y /= 1000;

	negative_drink_x /= 1000;
	negative_drink_y /= 1000;

	negative_piss_x /= 1000;
	negative_piss_y /= 1000;

	negative_canteen_x /= 1000;
	negative_canteen_y /= 1000;


	negative_sleep_x /= 1000;
	negative_sleep_y /= 1000;

	negative_weight_x /= 1000;
	negative_weight_y /= 1000;


	int menu_time = GetPrivateProfileInt("MISC", "MENU_TIME", 180, ".\\needs.ini");
	int menu_key = GetPrivateProfileInt("MISC", "MENU_KEY", 80, ".\\needs.ini");

	int food_gain = GetPrivateProfileInt("MISC", "FOOD_GAIN", 100, ".\\needs.ini");
	int alcohol = GetPrivateProfileInt("MISC", "ALCOHOL_BEHAVIOUR", 1, ".\\needs.ini");
	int alcohol_deh = GetPrivateProfileInt("MISC", "ALCOHOL_DEHYDRATION", 10, ".\\needs.ini");
	//0 - does nothing 1 - quenches thirst 2 - dehydrates you
	int during_missions = GetPrivateProfileInt("MISC", "WORKS_DURING_MISSIONS", 1, ".\\needs.ini");


	int drinking_anim = GetPrivateProfileInt("MISC", "CANTEEN_ANIMATION_TYPE", 1, ".\\needs.ini");

	bool drinking_walk = GetPrivateProfileInt("MISC", "THIRSTY_WALKSTYLE", 1, ".\\needs.ini");
	bool hunger_walk = GetPrivateProfileInt("MISC", "HUNGRY_WALKSTYLE", 1, ".\\needs.ini");
	bool sleep_walk = GetPrivateProfileInt("MISC", "SLEEP_WALKSTYLE", 1, ".\\needs.ini");

	bool release = GetPrivateProfileInt("MISC", "STUFF", 0, ".\\needs.ini");

	bool catalog_mode = GetPrivateProfileInt("MISC", "CATALOG_MODE", 0, ".\\needs.ini");
	
	bool holster_on = GetPrivateProfileInt("MISC", "HOLSTER_OFFHAND_ON", 1, ".\\needs.ini");

	int save_timer = 0;


	int piss_timer = 0;


	int consume_timer = 0;


	int wash_timer = 0;
	int wash_on = 0;



	int start_anim = 0;
	int drink_anim_timer = 0;

	int start_pissing = 0;
	int piss_anim_timer = 0;


	bool drink_anim = 0;

	bool drink_empty_anim = 0;
	bool hunger_empty_anim = 0;
	bool sleep_empty_anim = 0;


	bool extend_hud = 0;

	int extend_hud_timer = 0;

	int hunger_anim = 0;
	int sleep_anim = 0;



	int weight_core = 0;

	bool sleep_menu = 0;

	bool inAlt = 0;

	bool inWheel = 0;

	const int sleep_core_max = 100;

	bool is_sleeping = 0;

	int sleep_timer = 0;

	int exhaustion_timer = 0;

	int exhaustion_timer2 = 0;

	int weapon_timer = 0;

	bool holster = 0;

	bool force_walk = 0;

	bool in_hud = 0;

	stringstream testStream;

	int deadeyeTimer = 0;

	int horseTimer = 0;

	int satchelTimer = 0;

	bool satchel_opened = 0;

	bool camp_eat_timer = 0;

	int food_timer = 0;

	int last_stamina = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(PLAYER::PLAYER_PED_ID(), 1);
	int last_hp = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(PLAYER::PLAYER_PED_ID(), 0);
	int last_deadeye = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(PLAYER::PLAYER_PED_ID(), 2);

	int pee_press = 0;

	bool pissing = 0;

	bool survival_menu = 0;

	int core_drop_timer = 0;

	bool coffee_exists = 0;

	int piss_timer2 = 0;

	const int ARR_SIZE = 30000;
	Object objects[ARR_SIZE];
	int count = worldGetAllObjects(objects, ARR_SIZE);


	//load
	ifstream LoadFile;
	LoadFile.open("needs.dat", std::ofstream::in);

	LoadFile >> canteen >> food_core >> drink_core >> piss_core >> sleep_core >> canteen_type >> number_of_drinks;

	LoadFile.close();


	//save
	ofstream SaveFile;
	SaveFile.open("needs.dat", std::ofstream::trunc);

	SaveFile << canteen << " " << food_core << " " << drink_core << " " << piss_core << " " << sleep_core << " " << canteen_type << " " << number_of_drinks << " ";

	SaveFile.close();


	initialize();

	currentStaminaLevel = ATTRIBUTE::GET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1);
	currentDeadeyeLevel = ATTRIBUTE::GET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2);
	currentHealthLevel = ATTRIBUTE::GET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 0);


	//WORLD_HUMAN_BARTENDER_BEER

	int water_timer;
	int last_timer;
	bool testdiscard = false;
	bool shoulddrop = false;
	int droptimer = 0;
	
	int TestTimer = 0;

	int DropPromptTimer = 0;

	int drinksTimer = 0;
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, false);

	//EVENT::ADD_SHOCKING_EVENT_AT_POSITION(MISC::GET_HASH_KEY("EVENT_SHOCKING_VISIBLE_REACTION"), playerPos.x, playerPos.y, playerPos.z, 0, 0, 0, 0, 0, 0, 0);

	while (true)
	{
		Ped player = PLAYER::PLAYER_PED_ID();
		Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(player, true, false);
		Ped playerHorse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(0);

		HUD::_UIPROMPT_SET_ENABLED(Prompt_Sleep, 1);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Sleep, 1);

		HUD::_UIPROMPT_SET_ENABLED(Prompt_Leave, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Leave, 0);

		HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap1, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap1, 0);

		HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap2, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap2, 0);

		HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap3, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap3, 0);

		HUD::_UIPROMPT_SET_ENABLED(Prompt_Holster, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Holster, 0);

		HUD::_UIPROMPT_SET_ENABLED(Prompt_Walk, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Walk, 0);

		
		if (IsKeyPressed(VK_KEY_0)) {
			
		}

		//Entity testEntity;
		//testEntity = PED::_0x4D0D2E3D8BC000EB(player, "p_bottleBeer01x_PH_R_HAND", 1);

		//stringstream item_interaction;
		
		//item_interaction << TASK::_GET_ITEM_INTERACTION_FROM_PED(player);

		//"ADD_REASON_DEFAULT"

		//Misc::showSubtitle(item_interaction.str().c_str());

		//INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(playerID, DOCUMENT_SILTWATER_MOONSHINE_DISTILLERY_DEED, INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID())


		
		
	
			//const b1 = new ArrayBuffer(8 * 12);
			//const a2 = new DataView(b1);

			//const b2 = new ArrayBuffer(8 * 12);
			//const a3 = new DataView(b2);

			 
		struct a1
		{
			alignas(8) int unk1 = 0;
			alignas(8) int unk2 = 0;
			alignas(8) int unk3 = 0;
			alignas(8) int unk4 = 0;
			alignas(8) int unk5 = 0;
			alignas(8) int unk6 = 0;
			alignas(8) int unk7 = 0;
			alignas(8) int unk8 = 0;
			alignas(8) int unk9 = 0;
			alignas(8) int unk10 = 0;
			alignas(8) int unk11 = 0;
			alignas(8) int unk12 = 0;
		};

		struct a2
		{
			alignas(8) int unk1 = 0;
			alignas(8) int unk2 = 0;
			alignas(8) int unk3 = 0;
			alignas(8) int unk4 = 0;
			alignas(8) int unk5 = 0;
			alignas(8) int unk6 = 0;
			alignas(8) int unk7 = 0;
			alignas(8) int unk8 = 0;
			alignas(8) int unk9 = 0;
			alignas(8) int unk10 = 0;
			alignas(8) int unk11 = 0;
			alignas(8) int unk12 = 0;
		};

		a1 b1;
		a2 b2;

		//INVENTORY::_0xCB5D11F9508A928D(1, (Any*)&b1, (Any*)&b2, MISC::GET_HASH_KEY("UPGRADE_STAMINA_TANK_1"), 1084182731, 1, 752097756);


		
		HUD::_UIPROMPT_SET_ENABLED(Prompt_Drop, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drop, 0);
		/*
		if (PAD::IS_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_FRONTEND_CANCEL")) && WEAPON::_IS_WEAPON_TWO_HANDED(Misc::getCurrentPlayerWeapon())) {
			DropPromptTimer += 1;
			

		}
		if (!PED::IS_PED_STOPPED(player)) {
			DropPromptTimer = 0;
		}
		
		if (DropPromptTimer >= 60 && PED::IS_PED_STOPPED(player)) {
			HUD::_UIPROMPT_SET_ENABLED(Prompt_Drop, 1);
			HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drop, 1);
			
		}
		else {
			//DropPromptTimer = 0;
			HUD::_UIPROMPT_SET_ENABLED(Prompt_Drop, 0);
			HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drop, 0);
		}

		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Drop)) {
			DropPromptTimer = 0;
		
				if (WEAPON::_IS_WEAPON_TWO_HANDED(Misc::getCurrentPlayerWeapon()))
				{



					STREAMING::REQUEST_ANIM_DICT("mech_inventory@discard@fallback@base@longarms@idle");
					STREAMING::REQUEST_ANIM_DICT("mech_inventory@discard@fallback@base_crouch@longarms@idle");





					if (!PED::_GET_PED_CROUCH_MOVEMENT(player))
					{
						TASK::TASK_PLAY_ANIM(player, "mech_inventory@discard@fallback@base@longarms@idle", "discard", 3, -2, -1, 24, 0, 0, 0, 0, 0, 0);
					}
					else
					{
						TASK::TASK_PLAY_ANIM(player, "mech_inventory@discard@fallback@base_crouch@longarms@idle", "discard", 3, -2, -1, 24, 0, 0, 0, 0, 0, 0);
					}








				}



			
		}
		
			if ((ENTITY::_GET_ENTITY_ANIM_CURRENT_TIME(player, "mech_inventory@discard@fallback@base@longarms@idle", "discard") > (0.14)) || (ENTITY::_GET_ENTITY_ANIM_CURRENT_TIME(player, "mech_inventory@discard@fallback@base_crouch@longarms@idle", "discard") > (0.14)))
			{
				WEAPON::_0xCEF4C65DE502D367(player, 1, 0, 0, 0);
				TASK::STOP_ANIM_TASK(player, "mech_inventory@discard@fallback@base@longarms@idle", "discard", 1.0);
				TASK::STOP_ANIM_TASK(player, "mech_inventory@discard@fallback@base_crouch@longarms@idle", "discard", 1.0);

			}
			
			*/
			//STREAMING::REQUEST_ANIM_DICT("mech_weapons_longarms@loco@arthur@normal@rifle@idle");


			/*
			if (PED::IS_PED_STOPPED(PLAYER::PLAYER_PED_ID()))
			{

				if ((MISC::GET_GAME_TIMER() >= TestTimer) && (MISC::GET_GAME_TIMER() <= TestTimer))
				{
					if (TASK::_GET_ITEM_INTERACTION_FROM_PED(player) == 0)
					{
						_NAMESPACE29::_0x0EABF182FBB63D72(PLAYER::PLAYER_PED_ID(), 1, 1);
					}
				}


				if (!ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "mech_weapons_longarms@loco@arthur@normal@rifle@idle", "idle_mainhand_vertical", 0) && ((MISC::GET_GAME_TIMER() >= TestTimer) && (MISC::GET_GAME_TIMER() <= TestTimer + 50)))
				{

					TASK::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "mech_weapons_longarms@loco@arthur@normal@rifle@idle", "idle_mainhand_vertical", 1.0, 1, -1, 32, -1, 0, 0, 1, 0, 1);



				}
			}
			else
			{
				TestTimer = MISC::GET_GAME_TIMER() + 5000;



			}
			*/









		if (during_missions == 1 || MISC::GET_MISSION_FLAG() == 0) {

			if (IsKeyPressed(VK_KEY_0)) {
				//INVENTORY::_0xCB5D11F9508A928D(1, (Any*)&b1, (Any*)&b2, MISC::GET_HASH_KEY("UPGRADE_STAMINA_TANK_1"), 1084182731, 1, 752097756);
				//INVENTORY::_0xCB5D11F9508A928D(1, (Any*)&b1, (Any*)&b2, MISC::GET_HASH_KEY("UPGRADE_HEALTH_TANK_1"), 1084182731, 1, 752097756);
				//INVENTORY::_0xCB5D11F9508A928D(1, (Any*)&b1, (Any*)&b2, MISC::GET_HASH_KEY("UPGRADE_DEADEYE_TANK_1"), 1084182731, 1, 752097756);

				//DOCUMENT_SILTWATER_MOONSHINE_DISTILLERY_DEED
				//INVENTORY::_0xCB5D11F9508A928D(1, (Any*)&b1, (Any*)&b2, MISC::GET_HASH_KEY("CONSUMABLE_GIN_USED"), MISC::GET_HASH_KEY("SLOTID_WEAPON_0"), 1, 752097756);

				/*
				PED::CLEAR_PED_ENV_DIRT(player);
				PED::CLEAR_PED_BLOOD_DAMAGE(player);
				GRAPHICS::REMOVE_DECALS_IN_RANGE(playerPos.x, playerPos.y, playerPos.z, 5.f);
				//removeItemFromPedInventory(player, MISC::GET_HASH_KEY("CONSUMABLE_GIN"), "Water", "inventory_items");
				PED::CLEAR_PED_DAMAGE_DECAL_BY_ZONE(player, 10, "ALL");

				PED::SET_PED_WETNESS_HEIGHT(player, 10.f);
				*/
			}
			

			int current_water = INVENTORY::_0xE787F05DFC977BDE(1, MISC::GET_HASH_KEY("CONSUMABLE_GIN_USED"), 0);

			if (water_timer < MISC::GET_GAME_TIMER()) {
				last_timer = INVENTORY::_0xE787F05DFC977BDE(1, MISC::GET_HASH_KEY("CONSUMABLE_GIN_USED"), 0);

				water_timer = MISC::GET_GAME_TIMER() + 1000;
			}

			if (current_water < last_timer) {
				if (drink_core < 98) {
					number_of_drinks++;
					drink_core = 100;
				}
			}


			//if (drink_core < 98) {
				if (TASK::_GET_ITEM_INTERACTION_FROM_PED(player) == 788082563 || TASK::_GET_ITEM_INTERACTION_FROM_PED(player) == 3946734674) {
					if (drinksTimer < MISC::GET_GAME_TIMER()) {
						number_of_drinks++;

						drink_core = 100;
						drinksTimer = MISC::GET_GAME_TIMER() + 2000;
					}
				}
			//}


			save_timer++;
			if (save_timer >= 60) {
			


				ofstream SaveFile;
				SaveFile.open("needs.dat", std::ofstream::trunc);

				SaveFile << canteen << " " << food_core << " " << drink_core << " " << piss_core << " " << sleep_core << " " << canteen_type << " " << number_of_drinks << " ";

				SaveFile.close();
				save_timer = 0;
			}

		

			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Sleep)) {
				sleep_menu = 1;
			}

			if (sleep_menu) {

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap1, 1);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap1, 1);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap2, 1);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap2, 1);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap3, 1);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap3, 1);

				HUD::_UIPROMPT_SET_ACTIVE_GROUP_THIS_FRAME(1323335645, "SLEEP", 1, 0, 0, 0);
			}


			if (!TASK::IS_PED_ACTIVE_IN_SCENARIO(player, 1)) {
				sleep_menu = 0;
			}

			if (sleep_timer < MISC::GET_GAME_TIMER()) {
				if (isPlayerStartedSleepScenario() && !is_in_sequence) {
					if (!is_sleeping) {
						sleep_core += 80;
						is_sleeping = 1;

					}
				}
				else {
					is_sleeping = 0;
				}
			}


			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Nap1)) {
				sleep_menu = 0;
				sleep_timer = MISC::GET_GAME_TIMER() + 15000;
				sleepSeq(2);

			}
			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Nap2)) {
				sleep_menu = 0;
				sleep_timer = MISC::GET_GAME_TIMER() + 15000;
				sleepSeq(4);
			}
			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Nap3)) {
				sleep_menu = 0;
				sleep_timer = MISC::GET_GAME_TIMER() + 15000;
				sleepSeq(8);
			}



			if (sitting) {

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Leave, 1);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Leave, 1);



				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_MOVE_LR") || PAD::IS_DISABLED_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_MOVE_UD")))) {
					TASK::CLEAR_PED_TASKS(player, 1, 0);
					sitting = 0;
					HUD::_DISPLAY_HUD_COMPONENT(-1679307491);
					is_in_sequence = 0;
				}
				if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Leave)) {
					TASK::CLEAR_PED_TASKS(player, 1, 0);
					sitting = 0;
					HUD::_DISPLAY_HUD_COMPONENT(-1679307491);
					is_in_sequence = 0;
				}

			}



			if (PAD::IS_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_AIM"))) {
				//PED::SET_PED_MAX_MOVE_BLEND_RATIO(PLAYER::PLAYER_PED_ID(), 1.4f);

				if (force_walk) {
					HUD::_UIPROMPT_SET_TEXT(Prompt_Walk, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", runPrompt));
				}
				else {
					HUD::_UIPROMPT_SET_TEXT(Prompt_Walk, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", walkPrompt));
				}

				if (!INTERIOR::IS_INTERIOR_SCENE() && !PED::IS_PED_IN_COMBAT(player, 0) && Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_UNARMED")) {
					if (!release) {
						HUD::_UIPROMPT_SET_ENABLED(Prompt_Walk, 1);
						HUD::_UIPROMPT_SET_VISIBLE(Prompt_Walk, 1);
					}
				}
			}

			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Walk)) {
				force_walk = !force_walk;
			}

			if (PED::IS_PED_IN_COMBAT(PLAYER::PLAYER_PED_ID(), 0)) {
				force_walk = 0;
			}

			if (force_walk) {
				PED::SET_PED_MAX_MOVE_BLEND_RATIO(PLAYER::PLAYER_PED_ID(), 1.5f);
			}

			Vector3 horsePos = ENTITY::GET_ENTITY_COORDS(playerHorse, 1, 0);
			if (WEAPON::GET_ALLOW_DUAL_WIELD(player)) {
				Entity ent;
				if (PLAYER::IS_PLAYER_TARGETTING_ANYTHING(0) && distanceBetween(playerPos, horsePos) <= 8.7f) {
					if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(0, &ent)) {
						if (ENTITY::IS_ENTITY_A_PED(ent)) {
							Ped theTarget = ent;
							if (theTarget == playerHorse) {
								if (!release) {
									HUD::_UIPROMPT_SET_GROUP(Prompt_Holster, HUD::_UIPROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(ent), 0);
									if (holster_on) {
										HUD::_UIPROMPT_SET_ENABLED(Prompt_Holster, 1);
										HUD::_UIPROMPT_SET_VISIBLE(Prompt_Holster, 1);
									}
								}
							}
						}

					}
				}
				//WEAPON::_SET_ALLOW_DUAL_WIELD(player, 0);
				if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Holster)) {


					WEAPON::SET_CURRENT_PED_WEAPON(player, gatherWeapons(), 1, 0, 0, 0);

					TASK::TASK_ANIMAL_INTERACTION(player, playerHorse, MISC::GET_HASH_KEY("Interaction_HolsterRightPistol"), 0, 1);

					weapon_timer = MISC::GET_GAME_TIMER() + 1000;

					//WEAPON::_SET_ALLOW_DUAL_WIELD(player, !WEAPON::GET_ALLOW_DUAL_WIELD(player));

					holster = 1;

				}

				if (weapon_timer < MISC::GET_GAME_TIMER() && holster) {
					WEAPON::_SET_ALLOW_DUAL_WIELD(player, 0);
					holster = 0;
				}

			}
			else {
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Holster, 0);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Holster, 0);
			}

		


			if (drinking_walk) {
				if (drink_core == 0) {

					if (!drink_empty_anim) {
						GRAPHICS::ANIMPOSTFX_PLAY("PlayerRPGEmptyCoreStamina");
						AUDIO::PLAY_SOUND_FRONTEND("STAMINA", "Player_Core_Empty_Sounds", 1, 0);


						drink_empty_anim = 1;
					}

					PED::_0x923583741DC87BCE(player, "DEFAULT");
					PED::_0x89F5E7ADECCCB49C(player, "very_drunk");

					drink_anim = 1;


				}
				else {
					drink_empty_anim = 0;

					if (drink_anim) {
						if (PED::IS_PED_MODEL(player, 0xD7114C9))
						{
							PED::_0x923583741DC87BCE(player, "arthur_healthy");


						}
						else
							if (PED::IS_PED_MODEL(player, 0xB69710))
							{
								PED::_0x923583741DC87BCE(player, "john_marston");
							}
						drink_anim = 0;
					}
				}
			}
			if (hunger_walk) {
				if (food_core == 0) {

					if (!hunger_empty_anim) {
						GRAPHICS::ANIMPOSTFX_PLAY("PlayerRPGEmptyCoreHealth");
						AUDIO::PLAY_SOUND_FRONTEND("HEALTH", "Player_Core_Empty_Sounds", 1, 0);


						hunger_empty_anim = 1;
					}

					PED::_0x923583741DC87BCE(player, "DEFAULT");
					PED::_0x89F5E7ADECCCB49C(player, "very_drunk");

					hunger_anim = 1;


				}
				else {
					hunger_empty_anim = 0;

					if (hunger_anim) {
						if (PED::IS_PED_MODEL(player, 0xD7114C9))
						{
							PED::_0x923583741DC87BCE(player, "arthur_healthy");


						}
						else
							if (PED::IS_PED_MODEL(player, 0xB69710))
							{
								PED::_0x923583741DC87BCE(player, "john_marston");
							}
						hunger_anim = 0;
					}
				}

			}
			if (sleep_walk && sleep_on) {
				if (sleep_core == 0) {

					if (!sleep_empty_anim) {
						GRAPHICS::ANIMPOSTFX_PLAY("PlayerRPGEmptyCoreStamina");
						AUDIO::PLAY_SOUND_FRONTEND("STAMINA", "Player_Core_Empty_Sounds", 1, 0);
						GRAPHICS::ANIMPOSTFX_PLAY("PoisonDartPassOut");
						exhaustion_timer = MISC::GET_GAME_TIMER() + 8400;
						exhaustion_timer2 = MISC::GET_GAME_TIMER() + 999999999;
						sleep_empty_anim = 1;
					}

					if (exhaustion_timer < MISC::GET_GAME_TIMER()) {



						if (PED::IS_PED_ON_MOUNT(player)) {
							PED::SET_PED_TO_RAGDOLL(player, 5000, 25000, 0, 1, 1, 1);
						}


						TASK::_TASK_SMART_FLEE_STYLE_PED(playerHorse, player, 3, 0, -999.0f, -1, 0);



						TASK::_TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WORLD_HUMAN_DRUNK_PASSED_OUT_FLOOR"), -1, 1, 1, 1, 1);
						exhaustion_timer = MISC::GET_GAME_TIMER() + 999999999;
						exhaustion_timer2 = MISC::GET_GAME_TIMER() + 1200;


					}

					if (exhaustion_timer2 < MISC::GET_GAME_TIMER()) {

						CAM::DO_SCREEN_FADE_OUT(1500);

						CAM::DO_SCREEN_FADE_IN(0);

						HUD::_HIDE_HUD_COMPONENT(-1679307491);



						AUDIO::TRIGGER_MUSIC_EVENT("SLEEP_TRANSITION_START");


						if (CLOCK::GET_CLOCK_HOURS() >= 5 && CLOCK::GET_CLOCK_HOURS() < 9) {
							GRAPHICS::ANIMPOSTFX_PLAY("skytl_0600_01clear");
						}
						if (CLOCK::GET_CLOCK_HOURS() >= 9 && CLOCK::GET_CLOCK_HOURS() < 12) {
							GRAPHICS::ANIMPOSTFX_PLAY("skytl_0900_01clear");
						}
						if (CLOCK::GET_CLOCK_HOURS() >= 12 && CLOCK::GET_CLOCK_HOURS() < 15) {
							GRAPHICS::ANIMPOSTFX_PLAY("skytl_1200_01clear");
						}
						if (CLOCK::GET_CLOCK_HOURS() >= 15 && CLOCK::GET_CLOCK_HOURS() < 23) {
							GRAPHICS::ANIMPOSTFX_PLAY("skytl_1500_03clouds");
						}
						if (CLOCK::GET_CLOCK_HOURS() >= 23 || (CLOCK::GET_CLOCK_HOURS() >= 0 && CLOCK::GET_CLOCK_HOURS() < 5)) {
							GRAPHICS::ANIMPOSTFX_PLAY("skytl_0000_01clear");
						}

						CLOCK::ADVANCE_CLOCK_TIME_TO(CLOCK::GET_CLOCK_HOURS() + 2, 0, 0);


						sleep_core += 30;

						ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) + 50);


						WAIT(5000);


						sitting = 1;

				
						AUDIO::TRIGGER_MUSIC_EVENT("SLEEP_TRANSITION_STOP");

					}

					PED::_0x923583741DC87BCE(player, "DEFAULT");
					PED::_0x89F5E7ADECCCB49C(player, "very_drunk");

					sleep_anim = 1;


				}
				else {
					sleep_empty_anim = 0;

					if (sleep_anim) {
						if (PED::IS_PED_MODEL(player, 0xD7114C9))
						{
							PED::_0x923583741DC87BCE(player, "arthur_healthy");


						}
						else
							if (PED::IS_PED_MODEL(player, 0xB69710))
							{
								PED::_0x923583741DC87BCE(player, "john_marston");
							}
						sleep_anim = 0;
					}
				}

			}





			//foodCore(food_core);
			if (food_core > food_core_max) {
				food_core = food_core_max;
			}
			if (drink_core > drink_core_max) {
				drink_core = drink_core_max;
			}
			if (piss_core > piss_core_max) {
				piss_core = piss_core_max;
			}
			if (sleep_core > sleep_core_max) {
				sleep_core = sleep_core_max;
			}

			if (food_core < 0) {
				food_core = 0;
			}
			if (drink_core < 0) {
				drink_core = 0;
			}
			if (piss_core < 0) {
				piss_core = 0;
			}
			if (sleep_core < 0) {
				sleep_core = 0;
			}


			//WORLD_HUMAN_PEE


			std::stringstream text;



			if (GRAPHICS::ANIMPOSTFX_IS_RUNNING("WheelHUDIn") && PAD::IS_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_SELECT_RADAR_MODE"))) {
				if (!inAlt) {
					if (no_slowdown) {
						extend_hud_timer = MISC::GET_GAME_TIMER() + 368; //2950
					}


					inAlt = 1;
				}

			}
			else {
				inAlt = 0;
				if (PAD::IS_CONTROL_JUST_RELEASED(0, MISC::GET_HASH_KEY("INPUT_REVEAL_HUD"))) {



					extend_hud = 1;

					extend_hud_timer = MISC::GET_GAME_TIMER() + 2950; //2950

				}

			}





			if (extend_hud_timer < MISC::GET_GAME_TIMER()) {
				extend_hud = 0;
			}


			if (PAD::IS_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_OPEN_WHEEL_MENU")) && GRAPHICS::ANIMPOSTFX_IS_RUNNING("WheelHUDIn")) {
				extend_hud = 1;
				inWheel = 1;
			}
			else {
				if (inWheel) {
					extend_hud = 0;
				}
				inWheel = 0;
			}


			if (GRAPHICS::ANIMPOSTFX_IS_RUNNING("PauseMenuIn")) {
				extend_hud = 0;
				inAlt = 0;
				extend_hud_timer = 0;
			}


			if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_OPEN_SATCHEL_MENU")) || PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_OPEN_SATCHEL_HORSE_MENU"))) {

				satchel_opened = 1;
				satchelTimer = MISC::GET_GAME_TIMER() + 3000;

			}

			if (satchelTimer < MISC::GET_GAME_TIMER()) {
				satchel_opened = 0;

			}

			if (satchel_opened) {



				if (GRAPHICS::ANIMPOSTFX_IS_RUNNING("WheelHUDIn")) {
					extend_hud = 1;
					in_hud = 1;
					satchel_opened = 0;
				}


			}
			else {
				if (in_hud && !GRAPHICS::ANIMPOSTFX_IS_RUNNING("WheelHUDIn")) {
					HUD::_DISPLAY_HUD_COMPONENT(-1679307491);
					//INPUT_FRONTEND_ACCEPT = 0xC7B5340A,
					//INPUT_FRONTEND_CANCEL = 0x156F7119,

					in_hud = 0;
					extend_hud = 0;

				}

				if (in_hud) {
					HUD::_HIDE_HUD_COMPONENT(-1679307491);
					extend_hud = 1;
				}



			}


			if (food_on) {
				if (!extend_hud || !core_adjustment) {
					foodCore(food_core, 0.065 + food_x - negative_food_x, menuY - 0.012 + food_y - negative_food_y, 0, 0);
				}
				else {
					foodCore(food_core, 0.065 + 0.01 + food_x - negative_food_x, menuY - 0.109 - 0.012 + food_y - negative_food_y, 0.01, 0.02);
				}
			}
			if (drink_on) {

				if (!extend_hud || !core_adjustment) {
					drinkCore(drink_core, 0.092 + drink_x - negative_drink_x, menuY - 0.012 + drink_y - negative_drink_y, 0, 0, extend_hud);
				}
				else {
					drinkCore(drink_core, 0.092 + 0.026 + drink_x - negative_drink_x, menuY - 0.11 - 0.012 + drink_y - negative_drink_y, 0.01, 0.02, extend_hud);
				}
			}
			if (piss_on) {


				//!GRAPHICS::ANIMPOSTFX_IS_RUNNING("WheelHUDIn") && 
				if (!extend_hud || !core_adjustment) {
					peeCore(piss_core, 0.1175 + piss_x - negative_piss_x, menuY + 0.002 + piss_y - negative_piss_y, 0, 0, extend_hud);
				}
				else {
					peeCore(piss_core, 0.1175 + 0.035 + piss_x - negative_piss_x, menuY - 0.10 + 0.002 + piss_y - negative_piss_y, 0.01, 0.02, extend_hud);
				}

			}

			if (sleep_on) {
				if (!extend_hud || !core_adjustment) { //+ 0.002
					sleepCore(sleep_core, 0.079 + sleep_x - negative_sleep_x, menuY - 0.042 + sleep_y - negative_sleep_y, 0, 0, extend_hud);

				}
				else {
					sleepCore(sleep_core, 0.079 + 0.018 + sleep_x - negative_sleep_x, menuY - 0.042 - 0.108 + sleep_y - negative_sleep_y, 0.01, 0.02, extend_hud);

				}
			}

			if (weight_on) {



				if (!extend_hud || !core_adjustment) {

					if (survival_menu || global_weight >= 40) {
						weightCore(0, 0.13 + weight_x - negative_weight_x, menuY + 0.255 + weight_y - negative_weight_y, 0, 0);
					}
					else {
						weightCore(0, -10, -10, 0, 0);
					}

				}
				else {
					weightCore(0, 0.17 + weight_x - negative_weight_x, menuY + 0.255 + weight_y - negative_weight_y, 0.01, 0.02);


				}
			}



			if (drink_on) {

				
				
				if (!catalog_mode) {
					if (!extend_hud || !core_adjustment) {
						if (ENTITY::IS_ENTITY_IN_WATER(player) || survival_menu) {
							canteenCore(canteen, 0.015 + canteen_x - negative_canteen_x, menuY + 0.25 + canteen_y - negative_canteen_y, 0, 0, extend_hud);
						}
					}
					else {
						canteenCore(canteen, 0.015 + canteen_x - negative_canteen_x, menuY + 0.25 + canteen_y - negative_canteen_y, 0.01, 0.02, extend_hud);
					}
				}

			}
			
			if (HUD::IS_RADAR_HIDDEN() == true) {



			}
			else {
				/*
				if (!extend_hud) {

				}
				else {
					float coreX = 0.205;
					float coreY = 0.94;

					Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_1", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_1", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 10) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_1", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 20 && ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 10) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_2", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 30 && ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 20) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_3", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 50 && ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 30) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_4", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) == 50) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_5", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 60 && ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 50) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_6", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 70 && ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 60) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_7", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 80 && ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 70) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_8", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 90 && ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 80) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_9", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 90) {
						Misc::drawSprite("rpg_menu_item_effects", "rpg_tank_10", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}



					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) > 60) {
						Misc::drawSprite("rpg_textures", "rpg_overweight", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}
					if (ATTRIBUTE::GET_ATTRIBUTE_RANK(player, 13) <= 60) {
						Misc::drawSprite("rpg_textures", "rpg_underweight", coreX, coreY, 0.03, 0.05, 0, 255, 255, 255, 255);
					}


				}
				*/

			}


			core_drop_timer++;
			if (core_drop_timer > 60 * core_drop) {
				if (food_core == 0) {
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 2, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2) - food_drop_amount);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) - food_drop_amount);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 0, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0) - food_drop_amount);
					PED::APPLY_DAMAGE_TO_PED(player, food_damage_amount, 0, 0, 0);
				}
				if (drink_core == 0) {
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 2, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2) - drink_drop_amount);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) - drink_drop_amount);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 0, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0) - drink_drop_amount);
					PED::APPLY_DAMAGE_TO_PED(player, drink_damage_amount, 0, 0, 0);
				}



				core_drop_timer = 0;
			}








	

			if (piss_core == 0) {



				piss_timer2++;
				if (piss_timer2 > 600) {

					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 2, 0);

					GRAPHICS::ANIMPOSTFX_PLAY("PlayerRPGEmptyCoreDeadEye");

					piss_core = 100;
					number_of_drinks = 0;
				}
			}
			else {
				piss_timer2 = 0;
			}

			is_night = 0;
			is_running = 0;
			if (CLOCK::GET_CLOCK_HOURS() >= 23 || CLOCK::GET_CLOCK_HOURS() < 5) {
				is_night = 1;
			}
			if (TASK::IS_PED_SPRINTING(player) || TASK::IS_PED_RUNNING(player)) {
				if (!PED::IS_PED_ON_MOUNT(player) && !PED::IS_PED_IN_VEHICLE(player, PED::GET_VEHICLE_PED_IS_USING(player), 0)) {
					is_running = 1;
				}
			}

			if (sleep_on) {

				if (overpowerSleep) {
					if (overpowerSleepTimer < MISC::GET_GAME_TIMER()) {
						overpowerSleep = 0;
					}
				}

				sleep_drop_timer++;
				if (sleep_drop_timer > (60 * sleep_drop * 2) / (2 * (is_running + 1) * (is_night + 1)))
				{
					if (sleep_core > 0) {
						if (!PED::IS_PED_IN_VEHICLE(player, PED::GET_VEHICLE_PED_IS_USING(player), 0)) {
							if (!overpowerSleep && !MISC::GET_MISSION_FLAG()) {
								sleep_core--;
							}
						}
					}

					sleep_drop_timer = 0;
				}
			}

			if (food_on) {
				food_drop_timer++;
				if (food_drop_timer > 120 * (food_drop / temperature_food_drop))
				{
					if (food_core > 0) {
						food_core--;
					}


					food_drop_timer = 0;
				}
			}

			if (drink_on) {
				drink_drop_timer++;

				if (drink_drop_timer > 120 * (drink_drop / temperature_drink_drop)) {

					if (drink_core > 0) {
						drink_core--;
					}

					drink_drop_timer = 0;
				}


			}

			if (piss_on) {
				pee_drop_timer++;
				if (pee_drop_timer > 120 * (piss_drop / drinks_pee_drop)) {

					if (pissing == 0) {
						if (piss_core > 0) {
							piss_core--;
						}
					}


					pee_drop_timer = 0;
				}
			}


	


			/*
			if (ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0) > 99) {
				ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 0, 95);
			}
			if (ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) > 99) {
				ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, 95);
			}
			if (ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2) > 99) {
				ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 2, 95);
			}
			*/



			food_timer++;
			if (food_timer > 60) {

				last_stamina = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1);
				last_hp = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0);
				last_deadeye = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2);



				food_timer = 0;
			}






			consume_timer++;
			if (consume_timer > 20) {

				if (TASK::GET_IS_TASK_ACTIVE(PLAYER::PLAYER_PED_ID(), 471))
				{
					if (refillHungerCoreScenario()) {
						if (tAnimationSet < MISC::GET_GAME_TIMER()) {
							food_core += food_gain;

							consume_timer = 0;
						}
					}

				}

				if (TASK::GET_IS_TASK_ACTIVE(PLAYER::PLAYER_PED_ID(), 471))
				{
					if (refillThirstCoreScenario()) {
						if ((last_stamina < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) || last_deadeye < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2)) && last_hp >= ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0)) {

							if (alcohol == 1) {
								drink_core = 100;
							}
							if (alcohol == 2) {
								drink_core -= alcohol_deh;
							}

							consume_timer = 0;
						}
					}

				}







				if (INTERIOR::IS_INTERIOR_SCENE()) {
					if (last_hp < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0)) {
						//if (!PLAYER::IS_PLAYER_CONTROL_ON(0)) {
						food_core = 100;
						consume_timer = 0;
						//}

					}
				}
				else {

					if (PED::IS_PED_USING_ANY_SCENARIO(player)) {

						int playerScenarioPointType = (int)TASK::_GET_SCENARIO_POINT_TYPE_PED_IS_USING(player); //  _GET_SCENARIO_POINT_TYPE_PED_IS_USING
						//if (playerScenarioPointType == -1767895052 || playerScenarioPointType == 1020517461 || playerScenarioPointType == 1984305068) {

						if (camp_eat_timer < MISC::GET_GAME_TIMER()) {
							if (Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE")
								&& Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_JAWBONE")
								&& Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_JOHN")
								&& Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_MICAH")
								&& Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_MINER")
								&& Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_VAMPIRE")
								&& Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_CIVIL_WAR")
								&& Misc::getCurrentPlayerWeapon() != MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_BEAR")) {

								count = worldGetAllObjects(objects, ARR_SIZE);
								coffee_exists = 0;
								for (int i = 0; i < count; i++)
								{
									if (ENTITY::GET_ENTITY_MODEL(objects[i]) == 0xFB19BD43) {
										coffee_exists = 1;
									}
								}

								if (coffee_exists) {
									if (last_stamina < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) || last_deadeye < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2)) {

										overpowerSleep = 1;
										overpowerSleepTimer = MISC::GET_GAME_TIMER() + 60000 * 3;

										drink_core = 100;
										consume_timer = 0;
										camp_eat_timer = MISC::GET_GAME_TIMER() + 5000;

									}
								}
							}



							if (Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE")
								|| Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_JAWBONE")
								|| Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_JOHN")
								|| Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_MICAH")
								|| Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_MINER")
								|| Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_VAMPIRE")
								|| Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_CIVIL_WAR")
								|| Misc::getCurrentPlayerWeapon() == MISC::GET_HASH_KEY("WEAPON_MELEE_KNIFE_BEAR")) {
								if (last_hp < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0) || last_stamina < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) || last_deadeye < ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2)) {

									food_core = 100;
									consume_timer = 0;
									camp_eat_timer = MISC::GET_GAME_TIMER() + 5000;
								}
							}
						}

					}
				}


				/*
				if (refillThirstCoreScenario()) {
						if (tAnimationSet < MISC::GET_GAME_TIMER()) {
							if (alcohol == 1) {
								drink_core += 50;
							}
							if (alcohol == 2) {
								drink_core -= alcohol_deh;
							}
							food_timer = 0;
						}
					}
				}
				*/
			}



			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Fill) || HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Fill)) {

				//canteen = 4;


				canteen_type = 0;

				AUDIO::PLAY_SOUND_FRONTEND("Core_Fill_Up", "Consumption_Sounds", 1, 0);

				Vector3 Bayou3 = Misc::toVector3(2091.986, -807.265, 41.833);
				if (ZONE::_GET_MAP_ZONE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z, 0x12516AEC) || distanceBetween(playerPos, Bayou3) < 700) {
					//canteen_type = 1;
				}


				if (MISC::_GET_TEMPERATURE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z) <= 0) {
					//canteen_type = 2;
				}


				if (catalog_mode) {
					//canteen = 4;
					INVENTORY::_0xCB5D11F9508A928D(1, (Any*)&b1, (Any*)&b2, MISC::GET_HASH_KEY("CONSUMABLE_GIN_USED"), MISC::GET_HASH_KEY("SLOTID_WEAPON_0"), 4, 752097756);
					removeItemFromPedInventory(player, MISC::GET_HASH_KEY("CONSUMABLE_MOONSHINE"), "Water", "inventory_items");
				}
				else {
					canteen = 4;					
				}

				TASK::CLEAR_PED_TASKS(player, 1, 1);
				TASK::_TASK_START_SCENARIO_IN_PLACE(player, MISC::GET_HASH_KEY("WORLD_HUMAN_CROUCH_INSPECT"), 5000, true, true, 0, true);



			}


			if (canteen_type != 0) {

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Boil, 1);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Boil, 1);
			}
			else {
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Boil, 0);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Boil, 0);
			}


			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Boil) || HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Boil)) {
				CAM::DO_SCREEN_FADE_OUT(10);



				Misc::showSubtitle("Your water is clean");
				canteen_type = 0;


				WAIT(2000);

				CAM::DO_SCREEN_FADE_IN(1);


				TASK::CLEAR_PED_TASKS(player, 1, 1);
				TASK::_TASK_START_SCENARIO_IN_PLACE(0, MISC::GET_HASH_KEY("WORLD_HUMAN_CROUCH_INSPECT"), 5000, true, true, 1, true);

			}



			if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Drink) || HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Drink)) {

				drink_core = 100;

				Vector3 Bayou3 = Misc::toVector3(2091.986, -807.265, 41.833);
				if (ZONE::_GET_MAP_ZONE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z, 0x12516AEC) || distanceBetween(playerPos, Bayou3) < 700) {
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 0, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0) - 30);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) - 30);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 2, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2) - 30);
				}


				if (MISC::_GET_TEMPERATURE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z) <= 0) {
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) - 20);
				}

				TASK::CLEAR_PED_TASKS(player, 1, 1);
				TASK::_TASK_START_SCENARIO_IN_PLACE(player, MISC::GET_HASH_KEY("WORLD_HUMAN_BUCKET_DRINK_GROUND"), 15000, true, true, 1, true);

			}

			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Wash)) {

		
			
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Stop, 1); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Stop, 1);

				//PLAYER::RESTORE_PLAYER_STAMINA(player2, 1);

				TASK::CLEAR_PED_TASKS(player, 1, 1);
				TASK::_TASK_START_SCENARIO_IN_PLACE(player, MISC::GET_HASH_KEY("WORLD_HUMAN_WASH_FACE_BUCKET_GROUND"), -1, true, true, 1, true);


				wash_on = 1;
			}






			if (isPlayerBathing()) {
				piss_core = 100;
				number_of_drinks = 0;
			}
			if (catalog_mode) {
				if (drink_core > 98) {
					INVENTORY::_0x766315A564594401(1, MISC::GET_HASH_KEY("CONSUMABLE_GIN_USED"), 0);
				}
				else {
					INVENTORY::_0x6A564540FAC12211(1, MISC::GET_HASH_KEY("CONSUMABLE_GIN_USED"));
				}
			}

			if (survival_menu == 1) {

				//HUD::_UIPROMPT_SET_GROUP(Prompt_Contribute, -2019190071, 0);
				//HUD::_UIPROMPT_SET_GROUP(Prompt_Ledger, -2019190071, 0);
				if (!wash_on && !pissing) {
					HUD::_UIPROMPT_SET_ACTIVE_GROUP_THIS_FRAME(-2019190071, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Survival Menu"), 1, 0, 0, 0);
				}


				if (ENTITY::IS_ENTITY_IN_WATER(player) == true) {
					HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink_Flask, 0); // _UIPROMPT_SET_ENABLED
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink_Flask, 0);

					HUD::_UIPROMPT_SET_ENABLED(Prompt_Pee, 0); // _UIPROMPT_SET_ENABLED
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Pee, 0);

					
					if (catalog_mode) {
						if (INVENTORY::_0xE787F05DFC977BDE(1, MISC::GET_HASH_KEY("CONSUMABLE_GIN_USED"), 0) == 4) {//Your canteen is full
							HUD::_UIPROMPT_SET_TEXT(Prompt_Fill, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", fullCanteenPrompt)); //your canteen is full
							HUD::_UIPROMPT_SET_ENABLED(Prompt_Fill, 0); // _UIPROMPT_SET_ENABLED
						}
						else { 
							HUD::_UIPROMPT_SET_TEXT(Prompt_Fill, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", refillPrompt)); //Refill canteen
							HUD::_UIPROMPT_SET_ENABLED(Prompt_Fill, 1); // _UIPROMPT_SET_ENABLED
						}

					}
					else {
						if (canteen == 4) {
							HUD::_UIPROMPT_SET_TEXT(Prompt_Fill, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", fullCanteenPrompt)); //set text
							HUD::_UIPROMPT_SET_ENABLED(Prompt_Fill, 0); // _UIPROMPT_SET_ENABLED
						}
						else {
							HUD::_UIPROMPT_SET_TEXT(Prompt_Fill, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", refillPrompt)); //set text
							HUD::_UIPROMPT_SET_ENABLED(Prompt_Fill, 1); // _UIPROMPT_SET_ENABLED
						}
					}
					if (drink_core > 98) {
						HUD::_UIPROMPT_SET_TEXT(Prompt_Drink, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", notThirstyPrompt)); //You are not thirsty
						HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink, 0); // _UIPROMPT_SET_ENABLED
					}
					else {
						HUD::_UIPROMPT_SET_TEXT(Prompt_Drink, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", drinkPrompt)); //Drink
						HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink, 1); // _UIPROMPT_SET_ENABLED
					}

					HUD::_UIPROMPT_SET_ENABLED(Prompt_Wash, 1);


					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Fill, 1);
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink, 1);
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Wash, 1);

					HUD::_UIPROMPT_SET_GROUP(Prompt_Wash, -2019190071, 0);
					HUD::_UIPROMPT_SET_GROUP(Prompt_Drink, -2019190071, 0);
					HUD::_UIPROMPT_SET_GROUP(Prompt_Fill, -2019190071, 0);

				}
				else {
					HUD::_UIPROMPT_SET_ENABLED(Prompt_Fill, 0);
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Fill, 0);

					HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink, 0);
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink, 0);

					HUD::_UIPROMPT_SET_ENABLED(Prompt_Wash, 0);
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Wash, 0);


					if (canteen > 0) {




						if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Drink_Flask)) {
							if (drinking_anim == 0) {
								TASK::CLEAR_PED_TASKS(player, 1, 1);
								TASK::_TASK_START_SCENARIO_IN_PLACE(player, MISC::GET_HASH_KEY("WORLD_HUMAN_DRINK_FLASK"), 5000, true, true, 0, true);
								start_anim = 1;
								drink_anim_timer = MISC::GET_GAME_TIMER() + 4600;
							}
							if (drinking_anim == 2) {
								Object object_id = Misc::createProp("s_CMBitters_used_Sl", playerPos, 0, 0, 1);
								TASK::_TASK_ITEM_INTERACTION_2(player, MISC::GET_HASH_KEY("CONSUMABLE_RESTORATIVE_USED"), object_id, MISC::GET_HASH_KEY("PrimaryItem"), MISC::GET_HASH_KEY("USE_TONIC_SATCHEL_UNARMED_QUICK"), 1, 0, -1);

								start_anim = 1;
								drink_anim_timer = MISC::GET_GAME_TIMER() + 2000;

							}
							if (drinking_anim == 1) {

								//Object object_id = createProp("s_CMBitters_used_Sl", playerPos, 0, 0, 1);
								//TASK::_0x72F52AA2D2B172CC(player, MISC::GET_HASH_KEY("CONSUMABLE_RESTORATIVE_USED"), object_id, MISC::GET_HASH_KEY("PrimaryItem"), MISC::GET_HASH_KEY("USE_TONIC_SATCHEL_UNARMED_QUICK"), 1, 0, -1);

								TASK::_TASK_EMOTE(player, 1, 0, MISC::GET_HASH_KEY("KIT_EMOTE_ACTION_WET_YOUR_WHISTLE_1"), 1, 1, 0, 0, 0);

								start_anim = 1;
								drink_anim_timer = MISC::GET_GAME_TIMER() + 2400;
							}



							canteen--;

						}
					}

					if (drink_anim_timer < MISC::GET_GAME_TIMER() && start_anim) {
						if (canteen_type == 0) {
							GRAPHICS::ANIMPOSTFX_PLAY("PlayerOverpower");
							AUDIO::PLAY_SOUND_FRONTEND("Core_Fill_Up", "Consumption_Sounds", 1, 0);


						}
						if (canteen_type == 1) {
							ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 0, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0) - 30);
							ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) - 30);
							ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 2, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2) - 30);
							GRAPHICS::ANIMPOSTFX_PLAY("PlayerRPGEmptyCoreStamina");
							AUDIO::PLAY_SOUND_FRONTEND("STAMINA", "Player_Core_Empty_Sounds", 1, 0);
						}
						if (canteen_type == 2) {

							ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) - 20);
							GRAPHICS::ANIMPOSTFX_PLAY("PlayerRPGEmptyCoreStamina");
							AUDIO::PLAY_SOUND_FRONTEND("STAMINA", "Player_Core_Empty_Sounds", 1, 0);
						}
						drink_core = 100;
						start_anim = 0;
					}



					if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Pee)) {
						pissing = 1;
						HUD::_UIPROMPT_SET_ENABLED(Prompt_Stop, 1); // _UIPROMPT_SET_ENABLED
						HUD::_UIPROMPT_SET_VISIBLE(Prompt_Stop, 1);

						TASK::CLEAR_PED_TASKS(player, 1, 1);
						TASK::_TASK_START_SCENARIO_IN_PLACE(player, MISC::GET_HASH_KEY("WORLD_HUMAN_PEE"), -1, 1, 1, 1, 1);



						start_pissing = 1;
						piss_anim_timer = MISC::GET_GAME_TIMER() + 5500;

					}

					


					if (piss_core < 60) {

						HUD::_UIPROMPT_SET_ENABLED(Prompt_Pee, 1); // _UIPROMPT_SET_ENABLED
					}
					else {
						HUD::_UIPROMPT_SET_ENABLED(Prompt_Pee, 0);
					}

					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Pee, 1);
					if (!catalog_mode) {
						if (canteen > 0) {




							if (drink_core > 98) {
								HUD::_UIPROMPT_SET_TEXT(Prompt_Drink_Flask, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", notThirstyPrompt)); //set text
								HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink_Flask, 0); // _UIPROMPT_SET_ENABLED
							}
							else {
								HUD::_UIPROMPT_SET_TEXT(Prompt_Drink_Flask, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", drinkCanteenPrompt)); //Drink from canteen
								HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink_Flask, 1); // _UIPROMPT_SET_ENABLED
							}




						}
						else {
							HUD::_UIPROMPT_SET_TEXT(Prompt_Drink_Flask, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", canteenEmptyPrompt)); //Canteen is empty
							HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink_Flask, 0); // _UIPROMPT_SET_ENABLED
						}
					}
					
					HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink_Flask, 1);

			
						HUD::_UIPROMPT_SET_GROUP(Prompt_Pee, -2019190071, 0);
						HUD::_UIPROMPT_SET_GROUP(Prompt_Drink_Flask, -2019190071, 0);
					


				}


				if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt_Back) || HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Back)) {

					survival_menu = 0;
				}


				HUD::_UIPROMPT_SET_ENABLED(Prompt_Back, 1); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Back, 1);
				HUD::_UIPROMPT_SET_GROUP(Prompt_Back, -2019190071, 0);

			}
			else {
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink_Flask, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink_Flask, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Pee, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Pee, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Back, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Back, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Fill, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Fill, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Wash, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Wash, 0);

			}



			if (wash_on == 1) {
				
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Wash, 0); // _UIPROMPT_SET_ENABLED

				wash_timer++;
				if (wash_timer > 100) {
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1) + 5);
					wash_timer = 0;
				}
			}


			if (piss_anim_timer < MISC::GET_GAME_TIMER() && start_pissing) {
				piss_timer++;
				if (piss_timer > piss_replenish) {
					if (pissing == 1) {
						
						if (piss_core < 100) {
							piss_core += 5;
							if (number_of_drinks > 1) {
								number_of_drinks--;
							}

						}
						if (piss_core >= 100) {
							HUD::_UIPROMPT_SET_ENABLED(Prompt_Stop, 0); // _UIPROMPT_SET_ENABLED
							HUD::_UIPROMPT_SET_VISIBLE(Prompt_Stop, 0);
							pissing = 0;
							TASK::CLEAR_PED_TASKS(player, 1, 1);
							start_pissing = 0;

						}
					}
					piss_timer = 0;
				}
			}


			if (PAD::IS_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_FRONTEND_RS")) || IsKeyPressed(menu_key)) {

				pee_press++;
				if (pee_press > menu_time) {
					survival_menu = 1;

					pee_press = 0;


				}
			}
			else {
				pee_press = 0;
			}







			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Prompt_Stop)) {
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Stop, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Stop, 0);
				pissing = 0;
				wash_on = 0;
				TASK::CLEAR_PED_TASKS(player, 1, 1);


			}
			if (PED::IS_PED_ON_MOUNT(player)) {
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Pee, 0);
			}


			if (catalog_mode) {
				HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink_Flask, 0); 
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink_Flask, 0);

				if (PED::IS_PED_ON_MOUNT(player)) {
					survival_menu = 0;
				}

			}

			//INTERIOR::IS_VALID_INTERIOR
			//INTERIOR::GET_INTERIOR_AT_COORDS(playerPos);


			//_GET_TEMPERATURE_AT_COORDS

			
		} else {

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Holster, 0);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Holster, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink_Flask, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink_Flask, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Pee, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Pee, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Back, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Back, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Stop, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Stop, 0);


				HUD::_UIPROMPT_SET_ENABLED(Prompt_Fill, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Fill, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Drink, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Drink, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Wash, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Wash, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Sleep, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Sleep, 0); // _UIPROMPT_SET_VISIBLE

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Leave, 0); // _UIPROMPT_SET_ENABLED
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Leave, 0); // _UIPROMPT_SET_VISIBLE

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap1, 0);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap1, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap2, 0);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap2, 0);

				HUD::_UIPROMPT_SET_ENABLED(Prompt_Nap3, 0);
				HUD::_UIPROMPT_SET_VISIBLE(Prompt_Nap3, 0);



}

		WAIT(0);
	}
}

void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
