/**
 * ActionNextCombinationLockDialOnTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionNextCombinationLockDialOnTarget
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( super.ActionCondition( player, target, item ) )
		{
			Fence building = Fence.Cast( target.GetObject() );
			if ( building )
			{
				if ( building.ExpansionHasCodeLock( "codelock" ) )
					return false;
			}
			return true;
		}
		return false;
	}
};
