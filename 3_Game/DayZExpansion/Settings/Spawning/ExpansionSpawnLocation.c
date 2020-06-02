/**
 * ExpansionSpawnLocation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawnLocation
{
	string Name;
	ref array<vector> Positions;

	void ExpansionSpawnLocation( string name, array<vector> positions )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnLocation::ExpansionSpawnLocation - Start");
		#endif
		
		Name = name;
		Positions = positions;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnLocation::ExpansionSpawnLocation - End");
		#endif
	}
}