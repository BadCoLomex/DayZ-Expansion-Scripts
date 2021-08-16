modded class IntroSceneCharacter 
{
    override void LoadCharacterData(vector char_pos, vector char_rot, bool default_char = false)
    {
        super.LoadCharacterData(char_pos, char_rot, default_char);
        string charType;
        for (int i = m_Characters[ECharGender.Male].Count() - 1; i >= 0; i--)
        {
            charType = m_Characters[ECharGender.Male][i];
            if (charType.Contains("xpansion"))
                m_Characters[ECharGender.Male].Remove(i);
        }           
		for (int j = m_Characters[ECharGender.Female].Count() - 1; j >= 0; j--)
        {
            charType = m_Characters[ECharGender.Female][j];
            if (charType.Contains("xpansion"))
                m_Characters[ECharGender.Female].Remove(j);
        }        
    };
};