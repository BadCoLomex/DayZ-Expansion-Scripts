/**
 * ExpansionExplosive.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionExplosiveBase
 * @brief		
 **/
class ExpansionExplosiveBase extends ItemBase
{	
	protected int m_Time;
	protected int m_Timer;

	protected bool m_Executed;
	protected bool m_ExecutedSynchRemote;

	protected bool m_Exploded;
	protected bool m_ExplodedSynchRemote;

	protected EffectSound m_Sound;

	protected Particle m_ParticleEfx;	

	protected ref Timer	m_ExplosionTimer;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionExplosiveBase()
	{
		SetEventMask( EntityEvent.FRAME );
		
		m_Executed = false;
		m_ExecutedSynchRemote = false;

		m_ExplosionTimer = new Timer();

		RegisterNetSyncVariableInt( "m_Time" );

		RegisterNetSyncVariableBool( "m_ExecutedSynchRemote" );
		RegisterNetSyncVariableBool( "m_ExplodedSynchRemote" );
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionExplosiveBase()
	{
		if ( GetGame() && (GetGame().IsClient() || !GetGame().IsMultiplayer()) ) 
		{
			if (m_ParticleEfx)
				m_ParticleEfx.Stop();

			if (m_Sound) 
				m_Sound.SoundStop();
		}
	}

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}

	// ------------------------------------------------------------
	// EOnFrame
	// ------------------------------------------------------------
	override void EOnFrame(IEntity other, float timeSlice)
	{
		if ( IsMissionHost() && m_Executed )
		{
			if ( m_ExplosionTimer.GetRemaining() > 10 )
			{
				m_Time = 10;
			}
			else 
			{
				m_Time = Math.Round(m_ExplosionTimer.GetRemaining());
			}

			SetSynchDirty();
		}

		if ( IsMissionClient() && m_ExecutedSynchRemote )
		{
			TriggerSound( m_Time, timeSlice );
		}
	}
	
	// ------------------------------------------------------------
	// OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player )
	{
		float explosionTime = GetExpansionSettings().GetBaseBuilding().ExplosionTime; 

		m_Executed = true;
		m_ExecutedSynchRemote = true;

		if ( IsMissionHost() )
		{
			m_ExplosionTimer.Run( explosionTime, this, "Trigger", NULL, false ); 
		}

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) 
		{ 
			return false;
		}
		
		if( !m_Executed )
		{
			return true;
		}

		return false;
	}

	void RemoveLater()
	{
		ToDelete();

		SetPosition( vector.Zero );
		GetGame().ObjectDelete( this );
	}

	void Trigger()
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

	void TriggerSound( int time, float dT )
	{
		if ( time <= 10 )
		{
			if ( m_Timer < ( time * 100 ) )
			{
				m_Timer += 10 * dT;
			}
			else 
			{
				m_Timer = 0;
				
				m_Sound = SEffectManager.PlaySound("Expansion_Explosive_C4_SoundSet", GetPosition());
			}
		}	
	}

	private void HandleClientExplosion()
	{
		if ( m_ParticleEfx )
			m_ParticleEfx.Stop();

		if ( m_Sound ) 
			m_Sound.SoundStop();

		float distance_to_player = vector.Distance(GetPosition(), GetGame().GetPlayer().GetPosition());

		float strength_factor = Math.InverseLerp(GameConstants.CAMERA_SHAKE_GRENADE_DISTANCE, 0, distance_to_player);

		// GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake(strength_factor * 4);

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

	// ------------------------------------------------------------
	// OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( m_ExplodedSynchRemote && !m_Exploded ) 
		{
			m_Exploded = true;
			
			HandleClientExplosion();
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
}

