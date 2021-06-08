/**
 * ExpansionLogSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLogSettings
 * @brief		Log settings class
 **/
class ExpansionLogSettings: ExpansionSettingBase
{
	#ifdef EXPANSIONMODVEHICLE
	bool VehicleCarKey;			//! If enabled, generate logs about pairing, unpairing, locking, unlocking vehicles with car keys
	bool VehicleTowing;			//! If enabled, generate logs about towing and untowing vehicles
	bool VehicleLockPicking;	//! If enabled, generate logs about lockpicking a vehicle
	#endif

	#ifdef EXPANSIONMOD
	bool BaseBuildingRaiding;	//! If enabled, generate logs about raiding expansion basebuilding and safes
	bool CodeLockRaiding;		//! If enabled, generate logs about codelock raiding (wrong core)
	bool Territory;				//! If enabled, generate logs about creating, deleting territories and inviting players

	bool Killfeed;				//! If enabled, generate logs based on the killfeed module
	bool Safezone;				//! If enabled, generate logs when the player leave or enter a safezone
	bool Party;					//! If enabled, generate logs when the player create a party or invite someone
	bool Chat;					//! If enabled, generate logs of the chat
	bool AdminTools;			//! If enabled, generate logs of the adminhammer and expansionadmincarkey when used
	bool SpawnSelection;		//! If enabled, generate logs when the player spawn
	
	bool MissionAirdrop;		//! If enabled, generate logs of spawned airdrops from missions or player called
	//bool MissionHorde;		//! TODO
	#endif

	[NonSerialized()]
	private string m_FileTimestamp;

	[NonSerialized()]
	private FileHandle m_FileLog;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionLogSettings()
	{
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogSettings::OnRecieve - Start");
		#endif
		ExpansionLogSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionLogSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Log.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ref ExpansionLogSettings thisSetting = this;

		ctx.Write( thisSetting );
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Log, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionLogSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionLogSettings s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogSettings::CopyInternal - Start");
		#endif

		#ifdef EXPANSIONMODVEHICLE
		VehicleCarKey = s.VehicleCarKey;
		VehicleTowing = s.VehicleTowing;
		VehicleLockPicking = s.VehicleLockPicking;
		#endif

		#ifdef EXPANSIONMOD
		BaseBuildingRaiding = s.BaseBuildingRaiding;
		CodeLockRaiding = s.CodeLockRaiding;
		Killfeed = s.Killfeed;
		Safezone = s.Safezone;
		Party = s.Party;
		Territory = s.Territory;
		Chat = s.Chat;
		AdminTools = s.AdminTools;
		SpawnSelection = s.SpawnSelection;
		MissionAirdrop = s.MissionAirdrop;
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogSettings::CopyInternal - End");
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
		EXPrint("ExpansionLogSettings::Load - Start");
		#endif
		
		m_IsLoaded = true;

		m_FileTimestamp = EXPANSION_LOG_FOLDER + "\\" + "ExpLog_" + GetDateTime() + ".log";
		
		if ( FileExist( EXPANSION_LOG_SETTINGS ) )
		{
			Print("[ExpansionLogSettings] Loading settings");

			JsonFileLoader<ExpansionLogSettings>.JsonLoadFile( EXPANSION_LOG_SETTINGS, this );
	
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionLogSettings::Load - End - Loaded");
			#endif

			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogSettings::Load - End - Not Loaded");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionLogSettings] Saving settings");

		JsonFileLoader<ExpansionLogSettings>.JsonSaveFile( EXPANSION_LOG_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Log.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		Print("[ExpansionLogSettings] Loading default settings");

		#ifdef EXPANSIONMODVEHICLE
		VehicleCarKey = true;
		VehicleTowing = true;
		VehicleLockPicking = true;
		#endif

		#ifdef EXPANSIONMOD
		BaseBuildingRaiding = true;
		CodeLockRaiding = true;
		Killfeed = true;
		Safezone = true;
		Party = true;
		Territory = true;
		Chat = true;
		AdminTools = true;
		SpawnSelection = true;
		MissionAirdrop = true;
		#endif
	}
	
	override string SettingName()
	{
		return "Log Settings";
	}

	void PrintLog(string text)
	{
		if ( !FileExist( EXPANSION_LOG_FOLDER ) )
		{
			MakeDirectory( EXPANSION_LOG_FOLDER );
		}

		if ( !FileExist( m_FileTimestamp ) )
		{
			m_FileLog = OpenFile(m_FileTimestamp, FileMode.WRITE);
		}

		m_FileLog = OpenFile(m_FileTimestamp, FileMode.APPEND);
		FPrintln(m_FileLog, GetTime() + " " + text);
		CloseFile(m_FileLog);
	}
};