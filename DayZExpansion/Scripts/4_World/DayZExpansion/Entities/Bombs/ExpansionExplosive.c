/**
 * ExpansionExplosive.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionExplosive
 * @brief		
 **/
class ExpansionExplosive extends ItemBase
{
	protected int m_Time = 10;
	protected int m_Timer;
	protected float m_ExplosionTime;

	protected bool m_Armed;
	protected bool m_ArmedSynchRemote;

	protected bool m_Exploded;
	protected bool m_ExplodedSynchRemote;

	protected EffectSound m_Sound;

	protected Particle m_ParticleEfx;	

	protected ref Timer	m_ExplosionTimer;

	void ExpansionExplosive()
	{
		m_ExplosionTimer = new Timer();

		RegisterNetSyncVariableInt( "m_Time" );
		RegisterNetSyncVariableBool( "m_ArmedSynchRemote" );
		RegisterNetSyncVariableBool( "m_ExplodedSynchRemote" );
	}

	void ~ExpansionExplosive()
	{
		if ( GetGame() && (GetGame().IsClient() || !GetGame().IsMultiplayer()) ) 
		{
			if (m_ParticleEfx)
				m_ParticleEfx.Stop();

			if (m_Sound) 
				m_Sound.SoundStop();
		}
	}

	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}

	void OnFrame()
	{
		if ( IsMissionHost() && m_Armed && m_ExplosionTimer.GetRemaining() < 10 )
		{
			m_Time = Math.Round(m_ExplosionTimer.GetRemaining());

			SetSynchDirty();
		}

		if ( IsMissionClient() && m_ArmedSynchRemote )
		{
			TriggerSound(m_Time);
		}
	}

	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( IsMissionHost() )
		{
			m_ExplosionTime = GetExpansionSettings().GetRaid().ExplosionTime;

			m_Armed = true;
			m_ArmedSynchRemote = true;

			m_ExplosionTimer.Run( m_ExplosionTime, this, "TriggerExplosion", NULL, false );

			if ( GetExpansionSettings().GetLog().BaseBuildingRaiding )
				GetExpansionSettings().GetLog().PrintLog( "[BaseBuildingRaiding] Player \"" + player.GetIdentity().GetName() + "\" (id=" + player.GetIdentity().GetId() + " pos=" + player.GetPosition() + ")" + " deployed " + GetType() + " at " + GetPosition() );

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnFrame, 1, true );

			SetSynchDirty();
		}
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return super.CanPutInCargo(parent) && !m_Armed;
	}

	void RemoveLater()
	{
		ToDelete();

		SetPosition( vector.Zero );
		GetGame().ObjectDelete( this );
	}

	override bool ExpansionCanRecievePower()
	{
		return true;
	}
	
	override void OnWorkStart()
	{
		TriggerExplosion();
	}
	
	void TriggerExplosion()
	{
		if ( IsMissionHost() && !m_Exploded )
		{
			m_Exploded = true;
			m_ExplodedSynchRemote = true;

			//! Explode( DT_EXPLOSION, "ExpansionRocket_Ammo" );
			//ExpansionCreateExplosion( this, "ExpansionRocket_Ammo", 5, 500 );
			GetGame().CreateObject( "Expansion_C4_Explosion", this.GetPosition() );
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( RemoveLater, 150, false ); 

			if ( !GetGame().IsMultiplayer() )
			{
				HandleClientExplosion();
			} else
			{
				SetSynchDirty();
			}
		}
	}

	void TriggerSound(int time)
	{
		if ( m_Timer < ( time * 100 ) )
		{
			m_Timer += 10;
		}
		else 
		{
			m_Timer = 0;
			SoundC4Beep();
		}
	}
	
	protected void SoundC4Beep()
	{
		EffectSound sound = SEffectManager.PlaySound("Expansion_Explosive_C4_SoundSet", GetPosition());
		sound.SetSoundAutodestroy( true );
	}

	private void HandleClientExplosion()
	{
		if ( m_ParticleEfx )
			m_ParticleEfx.Stop();

		if ( m_Sound ) 
			m_Sound.SoundStop();

		float distance_to_player = vector.Distance(GetPosition(), GetGame().GetPlayer().GetPosition());

		float strength_factor = Math.InverseLerp(GameConstants.CAMERA_SHAKE_GRENADE_DISTANCE, 0, distance_to_player);

		// just don't
		//GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake(strength_factor * 4);

		SEffectManager.PlaySound("Expansion_Explosive_Large_SoundSet", GetPosition());

		vector n = GetPosition().VectorToAngles();

		Particle p2 = Particle.Play(ParticleList.EXPLOSION_LANDMINE, GetPosition());
		p2.SetOrientation(n);

		Particle p3 = Particle.Play(ParticleList.IMPACT_METAL_RICOCHET, GetPosition());
		p3.SetOrientation(n);
	
		Particle p4 = Particle.Play(ParticleList.IMPACT_GRAVEL_RICOCHET, GetPosition());
		p4.SetOrientation(n);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( RemoveLater, 100, false );  
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if ( m_ExplodedSynchRemote && !m_Exploded ) 
		{
			m_Exploded = true;
			
			HandleClientExplosion();
		}
		else if ( m_ArmedSynchRemote && !m_Armed )
		{
			m_Armed = true;

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnFrame, 1, true );
		}
	}

	override bool IsDeployable()
	{
		return true;
	}	

	override bool IsOneHandedBehaviour()
	{
		return true;
	}
	
	override bool IsExplosive()
	{
		return true;
	}
}

