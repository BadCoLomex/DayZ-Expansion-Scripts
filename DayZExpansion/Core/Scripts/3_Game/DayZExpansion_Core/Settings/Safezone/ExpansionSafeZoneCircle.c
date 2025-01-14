/**
 * ExpansionSafeZoneCircle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSafeZoneCircle
 * @brief
 **/
class ExpansionSafeZoneCircle: ExpansionSafeZoneElement
{
	vector Center;
	float Radius;

	// ------------------------------------------------------------
	// Expansion ExpansionSafeZoneCircle
	// ------------------------------------------------------------
	void ExpansionSafeZoneCircle( vector center, float radius )
	{
		this.Type = ExpansionSafeZoneTypes.CIRCLE;
		this.Center = center;
		this.Radius = radius;
	}

	override ExpansionZone Create()
	{
		return new ExpansionZoneCircle(ExpansionZoneType.SAFE, Center, Radius);
	}
};