# Cannon Fodder Original Engine Bugs
  
Issues in the original engine which have been fixed  (unfortunately this list is not complete, as some bugs have been fixed and forgotten)
  
* The second and third squad grenade and rocket count did not refresh after using either weapon

* If the mouse was kept in the sidebar while changing squads, the camera would get stuck in a back and forth pattern

* Out of bounds memory accesses. Numerous code accessed past either the beginning or end of a few arrays. This was not really a noticable problem on the Amiga or in Dos but is on a modern OS with virtual memory (especially on dynamically allocated memory)

* Having a grenade explode at the bottom right corner of a map would cause memory corruption after the map array

* Soldier sprite frame numbers occasionally went out of range, leading to crashes or incorrect sprite rendering. This wasn’t an issue in the original, as the memory layout ensured an overflow into the next set of soldier frame data

* During Sprite_Handle_Troop_Direct_TowardMouse, the sprite frame could become invalid if Sprite_Handle_Troop_Animation changed the animation type and the new type didn’t include the current frame number.



