/**
 * ExpansionHumanCommandVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandVehicle extends HumanCommandScript
{
	DayZPlayerImplement m_Player;
	ExpansionHumanST m_Table;
	HumanInputController m_Input;

	Object m_Vehicle;
	ExpansionVehicleScript m_VehicleEx;
	CarScript m_VehicleVn;

	int m_SeatIndex;
	int m_SeatAnim;
	int m_VehicleType;

	bool m_NeedFinish;

	bool m_IsGettingOut;
	bool m_IsGettingIn;
	bool m_IsIn;

	float m_Time;
	vector m_SeatTransform[4];
	vector m_SeatPosition;

	vector m_StartPosition;
	vector m_StartDirection;

	int m_PreviousInteractLayers;

	private float m_TimeGetIn;
	private float m_TimeGetOut;

	void ExpansionHumanCommandVehicle( DayZPlayerImplement player, Object vehicle, int seatIdx, int seat_anim, ExpansionHumanST table )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::ExpansionHumanCommandVehicle Start");
		#endif
		m_Player = player;
		m_Table = table;

		m_Vehicle = vehicle;

		Class.CastTo( m_VehicleEx, m_Vehicle );
		Class.CastTo( m_VehicleVn, m_Vehicle );

		m_SeatIndex = seatIdx;
		m_SeatAnim = seat_anim;

		m_Input = player.GetInputController();
		
		m_TimeGetIn = 1.0;
		m_TimeGetOut = 0.5;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::ExpansionHumanCommandVehicle End");
		#endif
	}

	override void OnActivate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnActivate Start");
		#endif
		m_IsGettingIn = true;

		m_StartPosition = m_Player.GetPosition();
		m_StartDirection = m_Player.GetDirection();

		vector entryPos;
		vector entryDir;
		vector tmPlayer[ 4 ];
		vector tmTarget[ 4 ];
		vector tmLocal[ 4 ];

		if ( m_VehicleEx )
		{
			m_VehicleEx.CrewTransform( m_SeatIndex, m_SeatTransform );
			m_SeatPosition = m_SeatTransform[3];

			m_Player.GetTransformWS( tmPlayer );
			m_Vehicle.GetTransform( tmTarget );
			Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );

			m_Player.LinkToLocalSpaceOf( m_Vehicle, m_SeatTransform );

			m_VehicleEx.CrewGetIn( m_Player, m_SeatIndex );
		} else if ( m_VehicleVn )
		{
			m_VehicleVn.CrewTransform( m_SeatIndex, m_SeatTransform );
			m_SeatPosition = m_SeatTransform[3];

			m_Player.GetTransformWS( tmPlayer );
			m_Vehicle.GetTransform( tmTarget );
			Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );

			m_Player.LinkToLocalSpaceOf( m_Vehicle, m_SeatTransform );

			m_VehicleVn.CrewGetIn( m_Player, m_SeatIndex );
		}

		m_Table.SetVehicleType( this, m_VehicleType );
		m_Table.CallVehicleGetIn( this, m_SeatAnim );

		dBodyEnableGravity( m_Player, false );

		dBodyActive( m_Player, ActiveState.INACTIVE );

		m_Player.OnCommandExpansionVehicleStart();

		m_Time = 0;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnActivate End");
		#endif
	}

	override void OnDeactivate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnDeactivate Start");
		#endif

		m_Table.SetVehicleType( this, -1 );
		
		if ( m_VehicleEx )
		{
			m_VehicleEx.CrewGetOut( m_SeatIndex );

			if ( !m_VehicleEx.CanObjectAttach( m_Player ) )
				m_Player.UnlinkFromLocalSpace();
		} else if ( m_VehicleVn )
		{
			m_VehicleVn.CrewGetOut( m_SeatIndex );

			if ( !m_VehicleVn.CanObjectAttach( m_Player ) )
				m_Player.UnlinkFromLocalSpace();
		}

		dBodyActive( m_Player, ActiveState.ALWAYS_ACTIVE );
		
		dBodyEnableGravity( m_Player, true );

		m_Player.OnCommandExpansionVehicleFinish();
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnDeactivate End");
		#endif
	}

	override void PreAnimUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PreAnimUpdate Start");
		#endif

		float heading = m_Input.GetHeadingAngle();
		PreAnim_SetFilteredHeading( 0, 0.3, 180 );

		m_Table.SetLook( this, true );
		m_Table.SetLookDirX( this, heading );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PreAnimUpdate End");
		#endif
	}

	override void PrePhysUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PrePhysUpdate Start");
		#endif

		if ( m_IsGettingIn )
		{
			if ( !m_IsIn )
			{
				m_IsIn = true;
			}			
		} else if ( !m_IsGettingOut )
		{
		} else
		{
		}
		
		PrePhys_SetTranslation( vector.Zero );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PrePhysUpdate End");
		#endif
	}

	override bool PostPhysUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PostPhysUpdate Start");
		#endif
		
		m_Time += pDt;
		if ( m_IsGettingIn )
		{
			m_IsGettingIn = false;

			if ( m_Time > m_TimeGetIn )
			{
				m_IsGettingIn = false;
			}
		} else if ( m_IsGettingOut )
		{
			if ( m_Time > m_TimeGetOut )
			{
				m_NeedFinish = true;
			}
		}
		
		float quat[4];
		Math3D.QuatIdentity(quat);
		
		PostPhys_SetPosition( m_SeatPosition );
		PostPhys_SetRotation( quat );
		PostPhys_LockRotation();
		
		return m_NeedFinish == false;
	}

	void GetOut()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::GetOut Start");
		#endif
		
		m_Time = 0;
		m_IsGettingIn = false;
		m_IsGettingOut = true;

		m_Table.CallVehicleGetOut( this );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::GetOut End");
		#endif
	}

	void SwitchSeat( int seatIdx, int seatAnimType )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::SwitchSeat Start");
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::SwitchSeat End");
		#endif
	}

	bool IsGettingOut()
	{
		return m_IsGettingOut;
	}

	bool IsGettingIn()
	{
		return m_IsGettingIn;
	}

	bool IsSwitchSeat()
	{
		return false;
	}

	ExpansionVehicleScript GetTransport()
	{
		return m_VehicleEx;
	}

	int GetVehicleSeat()
	{
		return m_SeatIndex;
	}

	void SetVehicleType( int vehicleType )
	{
		m_VehicleType = vehicleType;
	}
}
