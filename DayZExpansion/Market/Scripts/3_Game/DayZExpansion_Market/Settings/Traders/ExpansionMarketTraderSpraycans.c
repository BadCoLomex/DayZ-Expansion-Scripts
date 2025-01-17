/**
 * ExpansionMarketTraderSpraycans.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderSpraycans: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Spraycans";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_SPRAYCANS";
		m_FileName = "Spraycans";
		TraderIcon = "Bottle";
	
		Categories.Insert("Spraycans");
	}
}