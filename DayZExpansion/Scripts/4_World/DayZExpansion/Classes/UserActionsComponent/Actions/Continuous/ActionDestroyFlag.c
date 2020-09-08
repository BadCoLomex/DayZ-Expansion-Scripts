#ifdef DAYZ_1_09
class ActionDestroyFlagCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.BASEBUILDING_DECONSTRUCT_MEDIUM);
	}
};

class ActionDestroyFlag: ActionContinuousBase
{
	float m_DamageAmount;
	string m_SlotName;
	
	void ActionDestroyFlag()
	{
		m_CallbackClass = ActionDestroyFlagCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		
		m_DamageAmount = 2;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.BASEBUILDING );
	}
		
	override string GetText()
	{
		return "#unmount_barbed_wire";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !GetExpansionSettings().GetBaseBuilding().CanDismantleFlag )
			return false;

		Object targetObject = target.GetObject();
		
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );

			float state = base_building.GetAnimationPhase("flag_mast");
			if ( base_building.FindAttachmentBySlotName( "Material_FPole_Flag" ) && state >= 0.99 )
			{
				return true;
			}				
		}
		return false;
	}
		
	override void OnFinishProgressServer( ActionData action_data )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		Flag_Base flag = Flag_Base.Cast( base_building.FindAttachmentBySlotName( "Material_FPole_Flag" ) );
		
		flag.Delete();
		
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
	}
}
#endif