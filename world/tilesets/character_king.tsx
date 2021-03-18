<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.4" tiledversion="1.4.3" name="gfx_king" tilewidth="32" tileheight="32" tilecount="27" columns="27" objectalignment="topleft">
 <image source="../../gfx/characters/king.png" trans="ff00ff" width="864" height="32"/>
 <tile id="0" type="wait">
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="4" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="0" duration="800"/>
  </animation>
 </tile>
 <tile id="1" type="blink">
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="25" duration="100"/>
  </animation>
 </tile>
 <tile id="7" type="jump">
  <animation>
   <frame tileid="9" duration="100"/>
   <frame tileid="7" duration="100"/>
  </animation>
 </tile>
 <tile id="8" type="on_air">
  <animation>
   <frame tileid="8" duration="100"/>
  </animation>
 </tile>
 <tile id="10" type="walk">
  <animation>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
   <frame tileid="12" duration="100"/>
   <frame tileid="13" duration="100"/>
   <frame tileid="14" duration="100"/>
   <frame tileid="15" duration="100"/>
   <frame tileid="16" duration="100"/>
   <frame tileid="17" duration="100"/>
  </animation>
 </tile>
 <tile id="18" type="bubble">
  <animation>
   <frame tileid="18" duration="100"/>
   <frame tileid="19" duration="100"/>
   <frame tileid="20" duration="100"/>
   <frame tileid="19" duration="100"/>
   <frame tileid="18" duration="100"/>
  </animation>
 </tile>
 <tile id="21" type="hit">
  <animation>
   <frame tileid="21" duration="200"/>
   <frame tileid="22" duration="200"/>
   <frame tileid="23" duration="200"/>
  </animation>
 </tile>
</tileset>
