/**
 * ExpansionMarketTraderBoats.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderBoats: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Boats";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_BOATS";
		m_FileName = "Boats";
		TraderIcon = "Boat";
	
	#ifdef EXPANSIONMODVEHICLE
		Categories.Insert("Boats");
	#endif
		
		// VEHICLE PARTS
		AddItem("HeadlightH7_Box");
		AddItem("TruckBattery");

	#ifdef EXPANSIONMODVEHICLE
		AddItem("GlowPlug");
		
		// VEHICLE KEYS
		AddItem("ExpansionCarKey");
	#endif
		
		// LIQUIDS
		//AddItem("EngineOil");
		AddItem("CanisterGasoline");
	}
}