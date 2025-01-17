/**
 * ExpansionDebugSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionDebugSettings
 * @brief		Vehicle settings class
 **/
class ExpansionDebugSettings: ExpansionSettingBase
{
	static const int VERSION = 0;
	
	bool ShowVehicleDebugMarkers;

	int DebugVehicleSync;
	int DebugVehicleTransformSet;

	int DebugVehiclePlayerNetworkBubbleMode;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionDebugSettings()
	{
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::OnRecieve - Start");
		#endif
		ExpansionDebugSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionDebugSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Debug.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionDebugSettings thisSetting = this;

		ctx.Write( thisSetting );
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Debug, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionDebugSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ExpansionDebugSettings s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::CopyInternal - Start");
		#endif

		ShowVehicleDebugMarkers = s.ShowVehicleDebugMarkers;

		DebugVehicleSync = s.DebugVehicleSync;
		DebugVehicleTransformSet = s.DebugVehicleTransformSet;

		DebugVehiclePlayerNetworkBubbleMode = s.DebugVehiclePlayerNetworkBubbleMode;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::CopyInternal - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	override bool OnLoad()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::Load - Start");
		#endif
		
		m_IsLoaded = true;
		
		bool save;
		
		bool debugSettingsExist = FileExist(EXPANSION_DEBUG_SETTINGS);
		
		if (debugSettingsExist)
		{
			EXPrint("[ExpansionDebugSettings] Load existing setting file:" + EXPANSION_DEBUG_SETTINGS);

			JsonFileLoader<ExpansionDebugSettings>.JsonLoadFile( EXPANSION_DEBUG_SETTINGS, this );
	
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionDebugSettings::Load - End - Loaded");
			#endif
		}
		else
		{
			EXPrint("[ExpansionDebugSettings] No existing setting file:" + EXPANSION_DEBUG_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDebugSettings::Load - End - Not Loaded");
		#endif
		return debugSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionDebugSettings] Saving settings");

		JsonFileLoader<ExpansionDebugSettings>.JsonSaveFile( EXPANSION_DEBUG_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Debug.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		ShowVehicleDebugMarkers = false;

		DebugVehicleSync = 0;
		DebugVehicleTransformSet = 0;
		
		DebugVehiclePlayerNetworkBubbleMode = 0;
	}
	
	override string SettingName()
	{
		return "Debug Settings";
	}
};