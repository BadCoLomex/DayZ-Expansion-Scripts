/**
 * ExpansionMarketItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMarketItems
 * @brief		Item settings class
 **/

class ExpansionMarketItem
{
	static int m_CurrentItemId;

	[NonSerialized()]
	int ItemID;

	[NonSerialized()]
	int CategoryID;

	string ClassName;

	int MaxPriceThreshold;
	int MinPriceThreshold;

	//! @note this is a workaround for a serious Enforce bug (MaxPriceThreshold gets randomly reset to zero on client after a few buys/sells)
	[NonSerialized()]
	private int m_MaxPriceThreshold;
	[NonSerialized()]
	private int m_MinPriceThreshold;

	int SellPricePercent;

	int MaxStockThreshold;
	int MinStockThreshold;

	int QuantityPercent;

	autoptr array< string > SpawnAttachments;
	autoptr array< string > Variants;

	[NonSerialized()]
	autoptr array< int > m_AttachmentIDs;

	[NonSerialized()]
	bool m_IsVariant;

	[NonSerialized()]
	ExpansionMarketItem m_Parent;

	[NonSerialized()]
	bool m_StockOnly;

	[NonSerialized()]
	bool m_ShowInMenu;

	[NonSerialized()]
	bool m_Idx;

	[NonSerialized()]
	bool m_UpdateView;
	
	// ------------------------------------------------------------
	// ExpansionMarketItem Constructor
	// ------------------------------------------------------------
	void ExpansionMarketItem( int catID, string className, int minPrice, int maxPrice, int minStock, int maxStock, array<string> attachments = null, array<string> variants = null, int sellPricePercent = -1, int quantityPercent = -1, int itemID = -1, array<int> attachmentIDs = null)
	{
		if (itemID == -1)
			ItemID = ++m_CurrentItemId;
		else
			ItemID = itemID;

		CategoryID = catID;
		
		ClassName = className;
		ClassName.ToLower();

		MinPriceThreshold = minPrice;
		MaxPriceThreshold = maxPrice;

		SellPricePercent = sellPricePercent;

		MinStockThreshold = minStock;
		MaxStockThreshold = maxStock;

		QuantityPercent = quantityPercent;

		SpawnAttachments = new array< string >;
		if ( attachments )
		{
			foreach ( string attClsName : attachments )
			{
				attClsName.ToLower();
				//! Check if attachment is not same classname as parent to prevent infinite recursion (user error)
				if (attClsName == ClassName)
					Error("[ExpansionMarketItem] Trying to add " + ClassName + " as attachment to itself!");
				else
					SpawnAttachments.Insert( attClsName );
			}
		}

		Variants = new array< string >;
		if ( variants )
		{
			foreach ( string variantClsName : variants )
			{
				variantClsName.ToLower();
				Variants.Insert( variantClsName );
			}
		}

		if ( attachmentIDs )
			m_AttachmentIDs = attachmentIDs;

		SanityCheckAndRepair();
	}
	
	void SanityCheckAndRepair()
	{

		if ( MinPriceThreshold < 0 )
		{
			Error("[ExpansionMarketItem] The minimum price must be 0 or higher for '" + ClassName + "'");
			MinPriceThreshold = 0;
		}

		if ( MinStockThreshold < 0 )
		{
			Error("[ExpansionMarketItem] The minimum stock must be 0 or higher for '" + ClassName + "'");
			MinStockThreshold = 0;
		}

		if ( MinPriceThreshold > MaxPriceThreshold )
		{
			Error("[ExpansionMarketItem] The minimum price must be lower than or equal to the maximum price for '" + ClassName + "'");
			MaxPriceThreshold = MinPriceThreshold;
		}

		if ( MinStockThreshold > MaxStockThreshold )
		{
			Error("[ExpansionMarketItem] The minimum stock must be lower than or equal to the maximum stock for '" + ClassName + "'");
			MaxStockThreshold = MinStockThreshold;
		}

		m_MinPriceThreshold = MinPriceThreshold;
		m_MaxPriceThreshold = MaxPriceThreshold;
	}

	void SetAttachmentsFromIDs()
	{
		SpawnAttachments.Clear();
		foreach (int attachmentID: m_AttachmentIDs)
		{
			ExpansionMarketItem attachment = GetExpansionSettings().GetMarket().GetItem(attachmentID, false);
			if (attachment)
				SpawnAttachments.Insert(attachment.ClassName);
			else
				EXPrint("ExpansionMarketItem::SetAttachmentsFromIDs - WARNING: Attachment ID " + attachmentID + " does not exist!");
		}
		m_AttachmentIDs = NULL;
	}

	bool IsStaticStock()
	{
		return MaxStockThreshold > 0 && MinStockThreshold == MaxStockThreshold;
	}
	
	// ------------------------------------------------------------
	// Expansion CalculatePrice
	// Calculates the current price of the item for one item at the current stock level
	// ------------------------------------------------------------
	int CalculatePrice(int stock, float modifier = 1.0)
	{
		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("ExpansionMarketItem::CalculatePrice - Start - " + ClassName + " - stock " + stock + " modifier " + modifier + " minstock " + MinStockThreshold + " maxstock " + MaxStockThreshold + " maxprice " + m_MaxPriceThreshold + " minprice " + m_MinPriceThreshold + " pct " + SellPricePercent);
		#endif

		float price;

		if (!IsStaticStock() && MaxStockThreshold != 0)
			price = ExpansionMath.PowerConversion(MinStockThreshold, MaxStockThreshold, stock, m_MaxPriceThreshold, m_MinPriceThreshold, 6.0);
		else
			price = m_MinPriceThreshold;

		price = Math.Round(price * modifier);

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("ExpansionMarketItem::CalculatePrice - End and return calculated price: " + price);
		#endif
		
		return (int) price;
	}

	bool IsMagazine()
	{
		return GetGame().IsKindOf(ClassName, "Magazine_Base") && !GetGame().ConfigGetInt("CfgMagazines " + ClassName + " canBeSplit");
	}

	map<string, bool> GetAttachmentTypes(out int magAmmoCount)
	{
		map<string, bool> attachmentTypes = new map<string, bool>;

		bool isMag = IsMagazine();

		foreach (string attachmentName: SpawnAttachments)
		{
			bool isMagAmmo = false;
			if (!attachmentTypes.Find(attachmentName, isMagAmmo))
			{
				isMagAmmo = isMag && GetGame().IsKindOf(attachmentName, "Ammunition_Base");
				attachmentTypes.Insert(attachmentName, isMagAmmo);
			}
			if (isMagAmmo)
				magAmmoCount++;
		}

		return attachmentTypes;
	}

	map<string, int> GetMagAmmoQuantities(map<string, bool> attachmentTypes, int magAmmoCount)
	{
		if (!attachmentTypes.Count() || !magAmmoCount || !IsMagazine())
			return NULL;

		map<string, int> magAmmoCounts = new map<string, int>;
		int magCapacity = GetGame().ConfigGetInt("CfgMagazines " + ClassName + " count");

		int totalAmmo;
		int quantityPercent = QuantityPercent;
		if (quantityPercent < 0)
			quantityPercent = 100;
		int ammoMax = Math.Ceil(magCapacity * quantityPercent / 100);
		while (totalAmmo < ammoMax)
		{
			foreach (string attachmentName, bool isMagAmmo: attachmentTypes)
			{
				if (isMagAmmo)
				{
					int ammoQuantity;
					if (!magAmmoCounts.Find(attachmentName, ammoQuantity))
					{
						if (magAmmoCount == 1)
							ammoQuantity = ammoMax;
						else
							ammoQuantity = 1;
						magAmmoCounts.Insert(attachmentName, ammoQuantity);
						totalAmmo += ammoQuantity;
					}
					else
					{
						magAmmoCounts.Set(attachmentName, ammoQuantity + 1);
						totalAmmo++;
					}

					if (totalAmmo == ammoMax)
						break;
				}
			}
		}

		return magAmmoCounts;
	}

	void AddDefaultAttachments()
	{
		if (SpawnAttachments.Count())
			return;

		if (IsMagazine() && QuantityPercent)
		{
			//! Add ammo "attachment" (use 1st ammo item) if not yet present and quantity is not zero
			TStringArray ammoItems = new TStringArray;
			GetGame().ConfigGetTextArray("CfgMagazines " + ClassName + " ammoItems", ammoItems);
			if (ammoItems.Count())
			{
				string ammo = ammoItems[0];
				ammo.ToLower();
				if (SpawnAttachments.Find(ammo) == -1)
					SpawnAttachments.Insert(ammo);
			}
		}
	}

	bool IsVehicle()
	{
		return GetGame().IsKindOf(ClassName, "CarScript") || GetGame().IsKindOf(ClassName, "ExpansionVehicleBase");
	}
}
