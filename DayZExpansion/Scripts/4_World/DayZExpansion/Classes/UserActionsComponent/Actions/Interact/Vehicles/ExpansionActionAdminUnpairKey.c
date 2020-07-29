/**
 * ExpansionActionAdminUnpairKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionAdminUnpairKey: ActionInteractBase
{
	protected CarScript m_Car;

	void ExpansionActionAdminUnpairKey()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UNPAIR_KEY";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Car = NULL;

		if ( !target || !player )
			return false;

		if ( !Class.CastTo( m_Car, target.GetObject() ) )
		{
			return false;
		}

		ExpansionCarAdminKey key;
		if ( !Class.CastTo( key, player.GetItemInHands() ) )
		{
			return false;
		}

		if ( !m_Car.HasKey() )
		{
			return false;
		}
		
		return true;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );

		array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
		ExpansionCarKey.GetKeysForVehicle( m_Car, keys );

		for ( int i = 0; i < keys.Count(); ++i )
		{
			keys[i].Unpair( true );
		}

		if ( keys.Count() != 0 )
			m_Car.ResetKeyPairing();
	}

	override bool CanBeUsedInRestrain()
	{
		return false;
	}
}